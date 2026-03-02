#include "LogManager.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QStringConverter>

// 静态成员初始化
LogManager* LogManager::s_instance = nullptr;

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , m_logBasePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs")
    , m_daysToKeep(DEFAULT_DAYS_TO_KEEP)
{
    // 确保日志基础目录存在
    QDir().mkpath(m_logBasePath);
}

LogManager::~LogManager()
{
    QMutexLocker locker(&m_mutex);
    
    // 关闭所有日志文件和流
    for (auto it = m_logStreams.begin(); it != m_logStreams.end(); ++it) {
        if (it.value()) {
            it.value()->flush();
            delete it.value();
        }
    }
    m_logStreams.clear();
    
    for (auto it = m_logFiles.begin(); it != m_logFiles.end(); ++it) {
        if (it.value()) {
            if (it.value()->isOpen()) {
                it.value()->close();
            }
            delete it.value();
        }
    }
    m_logFiles.clear();
}

LogManager* LogManager::instance()
{
    if (!s_instance) {
        s_instance = new LogManager(QCoreApplication::instance());
    }
    return s_instance;
}

void LogManager::registerSingleton()
{
    // 单例通过 instance() 方法访问
    // QML 注册在 main.cpp 中通过 qmlRegisterSingletonInstance 完成
}

void LogManager::setLogBasePath(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    m_logBasePath = path;
    QDir().mkpath(m_logBasePath);
}

QString LogManager::levelToString(LogLevel level)
{
    switch (level) {
    case INFO:  return "INFO";
    case WARN:  return "WARN";
    case ERROR: return "ERROR";
    default:    return "INFO";
    }
}

LogManager::LogLevel LogManager::stringToLevel(const QString &level)
{
    QString upper = level.toUpper();
    if (upper == "WARN" || upper == "WARNING") {
        return WARN;
    } else if (upper == "ERROR") {
        return ERROR;
    }
    return INFO;
}

void LogManager::log(const QString &account, const QString &level, const QString &message)
{
    writeLog(stringToLevel(level), account, message);
}

void LogManager::logInfo(const QString &account, const QString &message)
{
    writeLog(INFO, account, message);
}

void LogManager::logWarn(const QString &account, const QString &message)
{
    writeLog(WARN, account, message);
}

void LogManager::logError(const QString &account, const QString &message)
{
    writeLog(ERROR, account, message);
}

QStringList LogManager::getRecentLogs(const QString &account, int count)
{
    QMutexLocker locker(&m_mutex);
    
    QString filePath = getLogFilePath(account);
    QFile file(filePath);
    QStringList logs;
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return logs;
    }
    
    // 优化：从文件末尾反向读取，避免读取整个文件
    // 对于大型日志文件，这显著提高性能
    qint64 fileSize = file.size();
    if (fileSize == 0) {
        file.close();
        return logs;
    }
    
    // 使用缓冲区从文件末尾反向搜索
    const int BUFFER_SIZE = 4096;
    QByteArray buffer;
    QList<QByteArray> lines;
    qint64 position = fileSize;
    
    while (lines.size() < count && position > 0) {
        // 调整读取位置
        qint64 readSize = qMin(BUFFER_SIZE, static_cast<int>(position));
        position -= readSize;
        
        if (!file.seek(position)) {
            break;
        }
        
        buffer = file.read(readSize);
        
        // 分割行并添加到列表
        QList<QByteArray> chunkLines = buffer.split('\n');
        
        // 如果不是第一次读取，第一个元素可能是不完整的行
        if (!lines.isEmpty() && !chunkLines.isEmpty()) {
            // 将不完整的行与之前的行合并
            chunkLines.last() = chunkLines.last() + lines.takeFirst();
        }
        
        // 将新行添加到开头
        for (int i = chunkLines.size() - 1; i >= 0; --i) {
            QString line = QString::fromUtf8(chunkLines[i]).trimmed();
            if (!line.isEmpty()) {
                lines.prepend(line.toUtf8());
            }
        }
    }
    
    file.close();
    
    // 转换为 QStringList 并限制数量
    for (int i = 0; i < qMin(count, lines.size()); ++i) {
        logs.append(QString::fromUtf8(lines[i]));
    }
    
    return logs;
}

