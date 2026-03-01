#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QString>

/**
 * @brief 账号信息数据结构
 */
struct AccountInfo
{
    int id;
    QString username;
    QString password;  // 加密存储
    QString sandboxName;
    qint64 maxRunTime;  // 最大运行时间（分钟）
    int rotationOrder;  // 轮换顺序
    bool enabled;       // 是否启用
    QString lastUsed;   // 最后使用时间
    
    AccountInfo() : id(0), maxRunTime(0), rotationOrder(0), enabled(true) {}
};

#endif // ACCOUNTINFO_H