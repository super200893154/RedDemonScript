---
stepsCompleted: ['step-01-init', 'step-02-context', 'step-03-starter', 'step-04-decisions', 'step-05-patterns', 'step-06-structure', 'step-07-validation', 'step-08-complete']
workflowStatus: 'complete'
completedAt: '2026-03-01'
inputDocuments:
  - product-brief-RedDemonScript-2026-02-27.md
  - prd.md
  - ux-design-specification.md
  - project-context.md
workflowType: 'architecture'
project_name: 'RedDemonScript'
user_name: 'ckm'
date: '2026-03-01'
---

# 架构决策文档

_此文档通过逐步发现协作构建。随着我们一起处理每个架构决策，各部分将被追加。_

## 项目上下文分析

### 需求概览

**功能需求（35 个）：**

| 类别 | 数量 | 架构意义 |
|------|------|----------|
| 核心脚本执行 | 6 个 | 需要接口抽象层（IGameLauncher、IDungeonEntry、IDungeonRunner） |
| 异常监控与恢复 | 4 个 | 需要独立监控线程和自动恢复机制 |
| 定时器管理 | 2 个 | 需要随机间隔定时器实现 |
| 多账号管理 | 8 个 | 需要 AccountManager 和 Sandboxie 集成层 |
| 图形界面 | 5 个 | 需要 Qt Quick UI 组件和状态绑定 |
| 日志系统 | 5 个 | 需要线程安全的日志记录器 |
| 配置管理 | 5 个 | 需要 JSON 配置持久化层 |

**非功能需求（15 个）：**

| 类别 | 关键 NFR | 架构影响 |
|------|----------|----------|
| 可靠性 | ≥12小时稳定运行 | 需要内存管理、线程安全设计 |
| 性能 | 内存<100MB、CPU<5% | 需要高效的事件循环和资源管理 |
| 集成 | DLL可替换 | 需要插件化架构或动态加载机制 |
| 可维护性 | 模块化设计 | 需要清晰的模块边界和接口定义 |

### 技术约束与依赖

**目标平台：** Windows 10/11（仅限）

**核心框架：**
- Qt 6.8+ / Qt Quick/QML 6.8
- CMake 3.16+
- C++ (CXX_STANDARD_REQUIRED)

**外部依赖：**
- Sandboxie Plus 5.55+（用户自行安装）
- 外部 DLL（图色识别、键鼠模拟，用户可替换）

**运行时特性：**
- 完全离线，无网络依赖
- 绿色免安装，解压即用
- 单例模式运行

### 已识别的横切关注点

| 关注点 | 影响模块 | 实现策略 |
|--------|----------|----------|
| **日志记录** | 所有模块 | 线程安全日志记录器，按账号隔离存储 |
| **配置管理** | 所有模块 | JSON 配置文件，启动加载/退出保存 |
| **线程安全** | 三大线程 | QMutex/QReadWriteLock 保护共享数据 |
| **异常处理** | 脚本执行、进程监控 | 统一异常捕获和恢复机制 |
| **状态同步** | UI ↔ 后台线程 | Qt 信号槽 + QueuedConnection |

### 规模与复杂度评估

- **项目复杂度：** 低
- **主要领域：** 桌面应用（Windows）
- **预估架构组件：** 8-12 个核心类
- **关键技术挑战：** 三线程协调、外部进程管理、DLL动态加载

## 起始模板评估

### 主要技术领域

根据项目需求分析确定为 **Windows 桌面应用**，使用 **Qt 6 C++/QML** 技术栈。

### 当前项目状态

项目已有一个基于 Qt Creator 模板的基础起始结构：

```
RedDemonScript/
├── CMakeLists.txt      ✅ 已配置
├── main.cpp            ✅ 基础入口
└── Main.qml            ✅ 基础窗口
```

### 选定的起始模板：Qt Creator 官方模板 + 扩展

**选择理由：**

1. **已存在的项目基础** - 项目已有符合 Qt 6 最佳实践的 CMake 配置
2. **官方模板** - Qt Creator 生成的模板遵循官方推荐结构
3. **低复杂度项目** - 项目复杂度为"低"，无需复杂的起始模板
4. **快速迭代** - 在现有基础上扩展比从头开始更高效

### 起始模板提供的架构决策

**语言和运行时：**
- C++17（需显式添加 `CMAKE_CXX_STANDARD 17`）
- Qt 6.8+ 运行时