void LogManager::writeLog(LogLevel level, const QString &account, const QString &message)
{
    QMutexLocker locker(&m_mutex);
    
    // 格式化日志条目
    QString formattedLog = formatLogEntry(level, account, message);
    
    // 获取或创建日志文件
    QFile* file = getOrCreateLogFile(account);
    if (!file || !file->isOpen()) {
        qWarning() << "LogManager: 无法打开日志文件，账号:" << account;
        return;
    }
    
    // 获取或创建文本流
    QTextStream* stream = m_logStreams.value(account, nullptr);
    if (!stream) {
        stream = new QTextStream(file);
        stream->setEncoding(QStringConverter::Utf8);
        m_logStreams[account] = stream;
    }
    
    // 写入日志
    *stream << formattedLog << Qt::endl;
    stream->flush();  // 确保立即写入（缓冲由 Qt 内部处理）
    
    // 发送信号通知 QML
    emit logAppended(account, formattedLog);
}

QString LogManager::getLogFilePath(const QString &account) const
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    return QString("%1/%2/%3.log").arg(m_logBasePath, account, date);
}

QFile* LogManager::getOrCreateLogFile(const QString &account)
{
    QString filePath = getLogFilePath(account);
    
    // 检查是否已有打开的文件
    QFile* file = m_logFiles.value(account, nullptr);
    
    // 检查文件路径是否变化（日期变化）
    if (file && file->fileName() != filePath) {
        // 关闭旧文件
        if (m_logStreams.contains(account)) {
            delete m_logStreams.take(account);
        }
        file->close();
        delete file;
        file = nullptr;
        m_logFiles.remove(account);
    }
    
    // 创建新文件
    if (!file) {
        ensureLogDirectory(account);
        file = new QFile(filePath);
        if (file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            m_logFiles[account] = file;
        } else {
            delete file;
            return nullptr;
        }
    }
    
    return file;
}

void LogManager::ensureLogDirectory(const QString &account)
{
    QString dirPath = QString("%1/%2").arg(m_logBasePath, account);
    QDir().mkpath(dirPath);
}

void LogManager::cleanupOldLogs()
{
    QDate cutoffDate = QDate::currentDate().addDays(-m_daysToKeep);

    QDir logsDir(m_logBasePath);
    if (!logsDir.exists()) {
        logInfo("system", "日志目录不存在，跳过清理");
        return;
    }

    logInfo("system", QString("开始执行日志清理，保留 %1 天内的日志").arg(m_daysToKeep));

    int totalDeleted = 0;
    qint64 totalFreed = 0;

    {
        // 使用局部作用域限制锁的范围，避免与 logInfo 死锁
        QMutexLocker locker(&m_mutex);

        // 遍历所有账号目录
        QFileInfoList accountDirs = logsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &accountDirInfo : accountDirs) {
            QDir accountDir(accountDirInfo.absoluteFilePath());
            LogCleanupResult result = deleteOldLogFiles(accountDir, cutoffDate);
            totalDeleted += result.deletedCount;
            totalFreed += result.freedSpace;
        }
    } // 锁在这里释放

    logInfo("system", QString("日志清理完成，删除 %1 个文件，释放 %2 字节")
        .arg(totalDeleted)
        .arg(totalFreed));
}

LogCleanupResult LogManager::deleteOldLogFiles(const QDir &logDir, const QDate &cutoffDate)
{
    LogCleanupResult result;

    QFileInfoList logFiles = logDir.entryInfoList(QStringList() << "*.log", QDir::Files);
    for (const QFileInfo &fileInfo : logFiles) {
        // 解析文件名中的日期（格式：yyyy-MM-dd.log）
        QDate fileDate = QDate::fromString(fileInfo.baseName(), "yyyy-MM-dd");

        // 如果文件名不是有效日期格式，跳过
        if (!fileDate.isValid()) {
            continue;
        }

        // 如果文件日期早于截止日期，删除
        if (fileDate < cutoffDate) {
            qint64 fileSize = fileInfo.size();
            if (QFile::remove(fileInfo.absoluteFilePath())) {
                result.deletedCount++;
                result.freedSpace += fileSize;
                qDebug() << "Deleted old log:" << fileInfo.absoluteFilePath();
            } else {
                logWarn("system", QString("无法删除日志文件: %1").arg(fileInfo.absoluteFilePath()));
            }
        }
    }

    return result;
}

QString LogManager::formatLogEntry(LogLevel level, const QString &account, const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString levelStr = levelToString(level);
    
    // 格式: [YYYY-MM-DD HH:mm:ss] [LEVEL] 账号名 - 操作描述
    return QString("[%1] [%2] %3 - %4")
        .arg(timestamp)
        .arg(levelStr)
        .arg(account)
        .arg(message);
}
