#include "ScriptController.h"
#include "ScriptThread.h"
#include "MonitorThread.h"
#include "TimerThread.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>

ScriptController::ScriptController(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_runTime(0)
    , m_status("已停止")
    , m_scriptThread(nullptr)
    , m_monitorThread(nullptr)
    , m_timerThread(nullptr)
    , m_runTimer(new QTimer(this))
    , m_elapsedTimer()
{
    // 初始化线程对象
    m_scriptThread = new ScriptThread(this);
    m_monitorThread = new MonitorThread(this);
    m_timerThread = new TimerThread(this);

    // 连接信号槽
    connect(m_scriptThread, &ScriptThread::started, this, &ScriptController::onScriptStarted);
    connect(m_scriptThread, &ScriptThread::finished, this, &ScriptController::onScriptStopped);
    connect(m_scriptThread, &ScriptThread::error, this, &ScriptController::onScriptError);
    connect(m_scriptThread, &ScriptThread::logMessage, this, &ScriptController::logMessage);
    
    connect(m_monitorThread, &MonitorThread::processCrashed, this, &ScriptController::onProcessCrashed);
    connect(m_monitorThread, &MonitorThread::processNotResponding, this, &ScriptController::onProcessNotResponding);
    
    connect(m_timerThread, &TimerThread::timeout, this, &ScriptController::onTimerTimeout);

    // 设置运行计时器
    connect(m_runTimer, &QTimer::timeout, this, [this]() {
        m_runTime = m_elapsedTimer.elapsed() / 1000; // 转换为秒
        emit runTimeChanged();
    });
    m_runTimer->setInterval(1000); // 每秒更新一次
}

ScriptController::~ScriptController()
{
    if (m_isRunning) {
        stopScript();
    }
}

bool ScriptController::isRunning() const
{
    return m_isRunning;
}

QString ScriptController::currentAccount() const
{
    return m_currentAccount;
}

qint64 ScriptController::runTime() const
{
    return m_runTime;
}

QString ScriptController::status() const
{
    return m_status;
}

void ScriptController::startScript()
{
    if (m_isRunning) {
        return; // 已经在运行
    }

    // 设置运行状态
    m_isRunning = true;
    emit isRunningChanged();
    
    updateStatus("正在启动脚本...");
    
    // 启动各线程
    m_scriptThread->start();
    m_monitorThread->start();
    m_timerThread->start();
    
    // 开始计时
    m_elapsedTimer.start();
    m_runTimer->start();
    
    updateStatus("脚本运行中");
}

void ScriptController::stopScript()
{
    if (!m_isRunning) {
        return; // 未在运行
    }

    // 停止各线程
    if (m_scriptThread) {
        m_scriptThread->stop();
    }
    if (m_monitorThread) {
        m_monitorThread->stop();
    }
    if (m_timerThread) {
        m_timerThread->stop();
    }
    
    // 停止计时
    m_runTimer->stop();
    
    m_isRunning = false;
    emit isRunningChanged();
    
    updateStatus("脚本已停止");
}

void ScriptController::onScriptStarted()
{
    updateStatus("脚本已启动");
}

void ScriptController::onScriptStopped()
{
    m_isRunning = false;
    emit isRunningChanged();
    updateStatus("脚本已停止");
    
    // 停止计时
    m_runTimer->stop();
}

void ScriptController::onScriptError(const QString &error)
{
    updateStatus("脚本错误: " + error);
    // 在实际应用中，这里可能会自动停止脚本或尝试恢复
}

void ScriptController::onProcessCrashed()
{
    updateStatus("检测到进程崩溃，正在尝试恢复...");
    // 在实际应用中，这里应该实现自动恢复逻辑
    emit logMessage("[ERROR] 游戏进程崩溃，正在尝试重启...");
}

void ScriptController::onProcessNotResponding()
{
    updateStatus("检测到进程无响应，正在处理...");
    // 在实际应用中，这里应该实现处理无响应进程的逻辑
    emit logMessage("[WARN] 游戏进程无响应，正在处理...");
}

void ScriptController::onTimerTimeout()
{
    updateStatus("定时重启触发");
    // 在实际应用中，这里应该实现定时重启逻辑
    emit logMessage("[INFO] 定时重启时间到达，准备重启...");
}

void ScriptController::updateStatus(const QString &newStatus)
{
    if (m_status != newStatus) {
        m_status = newStatus;
        emit statusChanged(newStatus);
    }
}