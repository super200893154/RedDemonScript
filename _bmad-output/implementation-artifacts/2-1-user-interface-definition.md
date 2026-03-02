# Story 2.1: 用户接口定义

Status: ready-for-dev

<!-- 注意：验证是可选的。在 dev-story 之前运行 validate-create-story 进行质量检查。 -->

## Story

作为一个 **开发者（用户）**，
我想要 **定义游戏启动、副本入口、副本战斗三个接口**，
以便 **我可以实现自己的游戏逻辑**。

## 验收标准

1. **假设** 项目结构已搭建完成（Story 1.1 完成）
2. **当** 开发者查看 `src/interfaces/` 目录
3. **那么** 存在 `IGameLauncher.h`、`IDungeonEntry.h`、`IDungeonRunner.h` 三个接口文件
4. **并且** 每个接口定义纯虚函数供用户实现
5. **并且** 接口命名遵循 I 前缀约定
6. **并且** 接口包含必要的 Qt 元对象支持（Q_OBJECT 宏）

**覆盖的 FR：** FR1, FR2, FR3

**架构来源：** architecture.md#项目结构与边界 - src/interfaces/ 用户接口目录

## 任务 / 子任务

- [ ] 任务 1：创建 IGameLauncher 游戏启动接口（AC：#3, #4, #5, #6）
  - [ ] 1.1 创建 `src/interfaces/IGameLauncher.h` 文件
  - [ ] 1.2 定义纯虚函数 `virtual bool launch(const QString& account) = 0;`
  - [ ] 1.3 添加必要的注释说明接口用途

- [ ] 任务 2：创建 IDungeonEntry 副本入口接口（AC：#3, #4, #5, #6）
  - [ ] 2.1 创建 `src/interfaces/IDungeonEntry.h` 文件
  - [ ] 2.2 定义纯虚函数 `virtual bool enterDungeon() = 0;`
  - [ ] 2.3 添加必要的注释说明接口用途

- [ ] 任务 3：创建 IDungeonRunner 副本运行接口（AC：#3, #4, #5, #6）
  - [ ] 3.1 创建 `src/interfaces/IDungeonRunner.h` 文件
  - [ ] 3.2 定义纯虚函数 `virtual bool runDungeon() = 0;`
  - [ ] 3.3 添加必要的注释说明接口用途

- [ ] 任务 4：更新 CMakeLists.txt（AC：#3）
  - [ ] 4.1 将新接口文件添加到项目构建

- [ ] 任务 5：验证构建（AC：全部）
  - [ ] 5.1 验证项目可以成功编译
  - [ ] 5.2 验证接口文件位于正确目录

## Dev Notes

### 核心需求分析

**架构决策来源：architecture.md#项目结构与边界**

```
src/interfaces/     # 用户接口（抽象类）
├── igamelauncher.h   # 游戏启动接口
├── idungeonentry.h   # 副本入口接口
└── idungeonrunner.h  # 副本运行接口
```

**来源：epics.md#Story 2.1 - 接口定义：**
- IGameLauncher: `virtual bool launch(const QString& account) = 0;`
- IDungeonEntry: `virtual bool enterDungeon() = 0;`
- IDungeonRunner: `virtual bool runDungeon() = 0;`

### 架构约束

**来源：architecture.md#架构边界**

| 边界 | 说明 | 通信方式 |
|------|------|----------|
| ScriptController ↔ 用户接口 | 主控制器调用用户实现 | 通过接口指针调用 |

**来源：project-context.md#关键实现规则**

- 接口驱动：IGameLauncher、IDungeonEntry、IDungeonRunner
- 用户实现具体游戏逻辑，框架提供稳定性保障

### 项目结构说明

**新增文件位置：**
- `src/interfaces/IGameLauncher.h`
- `src/interfaces/IDungeonEntry.h`
- `src/interfaces/IDungeonRunner.h`

**注意：** 接口只需要头文件（.h），无需 .cpp 文件，因为它们是纯虚类。

**目录检查：** Story 1.1 应已创建 `src/interfaces/` 目录。

### 技术要点

#### 1. IGameLauncher 游戏启动接口

```cpp
// src/interfaces/IGameLauncher.h
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
 *     bool launch(const QString& account) override;
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
     * @return true 启动成功
     * @return false 启动失败
     * 
     * @note 实现者应在此方法中完成：
     *       1. 启动游戏进程（可通过 Sandboxie）
     *       2. 等待游戏窗口出现
     *       3. 输入账号密码登录
     *       4. 等待进入游戏主界面
     */
    virtual bool launch(const QString& account) = 0;
};

Q_DECLARE_INTERFACE(IGameLauncher, "com.reddemon.IGameLauncher/1.0")

#endif // IGAMELAUNCHER_H
```

#### 2. IDungeonEntry 副本入口接口