**构建系统：**
```cmake
cmake_minimum_required(VERSION 3.16)
project(RedDemonScript VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
qt_standard_project_setup(REQUIRES 6.8)
```

**QML 模块配置：**
```cmake
qt_add_qml_module(appRedDemonScript
    URI RedDemonScript
    QML_FILES Main.qml
)
```

**需要添加的 Qt 组件：**
```cmake
find_package(Qt6 REQUIRED COMPONENTS 
    Quick          # 现有
    QuickControls2 # Material 主题
    Widgets        # 系统托盘
    Concurrent     # 多线程
)
```

### 建议的项目结构

```
RedDemonScript/
├── CMakeLists.txt
├── main.cpp
├── Main.qml
├── src/
│   ├── core/           # 核心框架
│   │   ├── scriptcontroller.h/cpp
│   │   ├── scriptthread.h/cpp
│   │   ├── monitorthread.h/cpp
│   │   └── timerthread.h/cpp
│   ├── interfaces/     # 用户接口
│   │   ├── igamelauncher.h
│   │   ├── idungeonentry.h
│   │   └── idungeonrunner.h
│   ├── helpers/        # API 封装
│   │   ├── imagehelper.h/cpp
│   │   └── inputhelper.h/cpp
│   ├── account/        # 账号管理
│   │   └── accountmanager.h/cpp
│   └── ui/             # UI 相关
│       └── systemtray.h/cpp
├── qml/                # QML 文件
│   ├── Main.qml
│   ├── StatusCard.qml
│   ├── AccountCard.qml
│   └── LogPanel.qml
└── config/             # 配置文件
    └── config.json
```

**注意：** 项目初始化（扩展 CMakeLists.txt 和创建目录结构）应该是第一个实施故事的一部分。

## 核心架构决策

### 决策优先级分析

**关键决策（阻塞实施）：**
- ✅ 三线程架构（已由项目上下文确定）
- ✅ 数据存储策略（JSON配置 + 文件日志）
- ✅ 状态管理模式（信号槽 + Q_PROPERTY）

**重要决策（影响架构）：**
- ✅ 账号密码加密方式
- ✅ 单例模式实现
- ✅ 组件设计策略

**推迟决策（MVP 后）：**
- 无

### 数据架构

| 决策项 | 决策 | 版本/详情 | 理由 |
|--------|------|-----------|------|
| 配置存储格式 | JSON | Qt QJsonDocument | 已由 PRD 确定，人类可读可编辑 |
| 日志存储方式 | 文件存储 | 按账号隔离，日志目录 | 简单、可读性强、便于调试 |
| 数据验证策略 | 启动时验证 | 配置加载后立即验证 | 尽早发现问题，符合"安心挂机"体验 |
| 日志保留策略 | 7天自动清理 | 启动时执行清理 | 简单、用户无感知 |

### 认证与安全

| 决策项 | 决策 | 版本/详情 | 理由 |
|--------|------|-----------|------|
| 账号密码加密 | Windows DPAPI | 系统级加密 API | 仅 Windows 平台，安全性高，无需密钥管理 |
| 配置文件权限 | 用户目录存储 | Windows 用户权限 | 敏感信息保护 |

### 前端架构

| 决策项 | 决策 | 版本/详情 | 理由 |
|--------|------|-----------|------|
| UI 框架 | Qt Quick Controls 2 | Qt 6.8+ | UX 规范已确定 |
| 主题 | Material Dark | Primary: #2196F3 | UX 规范已确定 |
| 状态管理 | Qt 信号槽 + Q_PROPERTY | 单例 ScriptController | Qt 原生，QML 自动绑定 |
| 组件设计 | 独立 QML 文件 | StatusCard, AccountCard, LogPanel, StatusBar | 便于复用和维护 |

### 基础设施与部署

| 决策项 | 决策 | 版本/详情 | 理由 |
|--------|------|-----------|------|
| 部署方式 | 绿色免安装 | 解压即用 | PRD 已确定 |
| 单例模式 | QtSingleApplication | Qt 解决方案 | 官方方案，成熟稳定 |
| 运行权限 | 普通用户权限 | 无需管理员 | PRD 已确定 |

### 决策影响分析

**实施顺序：**
1. 项目结构搭建（创建目录、扩展 CMakeLists.txt）
2. 核心框架实现（ScriptController + 三线程）
3. 数据层实现（配置管理、日志系统）
4. UI 实现（QML 组件）
5. 集成测试（三线程协调、异常恢复）

