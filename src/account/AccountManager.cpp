#include "AccountManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#ifdef Q_OS_WIN
#include <windows.h>
#include <dpapi.h>
#endif

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
    , m_currentAccountId(-1)
{
}

AccountManager::~AccountManager()
{
}

bool AccountManager::addAccount(const AccountInfo &account)
{
    // 检查是否已存在相同用户名的账号
    for (const auto &existingAccount : m_accounts) {
        if (existingAccount.username == account.username) {
            return false; // 账号已存在
        }
    }
    
    // 设置新账号的ID（使用列表大小作为ID）
    AccountInfo newAccount = account;
    newAccount.id = m_accounts.size() > 0 ? m_accounts.last().id + 1 : 1;
    
    m_accounts.append(newAccount);
    emit accountListChanged();
    return true;
}

bool AccountManager::updateAccount(const AccountInfo &account)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].id == account.id) {
            m_accounts[i] = account;
            emit accountListChanged();
            return true;
        }
    }
    return false; // 未找到账号
}

bool AccountManager::removeAccount(int accountId)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].id == accountId) {
            m_accounts.removeAt(i);
            if (m_currentAccountId == accountId) {
                m_currentAccountId = -1; // 重置当前账号
            }
            emit accountListChanged();
            return true;
        }
    }
    return false; // 未找到账号
}

AccountInfo AccountManager::getAccount(int accountId) const
{
    for (const auto &account : m_accounts) {
        if (account.id == accountId) {
            return account;
        }
    }
    return AccountInfo(); // 返回空的账号信息
}

QList<AccountInfo> AccountManager::getAllAccounts() const
{
    // 返回启用的账号，并按轮换顺序排序
    QList<AccountInfo> result = m_accounts;
    std::sort(result.begin(), result.end(), [](const AccountInfo &a, const AccountInfo &b) {
        if (a.enabled != b.enabled) {
            return a.enabled; // 启用的账号排在前面
        }
        if (a.enabled && b.enabled) {
            return a.rotationOrder < b.rotationOrder; // 按轮换顺序排序
        }
        return false; // 不启用的账号排在最后
    });
    return result;
}

AccountInfo AccountManager::getNextAccount() const
{
    if (m_accounts.isEmpty()) {
        return AccountInfo(); // 返回空的账号信息
    }
    
    // 获取启用的账号列表，按轮换顺序排序
    auto enabledAccounts = getAllAccounts();
    enabledAccounts.erase(
        std::remove_if(enabledAccounts.begin(), enabledAccounts.end(),
            [](const AccountInfo &acc) { return !acc.enabled; }),
        enabledAccounts.end()
    );
    
    if (enabledAccounts.isEmpty()) {
        return AccountInfo(); // 没有启用的账号
    }
    
    // 找到当前账号的索引，返回下一个
    int currentIndex = -1;
    for (int i = 0; i < enabledAccounts.size(); ++i) {
        if (enabledAccounts[i].id == m_currentAccountId) {
            currentIndex = i;
            break;
        }
    }
    
    int nextIndex = (currentIndex + 1) % enabledAccounts.size(); // 循环轮换
    return enabledAccounts[nextIndex];
}

AccountInfo AccountManager::getCurrentAccount() const
{
    return getAccount(m_currentAccountId);
}

void AccountManager::setCurrentAccount(int accountId)
{
    if (m_currentAccountId != accountId) {
        m_currentAccountId = accountId;
        emit currentAccountChanged(accountId);
    }
}

bool AccountManager::loadFromConfig(const QString &configPath)
{
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    QJsonObject rootObj = doc.object();
    QJsonArray accountsArray = rootObj["accounts"].toArray();
    
    m_accounts.clear();
    
    for (const auto &accountValue : accountsArray) {
        QJsonObject accountObj = accountValue.toObject();
        
        AccountInfo account;
        account.id = accountObj["id"].toInt();
        account.username = accountObj["username"].toString();
        account.password = decryptPassword(accountObj["password"].toString()); // 解密密码
        account.sandboxName = accountObj["sandboxName"].toString();
        account.enabled = accountObj["enabled"].toBool();
        account.maxRunTime = accountObj["maxRunTime"].toInt(0); // 默认0（无限制）
        account.rotationOrder = accountObj["rotationOrder"].toInt(0); // 默认0
        
        m_accounts.append(account);
    }
    
    emit accountListChanged();
    return true;
}

bool AccountManager::saveToConfig(const QString &configPath) const
{
    QJsonObject rootObj;
    QJsonArray accountsArray;
    
    for (const auto &account : m_accounts) {
        QJsonObject accountObj;
        accountObj["id"] = account.id;
        accountObj["username"] = account.username;
        accountObj["password"] = encryptPassword(account.password); // 加密密码
        accountObj["sandboxName"] = account.sandboxName;
        accountObj["enabled"] = account.enabled;
        accountObj["maxRunTime"] = account.maxRunTime;
        accountObj["rotationOrder"] = account.rotationOrder;
        
        accountsArray.append(accountObj);
    }
    
    rootObj["accounts"] = accountsArray;
    
    QJsonDocument doc(rootObj);
    
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    configFile.write(doc.toJson());
    return true;
}

QString AccountManager::encryptPassword(const QString &password) const
{
#ifdef Q_OS_WIN
    // Windows 平台使用 DPAPI 加密
    DATA_BLOB dataIn, dataOut;
    QString result;
    
    QByteArray passwordBytes = password.toUtf8();
    dataIn.cbData = passwordBytes.length();
    dataIn.pbData = (BYTE*)passwordBytes.data();
    
    if (CryptProtectData(&dataIn, L"RedDemonScript Password", 
                         NULL, NULL, NULL, 0, &dataOut)) {
        // 将加密后的数据转换为 Base64 字符串
        QByteArray encryptedData((char*)dataOut.pbData, dataOut.cbData);
        result = encryptedData.toBase64();
        LocalFree(dataOut.pbData);
    }
    
    return result;
#else
    // 其他平台的简单加密（实际应用中应使用更安全的方法）
    // 这里仅作演示，实际应用应使用适当的加密算法
    QString encrypted = password;
    for (auto &ch : encrypted) {
        ch = QChar(ch.unicode() ^ 0x55); // 简单异或加密
    }
    return encrypted.toBase64();
#endif
}

QString AccountManager::decryptPassword(const QString &encryptedPassword) const
{
#ifdef Q_OS_WIN
    // Windows 平台使用 DPAPI 解密
    DATA_BLOB dataIn, dataOut;
    QString result;
    
    QByteArray encryptedBytes = QByteArray::fromBase64(encryptedPassword.toLocal8Bit());
    dataIn.cbData = encryptedBytes.length();
    dataIn.pbData = (BYTE*)encryptedBytes.data();
    
    if (CryptUnprotectData(&dataIn, NULL, NULL, NULL, NULL, 0, &dataOut)) {
        result = QString::fromUtf8((char*)dataOut.pbData, dataOut.cbData);
        LocalFree(dataOut.pbData);
    }
    
    return result;
#else
    // 其他平台的简单解密（与加密对应）
    QByteArray decryptedBytes = QByteArray::fromBase64(encryptedPassword.toLocal8Bit());
    QString decrypted = QString::fromLocal8Bit(decryptedBytes);
    for (auto &ch : decrypted) {
        ch = QChar(ch.unicode() ^ 0x55); // 简单异或解密
    }
    return decrypted;
#endif
}