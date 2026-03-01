#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QList>
#include "AccountInfo.h"

/**
 * @brief 账号管理器，负责管理多个游戏账号
 */
class AccountManager : public QObject
{
    Q_OBJECT

public:
    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager();

    // CRUD 操作
    bool addAccount(const AccountInfo &account);
    bool updateAccount(const AccountInfo &account);
    bool removeAccount(int accountId);
    AccountInfo getAccount(int accountId) const;
    QList<AccountInfo> getAllAccounts() const;

    // 账号轮换
    AccountInfo getNextAccount() const;
    AccountInfo getCurrentAccount() const;
    void setCurrentAccount(int accountId);

    // 配置管理
    bool loadFromConfig(const QString &configPath);
    bool saveToConfig(const QString &configPath) const;

signals:
    void accountListChanged();
    void currentAccountChanged(int accountId);

private:
    QList<AccountInfo> m_accounts;
    int m_currentAccountId;

    // 辅助方法
    QString encryptPassword(const QString &password) const;
    QString decryptPassword(const QString &encryptedPassword) const;
};

#endif // ACCOUNTMANAGER_H