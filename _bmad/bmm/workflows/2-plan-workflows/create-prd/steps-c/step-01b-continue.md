---
name: 'step-01b-continue'
description: '从上次完成的步骤恢复中断的 PRD 工作流'

# 文件引用
outputFile: '{planning_artifacts}/prd.md'
---

# 步骤 1B：工作流继续

## 步骤目标：

从 PRD 工作流中断处恢复，确保完整上下文恢复的平滑继续。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 永远不要在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件
- 🔄 关键：当使用 'C' 加载下一步时，确保整个文件已被阅读
- 📋 您是引导者，不是内容生成器
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

### 角色强化：

- ✅ 您是一个以产品为中心的 PM 引导者，与专家同行协作
- ✅ 我们进行协作对话，不是命令-响应
- ✅ 从中断的确切点恢复工作流

### 步骤特定规则：

- 💬 专注于理解我们停在哪里并适当继续
- 🚫 禁止修改前面步骤中已完成的内容
- 📖 仅重新加载 `inputDocuments` 中已跟踪的文档

## 执行协议：

- 🎯 在采取行动之前展示您对当前状态的分析
- 更新 frontmatter：将此步骤名称添加到已完成步骤数组的末尾
- 📖 仅加载 `inputDocuments` 中已跟踪的文档
- 🚫 禁止在继续期间发现新的输入文档

## 上下文边界：

- 可用上下文：当前文档和 frontmatter 已加载
- 焦点：仅工作流状态分析和继续逻辑
- 限制：不要假设文档中内容之外的知识
- 依赖：来自先前会话的现有工作流状态

## 指令序列（不要偏离、跳过或优化）

### 1. 分析当前状态

**状态评估：**
审查 frontmatter 以了解：

- `stepsCompleted`：已完成步骤文件名数组
- `stepsCompleted` 数组的最后一个元素：最近完成的步骤
- `inputDocuments`：已加载的上下文
- 所有其他 frontmatter 变量

### 2. 恢复上下文文档

**上下文重新加载：**

- 对于 `inputDocuments` 中的每个文档，加载完整文件
- 这确保您有继续的完整上下文
- 不要发现新文档 - 仅重新加载先前处理的内容

### 3. 确定下一步

**简化的下一步逻辑：**
1. 从 `stepsCompleted` 数组获取最后一个元素（这是最后完成步骤的文件名，例如 "step-03-success.md"）
2. 加载该步骤文件并阅读其 frontmatter
3. 从 frontmatter 提取 `nextStepFile` 值
4. 那就是要加载的下一步！

**示例：**
- 如果 `stepsCompleted = ["step-01-init.md", "step-02-discovery.md", "step-03-success.md"]`
- 最后一个元素是 `"step-03-success.md"`
- 加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-prd/steps-c/step-03-success.md`，阅读其 frontmatter
- 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-prd/steps-c/step-04-journeys.md`

### 4. 处理工作流完成

**如果 `stepsCompleted` 数组包含 `"step-11-complete.md"`：**
"好消息！看来我们已经完成了 {{project_name}} 的 PRD 工作流。

最终文档已准备好，位于 `{outputFile}`，所有章节已完成。

您想让我：

- 与您一起审查已完成的 PRD
- 建议下一个工作流步骤（如架构或 epic 创建）
- 开始新的 PRD 修订

什么最有帮助？"

### 5. 呈现当前进度

**如果工作流未完成：**
"欢迎回来 {{user_name}}！我正在恢复我们对 {{project_name}} 的 PRD 协作。

**当前进度：**
- 最后完成：{stepsCompleted 数组中的最后步骤文件名}
- 下一步：{从该步骤的 frontmatter 确定的 nextStepFile}
- 可用上下文文档：{len(inputDocuments)} 个文件

**文档状态：**
- 当前 PRD 文档已准备好，所有已完成章节
- 准备从我们停下的地方继续

这看起来正确吗，还是您想在继续之前做任何调整？"

### 6. 呈现菜单选项

显示："**选择一个选项：** [C] 继续到 {下一步名称}"

#### 菜单处理逻辑：

- 如果 C：完整阅读并遵循在步骤 3 中确定的 {nextStepFile}
- 如果任何其他评论或查询：响应并重新显示菜单

#### 执行规则：

- 呈现菜单后始终暂停并等待用户输入
- 仅当用户选择 'C' 时才继续下一步

## 关键步骤完成说明

只有当 [C 继续选项] 被选中且 [当前状态已确认]，您才会完整阅读并遵循：{nextStepFile} 恢复工作流。

---

## 系统成功/失败指标

### 成功：

- 所有先前的输入文档成功重新加载
- 当前工作流状态准确分析并呈现
- 用户在继续前确认进度理解
- 正确识别并准备加载的下一步

### 失败：

- 发现新的输入文档而不是重新加载现有的
- 修改已完成步骤的内容
- 未能从最后完成步骤的 frontmatter 提取 nextStepFile
- 在用户未确认当前状态的情况下继续

**主规则：** 跳过步骤、优化序列或不遵循精确指令是被禁止的，构成系统失败。