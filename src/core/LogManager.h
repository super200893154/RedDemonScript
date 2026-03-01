#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>

/**
 * @brief 日志管理器，负责记录和管理应用程序日志
 * 
 * 功能：
 * - 按账号隔离存储日志到独立目录 logs/{accountName}/{date}.log
 * - 线程安全的日志写入
 * - 缓冲写入机制满足 < 50ms 延迟要求
 * - 提供 QML 可调用接口
 */
class LogManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 日志级别枚举
     */
    enum LogLevel {
        INFO = 0,
        WARN = 1,
        ERROR = 2
    };
    Q_ENUM(LogLevel)

    /**
     * @brief 获取单例实例
     */
    static LogManager* instance();

    /**
     * @brief 注册单例到 QML 引擎
     */
    static void registerSingleton();

    explicit LogManager(QObject *parent = nullptr);
    ~LogManager() override;

    // 禁止拷贝
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    // QML 可调用的日志方法
    Q_INVOKABLE void log(const QString &account, const QString &level, const QString &message);
    Q_INVOKABLE void logInfo(const QString &account, const QString &message);
    Q_INVOKABLE void logWarn(const QString &account, const QString &message);
    Q_INVOKABLE void logError(const QString &account, const QString &message);

    // 获取最近日志（供 QML 显示）
    Q_INVOKABLE QStringList getRecentLogs(const QString &account, int count = 100);

    // 配置方法
    void setLogBasePath(const QString &path);

    // 工具方法
    static QString levelToString(LogLevel level);
    static LogLevel stringToLevel(const QString &level);

signals:
    /**
     * @brief 日志追加信号，通知 QML 更新显示
     */
    void logAppended(const QString &account, const QString &formattedLog);

private:
    // 内部日志方法
    void writeLog(LogLevel level, const QString &account, const QString &message);

    // 文件管理
    QString getLogFilePath(const QString &account) const;
    QFile* getOrCreateLogFile(const QString &account);
    void ensureLogDirectory(const QString &account);
    QString formatLogEntry(LogLevel level, const QString &account, const QString &message);

    // 单例实例
    static LogManager* s_instance;

    // 数据成员
    mutable QMutex m_mutex;
    QString m_logBasePath;
    QMap<QString, QFile*> m_logFiles;          // 账号 -> 日志文件
    QMap<QString, QTextStream*> m_logStreams;  // 账号 -> 日志流
    static constexpr int MAX_RECENT_LOGS = 1000;
};

#endif // LOGMANAGER_H
