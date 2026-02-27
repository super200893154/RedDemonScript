---
name: 'step-01-validate-prerequisites'
description: '验证必需文档存在并提取所有需求用于 epic 和 story 创建'

# 路径定义
workflow_path: '{project-root}/_bmad/bmm/workflows/3-solutioning/create-epics-and-stories'

# 文件引用
thisStepFile: './step-01-validate-prerequisites.md'
nextStepFile: './step-02-design-epics.md'
workflowFile: '{workflow_path}/workflow.md'
outputFile: '{planning_artifacts}/epics.md'
epicsTemplate: '{workflow_path}/templates/epics-template.md'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'

# 模板引用
epicsTemplate: '{workflow_path}/templates/epics-template.md'
---

# 步骤 1：验证前提条件并提取需求

## 步骤目标：

验证所有必需的输入文档存在，并提取 epic 和 story 创建所需的所有需求（FR、NFR 以及来自 UX/架构的额外需求）。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 绝不在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前阅读完整的步骤文件
- 🔄 关键：当使用 'C' 加载下一步时，确保完整阅读文件
- 📋 你是协调者，而非内容生成者
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

### 角色强化：

- ✅ 你是产品策略师和技术规格文档撰写者
- ✅ 如果你已被赋予沟通或人设模式，在扮演此新角色时继续使用
- ✅ 我们进行协作对话，而非命令-响应
- ✅ 你带来需求提取专业知识
- ✅ 用户带来他们的产品愿景和上下文

### 步骤特定规则：

- 🎯 仅专注于提取和组织需求
- 🚫 禁止在此步骤中开始创建 epic 或 story
- 💬 从所有可用文档中提取需求
- 🚪 完全按照需要填充模板部分

## 执行协议：

- 🎯 从所有文档系统地提取需求
- 💾 用提取的需求填充 {outputFile}
- 📖 用提取进度更新 frontmatter
- 🚫 禁止在用户选择 'C' 且需求提取完成之前加载下一步

## 需求提取流程：

### 1. 欢迎和概览

欢迎 {user_name} 进行全面的 epic 和 story 创建！

**关键前提条件验证：**

验证必需文档存在且完整：

1. **PRD.md** - 包含需求（FR 和 NFR）和产品范围
2. **Architecture.md** - 包含技术决策、API 合约、数据模型
3. **UX Design.md**（如果存在 UI）- 包含交互模式、模型、用户流程

### 2. 文档发现和验证

使用以下模式搜索必需文档（分片意味着大型文档被拆分为多个带有 index.md 的小文件放入文件夹）- 如果找到完整文档，使用它而非分片版本：

**PRD 文档搜索优先级：**

1. `{planning_artifacts}/*prd*.md`（完整文档）
2. `{planning_artifacts}/*prd*/index.md`（分片版本）

**架构文档搜索优先级：**

1. `{planning_artifacts}/*architecture*.md`（完整文档）
2. `{planning_artifacts}/*architecture*/index.md`（分片版本）

**UX 设计文档搜索（可选）：**

1. `{planning_artifacts}/*ux*.md`（完整文档）
2. `{planning_artifacts}/*ux*/index.md`（分片版本）

在继续之前，询问用户是否有其他文档需要包含进行分析，以及发现的任何内容是否应该排除。等待用户确认。确认后，从 {epicsTemplate} 创建 {outputFile}，并在 frontmatter 的 `inputDocuments: []` 数组中列出文件。

### 3. 提取功能需求（FR）

从 PRD 文档（完整或分片），阅读整个文档并提取所有功能需求：

**提取方法：**

- 查找编号项目如 "FR1:"、"Functional Requirement 1:" 或类似
- 识别描述系统必须做什么的需求声明
- 包括用户操作、系统行为和业务规则

**格式化 FR 列表：**

```
FR1: [清晰、可测试的需求描述]
FR2: [清晰、可测试的需求描述]
...
```

### 4. 提取非功能需求（NFR）

从 PRD 文档提取所有非功能需求：

**提取方法：**

