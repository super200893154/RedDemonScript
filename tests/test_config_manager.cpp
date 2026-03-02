/**
 * @file test_config_manager.cpp
 * @brief ConfigManager 测试验证程序
 *
 * 测试项目：
 * - 6.1 验证配置文件正确加载
 * - 6.2 验证配置文件正确保存
 * - 6.3 验证默认配置创建
 * - 6.4 验证配置格式人类可读
 */

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QTimer>
#include "../src/core/ConfigManager.h"
#include "../src/core/LogManager.h"

class ConfigManagerTest : public QObject {
    Q_OBJECT

public:
    ConfigManagerTest(QObject *parent = nullptr) : QObject(parent) {}

    void runTests() {
        qDebug() << "\n========== ConfigManager 测试开始 ==========\n";

        int passed = 0;
        int failed = 0;

        // 测试 6.3: 验证默认配置创建
        if (testDefaultConfigCreation()) {
            passed++;
        } else {
            failed++;
        }

        // 测试 6.1: 验证配置文件正确加载
        if (testConfigLoading()) {
            passed++;
        } else {
            failed++;
        }

        // 测试 6.2: 验证配置文件正确保存
        if (testConfigSaving()) {
            passed++;
        } else {
            failed++;
        }

        // 测试 6.4: 验证配置格式人类可读
        if (testHumanReadableFormat()) {
            passed++;
        } else {
            failed++;
        }

        // 测试配置值访问
        if (testConfigValueAccess()) {
            passed++;
        } else {
            failed++;
        }

        // 测试账号操作
        if (testAccountOperations()) {
            passed++;
        } else {
            failed++;
        }

        qDebug() << "\n========== 测试结果 ==========";
        qDebug() << "通过:" << passed;
        qDebug() << "失败:" << failed;
        qDebug() << "==============================\n";

        QCoreApplication::exit(failed > 0 ? 1 : 0);
    }

private:
    bool testDefaultConfigCreation() {
        qDebug() << "[测试 6.3] 验证默认配置创建...";

        // 删除现有配置文件
        QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";
        QFile::remove(configPath);

        // 重新加载配置（应该创建默认配置）
        bool result = ConfigManager::instance()->loadConfig();

        if (!result) {
            qDebug() << "  ❌ 默认配置创建失败";
            return false;
        }

        // 验证默认配置值
        bool success = true;

        if (ConfigManager::instance()->restartInterval() != 7200) {
            qDebug() << "  ❌ 默认 restartInterval 不正确";
            success = false;
        }

        if (ConfigManager::instance()->sandboxiePath() != "C:\\Program Files\\Sandboxie-Plus\\Start.exe") {
            qDebug() << "  ❌ 默认 sandboxiePath 不正确";
            success = false;
        }

        if (ConfigManager::instance()->processName() != "game.exe") {
            qDebug() << "  ❌ 默认 processName 不正确";
            success = false;
        }

        // 验证配置文件已创建
        if (!QFile::exists(configPath)) {
            qDebug() << "  ❌ 配置文件未创建";
            success = false;
        }

        if (success) {
            qDebug() << "  ✅ 默认配置创建测试通过";
        }
        return success;
    }

