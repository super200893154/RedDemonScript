#include "TimerThread.h"
#include <QMutexLocker>
#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

TimerThread::TimerThread(QObject *parent)
    : QThread(parent)
    , m_shouldStop(false)
    , m_restartInterval(1800)    // 默认30分钟
    , m_randomOffset(300)        // 默认5分钟随机偏移
    , m_timer(new QTimer(this))
{
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        emit timeout();
        // 重新计算下一次定时重启
        qint64 nextInterval = calculateActualInterval() * 1000; // 转换为毫秒
        m_timer->start(nextInterval);
    });
}

TimerThread::~TimerThread()
{
    if (isRunning()) {
        stop();
        wait(); // 等待线程结束
    }
}

void TimerThread::setRestartInterval(qint64 intervalSeconds)
{
    QMutexLocker locker(&m_mutex);
    m_restartInterval = intervalSeconds;
}

void TimerThread::setRandomOffset(qint64 offsetSeconds)
{
    QMutexLocker locker(&m_mutex);
    m_randomOffset = offsetSeconds;
}

void TimerThread::start()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = false;
    locker.unlock();
    
    // 计算首次定时重启时间
    qint64 firstInterval = calculateActualInterval() * 1000; // 转换为毫秒
    
    m_timer->start(firstInterval);
    
    if (!isRunning()) {
        QThread::start(); // 启动线程事件循环
    }
    
    emit started();
}

void TimerThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = true;
    m_timer->stop();
    m_waitCondition.wakeAll();
}

void TimerThread::run()
{
    // 线程主循环 - 由于使用 QTimer，这里只需要保持线程运行
    exec(); // 启动事件循环
    
    emit finished();
}

qint64 TimerThread::calculateActualInterval() const
{
    if (m_randomOffset <= 0) {
        return m_restartInterval;
    }
    
    // 生成 [0, m_randomOffset*2] 范围内的随机数，然后减去 m_randomOffset 得到 [-m_randomOffset, m_randomOffset]
    qint64 randomValue = QRandomGenerator::global()->bounded(0, m_randomOffset * 2) - m_randomOffset;
    qint64 actualInterval = m_restartInterval + randomValue;
    
    // 确保间隔为正数
    return qMax(actualInterval, 1LL);
}