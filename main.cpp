#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "core/LogManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("RedDemonScript");
    app.setOrganizationName("RedDemon");
    
    // 初始化日志管理器单例
    LogManager::instance();
    
    QQmlApplicationEngine engine;
    
    // 注册 LogManager 单例到 QML
    engine.rootContext()->setContextProperty("LogManager", LogManager::instance());
    
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("RedDemonScript", "Main");

    return app.exec();
}