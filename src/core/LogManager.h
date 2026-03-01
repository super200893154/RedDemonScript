#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QMutex>
#include <QTimer>

/**
 * @brief 日志管理器，负责记录和管理应用程序日志
 */
class LogManager : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3
    };
    Q_ENUM(LogLevel)

    explicit LogManager(QObject *parent = nullptr);
    ~LogManager();

    // 日志记录方法
    void log(LogLevel level, const QString &message, const QString &account = QString());
    void debug(const QString &message, const QString &account = QString());
    void info(const QString &message, const QString &account = QString());
    void warn(const QString &message, const QString &account = QString());
    void error(const QString &message, const QString &account = QString());

    // 配置方法
    void setLogPath(const QString &path);
    void setMaxFileSize(qint64 size);
    void setMaxFiles(int count);
    void setLogLevel(LogLevel level);

    // 工具方法
    static QString levelToString(LogLevel level);

signals:
    void logMessage(const QString &timestamp, LogLevel level, const QString &message, const QString &account);

private slots:
    void cleanupOldLogs();

private:
    QFile *m_currentLogFile;
    QTextStream *m_logStream;
    QString m_logPath;
    LogLevel m_currentLogLevel;
    qint64 m_maxFileSize;
    int m_maxFiles;
    
    QMutex m_logMutex;
    QTimer *m_cleanupTimer;

    // 内部辅助方法
    QString getCurrentLogFileName() const;
    bool rotateLogFile();
    void ensureLogDirectory();
    void writeLogEntry(LogLevel level, const QString &message, const QString &account);
};

#endif // LOGMANAGER_H