---
stepsCompleted: [1, 2, 3, 4]
inputDocuments: []
session_topic: '游戏自动化脚本软件（Qt Quick UI）'
session_goals: '功能设计与架构规划 - 自动循环刷图、定时器、异常监控、多账号轮换'
selected_approach: 'ai-recommended'
techniques_used: ['Morphological Analysis', 'Five Whys', 'Six Thinking Hats']
ideas_generated: 15
context_file: ''
session_active: false
workflow_completed: true
---

# 头脑风暴会话结果

**主持人：** ckm
**日期：** 2026-02-27

## 会话概述

**话题：** 游戏自动化脚本软件（Qt Quick UI）

**目标：** 功能设计与架构规划

**核心功能需求：**
- 自动循环刷图
- 定时器：定时关闭/重启游戏
- 异常监控：检测游戏卡死等情况

### 会话设置

会话聚焦于开发一款基于 Qt Quick 的游戏自动化脚本软件，主要解决游戏挂机自动化场景下的三大核心需求。

## 技巧选择

**方法：** AI 推荐技巧
**分析上下文：** 游戏自动化脚本软件，聚焦于功能设计与架构规划

**推荐技巧：**

1. **Morphological Analysis（形态分析）**：系统性探索所有模块参数组合，确保架构完备性
2. **Five Whys（五个为什么）**：深挖功能根本需求，避免表面解决方案
3. **Six Thinking Hats（六顶思考帽）**：多角度审视方案，确保考虑周全

**AI 理由：** 软件涉及多模块交互，需要系统化的方法确保不遗漏关键功能，同时深入理解需求本质并全面评估风险。

## 技巧执行结果

### 技巧1：形态分析

**交互焦点：** 系统性探索游戏自动化软件的所有模块参数组合

**关键发现：**

| 维度 | 选择 | 关键决策 |
|------|------|---------|
| **执行方式** | 前台图色按键模拟 | 通用性强，技术成熟 |
| **定时策略** | 30分钟±15分钟随机 | 模拟真人行为 |
| **异常检测** | 进程无响应+闪退 | 覆盖主要异常场景 |
| **模块架构** | 三接口+用户实现 | 高度可定制 |
| **线程模型** | 三线程+信号槽 | 职责分离，Qt风格 |
| **配置管理** | JSON+UI混合 | 兼顾持久化与易用性 |
| **日志系统** | 7天保留+滚动显示 | 调试友好 |
| **UI设计** | 浅色主题+双窗口 | 灵活适配场景 |
| **外部API** | DLL封装层 | 底层可替换 |

### 技巧2：五个为什么

**需求洞察链：**

```
表面需求：脚本软件
    ↓ 为什么？
第二层：省时间，自动挂机
    ↓ 为什么？
第三层：积攒资源，利用睡眠时间
    ↓ 为什么？
第四层：保持竞争力
    ↓ 为什么？
第五层：解锁游戏内容
    ↓ 为什么？
根本动机：体验更多玩法 ← 【核心驱动力】
```

**设计启示：** 稳定性优先，支持长时间运行，快速恢复能力

### 技巧3：六顶思考帽

| 思考帽 | 核心发现 |
|--------|---------|
| 🎩 白帽 | 技术方案明确，外部DLL封装 |
| ❤️ 红帽 | 方案可行，用户有信心 |
| 💛 黄帽 | 稳定、灵活、可扩展 |
| 🖤 黑帽 | 风险存在但可控 |
| 💚 绿帽 | 多账号轮换功能扩展 |
| 🔵 蓝帽 | 架构设计完整 |

### 额外探索：多账号轮换系统

**核心需求：**
- 多账号管理，独立日志存储
- Sandboxie沙箱隔离
- 可配置最大运行时间和运行顺序
- 时间进度动画显示
- 换号间隔：游戏关闭后等待5秒以上

**轮换策略：** 最后一个账号运行完后持续循环运行该账号

**策略修正：** 最后一个账号不换号，但定时重启规则（30分钟±15分钟）仍然生效

---

## 最终架构设计文档

### 项目概述

**项目名称：** RedDemonScript - 游戏自动化脚本软件
**技术栈：** Qt 6 + Qt Quick (QML) + C++
**核心功能：** 自动循环刷图、定时重启、异常监控、多账号轮换

---

### 一、项目结构