**跨组件依赖：**
- ScriptController 依赖 AccountManager、ConfigManager、Logger
- 三个线程通过信号槽与 ScriptController 通信
- UI 通过 Q_PROPERTY 绑定 ScriptController 状态

## 实施模式与一致性规则

### 已识别的关键冲突点

AI Agent 可能做出不同选择的区域共 5 个，已全部定义一致性规则。

### 命名模式

**代码命名约定（继承自项目上下文）：**

| 类型 | 约定 | 示例 |
|------|------|------|
| 类名 | PascalCase | `ScriptController`, `AccountManager` |
| 函数/方法 | camelCase | `startScript()`, `stopScript()` |
| 成员变量 | m_ 前缀 + camelCase | `m_accountList`, `m_isRunning` |
| 常量 | 全大写 + 下划线 | `MAX_RETRY_COUNT` |
| 接口 | I 前缀 | `IGameLauncher`, `IDungeonEntry` |

**信号槽命名约定：**

| 类型 | 约定 | 示例 |
|------|------|------|
| 信号 | 动词过去式 | `started()`, `stopped()`, `errorOccurred()` |
| 槽函数 | on + 动词过去式 | `onStarted()`, `onStopped()` |
| QML 处理器 | on + 信号名 | `onStarted: {}`, `onStopped: {}` |

**Q_PROPERTY 命名约定：**

| 约定 | 示例 |
|------|------|
| 属性名：无前缀 camelCase | `isRunning`, `currentAccount`, `runTime` |
| 信号：属性名 + Changed | `isRunningChanged`, `currentAccountChanged` |
| QML 访问：直接使用属性名 | `controller.isRunning` |

**JSON 字段命名约定：**

| 约定 | 示例 |
|------|------|
| 字段名：camelCase | `{"maxRunTime": 120, "accountList": [...]}` |

### 结构模式

**文件组织约定（继承自项目上下文）：**

| 约定 | 说明 |
|------|------|
| 头文件后缀 | .h |
| 源文件后缀 | .cpp |
| QML 文件命名 | PascalCase.qml |
| 类文件组织 | 每个类一对 .h/.cpp 文件 |

**目录结构约定：**

```
src/
├── core/           # 核心框架（ScriptController, 三线程）
├── interfaces/     # 用户接口（IGameLauncher 等）
├── helpers/        # API 封装
├── account/        # 账号管理
└── ui/             # UI 相关
qml/                # QML 文件
config/             # 配置文件
```

### 格式模式

**日志格式约定：**

```
[YYYY-MM-DD HH:mm:ss] [LEVEL] 账号名 - 操作描述
```

**日志级别使用：**

| 级别 | 使用场景 |
|------|----------|
| INFO | 正常操作记录 |
| WARN | 警告（重试中、超时） |
| ERROR | 错误（异常、失败） |

### 通信模式

**线程间通信约定：**

| 场景 | 机制 | 说明 |
|------|------|------|
| 线程间通知 | 信号槽 + QueuedConnection | 状态变化、事件通知 |
| 共享数据访问 | QReadWriteLock | 账号列表、配置等 |
| 简单标志位 | QAtomicInt | 运行状态、计数器 |

**错误处理约定：**

| 场景 | 处理方式 |
|------|----------|
| 线程内异常 | 捕获 → 记录日志 → 发送信号 |
| 跨线程错误 | 信号传递（带错误类型和消息） |
| UI 错误显示 | 绑定错误状态属性 |

### 流程模式

**状态更新流程：**

1. 后台线程检测到状态变化
2. 发送信号通知主线程
3. 主线程更新状态属性
4. QML 自动响应属性变化

**异常恢复流程：**

1. 监控线程检测到异常
2. 记录日志（ERROR 级别）
3. 发送 `errorOccurred` 信号
4. 主控制器触发恢复流程
5. 恢复成功后发送 `recovered` 信号

### 执行指南

**所有 AI Agent 必须：**

1. 遵循项目上下文中定义的 45 条规则
2. 使用新式 Qt5/Qt6 信号槽连接语法（函数指针）
3. 跨线程通信使用 Qt::QueuedConnection
4. 不在子线程直接操作 UI 元素
5. 鼠标模拟操作后添加适当等待

### 模式示例

**良好示例：**

```cpp
// 信号声明
signals:
    void started();
    void stopped();
    void errorOccurred(const QString &type, const QString &message);

// Q_PROPERTY 声明
Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

// 跨线程连接
connect(m_scriptThread, &ScriptThread::started,
        this, &ScriptController::onStarted,
        Qt::QueuedConnection);
```

