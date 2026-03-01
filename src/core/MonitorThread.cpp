#include "MonitorThread.h"
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#endif

MonitorThread::MonitorThread(QObject *parent)
    : QThread(parent)
    , m_shouldStop(false)
    , m_processName("game.exe")  // 默认进程名
    , m_checkInterval(5000)      // 默认5秒检查一次
{
}

MonitorThread::~MonitorThread()
{
    if (isRunning()) {
        stop();
        wait(); // 等待线程结束
    }
}

void MonitorThread::setProcessName(const QString &processName)
{
    QMutexLocker locker(&m_mutex);
    m_processName = processName;
}

void MonitorThread::setCheckInterval(int intervalMs)
{
    QMutexLocker locker(&m_mutex);
    m_checkInterval = intervalMs;
}

void MonitorThread::start()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = false;
    locker.unlock();
    
    if (!isRunning()) {
        QThread::start(); // 调用 QThread::start() 启动 run() 方法
    }
}

void MonitorThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = true;
    m_waitCondition.wakeAll();
}

void MonitorThread::run()
{
    emit statusChecked(); // 发出初始状态检查信号
    
    while (true) {
        {
            QMutexLocker locker(&m_mutex);
            if (m_shouldStop) {
                break;
            }
        }
        
        checkProcessStatus();
        
        // 等待下次检查
        QMutexLocker locker(&m_mutex);
        if (m_waitCondition.wait(&m_mutex, m_checkInterval)) {
            // 如果被提前唤醒（例如停止信号），则退出循环
            break;
        }
    }
    
    emit statusChecked();
}

void MonitorThread::checkProcessStatus()
{
    if (!isProcessRunning()) {
        emit processCrashed();
        return;
    }
    
    if (!isProcessResponding()) {
        emit processNotResponding();
        return;
    }
    
    emit statusChecked();
}

bool MonitorThread::isProcessRunning() const
{
#ifdef Q_OS_WIN
    // Windows 平台实现
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry)) {
        do {
            if (QString::fromWCharArray(entry.szExeFile) == m_processName) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return false;
#else
    // 其他平台的实现（简化版）
    // 这里应该使用适当的进程检测方法
    Q_UNUSED(m_processName);
    return true; // 简化实现，总是返回 true
#endif
}

bool MonitorThread::isProcessResponding() const
{
#ifdef Q_OS_WIN
    // Windows 平台实现：检查窗口响应
    HWND hwnd = FindWindow(NULL, NULL);  // 需要更具体的窗口查找逻辑
    Q_UNUSED(hwnd);
    // 实现窗口响应检测
    return true; // 简化实现
#else
    // 其他平台的实现
    return true; // 简化实现
#endif
}