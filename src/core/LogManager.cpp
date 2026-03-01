#include "LogManager.h"
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QMutexLocker>
#include <QTextCodec>
#include <algorithm>

LogManager::LogManager(QObject *parent)
    : QObject(parent)
    , m_currentLogFile(nullptr)
    , m_logStream(nullptr)
    , m_logPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs")
    , m_currentLogLevel(INFO)
    , m_maxFileSize(10 * 1024 * 1024) // 10MB
    , m_maxFiles(5)
    , m_cleanupTimer(new QTimer(this))
{
    ensureLogDirectory();
    
    // 设置日志清理定时器（每天运行一次）
    m_cleanupTimer->setInterval(24 * 60 * 60 * 1000); // 24小时
    connect(m_cleanupTimer, &QTimer::timeout, this, &LogManager::cleanupOldLogs);
    m_cleanupTimer->start();
}

LogManager::~LogManager()
{
    if (m_currentLogFile) {
        if (m_currentLogFile->isOpen()) {
            m_currentLogFile->close();
        }
        delete m_currentLogFile;
        m_currentLogFile = nullptr;
    }
    if (m_logStream) {
        delete m_logStream;
        m_logStream = nullptr;
    }
}

void LogManager::log(LogLevel level, const QString &message, const QString &account)
{
    if (level < m_currentLogLevel) {
        return; // 级别不够，不记录
    }
    
    QMutexLocker locker(&m_logMutex);
    
    writeLogEntry(level, message, account);
    
    // 发送信号通知UI
    emit logMessage(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), 
                   level, message, account);
}

void LogManager::debug(const QString &message, const QString &account)
{
    log(DEBUG, message, account);
}

void LogManager::info(const QString &message, const QString &account)
{
    log(INFO, message, account);
}

void LogManager::warn(const QString &message, const QString &account)
{
    log(WARN, message, account);
}

void LogManager::error(const QString &message, const QString &account)
{
    log(ERROR, message, account);
}

void LogManager::setLogPath(const QString &path)
{
    QMutexLocker locker(&m_logMutex);
    m_logPath = path;
    ensureLogDirectory();
}

void LogManager::setMaxFileSize(qint64 size)
{
    QMutexLocker locker(&m_logMutex);
    m_maxFileSize = size;
}

void LogManager::setMaxFiles(int count)
{
    QMutexLocker locker(&m_logMutex);
    m_maxFiles = count;
}

void LogManager::setLogLevel(LogLevel level)
{
    QMutexLocker locker(&m_logMutex);
    m_currentLogLevel = level;
}

QString LogManager::levelToString(LogLevel level)
{
    switch (level) {
    case DEBUG: return "DEBUG";
    case INFO:  return "INFO";
    case WARN:  return "WARN";
    case ERROR: return "ERROR";
    default:    return "UNKNOWN";
    }
}

void LogManager::cleanupOldLogs()
{
    QMutexLocker locker(&m_logMutex);
    
    QDir logDir(m_logPath);
    QStringList logFiles = logDir.entryList(QStringList() << "*.log", QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    
    // 保留最新的 m_maxFiles 个文件，删除其余的
    for (int i = m_maxFiles; i < logFiles.size(); ++i) {
        QString filePath = m_logPath + "/" + logFiles[i];
        QFile::remove(filePath);
    }
}

QString LogManager::getCurrentLogFileName() const
{
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    return QString("%1/%2.log").arg(m_logPath).arg(dateStr);
}

bool LogManager::rotateLogFile()
{
    if (m_currentLogFile && m_currentLogFile->isOpen()) {
        m_currentLogFile->close();
    }
    
    QString logFileName = getCurrentLogFileName();
    m_currentLogFile = new QFile(logFileName);
    
    // 确保目录存在
    QFileInfo fileInfo(logFileName);
    QDir().mkpath(fileInfo.absolutePath());
    
    if (!m_currentLogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "无法打开日志文件:" << logFileName;
        return false;
    }
    
    // 设置UTF-8编码
    m_logStream = new QTextStream(m_currentLogFile);
    m_logStream->setCodec("UTF-8");
    
    return true;
}

void LogManager::ensureLogDirectory()
{
    QDir().mkpath(m_logPath);
}

void LogManager::writeLogEntry(LogLevel level, const QString &message, const QString &account)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString levelStr = levelToString(level);
    
    // 格式: [时间戳] [级别] 账号 - 消息
    QString logEntry;
    if (!account.isEmpty()) {
        logEntry = QString("[%1] [%2] 账号{%3} - %4")
                      .arg(timestamp)
                      .arg(levelStr)
                      .arg(account)
                      .arg(message);
    } else {
        logEntry = QString("[%1] [%2] %3")
                      .arg(timestamp)
                      .arg(levelStr)
                      .arg(message);
    }
    
    // 如果当前日志文件不存在或大小超过限制，则轮换
    if (!m_currentLogFile || 
        !m_currentLogFile->isOpen() || 
        m_currentLogFile->size() > m_maxFileSize) {
        if (!rotateLogFile()) {
            return; // 无法轮换日志文件
        }
    }
    
    // 写入日志
    *m_logStream << logEntry << Qt::endl;
    m_logStream->flush(); // 确保内容写入磁盘
}