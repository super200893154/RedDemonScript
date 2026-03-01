#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/**
 * @brief 监控线程，负责检测游戏进程状态
 */
class MonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit MonitorThread(QObject *parent = nullptr);
    ~MonitorThread();

    void setProcessName(const QString &processName);
    void setCheckInterval(int intervalMs);

signals:
    void processCrashed();
    void processNotResponding();
    void statusChecked();

public slots:
    void start();
    void stop();

protected:
    void run() override;

private:
    bool m_shouldStop;
    QString m_processName;
    int m_checkInterval;
    
    QMutex m_mutex;
    QWaitCondition m_waitCondition;

    // 监控逻辑
    void checkProcessStatus();
    bool isProcessRunning() const;
    bool isProcessResponding() const;
};

#endif // MONITORTHREAD_H