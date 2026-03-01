---
project_name: 'RedDemonScript'
user_name: 'ckm'
date: '2026-02-28'
sections_completed: ['technology_stack', 'language_rules', 'framework_rules', 'testing_rules', 'code_quality_rules', 'workflow_rules', 'critical_rules']
status: 'complete'
rule_count: 45
optimized_for_llm: true
---

# AI Agent 项目上下文

_此文件包含 AI agent 在此项目实现代码时必须遵循的关键规则和模式。专注于 agent 可能会遗漏的不明显细节。_

---

## 技术栈与版本

**核心框架：**
- Qt 6.8 + Qt Quick/QML 6.8
- CMake 3.16+
- C++ (CXX_STANDARD_REQUIRED)

**架构模式：**
- 三线程架构：脚本执行线程、异常监控线程、定时器线程
- 接口驱动：IGameLauncher、IDungeonEntry、IDungeonRunner
- 辅助封装：ImageHelper（图色识别）、InputHelper（键鼠模拟）

**目标平台：**
- Windows（仅 Windows）

## 关键实现规则

### C++ 规则

**Qt 信号槽连接：**
- 使用新式 Qt5/Qt6 连接语法（函数指针），避免旧式 SIGNAL/SLOT 宏
- 示例：`connect(&engine, &QQmlApplicationEngine::objectCreationFailed, ...)`

**内存管理：**
- 优先使用 Qt 父子对象模型，减少手动 delete
- QML 创建的对象由 QML 引擎管理
- 跨线程对象使用 `QObject::moveToThread` 或 `QThread` 子类

**线程安全：**
- 三线程架构需使用 `QMutex`、`QReadWriteLock` 或 `QAtomicInt` 保护共享数据
- 跨线程信号槽使用 `Qt::QueuedConnection`
- 避免在子线程直接操作 UI

### QML 规则

**模块加载：**
- 使用 Qt6 新式加载：`engine.loadFromModule("RedDemonScript", "Main")`
- QML 文件通过 `qt_add_qml_module` 注册

**信号槽连接：**
- QML 中使用 `Connections` 对象连接 C++ 信号
- C++ 类需使用 `Q_PROPERTY`、`Q_INVOKABLE` 或 `signals` 暴露给 QML

**类型注册：**
- 使用 `QML_ELEMENT` 宏注册自定义类型
- 在 CMake 中通过 `qt_add_qml_module` 添加 QML 类型

### Qt/QML 框架规则

**三线程架构约束：**
- ScriptThread：脚本执行线程，运行游戏操作循环
- MonitorThread：异常监控线程，检测进程状态、卡死、闪退
- TimerThread：定时器线程，定时重启、预防性维护
- 三个线程必须独立运行，互不阻塞

**游戏自动化专用 API：**
- IGameLauncher：游戏启动接口（用户实现具体游戏逻辑）
- IDungeonEntry：副本入口接口
- IDungeonRunner：副本运行接口
- ImageHelper：图色识别封装（屏幕截图、模板匹配）
- InputHelper：键鼠模拟封装（点击、移动、按键）

**QML UI 规则：**
- 主窗口提供：账号列表、启动/停止按钮、状态显示
- 日志区域使用 ListView + ListModel 实现滚动日志
- 状态更新通过 C++ 信号驱动 QML 属性绑定

### 测试规则

**测试策略：**
- 核心验收测试：实际挂机测试（连续3晚，每晚8小时以上）
- 无传统单元测试框架，以实际运行验证为主

**稳定性验证标准：**
- 单账号稳定运行 ≥ 12小时
- 多账号按计划轮换，切换正确率 100%
- 异常检测有效，能自动恢复
- 日志清晰可读，能定位问题

**异常测试场景：**
- 模拟游戏闪退，验证自动重启
- 模拟进程无响应，验证监控检测
- 模拟登录失败，验证异常恢复
- 多账号轮换完整性测试

### 代码质量与风格规则

**命名约定：**
- 类名：PascalCase（如 `ScriptController`、`AccountManager`）
- 函数/方法：camelCase（如 `startScript`、`stopScript`）
- 成员变量：m_ 前缀 + camelCase（如 `m_accountList`、`m_isRunning`）
- 常量：全大写 + 下划线（如 `MAX_RETRY_COUNT`）
- 接口：I 前缀（如 `IGameLauncher`、`IDungeonEntry`）

**文件组织：**
- 头文件：.h 后缀
- 源文件：.cpp 后缀
- QML 文件：PascalCase.qml
- 每个类一对 .h/.cpp 文件

**日志格式：**
- 必须包含时间戳：`[2026-02-28 10:30:45]`
- 日志级别：INFO / WARN / ERROR
- 操作记录格式：`[时间戳] [级别] 账号{x} - 操作描述`
- 异常详情需包含上下文信息

**配置文件：**
- 使用 JSON 格式存储配置
- 配置文件位置：应用目录下 config.json
- 包含：账号信息、定时参数、脚本参数

### 开发工作流规则

**Git 分支策略：**
- main：主分支，稳定版本
- feature/*：功能开发分支
- fix/*：问题修复分支

**提交消息格式：**
- 使用中文描述
- 格式：`[模块] 简短描述`
- 示例：`[AccountManager] 添加账号轮换逻辑`

**构建流程：**
- CMake 配置：`cmake -B build -S .`
- 构建：`cmake --build build`
- Qt 版本要求：Qt 6.8+

**发布检查清单：**
- [ ] 三线程架构正常运行，无死锁
- [ ] 单账号稳定运行 ≥ 12小时
- [ ] 多账号按计划轮换，切换正确
- [ ] 异常检测有效，能自动恢复
- [ ] 图形界面可正常操作
- [ ] 日志清晰可读，能定位问题

### 关键必知规则

**绝对禁止的反模式：**
- ❌ 在子线程直接操作 UI 元素（会导致崩溃）
- ❌ 使用旧式 SIGNAL/SLOT 宏连接（编译期无法检查）
- ❌ 鼠标模拟操作后不等待（游戏响应需要时间）
- ❌ 忽略游戏窗口最小化/失焦状态（图色识别会失败）
- ❌ 死循环中无限重试无延迟（会卡死线程）

**必须处理的边界情况：**
- 游戏进程意外终止（闪退检测 + 自动重启）
- 游戏窗口无响应（超时检测 + 强制结束）
- 登录过程卡住（登录超时 + 重试机制）
- 账号切换失败（Sandboxie 异常处理）
- 网络断开重连（等待网络恢复）

**性能关键点：**
- 图色识别使用缓存，避免重复截图
- 定时器间隔不小于 100ms，避免 CPU 占用过高
- 日志写入使用缓冲，避免频繁 IO
- 长时间运行需监控内存泄漏

**安全注意：**
- 账号密码加密存储，不明文保存
- 敏感操作需记录日志审计
- 配置文件权限限制

---

## 使用指南

**AI Agent：**
- 在实现任何代码之前阅读此文件
- 严格按照文档遵循所有规则
- 如有疑问，优先选择更严格的选项
- 如果出现新模式，更新此文件

**人类：**
- 保持此文件精简并专注于 agent 需求
- 当技术栈变化时更新
- 每季度审查过时的规则
- 随时间推移移除变得显而易见的规则

**最后更新：** 2026-02-28
