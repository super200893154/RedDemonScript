#include "ConfigManager.h"
#include "LogManager.h"
#include "../helpers/DpapiHelper.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

ConfigManager* ConfigManager::s_instance = nullptr;

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
    , m_configPath("")
{
    // 设置默认配置路径为应用目录下的 config/config.json
    QString appDir = QCoreApplication::applicationDirPath();
    m_configPath = appDir + "/config/config.json";
}

ConfigManager::~ConfigManager()
{
}

ConfigManager* ConfigManager::instance()
{
    if (!s_instance) {
        s_instance = new ConfigManager();
    }
    return s_instance;
}

void ConfigManager::registerSingleton()
{
    // 注册到 QML 上下文在 main.cpp 中完成
    qmlRegisterSingletonType<ConfigManager>("RedDemonScript", 1, 0, "ConfigManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return ConfigManager::instance();
        });
}

bool ConfigManager::loadConfig()
{
    QMutexLocker locker(&m_mutex);

    QFile file(m_configPath);
    if (!file.exists()) {
        if (LogManager::instance()) {
            LogManager::instance()->logInfo("system", "配置文件不存在，创建默认配置");
        } else {
            qDebug() << "[Config] Config file does not exist, creating default config";
        }
        createDefaultConfig();
        return saveConfigInternal();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        QString errorMsg = QString("无法打开配置文件: %1").arg(m_configPath);
        if (LogManager::instance()) {
            LogManager::instance()->logError("system", errorMsg);
        } else {
            qDebug() << "[Config] ERROR:" << errorMsg;
        }
        emit configError(errorMsg);
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        QString errorMsg = QString("配置文件解析失败: %1").arg(error.errorString());
        if (LogManager::instance()) {
            LogManager::instance()->logError("system", errorMsg);
        } else {
            qDebug() << "[Config] ERROR:" << errorMsg;
        }
        emit configError(errorMsg);
        return false;
    }

    m_config = doc.object();

    // 验证配置
    if (!validateConfig()) {
        QString errorMsg = "配置验证失败，使用默认配置";
        if (LogManager::instance()) {
            LogManager::instance()->logWarn("system", errorMsg);
        }
        createDefaultConfig();
    }

    if (LogManager::instance()) {
        LogManager::instance()->logInfo("system", "配置加载成功");
    } else {
        qDebug() << "[Config] Config loaded successfully";
    }

    emit configLoaded();
    return true;
}

bool ConfigManager::saveConfig()
{
    QMutexLocker locker(&m_mutex);
    return saveConfigInternal();
}

