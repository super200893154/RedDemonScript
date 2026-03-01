# Story 1.3: 日志自动清理

Status: ready-for-dev

<!-- 注意：验证是可选的。在 dev-story 之前运行 validate-create-story 进行质量检查。 -->

## Story

作为一个 **系统**，
我想要 **自动清理超过 7 天的日志文件**，
以便 **磁盘空间不会被历史日志占满**。

## 验收标准

1. **假设** 日志系统已实现（Story 1.2 完成）
2. **当** 应用启动时
3. **那么** 系统检查日志目录
4. **并且** 删除超过 7 天的日志文件
5. **并且** 记录清理操作到日志

**覆盖的 FR：** FR30

## 任务 / 子任务

- [ ] 任务 1：实现日志清理功能（AC：#3, #4, #5）
  - [ ] 1.1 在 LogManager 中添加 cleanupOldLogs() 方法
  - [ ] 1.2 实现遍历日志目录并检查文件日期
  - [ ] 1.3 实现删除过期文件的逻辑
  - [ ] 1.4 记录清理操作到日志

- [ ] 任务 2：集成到应用启动流程（AC：#2, #5）
  - [ ] 2.1 在应用启动时调用日志清理
  - [ ] 2.2 确保清理在日志系统初始化后执行

- [ ] 任务 3：测试验证（AC：全部）
  - [ ] 3.1 验证清理功能正确删除 7 天前的日志
  - [ ] 3.2 验证保留 7 天内的日志
  - [ ] 3.3 验证清理日志记录到系统日志

## Dev Notes

### 核心需求分析

**FR30：系统可以自动清理超过 7 天的日志文件**
- 清理触发时机：应用启动时
- 清理范围：logs/ 目录下所有账号的日志文件
- 清理条件：文件修改时间距今超过 7 天
- 清理操作：删除符合条件的日志文件

**数据架构来源：architecture.md#数据架构**

| 决策项 | 决策 | 详情 |
|--------|------|------|
| 日志保留策略 | 7天自动清理 | 启动时执行清理 |

### Story 1.2 上下文（前置 Story）

**Story 1.2 已完成内容：**

- ✅ LogManager 类实现完成（单例模式）
  - 头文件：`src/core/LogManager.h`
  - 源文件：`src/core/LogManager.cpp`
- ✅ 日志目录结构：`logs/{accountName}/{yyyy-MM-dd}.log`
- ✅ 日志格式：`[YYYY-MM-DD HH:mm:ss] [LEVEL] 账号名 - 操作描述`
- ✅ 线程安全：使用 QMutex 保护所有文件操作
- ✅ 缓冲写入：使用 QTextStream + 文件保持打开状态
- ✅ QML 集成：提供 Q_INVOKABLE 方法

**Story 1.2 记录的重要实现细节：**

```cpp
// 日志文件路径生成（来自 Story 1.2）
QString LogManager::getLogFilePath(const QString& account) {
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    return QString("logs/%1/%2.log").arg(account, date);
}
```

**Story 1.2 识别的潜在风险（与本 Story 相关）：**

> 2. **日志文件过大**：长时间运行可能产生大量日志
>    - 解决方案：Story 1.3 实现自动清理，本 Story 仅需确保格式正确

### 架构约束

**来源：architecture.md#数据架构**

| 决策项 | 决策 | 详情 |
|--------|------|------|
| 日志存储方式 | 文件存储 | 按账号隔离，logs/ 目录 |
| 日志保留策略 | 7天自动清理 | Story 1.3 实现（本 Story） |
| 日志清理时机 | 应用启动时 | Story 1.3 实现（本 Story） |

**来源：architecture.md#格式模式**

| 级别 | 使用场景 |
|------|----------|
| INFO | 正常操作记录（包括清理操作） |
| WARN | 警告（重试中、超时） |
| ERROR | 错误（异常、失败） |

### 项目结构说明

**新增/修改文件位置：**
- 头文件：`src/core/LogManager.h`（修改，添加清理方法声明）
- 源文件：`src/core/LogManager.cpp`（修改，添加清理实现）
- 入口文件：`main.cpp`（修改，调用清理方法）

**日志目录结构（运行时）：**
```
logs/
├── account_1/
│   ├── 2026-02-23.log    # 8天前 - 应被删除
│   ├── 2026-02-24.log    # 7天前 - 应被删除
│   ├── 2026-02-25.log    # 6天前 - 应保留
│   └── 2026-03-01.log    # 当天 - 应保留
├── account_2/
│   └── ...
└── system/               # 系统日志（如有）
    └── ...
```

### 技术要点

**1. 清理方法设计**

```cpp
// LogManager.h - 添加方法声明
class LogManager : public QObject {
    Q_OBJECT
public:
    static LogManager* instance();
    Q_INVOKABLE void cleanupOldLogs();  // 新增：清理过期日志
    // ... 现有方法
private:
    LogManager();
    ~LogManager();
    void deleteOldLogFiles(const QDir& logDir, const QDate& cutoffDate);
    int m_daysToKeep;  // 配置保留天数，默认7天
};
```

