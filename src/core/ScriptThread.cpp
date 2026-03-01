#include "ScriptThread.h"
#include <QMutexLocker>
#include <QDebug>
#include "helpers/InputHelper.h"

ScriptThread::ScriptThread(QObject *parent)
    : QThread(parent)
    , m_shouldStop(false)
    , m_gameLauncher(nullptr)
    , m_dungeonEntry(nullptr)
    , m_dungeonRunner(nullptr)
{
}

ScriptThread::~ScriptThread()
{
    if (isRunning()) {
        stop();
        wait(); // 等待线程结束
    }
}

void ScriptThread::setGameLauncher(IGameLauncher *launcher)
{
    QMutexLocker locker(&m_mutex);
    m_gameLauncher = launcher;
}

void ScriptThread::setDungeonEntry(IDungeonEntry *entry)
{
    QMutexLocker locker(&m_mutex);
    m_dungeonEntry = entry;
}

void ScriptThread::setDungeonRunner(IDungeonRunner *runner)
{
    QMutexLocker locker(&m_mutex);
    m_dungeonRunner = runner;
}

void ScriptThread::setAccount(const QString &account)
{
    QMutexLocker locker(&m_mutex);
    m_account = account;
}

void ScriptThread::start()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = false;
    locker.unlock();
    
    if (!isRunning()) {
        QThread::start(); // 调用 QThread::start() 启动 run() 方法
    }
    
    emit started();
}

void ScriptThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = true;
    m_waitCondition.wakeAll();
}

void ScriptThread::run()
{
    emit logMessage("[INFO] 脚本线程开始运行");
    
    while (true) {
        {
            QMutexLocker locker(&m_mutex);
            if (m_shouldStop) {
                break;
            }
        }
        
        if (!executeScriptLoop()) {
            emit error("脚本执行循环失败");
            break;
        }
        
        // 短暂休眠，避免过度占用 CPU
        InputHelper::wait(1000);
    }
    
    emit logMessage("[INFO] 脚本线程停止运行");
    emit finished();
}

bool ScriptThread::executeScriptLoop()
{
    // 执行游戏启动
    if (!launchGame()) {
        emit error("游戏启动失败");
        return false;
    }
    
    // 短暂等待游戏加载
    InputHelper::wait(5000);
    
    // 进入副本
    if (!enterDungeon()) {
        emit error("进入副本失败");
        return false;
    }
    
    // 运行副本（战斗）
    if (!runDungeon()) {
        emit error("运行副本失败");
        return false;
    }
    
    emit logMessage("[INFO] 完成一次副本循环");
    return true;
}

bool ScriptThread::launchGame()
{
    QMutexLocker locker(&m_mutex);
    if (!m_gameLauncher) {
        emit error("游戏启动器未设置");
        return false;
    }
    
    emit logMessage("[INFO] 正在启动游戏，账号: " + m_account);
    bool result = m_gameLauncher->launch(m_account);
    return result;
}

bool ScriptThread::enterDungeon()
{
    QMutexLocker locker(&m_mutex);
    if (!m_dungeonEntry) {
        emit error("副本入口未设置");
        return false;
    }
    
    emit logMessage("[INFO] 正在进入副本");
    bool result = m_dungeonEntry->enterDungeon();
    return result;
}

bool ScriptThread::runDungeon()
{
    QMutexLocker locker(&m_mutex);
    if (!m_dungeonRunner) {
        emit error("副本运行器未设置");
        return false;
    }
    
    emit logMessage("[INFO] 正在运行副本（战斗中）");
    bool result = m_dungeonRunner->runDungeon();
    return result;
}