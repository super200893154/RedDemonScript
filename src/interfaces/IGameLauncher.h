#ifndef IGAMELAUNCHER_H
#define IGAMELAUNCHER_H

#include <QString>

/**
 * @brief 游戏启动接口，用户需实现此接口来定义游戏启动逻辑
 */
class IGameLauncher
{
public:
    virtual ~IGameLauncher() = default;
    
    /**
     * @brief 启动游戏
     * @param account 账号信息
     * @return 启动是否成功
     */
    virtual bool launch(const QString& account) = 0;
};

#endif // IGAMELAUNCHER_H