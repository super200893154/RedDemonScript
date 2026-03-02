#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QDir>
#include <QQmlEngine>
#include <QJSEngine>

/**
 * @brief 配置管理器，负责加载和保存应用程序配置
 *
 * 功能：
 * - 从 JSON 文件加载配置
 * - 将配置保存到 JSON 文件
 * - 提供配置项访问接口
 * - 支持默认配置创建
 * - 线程安全的配置访问
 */
class ConfigManager : public QObject
{
    Q_OBJECT

    // QML 可访问的配置属性
    Q_PROPERTY(int restartInterval READ restartInterval WRITE setRestartInterval NOTIFY restartIntervalChanged)
    Q_PROPERTY(int randomOffset READ randomOffset WRITE setRandomOffset NOTIFY randomOffsetChanged)
    Q_PROPERTY(bool timerEnabled READ timerEnabled WRITE setTimerEnabled NOTIFY timerEnabledChanged)
    Q_PROPERTY(QString sandboxiePath READ sandboxiePath WRITE setSandboxiePath NOTIFY sandboxiePathChanged)
    Q_PROPERTY(QString processName READ processName WRITE setProcessName NOTIFY processNameChanged)
    Q_PROPERTY(QString dungeonName READ dungeonName WRITE setDungeonName NOTIFY dungeonNameChanged)
    Q_PROPERTY(int maxRuns READ maxRuns WRITE setMaxRuns NOTIFY maxRunsChanged)
    Q_PROPERTY(int retryCount READ retryCount WRITE setRetryCount NOTIFY retryCountChanged)
    Q_PROPERTY(int checkInterval READ checkInterval WRITE setCheckInterval NOTIFY checkIntervalChanged)
    Q_PROPERTY(int responseTimeout READ responseTimeout WRITE setResponseTimeout NOTIFY responseTimeoutChanged)
    Q_PROPERTY(bool crashRecoveryEnabled READ crashRecoveryEnabled WRITE setCrashRecoveryEnabled NOTIFY crashRecoveryEnabledChanged)
    Q_PROPERTY(QString logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(int logMaxFileSize READ logMaxFileSize WRITE setLogMaxFileSize NOTIFY logMaxFileSizeChanged)
    Q_PROPERTY(int logMaxFiles READ logMaxFiles WRITE setLogMaxFiles NOTIFY logMaxFilesChanged)
    Q_PROPERTY(QString logPath READ logPath WRITE setLogPath NOTIFY logPathChanged)
    Q_PROPERTY(QString configPath READ configPath)

public:
    /**
     * @brief 获取单例实例
     */
    static ConfigManager* instance();

    /**
     * @brief 注册单例到 QML 引擎
     */
    static void registerSingleton();

    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager() override;

    // 禁止拷贝
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // 加载/保存配置
    Q_INVOKABLE bool loadConfig();
    Q_INVOKABLE bool saveConfig();

    // 定时器配置访问接口 (FR33)
    int restartInterval() const;
    void setRestartInterval(int interval);
    int randomOffset() const;
    void setRandomOffset(int offset);
    bool timerEnabled() const;
    void setTimerEnabled(bool enabled);

    // 脚本配置访问接口 (FR34, FR35)
    QString sandboxiePath() const;
    void setSandboxiePath(const QString &path);
    QString processName() const;
    void setProcessName(const QString &name);
    QString dungeonName() const;
    void setDungeonName(const QString &name);
    int maxRuns() const;
    void setMaxRuns(int runs);
    int retryCount() const;
    void setRetryCount(int count);

    // 监控配置访问接口
    int checkInterval() const;
    void setCheckInterval(int interval);
    int responseTimeout() const;
    void setResponseTimeout(int timeout);
    bool crashRecoveryEnabled() const;
    void setCrashRecoveryEnabled(bool enabled);

    // 日志配置访问接口
    QString logLevel() const;
    void setLogLevel(const QString &level);
    int logMaxFileSize() const;
    void setLogMaxFileSize(int size);
    int logMaxFiles() const;
    void setLogMaxFiles(int count);
    QString logPath() const;
    void setLogPath(const QString &path);

    // 账号配置访问接口 (FR13)
    Q_INVOKABLE QJsonArray accounts() const;
    Q_INVOKABLE bool addAccount(const QJsonObject &account);
    Q_INVOKABLE bool removeAccount(int index);
    Q_INVOKABLE bool updateAccount(int index, const QJsonObject &account);

    // 获取配置路径
    QString configPath() const;

signals:
    // 配置变更信号
    void restartIntervalChanged(int interval);
    void randomOffsetChanged(int offset);
    void timerEnabledChanged(bool enabled);
    void sandboxiePathChanged(const QString &path);
    void processNameChanged(const QString &name);
    void dungeonNameChanged(const QString &name);
    void maxRunsChanged(int runs);
    void retryCountChanged(int count);
    void checkIntervalChanged(int interval);
    void responseTimeoutChanged(int timeout);
    void crashRecoveryEnabledChanged(bool enabled);
    void logLevelChanged(const QString &level);
    void logMaxFileSizeChanged(int size);
    void logMaxFilesChanged(int count);
    void logPathChanged(const QString &path);

    // 配置操作信号
    void configLoaded();
    void configSaved();
    void configError(const QString &message);
    void accountsChanged();

private:
    // 创建默认配置
    void createDefaultConfig();

    // 验证配置
    bool validateConfig();

    // 内部保存配置（不获取锁，调用者需确保已持锁）
    bool saveConfigInternal();

    // 获取配置值（带默认值）
    QJsonValue getValue(const QString &path, const QJsonValue &defaultValue = QJsonValue::Null) const;
    void setValue(const QString &path, const QJsonValue &value);

    // 确保配置目录存在
    bool ensureConfigDirectory();

    // 单例实例
    static ConfigManager* s_instance;

    // 数据成员
    mutable QMutex m_mutex;
    QString m_configPath;
    QJsonObject m_config;

    // 配置版本（用于向后兼容和迁移）
    static constexpr int CONFIG_VERSION = 1;

    // 默认值常量
    static constexpr int DEFAULT_RESTART_INTERVAL = 7200;
    static constexpr int DEFAULT_RANDOM_OFFSET = 300;
    static constexpr bool DEFAULT_TIMER_ENABLED = true;
    static constexpr int DEFAULT_CHECK_INTERVAL = 5000;
    static constexpr int DEFAULT_RESPONSE_TIMEOUT = 30000;
    static constexpr bool DEFAULT_CRASH_RECOVERY_ENABLED = true;
    static constexpr int DEFAULT_MAX_RUNS = 0;
    static constexpr int DEFAULT_RETRY_COUNT = 3;
    static constexpr int DEFAULT_LOG_MAX_FILE_SIZE = 10485760;
    static constexpr int DEFAULT_LOG_MAX_FILES = 5;
};

#endif // CONFIGMANAGER_H
