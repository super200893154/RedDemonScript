# Story 1.2: 日志系统实现

Status: ready-for-dev

<!-- 注意：验证是可选的。在 dev-story 之前运行 validate-create-story 进行质量检查。 -->

## Story

作为一个 **系统**，
我想要 **记录带时间戳的操作日志并按账号隔离存储**，
以便 **用户可以追溯问题和了解运行状态**。

## 验收标准

1. **假设** 项目结构已搭建完成
2. **当** 系统需要记录日志
3. **那么** 日志包含时间戳、级别、账号名和操作描述
4. **并且** 日志按账号隔离存储到独立目录
5. **并且** 日志写入延迟 < 50ms（NFR9）
6. **并且** 日志格式：`[YYYY-MM-DD HH:mm:ss] [LEVEL] 账号名 - 操作描述`

**覆盖的 FR：** FR26, FR28

**非功能需求：** NFR9（日志写入延迟 < 50ms）

## 任务 / 子任务

- [ ] 任务 1：实现 LogManager 类（AC：#3, #4, #6）
  - [ ] 1.1 定义日志级别枚举（INFO, WARN, ERROR）
  - [ ] 1.2 实现日志格式化方法（时间戳 + 级别 + 账号 + 描述）
  - [ ] 1.3 实现按账号隔离的日志文件路径生成
  - [ ] 1.4 实现线程安全的日志写入方法
  - [ ] 1.5 实现日志目录自动创建

- [ ] 任务 2：实现日志文件管理（AC：#4, #5）
  - [ ] 2.1 实现日志目录结构：logs/{accountName}/
  - [ ] 2.2 实现日志文件命名：{date}.log
  - [ ] 2.3 实现缓冲写入机制（满足 < 50ms 延迟）
  - [ ] 2.4 实现文件写入错误处理

- [ ] 任务 3：集成到现有架构（AC：#3）
  - [ ] 3.1 将 LogManager 注册为单例
  - [ ] 3.2 提供 Q_INVOKABLE 方法供 QML 调用
  - [ ] 3.3 提供信号通知日志更新
  - [ ] 3.4 更新 CMakeLists.txt 添加新源文件

- [ ] 任务 4：编写测试验证（AC：全部）
  - [ ] 4.1 验证日志格式正确性
  - [ ] 4.2 验证账号隔离正确性
  - [ ] 4.3 验证写入延迟 < 50ms
  - [ ] 4.4 验证线程安全性

## Dev Notes

### 核心需求分析

**FR26：系统可以记录带时间戳的操作日志**
- 每条日志必须包含精确到秒的时间戳
- 日志级别分为 INFO、WARN、ERROR 三种

**FR28：系统可以按账号隔离存储日志**
- 每个账号有独立的日志目录
- 目录命名：logs/{accountName}/
- 文件命名：{YYYY-MM-DD}.log

**NFR9：日志写入延迟 < 50ms**
- 使用缓冲机制避免频繁 IO
- 考虑使用 QTimer 延迟刷新或 QFile 缓冲

### 架构约束

**来源：architecture.md#数据架构**

| 决策项 | 决策 | 详情 |
|--------|------|------|
| 日志存储方式 | 文件存储 | 按账号隔离，logs/ 目录 |
| 日志保留策略 | 7天自动清理 | Story 1.3 实现 |
| 日志格式 | 标准化格式 | `[YYYY-MM-DD HH:mm:ss] [LEVEL] 账号名 - 操作描述` |

**来源：architecture.md#通信模式**

| 场景 | 机制 | 说明 |
|------|------|------|
| 线程间日志写入 | 线程安全 | 使用 QMutex 保护文件写入 |
| 日志更新通知 | 信号槽 | 发送 logAppended 信号 |

### 项目结构说明

**日志类位置：**
- 头文件：`src/core/logmanager.h`
- 源文件：`src/core/logmanager.cpp`

**日志目录结构（运行时生成）：**
```
logs/
├── account_1/
│   └── 2026-03-02.log
├── account_2/
│   └── 2026-03-02.log
└── account_3/
    └── 2026-03-02.log
```