```cpp
// src/interfaces/IDungeonEntry.h
#ifndef IDUNGEONENTRY_H
#define IDUNGEONENTRY_H

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
 *     bool enterDungeon() override;
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
     * @return true 成功进入副本
     * @return false 进入失败
     * 
     * @note 实现者应在此方法中完成：
     *       1. 导航到副本入口 NPC/传送点
     *       2. 选择目标副本和难度
     *       3. 确认进入副本
     *       4. 等待副本加载完成
     */
    virtual bool enterDungeon() = 0;
};

Q_DECLARE_INTERFACE(IDungeonEntry, "com.reddemon.IDungeonEntry/1.0")

#endif // IDUNGEONENTRY_H
```

#### 3. IDungeonRunner 副本运行接口

```cpp
// src/interfaces/IDungeonRunner.h
#ifndef IDUNGEONRUNNER_H
#define IDUNGEONRUNNER_H

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
 *     bool runDungeon() override;
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
     */
    virtual bool runDungeon() = 0;
};

Q_DECLARE_INTERFACE(IDungeonRunner, "com.reddemon.IDungeonRunner/1.0")

#endif // IDUNGEONRUNNER_H
```

#### 4. CMakeLists.txt 更新

```cmake
# 在 qt_add_qml_module 中添加头文件
qt_add_qml_module(appRedDemonScript
    URI RedDemonScript
    # ... 现有配置 ...
    SOURCES
        # ... 现有源文件 ...
        src/interfaces/IGameLauncher.h
        src/interfaces/IDungeonEntry.h
        src/interfaces/IDungeonRunner.h
)
```

### 命名约定

**来源：project-context.md#代码质量与风格规则**

| 类型 | 约定 | 示例 |
|------|------|------|
| 接口名 | I 前缀 + PascalCase | `IGameLauncher`, `IDungeonEntry`, `IDungeonRunner` |
| 文件名 | 与类名相同（大小写一致） | `IGameLauncher.h` |
| 方法名 | camelCase | `launch()`, `enterDungeon()`, `runDungeon()` |

### 与前序 Story 的关系

**Story 1.1 已完成内容：**
- ✅ 项目目录结构创建（包括 `src/interfaces/` 目录）
- ✅ CMakeLists.txt 配置
- ✅ 项目可编译运行

**Story 1.2-1.5 已完成内容：**
- ✅ LogManager 日志系统
- ✅ ConfigManager 配置管理（账号信息存储）
- ✅ DpapiHelper 密码加密

**承接要点：**
- 用户实现的 launch() 方法可通过 ConfigManager 获取账号密码
- 接口实现可使用 LogManager 记录操作日志

### 与后续 Story 的关系

**Story 2.2 外部 DLL 封装：**
- 接口实现可调用 ImageHelper/InputHelper 进行图色识别和键鼠模拟

**Story 2.3 ScriptController 主控制器：**
- ScriptController 将持有用户实现的接口指针
- ScriptThread 调用接口方法执行刷图流程

**Story 2.4 ScriptThread 脚本执行线程：**
- ScriptThread 循环调用：launch() → enterDungeon() → runDungeon() → 重复

### 关键设计决策

1. **纯虚接口 vs 抽象基类：**
   - 选择纯虚接口（无默认实现）
   - 理由：强制用户实现所有逻辑，框架不预设任何行为

2. **Qt 元对象支持：**
   - 使用 `Q_DECLARE_INTERFACE` 宏
   - 允许用户实现类使用 Qt 的信号槽机制
   - 用户实现需继承 QObject 并使用 `Q_INTERFACES` 宏

3. **返回值语义：**
   - `true` = 成功
   - `false` = 失败
   - 失败时框架可根据策略决定是否重试或切换账号

### 检测到的潜在风险

1. **接口变更影响：**
   - 接口一旦发布，修改可能破坏用户实现
   - 解决方案：版本化接口（Q_DECLARE_INTERFACE 包含版本号）

2. **用户实现错误：**
   - 用户实现可能死循环或抛异常
   - 解决方案：后续 Story 中 ScriptController 添加超时保护

3. **线程安全：**
   - 接口方法将在 ScriptThread 中调用
   - 解决方案：用户实现需自行处理线程安全（文档说明）

### QML 集成要求

此 Story 不涉及 QML 集成。接口为纯 C++ 抽象类。

### 参考资料

- [来源：epics.md#Story 2.1] - Story 定义和验收标准
- [来源：architecture.md#项目结构与边界] - 文件位置
- [来源：architecture.md#需求到结构映射] - FR1-FR3 映射
- [来源：project-context.md#游戏自动化专用 API] - 接口设计意图
- [来源：Qt Documentation - Q_DECLARE_INTERFACE] - Qt 接口声明宏

## Dev Agent Record

### 使用的 Agent 模型

{{agent_model_name_version}}

### 调试日志引用

（此 Story 无运行时日志，仅为接口定义）

### 完成笔记列表

（开发完成后填写）

### 文件列表

- src/interfaces/IGameLauncher.h（新增）
- src/interfaces/IDungeonEntry.h（新增）
- src/interfaces/IDungeonRunner.h（新增）
- CMakeLists.txt（修改：添加接口文件）
