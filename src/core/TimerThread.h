#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>

/**
 * @brief 定时器线程，负责管理定时重启
 */
class TimerThread : public QThread
{
    Q_OBJECT

public:
    explicit TimerThread(QObject *parent = nullptr);
    ~TimerThread();

    void setRestartInterval(qint64 intervalSeconds);
    void setRandomOffset(qint64 offsetSeconds);

signals:
    void timeout();
    void started();
    void finished();

public slots:
    void start();
    void stop();

protected:
    void run() override;

private:
    bool m_shouldStop;
    qint64 m_restartInterval;  // 基础间隔（秒）
    qint64 m_randomOffset;     // 随机偏移（秒）
    
    QTimer *m_timer;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;

    qint64 calculateActualInterval() const;
};

#endif // TIMERTHREAD_H