bool ConfigManager::saveConfigInternal()
{
    // 注意：调用此方法前必须已持有 m_mutex 锁

    // 确保配置目录存在
    if (!ensureConfigDirectory()) {
        QString errorMsg = QString("无法创建配置目录");
        if (LogManager::instance()) {
            LogManager::instance()->logError("system", errorMsg);
        }
        emit configError(errorMsg);
        return false;
    }

    // 备份现有配置（如果存在）
    QString backupPath = m_configPath + ".bak";
    QFile actualFile(m_configPath);
    bool hasExistingConfig = actualFile.exists();
    if (hasExistingConfig) {
        // 删除旧备份
        QFile oldBackup(backupPath);
        if (oldBackup.exists()) {
            oldBackup.remove();
        }
        // 创建新备份
        if (!QFile::copy(m_configPath, backupPath)) {
            if (LogManager::instance()) {
                LogManager::instance()->logWarn("system", "无法创建配置备份");
            }
        }
    }

    // 原子写入：先写临时文件，再重命名
    QString tempPath = m_configPath + ".temp";
    QFile tempFile(tempPath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        QString errorMsg = QString("无法写入临时配置文件: %1").arg(tempPath);
        if (LogManager::instance()) {
            LogManager::instance()->logError("system", errorMsg);
        }
        emit configError(errorMsg);
        return false;
    }

    QJsonDocument doc(m_config);
    QByteArray data = doc.toJson(QJsonDocument::Indented);
    tempFile.write(data);
    tempFile.flush();  // 确保数据写入磁盘
    tempFile.close();

    // 删除现有配置文件（如果存在）
    if (actualFile.exists()) {
        if (!actualFile.remove()) {
            QString errorMsg = QString("无法删除旧配置文件: %1").arg(m_configPath);
            if (LogManager::instance()) {
                LogManager::instance()->logError("system", errorMsg);
            }
            // 清理临时文件
            QFile::remove(tempPath);
            emit configError(errorMsg);
            return false;
        }
    }

    // 重命名临时文件为正式文件
    if (!QFile::rename(tempPath, m_configPath)) {
        QString errorMsg = QString("无法保存配置文件: %1").arg(m_configPath);
        if (LogManager::instance()) {
            LogManager::instance()->logError("system", errorMsg);
        }
        
        // 尝试从备份恢复
        if (hasExistingConfig && QFile::exists(backupPath)) {
            if (QFile::copy(backupPath, m_configPath)) {
                if (LogManager::instance()) {
                    LogManager::instance()->logWarn("system", "已从备份恢复配置文件");
                }
            }
        }
        
        // 清理临时文件
        QFile::remove(tempPath);
        emit configError(errorMsg);
        return false;
    }

    if (LogManager::instance()) {
        LogManager::instance()->logInfo("system", "配置保存成功");
    } else {
        qDebug() << "[Config] Config saved successfully";
    }

    emit configSaved();
    return true;
}

void ConfigManager::createDefaultConfig()
{
    m_config = QJsonObject{
        {"version", CONFIG_VERSION},
        {"accounts", QJsonArray()},
        {"script", QJsonObject{
            {"dungeonName", ""},
            {"maxRuns", DEFAULT_MAX_RUNS},
            {"retryCount", DEFAULT_RETRY_COUNT},
            {"sandboxiePath", "C:\\Program Files\\Sandboxie-Plus\\Start.exe"},
            {"processName", "game.exe"}
        }},
        {"timer", QJsonObject{
            {"restartInterval", DEFAULT_RESTART_INTERVAL},
            {"randomOffset", DEFAULT_RANDOM_OFFSET},
            {"enabled", DEFAULT_TIMER_ENABLED}
        }},
        {"monitor", QJsonObject{
            {"checkInterval", DEFAULT_CHECK_INTERVAL},
            {"responseTimeout", DEFAULT_RESPONSE_TIMEOUT},
            {"crashRecoveryEnabled", DEFAULT_CRASH_RECOVERY_ENABLED}
        }},
        {"logging", QJsonObject{
            {"level", "INFO"},
            {"maxFileSize", DEFAULT_LOG_MAX_FILE_SIZE},
            {"maxFiles", DEFAULT_LOG_MAX_FILES},
            {"logPath", "logs/"}
        }}
    };
}

bool ConfigManager::validateConfig()
{
    // 验证必需的顶层键存在
    QStringList requiredKeys = {"accounts", "script", "timer", "monitor", "logging"};
    for (const QString &key : requiredKeys) {
        if (!m_config.contains(key) || !m_config[key].isObject()) {
            return false;
        }
    }

    // 验证 timer 对象 - 检查类型和范围
    QJsonObject timer = m_config["timer"].toObject();
    if (!timer.contains("restartInterval") || !timer["restartInterval"].isDouble()) {
        return false;
    }
    int restartInterval = timer["restartInterval"].toInt();
    if (restartInterval < 60 || restartInterval > 86400) { // 1分钟到24小时
        return false;
    }

    if (!timer.contains("enabled") || !timer["enabled"].isBool()) {
        return false;
    }

    // 验证 script 对象 - 检查类型
    QJsonObject script = m_config["script"].toObject();
    if (!script.contains("sandboxiePath") || !script["sandboxiePath"].isString()) {
        return false;
    }
    if (!script.contains("processName") || !script["processName"].isString()) {
        return false;
    }
    if (script.contains("maxRuns") && !script["maxRuns"].isDouble()) {
        return false;
    }
    if (script.contains("retryCount") && !script["retryCount"].isDouble()) {
        return false;
    }

    // 验证 monitor 对象
    QJsonObject monitor = m_config["monitor"].toObject();
    if (monitor.contains("checkInterval") && !monitor["checkInterval"].isDouble()) {
        return false;
    }
    if (monitor.contains("responseTimeout") && !monitor["responseTimeout"].isDouble()) {
        return false;
    }

    // 验证 logging 对象
    QJsonObject logging = m_config["logging"].toObject();
    if (logging.contains("level") && !logging["level"].isString()) {
        return false;
    }

    // 验证 accounts 数组
    if (!m_config["accounts"].isArray()) {
        return false;
    }

    return true;
}

