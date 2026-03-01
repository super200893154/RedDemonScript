#include "LogManager.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QDebug>

// 静态成员初始化
LogManager* LogManager::s_instance = nullptr;

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , m_logBasePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs")
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
    
    QTextStream in(&file);
    in.setCodec("UTF-8");
    
    // 读取所有行
    QStringList allLines;
    while (!in.atEnd()) {
        allLines.append(in.readLine());
    }
    file.close();
    
    // 返回最后 count 行
    int start = qMax(0, allLines.size() - count);
    for (int i = start; i < allLines.size(); ++i) {
        logs.append(allLines[i]);
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
        stream->setCodec("UTF-8");
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
