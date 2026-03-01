# Story 1.1: 项目结构搭建

Status: ready-for-dev

<!-- 注意：验证是可选的。在 dev-story 之前运行 validate-create-story 进行质量检查。 -->

## Story

作为一个 **开发者**，
我想要 **建立完整的项目目录结构和构建配置**，
以便 **后续开发有清晰的代码组织基础**。

## 验收标准

1. **假设** 项目已有基础的 Qt Creator 模板文件
2. **当** 开发者执行构建配置
3. **那么** 项目目录结构符合架构设计
4. **并且** CMakeLists.txt 配置 C++17 和所需 Qt 组件
5. **并且** 所有目录创建完成
6. **并且** 项目可以成功编译运行空白窗口

**覆盖的 FR：** 无（基础设施）

**架构需求：** 起始模板扩展 + 目录结构搭建

## 任务 / 子任务

- [ ] 任务 1：扩展 CMakeLists.txt 配置（AC：#2）
  - [ ] 1.1 添加 CMAKE_CXX_STANDARD 17 设置
  - [ ] 1.2 添加 Qt 组件依赖：QuickControls2、Widgets、Concurrent
  - [ ] 1.3 配置 QML 模块和资源文件
  - [ ] 1.4 配置 qtquickcontrols2.conf 资源

- [ ] 任务 2：创建项目目录结构（AC：#3）
  - [ ] 2.1 创建 C++ 源码目录：src/core/, src/interfaces/, src/helpers/, src/account/, src/ui/
  - [ ] 2.2 创建 QML 目录：qml/components/
  - [ ] 2.3 创建配置目录：config/
  - [ ] 2.4 创建配置示例文件：config/config.example.json

- [ ] 任务 3：配置 Material Dark 主题（AC：#4）
  - [ ] 3.1 创建 qtquickcontrols2.conf 配置文件
  - [ ] 3.2 配置 Material Dark 主题参数

- [ ] 任务 4：验证构建（AC：#4）
  - [ ] 4.1 执行 CMake 配置
  - [ ] 4.2 执行构建编译
  - [ ] 4.3 运行空白窗口验证

## Dev Notes

### 项目当前状态

项目已有一个基于 Qt Creator 模板的基础结构：

```
RedDemonScript/
├── CMakeLists.txt      ✅ 基础配置（需扩展）
├── main.cpp            ✅ 基础入口（使用 QGuiApplication）
└── Main.qml            ✅ 基础窗口（空白）
```

### 现有 CMakeLists.txt 分析

**已配置项：**
- cmake_minimum_required(VERSION 3.16) ✅
- project(RedDemonScript VERSION 0.1 LANGUAGES CXX) ✅
- CMAKE_CXX_STANDARD_REQUIRED ON ✅
- find_package(Qt6 REQUIRED COMPONENTS Quick) ⚠️ 仅 Quick
- qt_standard_project_setup(REQUIRES 6.8) ✅
- WIN32_EXECUTABLE TRUE ✅

**缺失项：**
- ❌ CMAKE_CXX_STANDARD 17（仅有 STANDARD_REQUIRED）
- ❌ QuickControls2 组件（Material 主题需要）
- ❌ Widgets 组件（系统托盘需要）
- ❌ Concurrent 组件（多线程需要）
- ❌ qtquickcontrols2.conf 资源配置

### 架构约束（来源：architecture.md）

**必需的 Qt 组件：**
```cmake
find_package(Qt6 REQUIRED COMPONENTS 
    Quick          # 现有
    QuickControls2 # Material 主题
    Widgets        # 系统托盘
    Concurrent     # 多线程
)
```

**目标目录结构：**
```
RedDemonScript/
├── CMakeLists.txt
├── main.cpp
├── Main.qml
├── src/
│   ├── core/           # 核心框架（ScriptController, 三线程, Logger, ConfigManager）
│   ├── interfaces/     # 用户接口（IGameLauncher, IDungeonEntry, IDungeonRunner）
│   ├── helpers/        # API 封装（ImageHelper, InputHelper）
│   ├── account/        # 账号管理（AccountManager, AccountInfo, SandboxieHelper）
│   └── ui/             # UI 相关（SystemTray）
├── qml/
│   └── components/     # 自定义 QML 组件（StatusCard, AccountCard, LogPanel, StatusBar）
└── config/
    └── config.example.json  # 配置示例
```

### 技术规范（来源：project-context.md）

**C++ 标准：** C++17（CMAKE_CXX_STANDARD 17）

**Qt 版本：** Qt 6.8+

**信号槽连接：** 使用新式 Qt5/Qt6 连接语法（函数指针），避免旧式 SIGNAL/SLOT 宏

**命名约定：**
- 类名：PascalCase
- 函数/方法：camelCase
- 成员变量：m_ 前缀 + camelCase
- 常量：全大写 + 下划线
- 接口：I 前缀

### UX 设计规范（来源：ux-design-specification.md）

**主题配置：**
```ini
[Controls]
Style=Material

[Material]
Theme=Dark
Primary=#2196F3
Accent=#FF5722
Background=#1e1e1e
```

**窗口尺寸：**
- 最小：800×600px
- 默认：1024×768px

### 项目结构说明

**目录用途说明：**

| 目录 | 用途 | 后续 Story 将添加的文件 |
|------|------|------------------------|
| src/core/ | 核心框架 | scriptcontroller.h/cpp, scriptthread.h/cpp, monitorthread.h/cpp, timerthread.h/cpp, logger.h/cpp, configmanager.h/cpp |
| src/interfaces/ | 用户接口 | igamelauncher.h, idungeonentry.h, idungeonrunner.h |
| src/helpers/ | API 封装 | imagehelper.h/cpp, inputhelper.h/cpp |
| src/account/ | 账号管理 | accountmanager.h/cpp, accountinfo.h, sandboxiehelper.h/cpp |
| src/ui/ | UI 相关 | systemtray.h/cpp |
| qml/components/ | 自定义组件 | StatusCard.qml, AccountCard.qml, LogPanel.qml, StatusBar.qml |
| config/ | 配置文件 | config.example.json |

### 注意事项

1. **main.cpp 应用类型**：当前使用 `QGuiApplication`，后续添加系统托盘时可能需要改用 `QApplication`（Widgets 组件需要）
2. **资源文件**：qtquickcontrols2.conf 需要作为 Qt 资源文件 (.qrc) 添加到项目
3. **Windows 部署**：现有 windeployqt 配置已正确设置
4. **目录占位**：可以创建 .gitkeep 或 CMakeLists.txt 占位文件确保空目录被 Git 跟踪

### 参考资料

- [来源：architecture.md#项目结构与边界] - 完整目录结构定义
- [来源：architecture.md#起始模板评估] - CMake 配置模板
- [来源：project-context.md#技术栈与版本] - Qt 6.8+, C++17
- [来源：project-context.md#代码质量与风格规则] - 命名约定
- [来源：ux-design-specification.md#设计系统基础] - Material Dark 主题配置

## Dev Agent Record

### 使用的 Agent 模型

{{agent_model_name_version}}

### 调试日志引用

（待开发时填写）

### 完成笔记列表

（待开发时填写）

### 文件列表

**需要修改的文件：**
- CMakeLists.txt（扩展配置）

**需要创建的文件：**
- src/core/.gitkeep
- src/interfaces/.gitkeep
- src/helpers/.gitkeep
- src/account/.gitkeep
- src/ui/.gitkeep
- qml/components/.gitkeep
- config/config.example.json
- qtquickcontrols2.conf（或作为资源文件）