QJsonValue ConfigManager::getValue(const QString &path, const QJsonValue &defaultValue) const
{
    QStringList keys = path.split(".");
    QJsonObject current = m_config;

    for (int i = 0; i < keys.size() - 1; ++i) {
        if (!current.contains(keys[i]) || !current[keys[i]].isObject()) {
            return defaultValue;
        }
        current = current[keys[i]].toObject();
    }

    if (current.contains(keys.last())) {
        return current[keys.last()];
    }

    return defaultValue;
}

void ConfigManager::setValue(const QString &path, const QJsonValue &value)
{
    QStringList keys = path.split(".");
    if (keys.isEmpty()) return;

    // 使用递归方式正确更新嵌套对象
    std::function<QJsonObject(QJsonObject&, int)> updateNested = 
        [&](QJsonObject &obj, int depth) -> QJsonObject {
        if (depth == keys.size() - 1) {
            // 到达目标层级，设置值
            obj[keys[depth]] = value;
            return obj;
        }

        // 获取或创建下一层对象
        QJsonObject nested = obj.contains(keys[depth]) ? obj[keys[depth]].toObject() : QJsonObject();
        QJsonObject updated = updateNested(nested, depth + 1);
        obj[keys[depth]] = updated;
        return obj;
    };

    updateNested(m_config, 0);
}

bool ConfigManager::ensureConfigDirectory()
{
    QFileInfo fileInfo(m_configPath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}

// 定时器配置实现
int ConfigManager::restartInterval() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("timer.restartInterval", DEFAULT_RESTART_INTERVAL).toInt();
}

void ConfigManager::setRestartInterval(int interval)
{
    QMutexLocker locker(&m_mutex);
    if (restartInterval() != interval) {
        setValue("timer.restartInterval", interval);
        emit restartIntervalChanged(interval);
    }
}

int ConfigManager::randomOffset() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("timer.randomOffset", DEFAULT_RANDOM_OFFSET).toInt();
}

void ConfigManager::setRandomOffset(int offset)
{
    QMutexLocker locker(&m_mutex);
    if (randomOffset() != offset) {
        setValue("timer.randomOffset", offset);
        emit randomOffsetChanged(offset);
    }
}

bool ConfigManager::timerEnabled() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("timer.enabled", DEFAULT_TIMER_ENABLED).toBool();
}

void ConfigManager::setTimerEnabled(bool enabled)
{
    QMutexLocker locker(&m_mutex);
    if (timerEnabled() != enabled) {
        setValue("timer.enabled", enabled);
        emit timerEnabledChanged(enabled);
    }
}

// 脚本配置实现
QString ConfigManager::sandboxiePath() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("script.sandboxiePath", "C:\\Program Files\\Sandboxie-Plus\\Start.exe").toString();
}

void ConfigManager::setSandboxiePath(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    if (sandboxiePath() != path) {
        setValue("script.sandboxiePath", path);
        emit sandboxiePathChanged(path);
    }
}

QString ConfigManager::processName() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("script.processName", "game.exe").toString();
}

void ConfigManager::setProcessName(const QString &name)
{
    QMutexLocker locker(&m_mutex);
    if (processName() != name) {
        setValue("script.processName", name);
        emit processNameChanged(name);
    }
}

