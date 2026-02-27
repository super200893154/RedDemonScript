---
name: 'step-02-design-epics'
description: '设计并批准将所有需求组织为以用户价值为中心的 epic 的 epics_list'

# 路径定义
workflow_path: '{project-root}/_bmad/bmm/workflows/3-solutioning/create-epics-and-stories'

# 文件引用
thisStepFile: './step-02-design-epics.md'
nextStepFile: './step-03-create-stories.md'
workflowFile: '{workflow_path}/workflow.md'
outputFile: '{planning_artifacts}/epics.md'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'

# 模板引用
epicsTemplate: '{workflow_path}/templates/epics-template.md'
---

# 步骤 2：设计 Epic 列表

## 步骤目标：

设计并获得批准，将所有需求组织为以用户价值为中心的 epic 的 epics_list。

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
- ✅ 你带来产品策略和 epic 设计专业知识
- ✅ 用户带来他们的产品愿景和优先级

### 步骤特定规则：

- 🎯 仅专注于创建 epics_list
- 🚫 禁止在此步骤中创建单个 story
- 💬 围绕用户价值而非技术层组织 epic
- 🚪 获得对 epics_list 的明确批准
- 🔗 **关键：每个 epic 必须独立且能够启用未来 epic，而不需要未来 epic 才能运行**

## 执行协议：

- 🎯 基于提取的需求协作设计 epic
- 💾 更新 {outputFile} 中的 {{epics_list}}
- 📖 记录 FR 覆盖映射
- 🚫 禁止在用户批准 epics_list 之前加载下一步

## Epic 设计流程：

### 1. 审查提取的需求

加载 {outputFile} 并审查：

- **功能需求：** 计数并审查步骤1的 FR
- **非功能需求：** 审查需要解决的 NFR
- **额外需求：** 审查技术和 UX 需求

### 2. 解释 Epic 设计原则

**EPIC 设计原则：**

1. **用户价值优先**：每个 epic 必须让用户能够完成有意义的事情
2. **需求分组**：将交付一致用户成果的相关 FR 分组
3. **增量交付**：每个 epic 应独立交付价值
4. **逻辑流程**：从用户角度看的自然进程
5. **🔗 Epic 内无依赖**：Epic 内的 story 必须不依赖于未来 story

**⚠️ 关键原则：**
按用户价值组织，而非技术层：

**✅ 正确的 Epic 示例（独立且启用未来 Epic）：**

- Epic 1：用户认证和个人资料（用户可以注册、登录、管理个人资料）- **独立：完整的认证系统**
- Epic 2：内容创建（用户可以创建、编辑、发布内容）- **独立：使用认证，创建内容**
- Epic 3：社交互动（用户可以关注、评论、点赞内容）- **独立：使用认证 + 内容**
- Epic 4：搜索和发现（用户可以找到内容和其他用户）- **独立：使用所有之前的**

**❌ 错误的 Epic 示例（技术层或依赖）：**

- Epic 1：数据库设置（预先创建所有表）- **无用户价值**
- Epic 2：API 开发（构建所有端点）- **无用户价值**
- Epic 3：前端组件（创建可复用组件）- **无用户价值**
- Epic 4：部署管道（CI/CD 设置）- **无用户价值**

**🔗 依赖规则：**

- 每个 epic 必须为其领域交付完整功能
- Epic 2 必须不需要 Epic 3 就能运行
- Epic 3 可以建立在 Epic 1 和 2 之上，但必须独立

### 3. 协作设计 Epic 结构

**步骤 A：识别用户价值主题**

- 在 FR 中寻找自然分组
- 识别用户旅程或工作流
- 考虑用户类型及其目标

**步骤 B：提议 Epic 结构**
对于每个提议的 epic：

1. **Epic 标题**：以用户为中心，价值导向
2. **用户成果**：用户在此 epic 后可以完成什么
3. **FR 覆盖**：此 epic 涵盖哪些 FR 编号
4. **实施说明**：任何技术或 UX 考虑

**步骤 C：创建 epics_list**

格式化 epics_list 为：

```
## Epic 列表

### Epic 1: [Epic 标题]
[Epic 目标声明 - 用户可以完成什么]
**覆盖的 FR：** FR1, FR2, FR3 等

### Epic 2: [Epic 标题]
[Epic 目标声明 - 用户可以完成什么]
**覆盖的 FR：** FR4, FR5, FR6 等

[继续所有 epic]
```

### 4. 呈现 Epic 列表以供审查

向用户显示完整的 epics_list，包括：

- epic 总数
- 每个 epic 的 FR 覆盖
- 每个 epic 交付的用户价值
- 任何自然依赖

### 5. 创建需求覆盖图

创建 {{requirements_coverage_map}} 显示每个 FR 如何映射到 epic：

```
### FR 覆盖图

FR1: Epic 1 - [简要描述]
FR2: Epic 1 - [简要描述]
FR3: Epic 2 - [简要描述]
...
```

这确保没有 FR 被遗漏。

### 6. 协作优化

询问用户：

- "这个 epic 结构是否与您的产品愿景一致？"
- "所有用户成果是否正确捕获？"
- "我们应该调整任何 epic 分组吗？"
- "是否有我们遗漏的自然依赖？"

### 7. 获得最终批准

**关键：** 必须获得明确的用户批准：
"您是否批准这个 epic 结构以继续进行 story 创建？"

如果用户想要更改：

- 进行请求的调整
- 更新 epics_list
- 重新呈请批准
- 重复直到获得批准

## 更新到文档的内容：

批准后，更新 {outputFile}：

1. 用批准的 epic 列表替换 {{epics_list}} 占位符
2. 用覆盖图替换 {{requirements_coverage_map}}
3. 确保所有 FR 映射到 epic

### 8. 呈现菜单选项

显示：`**选择一个选项：** [A] 高级引导 [P] 多角色模式 [C] 继续`

#### 菜单处理逻辑：

- 如果 A：完整阅读并遵循：{advancedElicitationTask}
- 如果 P：完整阅读并遵循：{partyModeWorkflow}
- 如果 C：将批准的 epics_list 保存到 {outputFile}，更新 frontmatter，然后完整阅读并遵循：{nextStepFile}
- 如果有任何其他评论或查询：帮助用户响应，然后[重新显示菜单选项](#8-present-menu-options)

#### 执行规则：

- 始终在呈现菜单后暂停并等待用户输入
- 仅当用户选择 'C' 时进入下一步
- 其他菜单项执行完成后，重新显示菜单
- 用户可以聊天或提问 - 对话结束时始终响应，重新显示菜单选项

## 关键步骤完成说明

只有当选择 C 且批准的 epics_list 已保存到文档后，你才会完整阅读并遵循：{nextStepFile} 开始 story 创建步骤。

---

## 🚨 系统成功/失败指标

### ✅ 成功：

- Epic 围绕用户价值设计
- 所有 FR 映射到具体 epic
- epics_list 创建并正确格式化
- 需求覆盖图完成
- 用户明确批准 epic 结构
- 文档用批准的 epic 更新

### ❌ 系统失败：

- Epic 按技术层组织
- 覆盖图中缺少 FR
- 未获得用户批准
- epics_list 未保存到文档

**主规则：** 跳过步骤、优化序列或不遵循确切指令是被禁止的，构成系统失败。