    bool testConfigLoading() {
        qDebug() << "[测试 6.1] 验证配置文件正确加载...";

        // 创建一个测试配置文件
        QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";
        QJsonObject testConfig{
            {"version", 1},
            {"accounts", QJsonArray()},
            {"script", QJsonObject{
                {"dungeonName", "测试副本"},
                {"maxRuns", 10},
                {"retryCount", 5},
                {"sandboxiePath", "D:\\Sandboxie\\Start.exe"},
                {"processName", "test.exe"}
            }},
            {"timer", QJsonObject{
                {"restartInterval", 3600},
                {"randomOffset", 600},
                {"enabled", false}
            }},
            {"monitor", QJsonObject{
                {"checkInterval", 10000},
                {"responseTimeout", 60000},
                {"crashRecoveryEnabled", false}
            }},
            {"logging", QJsonObject{
                {"level", "DEBUG"},
                {"maxFileSize", 5242880},
                {"maxFiles", 10},
                {"logPath", "test_logs/"}
            }}
        };

        QJsonDocument doc(testConfig);
        QFile file(configPath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "  ❌ 无法创建测试配置文件";
            return false;
        }
        file.write(doc.toJson());
        file.close();

        // 重新加载配置
        bool result = ConfigManager::instance()->loadConfig();

        if (!result) {
            qDebug() << "  ❌ 配置加载失败";
            return false;
        }

        // 验证加载的配置值
        bool success = true;

        if (ConfigManager::instance()->restartInterval() != 3600) {
            qDebug() << "  ❌ restartInterval 加载不正确:" << ConfigManager::instance()->restartInterval();
            success = false;
        }

        if (ConfigManager::instance()->sandboxiePath() != "D:\\Sandboxie\\Start.exe") {
            qDebug() << "  ❌ sandboxiePath 加载不正确:" << ConfigManager::instance()->sandboxiePath();
            success = false;
        }

        if (ConfigManager::instance()->processName() != "test.exe") {
            qDebug() << "  ❌ processName 加载不正确:" << ConfigManager::instance()->processName();
            success = false;
        }

        if (ConfigManager::instance()->dungeonName() != "测试副本") {
            qDebug() << "  ❌ dungeonName 加载不正确:" << ConfigManager::instance()->dungeonName();
            success = false;
        }

        if (success) {
            qDebug() << "  ✅ 配置文件加载测试通过";
        }
        return success;
    }

    bool testConfigSaving() {
        qDebug() << "[测试 6.2] 验证配置文件正确保存...";

        // 修改配置值
        ConfigManager::instance()->setRestartInterval(1800);
        ConfigManager::instance()->setSandboxiePath("E:\\Tools\\Sandboxie\\Start.exe");
        ConfigManager::instance()->setProcessName("game_client.exe");
        ConfigManager::instance()->setDungeonName("新副本");

        // 保存配置
        bool saveResult = ConfigManager::instance()->saveConfig();

        if (!saveResult) {
            qDebug() << "  ❌ 配置保存失败";
            return false;
        }

        // 重新加载配置
        bool loadResult = ConfigManager::instance()->loadConfig();

        if (!loadResult) {
            qDebug() << "  ❌ 重新加载配置失败";
            return false;
        }

        // 验证保存的值
        bool success = true;

        if (ConfigManager::instance()->restartInterval() != 1800) {
            qDebug() << "  ❌ restartInterval 保存不正确";
            success = false;
        }

        if (ConfigManager::instance()->sandboxiePath() != "E:\\Tools\\Sandboxie\\Start.exe") {
            qDebug() << "  ❌ sandboxiePath 保存不正确";
            success = false;
        }

        if (ConfigManager::instance()->processName() != "game_client.exe") {
            qDebug() << "  ❌ processName 保存不正确";
            success = false;
        }

        if (ConfigManager::instance()->dungeonName() != "新副本") {
            qDebug() << "  ❌ dungeonName 保存不正确";
            success = false;
        }

        if (success) {
            qDebug() << "  ✅ 配置文件保存测试通过";
        }
        return success;
    }

    bool testHumanReadableFormat() {
        qDebug() << "[测试 6.4] 验证配置格式人类可读...";

        QString configPath = ConfigManager::instance()->configPath();
        QFile file(configPath);

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "  ❌ 无法读取配置文件";
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        QString content = QString::fromUtf8(data);

        // 检查是否有缩进（人类可读的标志）
        bool hasIndentation = content.contains("  \"");

        // 检查是否有换行
        bool hasNewlines = content.contains('\n');

        // 检查是否包含所有必需的节
        bool hasAccounts = content.contains("\"accounts\"");
        bool hasScript = content.contains("\"script\"");
        bool hasTimer = content.contains("\"timer\"");
        bool hasMonitor = content.contains("\"monitor\"");
        bool hasLogging = content.contains("\"logging\"");

        bool success = hasIndentation && hasNewlines && hasAccounts && 
                      hasScript && hasTimer && hasMonitor && hasLogging;

        if (!hasIndentation) {
            qDebug() << "  ❌ 配置文件缺少缩进格式";
        }
        if (!hasNewlines) {
            qDebug() << "  ❌ 配置文件缺少换行";
        }
        if (!hasAccounts || !hasScript || !hasTimer || !hasMonitor || !hasLogging) {
            qDebug() << "  ❌ 配置文件缺少必需的配置节";
        }

        if (success) {
            qDebug() << "  ✅ 配置格式人类可读测试通过";
        }
        return success;
    }