**2. 清理实现逻辑**

```cpp
// LogManager.cpp - 清理实现
void LogManager::cleanupOldLogs() {
    QDate cutoffDate = QDate::currentDate().addDays(-m_daysToKeep);  // 7天前
    
    QMutexLocker locker(&m_mutex);
    
    QDir logsDir("logs");
    if (!logsDir.exists()) {
        return;
    }
    
    // 遍历所有账号目录
    QFileInfoList accountDirs = logsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& accountDirInfo : accountDirs) {
        QDir accountDir(accountDirInfo.absoluteFilePath());
        deleteOldLogFiles(accountDir, cutoffDate);
    }
    
    logInfo("system", QString("日志清理完成，保留 %1 天内的日志").arg(m_daysToKeep));
}

void LogManager::deleteOldLogFiles(const QDir& logDir, const QDate& cutoffDate) {
    QFileInfoList logFiles = logDir.entryInfoList(QStringList() << "*.log", QDir::Files);
    for (const QFileInfo& fileInfo : logFiles) {
        QDate fileDate = QDate::fromString(fileInfo.baseName(), "yyyy-MM-dd");
        if (fileDate.isValid() && fileDate < cutoffDate) {
            if (logDir.remove(fileInfo.fileName())) {
                qDebug() << "Deleted old log:" << fileInfo.absoluteFilePath();
            }
        }
    }
}
```

**3. 应用启动集成**

```cpp
// main.cpp - 应用启动时调用
int main(int argc, char *argv[]) {
    // ... 现有初始化代码
    
    // 初始化日志系统后执行清理
    LogManager::instance()->cleanupOldLogs();
    
    // ... 启动应用
}
```

**4. 日志记录要求**

清理操作本身也需要记录日志（使用 INFO 级别）：

```cpp
// 清理开始时
logInfo("system", "开始执行日志清理...");

// 清理完成后
logInfo("system", QString("日志清理完成，删除 %1 个文件，释放 %2 MB")
    .arg(deletedCount)
    .arg(freedSpace));
```

### 命名约定

**来源：project-context.md#代码质量与风格规则**

| 类型 | 约定 | 示例 |
|------|------|------|
| 类名 | PascalCase | `LogManager` |
| 方法 | camelCase | `cleanupOldLogs()`, `deleteOldLogFiles()` |
| 成员变量 | m_ 前缀 + camelCase | `m_daysToKeep`, `m_mutex` |
| 常量 | 全大写 + 下划线 | `DEFAULT_DAYS_TO_KEEP` |

### 与前一个 Story 的关系

**Story 1.2 已完成内容：**
- ✅ LogManager 类完整实现
- ✅ 账号隔离日志存储
- ✅ 线程安全机制
- ✅ QML 集成

**承接要点：**
- 本 Story 在 Story 1.2 基础上添加清理功能
- 复用 LogManager 单例和现有日志机制
- 清理操作使用已有的线程安全机制

### 与后续 Story 的关系

**Story 1.4（配置管理器）待实现：**
- 配置管理器可能提供配置项：日志保留天数
- 本 Story 可以预留接口支持配置化（可选优化）

### 检测到的潜在风险

1. **清理时机冲突**：如果在日志写入时执行清理，可能导致文件锁定
   - 解决方案：使用 QMutex 保护，在清理前先刷新所有缓冲

2. **账号目录不存在**：logs/ 目录可能为空或不存在的账号目录
   - 解决方案：使用 QDir::exists() 检查后再操作

3. **文件名格式不一致**：如果文件名不符合预期格式（yyyy-MM-dd.log）
   - 解决方案：使用 QDate::fromString() 验证日期有效性，无效则跳过

4. **权限问题**：如果日志文件被其他程序占用或只读
   - 解决方案：使用 QFile::remove() 返回值判断，失败则记录 WARNING

### 参考资料

- [来源：epics.md#Story 1.3] - Story 定义和验收标准
- [来源：architecture.md#数据架构] - 日志保留策略决策
- [来源：architecture.md#格式模式] - 日志级别使用指南
- [来源：1-2-logging-system-implementation.md] - 前一个 Story 完成状态（LogManager 实现细节）
- [来源：project-context.md#代码质量与风格规则] - 命名约定

## Dev Agent Record

### 使用的 Agent 模型

（待开发完成后填写）

### 调试日志引用

（待开发完成后填写）

### 完成笔记列表

（待开发完成后填写）

### 文件列表

（待开发完成后填写）

- `src/core/LogManager.h` - 修改
- `src/core/LogManager.cpp` - 修改
- `main.cpp` - 修改

## Change Log

（待开发完成后填写）
