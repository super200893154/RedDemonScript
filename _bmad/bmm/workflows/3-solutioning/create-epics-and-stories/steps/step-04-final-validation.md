---
name: 'step-04-final-validation'
description: '验证所有需求的完整覆盖并确保实施准备度'

# 路径定义
workflow_path: '{project-root}/_bmad/bmm/workflows/3-solutioning/create-epics-and-stories'

# 文件引用
thisStepFile: './step-04-final-validation.md'
workflowFile: '{workflow_path}/workflow.md'
outputFile: '{planning_artifacts}/epics.md'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'

# 模板引用
epicsTemplate: '{workflow_path}/templates/epics-template.md'
---

# 步骤 4：最终验证

## 步骤目标：

验证所有需求的完整覆盖并确保 story 准备好进行开发。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 绝不在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前阅读完整的步骤文件
- 🔄 关键：按顺序处理验证，不跳过
- 📋 你是协调者，而非内容生成者
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

### 角色强化：

- ✅ 你是产品策略师和技术规格文档撰写者
- ✅ 如果你已被赋予沟通或人设模式，在扮演此新角色时继续使用
- ✅ 我们进行协作对话，而非命令-响应
- ✅ 你带来验证专业知识和质量保证
- ✅ 用户带来他们的实施优先级和最终审查

### 步骤特定规则：

- 🎯 仅专注于验证完整需求覆盖
- 🚫 禁止跳过任何验证检查
- 💬 验证 FR 覆盖、story 完整性和依赖
- 🚪 确保所有 story 准备好开发

## 执行协议：

- 🎯 验证每个需求有 story 覆盖
- 💾 检查 story 依赖和流程
- 📖 验证架构合规性
- 🚫 禁止批准不完整的覆盖

## 上下文边界：

- 可用上下文：前序步骤的完整 epic 和 story 分解
- 专注：最终验证需求覆盖和 story 准备度
- 限制：仅验证，无新内容创建
- 依赖：步骤3完成的 story 生成

## 验证流程：

### 1. FR 覆盖验证

审查完整的 epic 和 story 分解，确保每个 FR 被覆盖：

**关键检查：**

- 遍历需求清单中的每个 FR
- 验证它出现在至少一个 story 中
- 检查验收标准完全解决 FR
- 不应遗留未覆盖的 FR

### 2. 架构实施验证

**检查起始模板设置：**

- 架构文档是否指定起始模板？
- 如果是：Epic 1 Story 1 必须是"从起始模板设置初始项目"
- 这包括克隆、安装依赖、初始配置

**数据库/实体创建验证：**

- 数据库表/实体是否仅在 story 需要时创建？
- ❌ 错误：Epic 1 预先创建所有表
- ✅ 正确：表作为需要它们的第一个 story 的一部分创建
- 每个 story 应仅创建/修改它需要的

### 3. Story 质量验证

**每个 story 必须：**

- 可由单个开发 Agent 完成
- 有清晰的验收标准
- 引用具体实施的 FR
- 包含必要的技术细节
- **没有前向依赖**（只能依赖之前的 story）
- 可实施而无需等待未来 story

### 4. Epic 结构验证

**检查：**

- Epic 交付用户价值，而非技术里程碑
- 依赖自然流动
- 基础 story 仅设置所需内容
- 无大量前期技术工作

### 5. 依赖验证（关键）

**Epic 独立性检查：**

- 每个 epic 是否为其领域交付完整功能？
- Epic 2 是否可以在不实施 Epic 3 的情况下运行？
- Epic 3 是否可以使用 Epic 1 和 2 的输出独立运行？
- ❌ 错误：Epic 2 需要 Epic 3 的功能才能工作
- ✅ 正确：每个 epic 独立有价值

**Epic 内 Story 依赖检查：**
对于每个 epic，按顺序审查 story：

- Story N.1 是否可以在没有 Stories N.2、N.3 等的情况下完成？
- Story N.2 是否可以仅使用 Story N.1 的输出完成？
- Story N.3 是否可以仅使用 Stories N.1 和 N.2 的输出完成？
- ❌ 错误："此 story 依赖于未来 story"
- ❌ 错误：Story 引用尚未实施的功能
- ✅ 正确：每个 story 仅建立在之前 story 之上

### 6. 完成并保存

如果所有验证通过：

- 更新文档中任何剩余占位符
- 确保格式正确
- 保存最终 epics.md

**呈现最终菜单：**
**所有验证完成！** [C] 完成工作流

选择 C 时，工作流完成，epics.md 准备开发。

Epic 和 Story 完成。完整阅读并遵循：`{project-root}/_bmad/core/tasks/help.md`

完成任务输出后：主动回答关于 Epic 和 Story 的任何问题。