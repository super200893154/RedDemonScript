#ifndef SCRIPTTHREAD_H
#define SCRIPTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "interfaces/IGameLauncher.h"
#include "interfaces/IDungeonEntry.h"
#include "interfaces/IDungeonRunner.h"

/**
 * @brief 脚本执行线程，负责执行游戏自动化脚本
 */
class ScriptThread : public QThread
{
    Q_OBJECT

public:
    explicit ScriptThread(QObject *parent = nullptr);
    ~ScriptThread();

    void setGameLauncher(IGameLauncher *launcher);
    void setDungeonEntry(IDungeonEntry *entry);
    void setDungeonRunner(IDungeonRunner *runner);
    void setAccount(const QString &account);

signals:
    void started();
    void finished();
    void error(const QString &message);
    void logMessage(const QString &message);

public slots:
    void start();
    void stop();

protected:
    void run() override;

private:
    bool m_shouldStop;
    QString m_account;
    
    IGameLauncher *m_gameLauncher;
    IDungeonEntry *m_dungeonEntry;
    IDungeonRunner *m_dungeonRunner;

    QMutex m_mutex;
    QWaitCondition m_waitCondition;

    // 核心执行逻辑
    bool executeScriptLoop();
    bool launchGame();
    bool enterDungeon();
    bool runDungeon();
};

#endif // SCRIPTTHREAD_H