**反模式（禁止）：**

```cpp
// ❌ 旧式 SIGNAL/SLOT 宏
connect(this, SIGNAL(started()), this, SLOT(onStarted()));

// ❌ 子线程操作 UI
m_statusLabel->setText("Running");  // 在子线程中

// ❌ 无等待的鼠标操作
InputHelper::click(x, y);  // 缺少等待
```

## 项目结构与边界

### 完整项目目录结构

```
RedDemonScript/
├── CMakeLists.txt                    # 主构建配置
├── main.cpp                          # 应用入口
├── Main.qml                          # 主 QML 窗口
├── README.md                         # 项目说明
├── LICENSE                           # 许可证
├── .gitignore                        # Git 忽略规则
│
├── src/                              # C++ 源代码
│   ├── core/                         # 核心框架
│   │   ├── scriptcontroller.h/cpp    # 主控制器（单例）
│   │   ├── scriptthread.h/cpp        # 脚本执行线程
│   │   ├── monitorthread.h/cpp       # 异常监控线程
│   │   ├── timerthread.h/cpp         # 定时器线程
│   │   ├── logger.h/cpp              # 日志记录器
│   │   └── configmanager.h/cpp       # 配置管理器
│   │
│   ├── interfaces/                   # 用户接口（抽象类）
│   │   ├── igamelauncher.h           # 游戏启动接口
│   │   ├── idungeonentry.h           # 副本入口接口
│   │   └── idungeonrunner.h          # 副本运行接口
│   │
│   ├── helpers/                      # API 封装
│   │   ├── imagehelper.h/cpp         # 图色识别封装
│   │   └── inputhelper.h/cpp         # 键鼠模拟封装
│   │
│   ├── account/                      # 账号管理
│   │   ├── accountmanager.h/cpp      # 账号管理器
│   │   ├── accountinfo.h             # 账号信息结构
│   │   └── sandboxiehelper.h/cpp     # Sandboxie 集成
│   │
│   └── ui/                           # UI 相关 C++
│       └── systemtray.h/cpp          # 系统托盘
│
├── qml/                              # QML 文件
│   ├── Main.qml                      # 主窗口
│   ├── components/                   # 自定义组件
│   │   ├── StatusCard.qml            # 状态卡片
│   │   ├── AccountCard.qml           # 账号卡片
│   │   ├── LogPanel.qml              # 日志面板
│   │   └── StatusBar.qml             # 状态栏
│   └── pages/                        # 页面（可选扩展）
│       └── SettingsPage.qml          # 设置页面
│
├── config/                           # 配置文件
│   ├── config.json                   # 主配置文件
│   └── config.example.json           # 配置示例
│
├── logs/                             # 日志目录（运行时生成）
│   └── account_*/                    # 按账号隔离
│
└── build/                            # 构建输出（Git 忽略）
```

### 架构边界

**组件边界：**

| 边界 | 说明 | 通信方式 |
|------|------|----------|
| ScriptController ↔ 三线程 | 主控制器与工作线程 | 信号槽 + QueuedConnection |
| ScriptController ↔ UI | 主控制器与界面 | Q_PROPERTY 绑定 |
| AccountManager ↔ SandboxieHelper | 账号管理与沙箱 | 直接调用 |
| Helpers ↔ 外部 DLL | 封装层与外部库 | 动态加载（QLibrary） |

**数据边界：**

| 边界 | 说明 | 数据格式 |
|------|------|----------|
| ConfigManager → config.json | 配置读写 | JSON |
| Logger → logs/ | 日志写入 | 文本文件 |
| AccountManager → 账号数据 | 内存管理 | QList<AccountInfo> |

### 需求到结构映射

**功能需求映射：**

| FR 类别 | 实现位置 |
|---------|----------|
| FR1-FR6 核心脚本执行 | `src/interfaces/`, `src/helpers/`, `src/core/scriptthread.*` |
| FR7-FR10 异常监控与恢复 | `src/core/monitorthread.*` |
| FR11-FR12 定时器管理 | `src/core/timerthread.*` |
| FR13-FR20 多账号管理 | `src/account/` |
| FR21-FR25 图形界面 | `qml/`, `src/ui/` |
| FR26-FR30 日志系统 | `src/core/logger.*` |
| FR31-FR35 配置管理 | `src/core/configmanager.*` |

