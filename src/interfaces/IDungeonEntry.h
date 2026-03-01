#ifndef IDUNGEONENTRY_H
#define IDUNGEONENTRY_H

/**
 * @brief 副本入口接口，用户需实现此接口来定义进入副本的逻辑
 */
class IDungeonEntry
{
public:
    virtual ~IDungeonEntry() = default;
    
    /**
     * @brief 进入副本
     * @return 进入是否成功
     */
    virtual bool enterDungeon() = 0;
};

#endif // IDUNGEONENTRY_H