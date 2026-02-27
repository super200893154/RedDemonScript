# 步骤 1b：工作流继续处理器

## 强制执行规则（首先阅读）：

- 🛑 绝不在没有用户输入的情况下生成内容

- 📖 关键：始终在采取任何行动之前阅读完整的步骤文件 - 部分理解会导致不完整的决策
- 🔄 关键：当使用 'C' 加载下一步时，确保完整阅读并理解文件后再继续
- ✅ 始终将此视为架构同伴之间的协作探索
- 📋 你是协调者，而非内容生成者
- 💬 专注于理解当前状态并获取用户确认
- 🚪 平滑透明地处理工作流恢复
- ⚠️ 绝对禁止时间估算 - AI 开发速度已经发生了根本性变化
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示你的分析
- 📖 完整阅读现有文档以理解当前状态
- 💾 更新 frontmatter 以反映继续
- 🚫 禁止在无用户确认的情况下进入下一步

## 上下文边界：

- 现有文档和 frontmatter 可用
- 已加载的输入文档应在 frontmatter `inputDocuments` 中
- 已完成的步骤在 `stepsCompleted` 数组中
- 专注于理解我们停在哪里

## 你的任务：

通过分析现有工作并引导用户在适当的步骤恢复来处理工作流继续。

## 继续序列：

### 1. 分析当前文档状态

完整阅读现有架构文档并分析：

**Frontmatter 分析：**

- `stepsCompleted`：已完成哪些步骤
- `inputDocuments`：加载了哪些文档
- `lastStep`：最后执行的步骤
- `project_name`、`user_name`、`date`：基本上下文

**内容分析：**

- 文档中存在哪些部分
- 已做出哪些架构决策
- 哪些看起来不完整或进行中
- 任何剩余的 TODO 或占位符

### 2. 呈现继续摘要

向用户展示当前进度：

"欢迎回来 {{user_name}}！我发现了您关于 {{project_name}} 的架构工作。

**当前进度：**

- 已完成步骤：{{stepsCompleted 列表}}
- 最后工作的步骤：步骤 {{lastStep}}
- 加载的输入文档：{{inputDocuments 数量}} 个文件

**发现的文档部分：**
{列出文档中所有 H2/H3 部分}

{if_incomplete_sections}
**不完整区域：**

- {看起来不完整或有占位符的区域}
  {/if_incomplete_sections}

**您想做什么？**
[R] 从我们停下的地方恢复
[C] 继续到下一个逻辑步骤
[O] 查看所有剩余步骤概览
[X] 重新开始（将覆盖现有工作）
"

### 3. 处理用户选择

#### 如果 'R'（从停下的地方恢复）：

- 根据 `stepsCompleted` 识别下一步
- 加载适当的步骤文件继续
- 例如：如果 `stepsCompleted: [1, 2, 3]`，加载 `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-04-decisions.md`

#### 如果 'C'（继续到下一个逻辑步骤）：

- 分析文档内容确定逻辑下一步
- 可能需要审查内容质量和完整性
- 如果当前步骤内容看起来完整，前进到下一步
- 如果内容看起来不完整，建议停留在当前步骤

#### 如果 'O'（查看所有剩余步骤概览）：

- 提供所有剩余步骤的简要描述
- 让用户选择要处理的步骤
- 不假设顺序推进总是最佳选择

#### 如果 'X'（重新开始）：

- 确认："这将删除所有现有架构决策。您确定吗？(y/n)"
- 如果确认：删除现有文档，完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-01-init.md`
- 如果不确认：返回继续菜单

### 4. 导航到选定的步骤

用户做出选择后：

**加载选定的步骤文件：**

- 更新 frontmatter `lastStep` 以反映当前导航
- 执行选定的步骤文件
- 让该步骤处理详细的继续逻辑

**状态保留：**

- 维护文档中的所有现有内容
- 保持 `stepsCompleted` 准确
- 在工作流状态中跟踪恢复

### 5. 特殊继续情况

#### 如果 `stepsCompleted` 为空但文档有内容：

- 这暗示工作流被中断
- 询问用户："我看到文档有内容但没有标记任何步骤完成。我应该分析这里的内容并设置适当的步骤状态吗？"

#### 如果文档看起来损坏或不完整：

- 询问用户："文档看起来不完整。您想让我尝试恢复这里的内容，还是更愿意重新开始？"

#### 如果文档完整但工作流未标记为完成：

- 询问用户："架构看起来已完成！我应该将此工作流标记为完成，还是您想继续处理？"

## 成功指标：

✅ 正确分析和理解现有文档状态
✅ 向用户呈现清晰的继续选项
✅ 适当地透明处理用户选择
✅ 正确保留和更新工作流状态
✅ 平滑处理导航到适当步骤

## 失败模式：

❌ 在提出建议之前未完整阅读现有文档
❌ 丢失实际完成步骤的跟踪
❌ 未经用户确认下一步就自动继续
❌ 未检查不完整或占位符内容
❌ 在恢复期间丢失现有文档内容

❌ **关键**：仅读取部分步骤文件 - 导致理解不完整和决策不当
❌ **关键**：未完整阅读并理解下一步文件就使用 'C' 继续
❌ **关键**：未完整理解步骤要求和协议就做出决策

## 下一步：

用户选择继续选项后，根据选择加载适当的步骤文件。步骤文件将从此处处理详细工作。

可加载的有效步骤文件：
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-02-context.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-03-starter.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-04-decisions.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-05-patterns.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-06-structure.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-07-validation.md`
- `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-08-complete.md`

记住：目标是平滑、透明的恢复，尊重已完成的工作，同时让用户控制如何继续。