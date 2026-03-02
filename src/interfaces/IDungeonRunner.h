#ifndef IDUNGEONRUNNER_H
#define IDUNGEONRUNNER_H

#include <QString>

/**
 * @brief 副本运行接口
 *
 * 用户需实现此接口以定义副本内的战斗逻辑。
 * 包括：清怪、拾取、BOSS 战斗等。
 *
 * 实现示例：
 * @code
 * class MyDungeonRunner : public QObject, public IDungeonRunner
 * {
 *     Q_OBJECT
 *     Q_INTERFACES(IDungeonRunner)
 * public:
 *     bool runDungeon(QString& errorMessage) override;
 * };
 * @endcode
 */
class IDungeonRunner
{
public:
    virtual ~IDungeonRunner() = default;

    /**
     * @brief 运行副本战斗流程
     *
     * @param errorMessage 错误信息输出参数，当返回 false 时填充详细的错误描述
     * @return true 副本完成（成功通关）
     * @return false 副本失败（角色死亡/超时等）
     *
     * @note 实现者应在此方法中完成：
     *       1. 清理副本内所有怪物
     *       2. 拾取掉落物品
     *       3. 击败 BOSS（如有）
     *       4. 返回结果
     *
     * @note 此方法应阻塞直到副本完成或失败
     *       框架会在返回后调用 enterDungeon() 开始下一轮
     * @note 失败时应在 errorMessage 中填写详细的错误原因
     */
    virtual bool runDungeon(QString& errorMessage) = 0;
};

Q_DECLARE_INTERFACE(IDungeonRunner, "com.reddemonscript.IDungeonRunner/1.0")

#endif // IDUNGEONRUNNER_H