QString ConfigManager::dungeonName() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("script.dungeonName", "").toString();
}

void ConfigManager::setDungeonName(const QString &name)
{
    QMutexLocker locker(&m_mutex);
    if (dungeonName() != name) {
        setValue("script.dungeonName", name);
        emit dungeonNameChanged(name);
    }
}

int ConfigManager::maxRuns() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("script.maxRuns", DEFAULT_MAX_RUNS).toInt();
}

void ConfigManager::setMaxRuns(int runs)
{
    QMutexLocker locker(&m_mutex);
    if (maxRuns() != runs) {
        setValue("script.maxRuns", runs);
        emit maxRunsChanged(runs);
    }
}

int ConfigManager::retryCount() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("script.retryCount", DEFAULT_RETRY_COUNT).toInt();
}

void ConfigManager::setRetryCount(int count)
{
    QMutexLocker locker(&m_mutex);
    if (retryCount() != count) {
        setValue("script.retryCount", count);
        emit retryCountChanged(count);
    }
}

// 监控配置实现
int ConfigManager::checkInterval() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("monitor.checkInterval", DEFAULT_CHECK_INTERVAL).toInt();
}

void ConfigManager::setCheckInterval(int interval)
{
    QMutexLocker locker(&m_mutex);
    if (checkInterval() != interval) {
        setValue("monitor.checkInterval", interval);
        emit checkIntervalChanged(interval);
    }
}

int ConfigManager::responseTimeout() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("monitor.responseTimeout", DEFAULT_RESPONSE_TIMEOUT).toInt();
}

void ConfigManager::setResponseTimeout(int timeout)
{
    QMutexLocker locker(&m_mutex);
    if (responseTimeout() != timeout) {
        setValue("monitor.responseTimeout", timeout);
        emit responseTimeoutChanged(timeout);
    }
}

bool ConfigManager::crashRecoveryEnabled() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("monitor.crashRecoveryEnabled", DEFAULT_CRASH_RECOVERY_ENABLED).toBool();
}

void ConfigManager::setCrashRecoveryEnabled(bool enabled)
{
    QMutexLocker locker(&m_mutex);
    if (crashRecoveryEnabled() != enabled) {
        setValue("monitor.crashRecoveryEnabled", enabled);
        emit crashRecoveryEnabledChanged(enabled);
    }
}

// 日志配置实现
QString ConfigManager::logLevel() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("logging.level", "INFO").toString();
}

void ConfigManager::setLogLevel(const QString &level)
{
    QMutexLocker locker(&m_mutex);
    if (logLevel() != level) {
        setValue("logging.level", level);
        emit logLevelChanged(level);
    }
}

int ConfigManager::logMaxFileSize() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("logging.maxFileSize", DEFAULT_LOG_MAX_FILE_SIZE).toInt();
}

void ConfigManager::setLogMaxFileSize(int size)
{
    QMutexLocker locker(&m_mutex);
    if (logMaxFileSize() != size) {
        setValue("logging.maxFileSize", size);
        emit logMaxFileSizeChanged(size);
    }
}

int ConfigManager::logMaxFiles() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("logging.maxFiles", DEFAULT_LOG_MAX_FILES).toInt();
}

void ConfigManager::setLogMaxFiles(int count)
{
    QMutexLocker locker(&m_mutex);
    if (logMaxFiles() != count) {
        setValue("logging.maxFiles", count);
        emit logMaxFilesChanged(count);
    }
}

QString ConfigManager::logPath() const
{
    QMutexLocker locker(&m_mutex);
    return getValue("logging.logPath", "logs/").toString();
}

void ConfigManager::setLogPath(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    if (logPath() != path) {
        setValue("logging.logPath", path);
        emit logPathChanged(path);
    }
}

