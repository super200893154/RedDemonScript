# 步骤 1B：UX 设计工作流继续

## 强制执行规则（首先阅读）：

- 🛑 永远不要在没有用户输入的情况下生成内容

- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件 - 部分理解会导致不完整的决策
- 🔄 关键：当使用 'C' 加载下一步时，确保已完整阅读并理解整个文件
- ✅ 始终将此视为 UX 引导者与利益相关者之间的协作探索
- 📋 您是 UX 引导者，不是内容生成器
- 💬 专注于理解我们中断的位置并适当继续
- 🚪 从中断的确切点恢复工作流
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取行动之前展示您对当前状态的分析
- 💾 保留现有 frontmatter `stepsCompleted` 值
- 📖 仅加载已在 `inputDocuments` 中跟踪的文档
- 🚫 禁止修改先前步骤中完成的内容

## 上下文边界：

- 当前文档和 frontmatter 已加载
- 先前上下文 = 完整文档 + 现有 frontmatter
- Frontmatter 中列出的输入文档已处理
- 最后完成的步骤 = frontmatter 中的 `lastStep` 值

## 您的任务：

从中断的位置恢复 UX 设计工作流，确保平滑继续。

## 继续序列：

### 1. 分析当前状态

审查 frontmatter 以理解：

- `stepsCompleted`：哪些步骤已完成
- `lastStep`：最近完成的步骤编号
- `inputDocuments`：已加载什么上下文
- 所有其他 frontmatter 变量

### 2. 加载所有输入文档

重新加载 `inputDocuments` 中列出的上下文文档：

- 对于 `inputDocuments` 中的每个文档，加载完整文件
- 这确保您有完整的上下文继续
- 不要发现新文档 - 仅重新加载先前处理的内容

### 3. 总结当前进度

欢迎用户回来并提供上下文：
"欢迎回来 {{user_name}}！我正在继续我们为 {{project_name}} 的 UX 设计协作。

**当前进度：**

- 已完成步骤：{stepsCompleted}
- 最后工作步骤：步骤 {lastStep}
- 可用上下文文档：{len(inputDocuments)} 个文件
- 当前 UX 设计规范已准备好所有完成的部分

**文档状态：**

- 当前 UX 设计文档已准备好所有完成的部分
- 准备从中断的位置继续

这看起来正确吗，还是您想在继续之前进行任何调整？"

### 4. 确定下一步

基于 `lastStep` 值，确定要加载哪个步骤：

- 如果 `lastStep = 1` → 加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-02-discovery.md`
- 如果 `lastStep = 2` → 加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-03-core-experience.md`
- 如果 `lastStep = 3` → 加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-04-emotional-response.md`
- 继续此模式用于所有步骤
- 如果 `lastStep` 指示最终步骤 → 工作流已完成

### 5. 呈现继续选项

呈现当前进度后，询问：
"准备继续步骤 {nextStepNumber}：{nextStepTitle}？

[C] 继续到步骤 {nextStepNumber}"

## 成功指标：

✅ 成功重新加载所有先前的输入文档
✅ 准确分析并呈现当前工作流状态
✅ 用户确认理解进度
✅ 识别并准备正确的下一步

## 失败模式：

❌ 发现新的输入文档而不是重新加载现有文档
❌ 修改已完成步骤的内容
❌ 基于 `lastStep` 值加载错误的下一步
❌ 在未确认当前状态的情况下继续

❌ **关键**：仅读取部分步骤文件 - 导致理解不完整和决策不当
❌ **关键**：在未完整阅读和理解下一步文件的情况下继续 'C'
❌ **关键**：在未完全理解步骤要求和协议的情况下做出决策

## 工作流已完成？

如果 `lastStep` 指示最终步骤已完成：
"好消息！看起来我们已经完成了 {{project_name}} 的 UX 设计工作流。

最终 UX 设计规范已准备好，位于 {output_folder}/ux-design-specification.md，所有部分都已完成，直到步骤 {finalStepNumber}。

完整的 UX 设计包括视觉基础、用户流程和准备好实施的 设计规范。

您希望我：

- 与您一起审查完成的 UX 设计规范
- 建议下一步工作流（如线框图生成或架构）
- 开始新的 UX 设计修订

什么最有帮助？"

## 下一步：

用户确认准备继续后，根据 frontmatter 中的 `lastStep` 值加载适当的下一步文件。

记住：在用户明确选择 [C] 继续之前不要加载下一步！