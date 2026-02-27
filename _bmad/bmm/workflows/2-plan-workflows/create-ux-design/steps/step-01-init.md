# 步骤 1：UX 设计工作流初始化

## 强制执行规则（首先阅读）：

- 🛑 永远不要在没有用户输入的情况下生成内容

- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件 - 部分理解会导致不完整的决策
- 🔄 关键：当使用 'C' 加载下一步时，确保已完整阅读并理解整个文件
- ✅ 始终将此视为 UX 引导者与利益相关者之间的协作探索
- 📋 您是 UX 引导者，不是内容生成器
- 💬 仅专注于初始化和设置 - 不要向前看未来步骤
- 🚪 检测现有工作流状态并正确处理继续
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示您的分析
- 💾 初始化文档并更新 frontmatter
- 📖 在加载下一步之前设置 frontmatter `stepsCompleted: [1]`
- 🚫 在设置完成之前禁止加载下一步

## 上下文边界：

- 来自 workflow.md 的变量在内存中可用
- 先前上下文 = 输出文档中的内容 + frontmatter
- 不要假设来自其他步骤的知识
- 输入文档发现发生在此步骤中

## 您的任务：

通过检测继续状态和设置设计规范文档来初始化 UX 设计工作流。

## 初始化序列：

### 1. 检查现有工作流

首先，检查输出文档是否已存在：

- 在 `{planning_artifacts}/*ux-design-specification*.md` 查找文件
- 如果存在，阅读包括 frontmatter 在内的完整文件
- 如果不存在，这是一个新工作流

### 2. 处理继续（如果文档存在）

如果文档存在且 frontmatter 中有 `stepsCompleted`：

- **在此停止**并立即加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-01b-continue.md`
- 不要继续进行任何初始化任务
- 让 step-01b 处理继续逻辑

### 3. 新工作流设置（如果没有文档）

如果没有文档存在或 frontmatter 中没有 `stepsCompleted`：

#### A. 输入文档发现

使用智能发现发现并加载上下文文档。文档可以在以下位置：
- {planning_artifacts}/**
- {output_folder}/**
- {product_knowledge}/**
- docs/**

此外 - 搜索时 - 文档可以是单个 markdown 文件，或包含索引和多个文件的文件夹。例如，如果搜索 `*foo*.md` 未找到，还要搜索名为 *foo*/index.md 的文件夹（这表示分片内容）

尝试发现以下内容：
- 产品简报（`*brief*.md`）
- 研究文档（`*prd*.md`）
- 项目文档（通常在 `{product_knowledge}` 或 `docs` 文件夹中可能找到多个文档）
- 项目上下文（`**/project-context.md`）

<critical>与用户确认您找到的内容，并询问用户是否想提供其他任何内容。只有在确认后，您才会继续遵循加载规则</critical>

**加载规则：**

- 完整加载用户确认或提供的所有发现文件（无偏移/限制）
- 如果有项目上下文，相关内容应在此后的整个工作流过程中有所倾向
- 对于分片文件夹，加载所有文件以获取完整图景，首先使用索引以潜在了解每个文档的内容
- 在 frontmatter `inputDocuments` 数组中跟踪所有成功加载的文件

#### B. 创建初始文档

**文档设置：**

- 将模板从 `{installed_path}/ux-design-template.md` 复制到 `{planning_artifacts}/ux-design-specification.md`
- 用适当结构初始化 frontmatter，包括 inputDocuments 数组。

#### C. 完成初始化并报告

完成设置并向用户报告：

**文档设置：**

- 已创建：`{planning_artifacts}/ux-design-specification.md` 来自模板
- 已用工作流状态初始化 frontmatter

**已发现的输入文档：**
报告找到的内容：
"欢迎 {{user_name}}！我已为 {{project_name}} 设置了您的 UX 设计工作区。

**找到的文档：**

- PRD：{已加载的 PRD 文件数量或"未找到"}
- 产品简报：{已加载的简报文件数量或"未找到"}
- 其他上下文：{已加载的其他文件数量或"未找到"}

**已加载的文件：** {具体文件名列表或"未找到其他文档"}

您是否有其他文档想让我包含，还是我们继续下一步？

[C] 继续到 UX 探索"

## 下一步：

用户选择 [C] 继续后，确保文件 `{planning_artifacts}/ux-design-specification.md` 已创建并保存，然后加载 `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-02-discovery.md` 开始 UX 探索阶段。

记住：在输出文件更新且用户明确选择 [C] 继续之前，不要进入 step-02！

## 成功指标：

✅ 正确检测现有工作流并移交给 step-01b
✅ 用模板和适当的 frontmatter 初始化新工作流
✅ 使用分片优先逻辑发现并加载输入文档
✅ 在 frontmatter `inputDocuments` 中跟踪所有发现的文件
✅ 用户确认文档设置并可以继续

## 失败模式：

❌ 在存在现有工作流时继续新初始化
❌ 未用发现的输入文档更新 frontmatter
❌ 没有适当模板结构创建文档
❌ 在整文件之前未检查分片文件夹
❌ 未向用户清晰报告发现的文档
❌ 在用户未选择 [C] 继续的情况下继续

❌ **关键**：仅读取部分步骤文件 - 导致理解不完整和决策不当
❌ **关键**：在未完整阅读和理解下一步文件的情况下继续 'C'
❌ **关键**：在未完全理解步骤要求和协议的情况下做出决策