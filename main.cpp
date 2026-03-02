#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QIcon>
#include <QDebug>
#include <QWindow>

#include "core/LogManager.h"
#include "core/ConfigManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("RedDemonScript");
    app.setOrganizationName("RedDemon");

    // 初始化日志管理器单例
    LogManager::instance();

    // 初始化配置管理器并加载配置
    ConfigManager::instance();
    ConfigManager::instance()->loadConfig();

    // 执行日志清理（Story 1.3）
    LogManager::instance()->cleanupOldLogs();

    QQmlApplicationEngine engine;

    // 注册 LogManager 单例到 QML
    engine.rootContext()->setContextProperty("LogManager", LogManager::instance());

    // 注册 ConfigManager 单例到 QML
    engine.rootContext()->setContextProperty("ConfigManager", ConfigManager::instance());

    // 连接警告信号以便看到 QML 警告
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                    [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            qWarning() << "[QML Warning]:" << warning.toString();
        }
    });

    // 连接对象创建失败信号
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qCritical() << "[Main] QML object creation failed!";
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // 加载 QML 模块
    engine.loadFromModule("RedDemonScript", "Main");

    // 检查是否有根对象
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[Main] No root objects! QML may not have loaded.";
        return -1;
    }

    return app.exec();
}