**横切关注点映射：**

| 关注点 | 实现位置 |
|--------|----------|
| 线程安全 | `src/core/scriptcontroller.*`（协调） |
| 日志记录 | `src/core/logger.*`（全局单例） |
| 配置管理 | `src/core/configmanager.*`（全局单例） |
| 错误处理 | 各模块通过信号传递到 ScriptController |

### 集成点

**内部通信：**

```
┌─────────────────────────────────────────────────────────┐
│                    ScriptController                      │
│                    （主控制器/单例）                      │
├─────────────────────────────────────────────────────────┤
│   ▲ signals          ▲ signals          ▲ signals       │
│   │                  │                  │               │
│   ▼ slots            ▼ slots            ▼ slots         │
├─────────────┬─────────────┬─────────────┬───────────────┤
│ScriptThread │MonitorThread│ TimerThread │ UI (QML)      │
│脚本执行线程  │异常监控线程 │ 定时器线程  │ 界面层        │
└─────────────┴─────────────┴─────────────┴───────────────┘
```

**外部集成：**

| 集成点 | 方式 | 文件 |
|--------|------|------|
| 外部 DLL（图色/键鼠） | QLibrary 动态加载 | `src/helpers/` |
| Sandboxie | 进程启动（QProcess） | `src/account/sandboxiehelper.*` |
| 游戏进程 | QProcess 管理 | `src/core/scriptthread.*` |

### 开发工作流

**构建流程：**

```bash
cmake -B build -S .
cmake --build build
```

**运行流程：**

```bash
./build/appRedDemonScript
```

**配置流程：**

1. 复制 `config/config.example.json` 到 `config/config.json`
2. 编辑配置文件填写账号信息
3. 启动应用

## 架构验证结果

### 一致性验证 ✅

**决策兼容性：** 所有技术选择（Qt 6.8+、C++17、CMake、三线程架构）无冲突协同工作。

**模式一致性：** 命名约定、信号槽命名、Q_PROPERTY 命名、JSON 字段命名全部遵循 camelCase/PascalCase 规则，保持一致。

**结构对齐：** 项目目录结构按功能模块组织，边界清晰，支持所有架构决策。

### 需求覆盖验证 ✅

**功能需求覆盖：** 全部 35 个功能需求（FR1-FR35）均有对应的架构组件支持。

**非功能需求覆盖：** 全部 15 个非功能需求（NFR1-NFR15）已在架构设计中解决。

### 实施准备度验证 ✅

**决策完整性：** 所有关键决策带版本记录，实施模式足够全面。

**结构完整性：** 项目结构完整具体，所有文件和目录已定义。

**模式完整性：** 所有潜在冲突点已解决，命名、通信、流程模式完整。

### 缺口分析结果

**关键缺口：** 无

**次要缺口：** 单元测试策略（MVP 使用实际挂机测试）

### 架构完整性清单

**✅ 需求分析**
- [x] 项目上下文彻底分析
- [x] 规模和复杂度评估
- [x] 技术约束识别
- [x] 横切关注点映射

**✅ 架构决策**
- [x] 关键决策带版本记录
- [x] 技术栈完全指定
- [x] 集成模式定义
- [x] 性能考虑已解决

**✅ 实施模式**
- [x] 命名约定建立
- [x] 结构模式定义
- [x] 通信模式指定
- [x] 流程模式记录

**✅ 项目结构**
- [x] 完整目录结构定义
- [x] 组件边界建立
- [x] 集成点映射
- [x] 需求到结构映射完成

### 架构准备度评估

**整体状态：** ✅ 准备实施

**信心级别：** 高

**关键优势：**
1. 三线程架构设计清晰，职责分离
2. 接口驱动设计，用户可定制
3. 完整的实施模式和命名约定
4. 所有需求有明确架构支持

**未来可改进区域：**
1. 后续版本可添加单元测试框架
2. 可扩展日志分析功能

### 实施交接

**AI Agent 指南：**

1. 完全按照文档记录遵循所有架构决策
2. 在所有组件中一致使用实施模式
3. 遵守项目结构和边界
4. 对于所有架构问题参考此文档

**首要实施优先级：**

1. 扩展 CMakeLists.txt 添加新模块
2. 创建目录结构（src/, qml/, config/）
3. 实现核心框架（ScriptController + 三线程）
4. 实现数据层（ConfigManager + Logger）
5. 实现 UI 层（QML 组件）

