#ifndef IDUNGEONRUNNER_H
#define IDUNGEONRUNNER_H

/**
 * @brief 副本运行接口，用户需实现此接口来定义副本战斗逻辑
 */
class IDungeonRunner
{
public:
    virtual ~IDungeonRunner() = default;
    
    /**
     * @brief 运行副本（战斗逻辑）
     * @return 运行是否成功
     */
    virtual bool runDungeon() = 0;
};

#endif // IDUNGEONRUNNER_H