### 技术要点

**1. 线程安全设计**
```cpp
// 使用 QMutex 保护日志写入
QMutex m_mutex;

void LogManager::log(const QString& account, LogLevel level, const QString& message) {
    QMutexLocker locker(&m_mutex);
    // 写入逻辑
}
```

**2. 日志格式实现**
```cpp
QString LogManager::formatLog(const QString& account, LogLevel level, const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString levelStr = levelToString(level);
    return QString("[%1] [%2] %3 - %4").arg(timestamp, levelStr, account, message);
}
```

**3. 账号隔离存储**
```cpp
QString LogManager::getLogFilePath(const QString& account) {
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    return QString("logs/%1/%2.log").arg(account, date);
}
```

**4. 性能优化（满足 < 50ms 延迟）**
- 使用 QTextStream 缓冲写入
- 避免每次日志都打开/关闭文件
- 使用 QFile::append 模式打开文件后保持打开状态

### 命名约定

**来源：project-context.md#代码质量与风格规则**

| 类型 | 约定 | 示例 |
|------|------|------|
| 类名 | PascalCase | `LogManager` |
| 方法 | camelCase | `log()`, `getLogFilePath()` |
| 成员变量 | m_ 前缀 + camelCase | `m_mutex`, `m_currentAccount` |
| 常量 | 全大写 + 下划线 | `LOG_DIR`, `DATE_FORMAT` |
| 枚举 | PascalCase | `enum LogLevel { Info, Warn, Error }` |

### 与前一个 Story 的关系

**Story 1.1 已完成内容：**
- ✅ 项目目录结构已创建（src/core/ 已存在）
- ✅ CMakeLists.txt 已配置 Qt 组件
- ✅ Material Dark 主题已配置
- ✅ LogManager 类骨架已创建（需完善实现）

**承接要点：**
- LogManager 类已在 src/core/ 目录创建骨架
- 需要完善日志写入、格式化、账号隔离等功能
- 需要确保线程安全（三线程架构会并发写入日志）

### 日志级别使用指南

**来源：architecture.md#格式模式**

| 级别 | 使用场景 | 示例 |
|------|----------|------|
| INFO | 正常操作记录 | 开始刷图、账号切换、定时重启 |
| WARN | 警告（重试中、超时） | 登录超时、网络不稳定 |
| ERROR | 错误（异常、失败） | 游戏闪退、进程无响应 |

### QML 集成要求

**来源：architecture.md#前端架构**

LogManager 需要提供以下 QML 可访问接口：

```cpp
Q_INVOKABLE void log(const QString& account, const QString& level, const QString& message);
Q_INVOKABLE QStringList getRecentLogs(const QString& account, int count = 100);

signals:
    void logAppended(const QString& account, const QString& formattedLog);
```

### 检测到的潜在风险

1. **多线程并发写入**：三线程架构（ScriptThread、MonitorThread、TimerThread）可能同时写入日志
   - 解决方案：使用 QMutex 保护所有文件操作

2. **日志文件过大**：长时间运行可能产生大量日志
   - 解决方案：Story 1.3 实现自动清理，本 Story 仅需确保格式正确

3. **账号名包含特殊字符**：账号名可能包含不适合做目录名的字符
   - 解决方案：对账号名进行清理或编码（可选优化）

### 参考资料

- [来源：epics.md#Story 1.2] - Story 定义和验收标准
- [来源：architecture.md#数据架构] - 日志存储策略
- [来源：architecture.md#通信模式] - 线程安全要求
- [来源：architecture.md#格式模式] - 日志格式定义
- [来源：project-context.md#代码质量与风格规则] - 命名约定
- [来源：project-context.md#关键必知规则] - 禁止反模式
- [来源：1-1-project-structure-setup.md] - 前一个 Story 完成状态

## Dev Agent Record

### 使用的 Agent 模型

{{agent_model_name_version}}

### 调试日志引用

### 完成笔记列表

### 文件列表