    bool testConfigValueAccess() {
        qDebug() << "[测试] 验证配置值访问接口...";

        bool success = true;

        // 测试定时器配置
        ConfigManager::instance()->setRestartInterval(3600);
        if (ConfigManager::instance()->restartInterval() != 3600) {
            qDebug() << "  ❌ restartInterval 访问失败";
            success = false;
        }

        ConfigManager::instance()->setRandomOffset(500);
        if (ConfigManager::instance()->randomOffset() != 500) {
            qDebug() << "  ❌ randomOffset 访问失败";
            success = false;
        }

        ConfigManager::instance()->setTimerEnabled(false);
        if (ConfigManager::instance()->timerEnabled() != false) {
            qDebug() << "  ❌ timerEnabled 访问失败";
            success = false;
        }

        // 测试脚本配置
        ConfigManager::instance()->setMaxRuns(100);
        if (ConfigManager::instance()->maxRuns() != 100) {
            qDebug() << "  ❌ maxRuns 访问失败";
            success = false;
        }

        ConfigManager::instance()->setRetryCount(5);
        if (ConfigManager::instance()->retryCount() != 5) {
            qDebug() << "  ❌ retryCount 访问失败";
            success = false;
        }

        // 测试监控配置
        ConfigManager::instance()->setCheckInterval(3000);
        if (ConfigManager::instance()->checkInterval() != 3000) {
            qDebug() << "  ❌ checkInterval 访问失败";
            success = false;
        }

        ConfigManager::instance()->setCrashRecoveryEnabled(false);
        if (ConfigManager::instance()->crashRecoveryEnabled() != false) {
            qDebug() << "  ❌ crashRecoveryEnabled 访问失败";
            success = false;
        }

        // 测试日志配置
        ConfigManager::instance()->setLogLevel("DEBUG");
        if (ConfigManager::instance()->logLevel() != "DEBUG") {
            qDebug() << "  ❌ logLevel 访问失败";
            success = false;
        }

        if (success) {
            qDebug() << "  ✅ 配置值访问接口测试通过";
        }
        return success;
    }

    bool testAccountOperations() {
        qDebug() << "[测试] 验证账号操作...";

        bool success = true;

        // 添加账号
        QJsonObject account1{
            {"id", 1},
            {"username", "test_user_1"},
            {"password", "test_pass_1"},
            {"sandboxName", "Sandbox1"},
            {"enabled", true}
        };

        if (!ConfigManager::instance()->addAccount(account1)) {
            qDebug() << "  ❌ 添加账号失败";
            success = false;
        }

        // 验证账号已添加
        QJsonArray accounts = ConfigManager::instance()->accounts();
        if (accounts.size() != 1) {
            qDebug() << "  ❌ 账号数量不正确:" << accounts.size();
            success = false;
        }

        // 更新账号
        QJsonObject updatedAccount{
            {"id", 1},
            {"username", "updated_user"},
            {"password", "updated_pass"},
            {"sandboxName", "Sandbox2"},
            {"enabled", false}
        };

        if (!ConfigManager::instance()->updateAccount(0, updatedAccount)) {
            qDebug() << "  ❌ 更新账号失败";
            success = false;
        }

        // 验证更新
        accounts = ConfigManager::instance()->accounts();
        QJsonObject savedAccount = accounts[0].toObject();
        if (savedAccount["username"].toString() != "updated_user") {
            qDebug() << "  ❌ 账号更新未生效";
            success = false;
        }

        // 删除账号
        if (!ConfigManager::instance()->removeAccount(0)) {
            qDebug() << "  ❌ 删除账号失败";
            success = false;
        }

        // 验证删除
        accounts = ConfigManager::instance()->accounts();
        if (accounts.size() != 0) {
            qDebug() << "  ❌ 账号删除未生效";
            success = false;
        }

        if (success) {
            qDebug() << "  ✅ 账号操作测试通过";
        }
        return success;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("RedDemonScriptTest");
    app.setOrganizationName("RedDemon");

    // 初始化日志管理器
    LogManager::instance();

    // 初始化配置管理器
    ConfigManager::instance();

    // 运行测试
    ConfigManagerTest tester;
    QTimer::singleShot(0, &tester, &ConfigManagerTest::runTests);

    return app.exec();
}

#include "test_config_manager.moc"