// 账号配置实现
QJsonArray ConfigManager::accounts() const
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    QJsonArray decryptedAccounts;

    for (const QJsonValue &value : accounts) {
        QJsonObject account = value.toObject();

        // 解密密码字段
        if (account.contains("passwordEncrypted") && account["passwordEncrypted"].toBool()) {
            QByteArray encryptedPassword = account["password"].toString().toUtf8();
            QByteArray decryptedPassword;
            QString description;

            if (DpapiHelper::decrypt(encryptedPassword, decryptedPassword, description)) {
                account["password"] = QString::fromUtf8(decryptedPassword);
            } else {
                // 解密失败，记录日志但返回加密值
                LogManager *logger = LogManager::instance();
                if (logger) {
                    logger->logWarn("system",
                        QString("账号 %1 密码解密失败").arg(account["username"].toString()));
                }
            }
        }

        decryptedAccounts.append(account);
    }

    return decryptedAccounts;
}

bool ConfigManager::addAccount(const QJsonObject &account)
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();

    // 复制账号对象以便加密密码
    QJsonObject encryptedAccount = account;

    // 加密密码字段
    if (account.contains("password") && !account["password"].toString().isEmpty()) {
        QString password = account["password"].toString();
        QByteArray encryptedPassword;

        if (DpapiHelper::encrypt(password.toUtf8(), encryptedPassword, "RedDemonScript Password")) {
            encryptedAccount["password"] = QString(encryptedPassword);
            encryptedAccount["passwordEncrypted"] = true;  // 标记已加密
        } else {
            LogManager::instance()->logError("system", "账号密码加密失败");
            return false;
        }
    }

    accounts.append(encryptedAccount);
    m_config["accounts"] = accounts;

    // 自动保存配置
    bool saved = saveConfigInternal();
    if (saved) {
        emit accountsChanged();
    }
    return saved;
}

bool ConfigManager::removeAccount(int index)
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    if (index >= 0 && index < accounts.size()) {
        accounts.removeAt(index);
        m_config["accounts"] = accounts;
        
        // 自动保存配置
        bool saved = saveConfigInternal();
        if (saved) {
            emit accountsChanged();
        }
        return saved;
    }
    return false;
}

bool ConfigManager::updateAccount(int index, const QJsonObject &account)
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    if (index < 0 || index >= accounts.size()) {
        return false;
    }

    // 复制账号对象以便加密密码
    QJsonObject encryptedAccount = account;

    // 加密密码字段（仅当密码不为空且未加密时才加密）
    if (account.contains("password") && !account["password"].toString().isEmpty()) {
        // 检查是否需要重新加密（如果密码是明文或与存储的不同）
        bool needsEncryption = true;
        QJsonObject existingAccount = accounts[index].toObject();

        if (existingAccount.contains("passwordEncrypted") &&
            existingAccount["passwordEncrypted"].toBool() &&
            existingAccount.contains("password")) {
            // 已加密，检查密码是否改变
            QByteArray storedEncrypted = existingAccount["password"].toString().toUtf8();
            QByteArray decryptedPassword;
            QString description;
            if (DpapiHelper::decrypt(storedEncrypted, decryptedPassword, description)) {
                if (QString::fromUtf8(decryptedPassword) == account["password"].toString()) {
                    needsEncryption = false; // 密码未改变，无需重新加密
                }
            }
        }

        if (needsEncryption) {
            QString password = account["password"].toString();
            QByteArray encryptedPassword;

            if (DpapiHelper::encrypt(password.toUtf8(), encryptedPassword, "RedDemonScript Password")) {
                encryptedAccount["password"] = QString(encryptedPassword);
                encryptedAccount["passwordEncrypted"] = true;
            } else {
                LogManager::instance()->logError("system", "账号密码加密失败");
                return false;
            }
        }
    }

    accounts[index] = encryptedAccount;
    m_config["accounts"] = accounts;

    // 自动保存配置
    bool saved = saveConfigInternal();
    if (saved) {
        emit accountsChanged();
    }
    return saved;
}

QString ConfigManager::configPath() const
{
    return m_configPath;
}