```
RedDemonScript/
├── CMakeLists.txt
├── main.cpp
├── Main.qml
├── src/
│   ├── core/                    # 核心框架
│   │   ├── ScriptController.h/cpp      # 主控制器
│   │   ├── ScriptThread.h/cpp          # 脚本执行线程
│   │   ├── MonitorThread.h/cpp         # 异常监控线程
│   │   ├── TimerThread.h/cpp           # 定时器线程
│   │   └── SharedState.h               # 共享状态
│   ├── account/                 # 账号管理
│   │   ├── AccountManager.h/cpp        # 账号管理器
│   │   ├── Account.h/cpp               # 账号数据模型
│   │   └── AccountRotationController.h/cpp  # 账号轮换控制器
│   ├── interfaces/              # 用户实现接口
│   │   ├── IGameLauncher.h             # 游戏启动器接口
│   │   ├── IDungeonEntry.h             # 副本入口接口
│   │   └── IDungeonRunner.h            # 副本运行接口
│   ├── api/                     # API封装层
│   │   ├── ImageHelper.h/cpp           # 图色API封装
│   │   └── InputHelper.h/cpp           # 键鼠API封装
│   └── utils/                   # 工具类
│       ├── ConfigManager.h/cpp         # 配置管理
│       ├── Logger.h/cpp                # 日志系统
│       └── SandboxieHelper.h/cpp       # Sandboxie调用封装
├── qml/                         # QML界面
│   ├── MainWindow.qml          # 主窗口
│   ├── MiniWindow.qml          # 迷你窗口
│   ├── AccountList.qml         # 账号列表
│   └── ProgressBar.qml         # 进度条组件
├── config/                      # 配置文件
│   └── config.json             # 主配置
├── logs/                        # 日志目录
│   ├── account_001/
│   ├── account_002/
│   └── ...
└── dll/                         # 外部DLL
    ├── ImageCapture.dll        # 图色识别DLL
    └── InputSimulate.dll       # 键鼠模拟DLL
```

---

### 二、核心接口定义（用户需实现）

#### 2.1 IGameLauncher - 游戏启动器接口

```cpp
// ========== interfaces/IGameLauncher.h ==========
// 职责：从桌面启动游戏，直到进入游戏大厅
// 用户需要实现此类，填充具体逻辑

#pragma once
#include <QString>

class IGameLauncher {
public:
    virtual ~IGameLauncher() = default;

    // 启动游戏进程
    // @param sandboxName: Sandboxie沙箱名称，为空则普通启动
    // @return: 启动是否成功
    virtual bool startGame(const QString& sandboxName = "") = 0;

    // 等待进入大厅
    // @param timeoutMs: 超时时间（毫秒）
    // @return: 是否成功进入大厅
    virtual bool waitForLobby(int timeoutMs = 60000) = 0;

    // 检测是否在游戏大厅
    // @return: 当前是否在大厅界面
    virtual bool isInLobby() = 0;

    // 输入账号密码登录
    // @param username: 账号
    // @param password: 密码
    // @return: 登录是否成功
    virtual bool login(const QString& username, const QString& password) = 0;
};
```

#### 2.2 IDungeonEntry - 副本入口接口

```cpp
// ========== interfaces/IDungeonEntry.h ==========
// 职责：从大厅进入副本入口界面
// 用户需要实现此类，填充具体逻辑

#pragma once
#include <QString>

class IDungeonEntry {
public:
    virtual ~IDungeonEntry() = default;

    // 选择副本
    // @param dungeonId: 副本ID或名称标识
    // @return: 选择是否成功
    virtual bool selectDungeon(const QString& dungeonId) = 0;

    // 进入副本
    // @return: 是否成功进入
    virtual bool enterDungeon() = 0;

    // 检测是否已进入副本
    // @return: 当前是否在副本内
    virtual bool isInDungeon() = 0;
};
```

#### 2.3 IDungeonRunner - 副本运行接口

```cpp
// ========== interfaces/IDungeonRunner.h ==========
// 职责：副本内的战斗循环逻辑
// 用户需要实现此类，填充具体逻辑

#pragma once

class IDungeonRunner {
public:
    virtual ~IDungeonRunner() = default;

    // 执行战斗逻辑
    // 框架会循环调用此方法直到 isCombatFinished() 返回 true
    virtual void doCombat() = 0;

    // 检测战斗是否结束
    // @return: 战斗是否已完成
    virtual bool isCombatFinished() = 0;

    // 退出副本并领取奖励
    // @return: 是否成功退出
    virtual bool exitAndReward() = 0;

    // 检测是否回到大厅/副本入口
    // @return: 是否已返回入口界面
    virtual bool isBackToEntry() = 0;
};
```

---

### 三、API封装层（框架提供，用户可调整）

