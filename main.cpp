#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QIcon>
#include <QDebug>
#include <QWindow>

#include "core/LogManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("RedDemonScript");
    app.setOrganizationName("RedDemon");

    qDebug() << "[Main] Application created";

    // 初始化日志管理器单例
    LogManager::instance();
    qDebug() << "[Main] LogManager initialized";

    // 执行日志清理（Story 1.3）
    LogManager::instance()->cleanupOldLogs();
    qDebug() << "[Main] Log cleanup executed";

    QQmlApplicationEngine engine;
    qDebug() << "[Main] QQmlApplicationEngine created";

    // 注册 LogManager 单例到 QML
    engine.rootContext()->setContextProperty("LogManager", LogManager::instance());
    qDebug() << "[Main] LogManager registered to QML context";

    // 连接警告信号以便看到 QML 警告
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                    [](const QList<QQmlError> &warnings) {
        qWarning() << "[QML] Warnings count:" << warnings.size();
        for (const QQmlError &warning : warnings) {
            qWarning() << "[QML] Warning:" << warning.toString();
        }
    });

    // 连接对象创建失败信号
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qCritical() << "[Main] ERROR: QML object creation failed!";
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // 加载 QML 模块
    qDebug() << "[Main] Loading QML module 'RedDemonScript'...";
    engine.loadFromModule("RedDemonScript", "Main");
    qDebug() << "[Main] loadFromModule completed";

    // 检查是否有根对象
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[Main] ERROR: No root objects! QML may not have loaded.";
        qCritical() << "[Main] Trying to continue anyway...";
    } else {
        qDebug() << "[Main] Root objects count:" << engine.rootObjects().size();

        // 检查根对象类型
        QObject *rootObj = engine.rootObjects().first();
        qDebug() << "[Main] Root object class:" << rootObj->metaObject()->className();

        // 如果是 QWindow，确保它可见
        if (QWindow *window = qobject_cast<QWindow*>(rootObj)) {
            qDebug() << "[Main] Root is QWindow, showing...";
            window->show();
            window->raise();
            window->requestActivate();
            qDebug() << "[Main] Window show requested";
        }
    }

    qDebug() << "[Main] Entering event loop...";
    return app.exec();
}