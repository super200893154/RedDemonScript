#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include <QElapsedTimer>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "interfaces/IGameLauncher.h"
#include "interfaces/IDungeonEntry.h"
#include "interfaces/IDungeonRunner.h"

// 前向声明
class ScriptThread;
class MonitorThread;
class TimerThread;

/**
 * @brief 脚本主控制器，管理整个脚本执行流程
 */
class ScriptController : public QObject
{
    Q_OBJECT

    // QML 属性声明
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(QString currentAccount READ currentAccount NOTIFY currentAccountChanged)
    Q_PROPERTY(qint64 runTime READ runTime NOTIFY runTimeChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit ScriptController(QObject *parent = nullptr);
    ~ScriptController();

    // QML 属性访问器
    bool isRunning() const;
    QString currentAccount() const;
    qint64 runTime() const;
    QString status() const;

    // 公共接口
    Q_INVOKABLE void startScript();
    Q_INVOKABLE void stopScript();

signals:
    void isRunningChanged();
    void currentAccountChanged();
    void runTimeChanged();
    void statusChanged(const QString &newStatus);
    void logMessage(const QString &message);

private slots:
    void onScriptStarted();
    void onScriptStopped();
    void onScriptError(const QString &error);
    void onProcessCrashed();
    void onProcessNotResponding();
    void onTimerTimeout();

private:
    bool m_isRunning;
    QString m_currentAccount;
    qint64 m_runTime;
    QString m_status;
    
    // 线程对象
    ScriptThread *m_scriptThread;
    MonitorThread *m_monitorThread;
    TimerThread *m_timerThread;
    
    // 计时器
    QTimer *m_runTimer;
    QElapsedTimer m_elapsedTimer;

    // 工作方法
    void updateStatus(const QString &newStatus);
};

#endif // SCRIPTCONTROLLER_H