```cpp
// ========== api/ImageHelper.h ==========
// 图色识别API封装
// 底层调用外部DLL，用户可修改DLL路径或调整实现

#pragma once
#include <QString>
#include <QColor>
#include <QPixmap>

class ImageHelper {
public:
    // 初始化（加载DLL）
    static bool initialize(const QString& dllPath);

    // 查找图片
    // @param target: 目标图片路径
    // @param x, y: 找到的坐标
    // @param similarity: 相似度阈值 (0.0-1.0)
    // @return: 是否找到
    static bool findImage(const QString& target, int& x, int& y, double similarity = 0.9);

    // 查找颜色
    // @param color: 目标颜色
    // @param x, y: 找到的坐标
    // @param tolerance: 颜色容差
    // @return: 是否找到
    static bool findColor(const QColor& color, int& x, int& y, int tolerance = 10);

    // 截取屏幕
    // @return: 截图的QPixmap
    static QPixmap captureScreen();

    // 比较图片相似度
    static bool compareImage(const QString& target, double similarity = 0.9);
};

// ========== api/InputHelper.h ==========
// 键鼠模拟API封装
// 底层调用外部DLL，用户可修改DLL路径或调整实现

#pragma once
#include <QString>

class InputHelper {
public:
    // 初始化（加载DLL）
    static bool initialize(const QString& dllPath);

    // 鼠标点击
    static void click(int x, int y, int delay = 100);

    // 鼠标移动
    static void move(int x, int y);

    // 鼠标滚轮
    static void scroll(int delta);

    // 按键按下（虚拟键码）
    static void keyPress(int key, int delay = 50);

    // 按键按下（字符串形式）
    static void keyPress(const QString& key, int delay = 50);

    // 组合键
    static void keyCombine(const QStringList& keys);

    // 延迟
    static void delay(int ms);
};
```

---

### 四、核心框架（伪代码）

#### 4.1 主控制器

```cpp
// ========== core/ScriptController.h ==========
class ScriptController : public QObject {
    Q_OBJECT

public:
    // 用户设置模块实现
    void setGameLauncher(IGameLauncher* launcher);
    void setDungeonEntry(IDungeonEntry* entry);
    void setDungeonRunner(IDungeonRunner* runner);

public slots:
    void start();
    void stop();

private slots:
    // 响应各线程信号
    void onExceptionDetected(const QString& type);
    void onProcessDied();
    void onRestartTimeout();
    void onAccountTimeExpired();

private:
    void restartGame();
    void switchToNextAccount();

    // 用户模块实例
    IGameLauncher* m_launcher = nullptr;
    IDungeonEntry* m_entry = nullptr;
    IDungeonRunner* m_runner = nullptr;

    // 框架组件
    ScriptThread* m_scriptThread;
    MonitorThread* m_monitorThread;
    TimerThread* m_timerThread;
    AccountRotationController* m_accountRotation;
};
```

#### 4.2 账号轮换控制器

```cpp
// ========== account/AccountRotationController.h ==========
class AccountRotationController : public QObject {
    Q_OBJECT

public:
    // 检查是否在最后一个账号
    bool isLastAccount() const;

public slots:
    void startRotation();
    void switchToNextAccount();

signals:
    void accountTimeExpired();

private:
    Account* getNextEnabledAccount();
    
    // 如果是最后一个账号，返回当前账号（不换号）
    // 但定时重启逻辑仍由 TimerThread 控制
};
```

#### 4.3 Sandboxie封装

```cpp
// ========== utils/SandboxieHelper.h ==========
class SandboxieHelper {
public:
    // 在指定沙箱中启动程序
    static bool launchInSandbox(const QString& sandboxName, 
                                const QString& programPath);

    // 终止沙箱中的所有进程
    static bool terminateAllInSandbox(const QString& sandboxName);
};
```

---

### 五、配置文件结构

```json
{
  "version": "1.0",
  "sandboxie": {
    "installPath": "C:\\Program Files\\Sandboxie\\"
  },
  "game": {
    "processName": "game.exe"
  },
  "timer": {
    "restartIntervalMinutes": 30,
    "randomOffsetMinutes": 15
  },
  "monitor": {
    "checkIntervalSeconds": 5
  },
  "log": {
    "retentionDays": 7
  },
  "accounts": [
    {
      "id": "account_001",
      "name": "主账号",
      "username": "user1",
      "password": "加密存储",
      "sandboxName": "Account1",
      "maxRunTime": 120,
      "order": 1,
      "enabled": true
    }
  ]
}
```

---

### 六、实现优先级

| 优先级 | 模块 | 说明 |
|--------|------|------|
| P0 | 核心框架 | ScriptController + 三线程架构 |
| P0 | 用户接口 | IGameLauncher/IDungeonEntry/IDungeonRunner |
| P0 | API封装 | ImageHelper/InputHelper |
| P1 | 账号管理 | AccountManager + 账号轮换 |
| P1 | Sandboxie | SandboxieHelper封装 |
| P2 | UI界面 | 主窗口 + 迷你窗口 + 进度动画 |
| P2 | 日志系统 | Logger |
| P3 | 配置管理 | ConfigManager |

---

## 会话总结

**核心成就：**
- 系统化架构设计：三线程 + 信号槽 + 模块化接口
- 多账号轮换系统：顺序切换、独立日志、Sandboxie隔离
- 用户友好UI：进度动画、账号队列、双窗口模式

**用户需实现：**
1. `IGameLauncher` - 游戏启动和登录逻辑
2. `IDungeonEntry` - 副本选择和进入逻辑  
3. `IDungeonRunner` - 战斗循环逻辑
4. 图色/键鼠DLL的具体调用实现

**框架提供：**
1. 三线程调度框架
2. API封装层（可调整）
3. 账号轮换系统
4. UI组件和进度动画
5. 日志和配置管理
