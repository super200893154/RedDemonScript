#ifndef IGAMELAUNCHER_H
#define IGAMELAUNCHER_H

#include <QString>

/**
 * @brief 游戏启动接口
 *
 * 用户需实现此接口以定义特定游戏的启动逻辑。
 * 包括：启动游戏客户端、选择服务器、输入账号密码登录等。
 *
 * 实现示例：
 * @code
 * class MyGameLauncher : public QObject, public IGameLauncher
 * {
 *     Q_OBJECT
 *     Q_INTERFACES(IGameLauncher)
 * public:
 *     bool launch(const QString& account, QString& errorMessage) override;
 * };
 * @endcode
 */
class IGameLauncher
{
public:
    virtual ~IGameLauncher() = default;

    /**
     * @brief 启动游戏并登录指定账号
     *
     * @param account 账号名称（用于从配置中获取账号密码）
     * @param errorMessage 错误信息输出参数，当返回 false 时填充详细的错误描述
     * @return true 启动成功
     * @return false 启动失败
     *
     * @note 实现者应在此方法中完成：
     *       1. 启动游戏进程（可通过 Sandboxie）
     *       2. 等待游戏窗口出现
     *       3. 输入账号密码登录
     *       4. 等待进入游戏主界面
     * @note 失败时应在 errorMessage 中填写详细的错误原因
     */
    virtual bool launch(const QString& account, QString& errorMessage) = 0;
};

Q_DECLARE_INTERFACE(IGameLauncher, "com.reddemonscript.IGameLauncher/1.0")

#endif // IGAMELAUNCHER_H