- 查找性能、安全、可用性、可靠性需求
- 识别约束和质量属性
- 包括技术标准和合规需求

**格式化 NFR 列表：**

```
NFR1: [性能/安全/可用性需求]
NFR2: [性能/安全/可用性需求]
...
```

### 5. 从架构中提取额外需求

审查架构文档中影响 epic 和 story 创建的技术需求：

**查找：**

- **起始模板**：架构是否指定了起始/绿地模板？如果是，为 Epic 1 Story 1 记录此信息
- 基础设施和部署需求
- 与外部系统的集成需求
- 数据迁移或设置需求
- 监控和日志需求
- API 版本控制或兼容性需求
- 安全实施需求

**重要**：如果架构中提到起始模板，请显著标记。这将影响 Epic 1 Story 1。

**格式化额外需求：**

```
- [影响实施的架构技术需求]
- [基础设施设置需求]
- [集成需求]
...
```

### 6. 从 UX 中提取额外需求（如存在）

审查 UX 文档中影响 epic 和 story 创建的需求：

**查找：**

- 响应式设计需求
- 可访问性需求
- 浏览器/设备兼容性
- 需要实施的用户交互模式
- 动画或过渡需求
- 错误处理 UX 需求

**将这些添加到额外需求列表。**

### 7. 加载并初始化模板

加载 {epicsTemplate} 并初始化 {outputFile}：

1. 将整个模板复制到 {outputFile}
2. 用实际项目名称替换 {{project_name}}
3. 用提取的需求替换占位符部分：
   - {{fr_list}} → 提取的 FR
   - {{nfr_list}} → 提取的 NFR
   - {{additional_requirements}} → 提取的额外需求
4. 暂时保留 {{requirements_coverage_map}} 和 {{epics_list}} 作为占位符

### 8. 呈现提取的需求

向用户显示：

**提取的功能需求：**

- 显示发现的 FR 数量
- 显示前几个 FR 作为示例
- 询问是否有任何 FR 遗漏或捕获不正确

**提取的非功能需求：**

- 显示发现的 NFR 数量
- 显示关键 NFR
- 询问是否遗漏了任何约束

**额外需求：**

- 总结架构中的技术需求
- 总结 UX 需求（如适用）
- 验证完整性

### 9. 获取用户确认

询问："这些提取的需求是否准确代表了需要构建的内容？有任何添加或修正吗？"

根据用户反馈更新需求，直到收到确认。

## 保存到文档的内容：

提取和确认后，用以下内容更新 {outputFile}：

- {{fr_list}} 部分的完整 FR 列表
- {{nfr_list}} 部分的完整 NFR 列表
- {{additional_requirements}} 部分的所有额外需求

### 10. 呈现菜单选项

显示：`**确认需求完整正确以 [C] 继续：**`

#### 执行规则：

- 始终在呈现菜单后暂停并等待用户输入
- 仅当用户选择 'C' 时进入下一步
- 用户可以聊天或提问 - 始终响应，然后再次显示菜单选项

#### 菜单处理逻辑：

- 如果 C：保存所有内容到 {outputFile}，更新 frontmatter，然后完整阅读并遵循：{nextStepFile}
- 如果有任何其他评论或查询：帮助用户响应，然后[重新显示菜单选项](#10-present-menu-options)

## 关键步骤完成说明

只有当选择 C 且所有需求已保存到文档并更新 frontmatter 后，你才会完整阅读并遵循：{nextStepFile} 开始 epic 设计步骤。

---

## 🚨 系统成功/失败指标

### ✅ 成功：

- 找到并验证所有必需文档
- 提取并正确格式化所有 FR
- 提取并正确格式化所有 NFR
- 识别架构/UX 中的额外需求
- 用需求初始化模板
- 用户确认需求完整准确

### ❌ 系统失败：

- 缺少必需文档
- 需求提取不完整
- 模板未正确初始化
- 未将需求保存到输出文件

**主规则：** 跳过步骤、优化序列或不遵循确切指令是被禁止的，构成系统失败。