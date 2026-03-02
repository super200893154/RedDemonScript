#ifndef IDUNGEONENTRY_H
#define IDUNGEONENTRY_H

#include <QString>

/**
 * @brief 副本入口接口
 *
 * 用户需实现此接口以定义进入副本的逻辑。
 * 包括：导航到副本入口、选择副本难度、确认进入等。
 *
 * 实现示例：
 * @code
 * class MyDungeonEntry : public QObject, public IDungeonEntry
 * {
 *     Q_OBJECT
 *     Q_INTERFACES(IDungeonEntry)
 * public:
 *     bool enterDungeon(QString& errorMessage) override;
 * };
 * @endcode
 */
class IDungeonEntry
{
public:
    virtual ~IDungeonEntry() = default;

    /**
     * @brief 进入副本
     *
     * @param errorMessage 错误信息输出参数，当返回 false 时填充详细的错误描述
     * @return true 成功进入副本
     * @return false 进入失败
     *
     * @note 实现者应在此方法中完成：
     *       1. 导航到副本入口 NPC/传送点
     *       2. 选择目标副本和难度
     *       3. 确认进入副本
     *       4. 等待副本加载完成
     * @note 失败时应在 errorMessage 中填写详细的错误原因
     */
    virtual bool enterDungeon(QString& errorMessage) = 0;
};

Q_DECLARE_INTERFACE(IDungeonEntry, "com.reddemonscript.IDungeonEntry/1.0")

#endif // IDUNGEONENTRY_H
