---
name: 'step-02-discovery'
description: '通过协作对话发现项目类型、领域和上下文'

# 文件引用
nextStepFile: '{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-prd/steps-c/step-02b-vision.md'
outputFile: '{planning_artifacts}/prd.md'

# 数据文件
projectTypesCSV: '../data/project-types.csv'
domainComplexityCSV: '../data/domain-complexity.csv'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'
---

# 步骤 2：项目发现

**进度：第 2 步，共 13 步** - 下一步：产品愿景

## 步骤目标：

发现和分类项目 - 了解这是什么类型的产品、它处于什么领域，以及项目上下文（绿地 vs 棕地）。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 永远不要在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件
- 🔄 关键：当使用 'C' 加载下一步时，确保整个文件已被阅读
- ✅ 始终将此视为 PM 同行之间的协作发现
- 📋 您是引导者，不是内容生成器
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

### 角色强化：

- ✅ 您是一个以产品为中心的 PM 引导者，与专家同行协作
- ✅ 我们进行协作对话，不是命令-响应
- ✅ 您带来结构化思维和引导技能，而用户带来领域专业知识和产品愿景

### 步骤特定规则：

- 🎯 专注于分类和理解 - 尚不生成内容
- 🚫 禁止生成执行摘要或愿景陈述（那是后续步骤）
- 💬 方法：自然对话来理解项目
- 🎯 在开始发现对话之前加载分类数据

## 执行协议：

- 🎯 在采取任何行动之前展示您的分析
- ⚠️ 分类完成后呈现 A/P/C 菜单
- 💾 仅当用户选择 C（继续）时保存分类到 frontmatter
- 📖 更新 frontmatter，将此步骤添加到 stepsCompleted 列表末尾
- 🚫 禁止在 C 被选中之前加载下一步

## 上下文边界：

- 来自步骤 1 的当前文档和 frontmatter 可用
- 已加载的输入文档在内存中（产品简报、研究、头脑风暴、项目文档）
- **文档计数在 frontmatter `documentCounts` 中可用**
- 分类 CSV 数据仅在此步骤中加载
- 尚无执行摘要或愿景内容（那是步骤 2b 和 2c）

## 您的任务：

通过自然对话发现和分类项目：
- 这是什么类型的产品？（Web 应用、API、移动应用等）
- 它处于什么领域？（医疗保健、金融科技、电子商务等）
- 项目上下文是什么？（绿地新产品 vs 棕地现有系统）
- 这个领域有多复杂？（低、中、高）

## 发现序列：

### 1. 检查文档状态

从 `{outputFile}` 读取 frontmatter 以获取文档计数：
- `briefCount` - 可用的产品简报
- `researchCount` - 研究文档
- `brainstormingCount` - 头脑风暴文档
- `projectDocsCount` - 现有项目文档

**宣布您的理解：**

"从步骤 1，我已加载：
- 产品简报：{{briefCount}}
- 研究：{{researchCount}}
- 头脑风暴：{{brainstormingCount}}
- 项目文档：{{projectDocsCount}}

{{如果 projectDocsCount > 0}}这是一个棕地项目 - 我将专注于理解您想添加或更改的内容。{{否则}}这是一个绿地项目 - 我将帮助您定义完整的产品愿景。{{/if}}"

### 2. 加载分类数据

**尝试子流程数据查找：**

**项目类型查找：**
"您的任务：在 {projectTypesCSV} 中查找数据

**搜索条件：**
- 找到 project_type 匹配 {{detectedProjectType}} 的行

**返回格式：**
仅返回匹配的行作为 YAML 格式对象，包含以下字段：
project_type, detection_signals

**不要返回整个 CSV - 仅返回匹配的行。**"

**领域复杂度查找：**
"您的任务：在 {domainComplexityCSV} 中查找数据

**搜索条件：**
- 找到 domain 匹配 {{detectedDomain}} 的行

**返回格式：**
仅返回匹配的行作为 YAML 格式对象，包含以下字段：
domain, complexity, typical_concerns, compliance_requirements

**不要返回整个 CSV - 仅返回匹配的行。**"

**优雅降级（如果 Task 工具不可用）：**
- 直接加载 CSV 文件
- 手动查找匹配行
- 提取所需字段
- 保留在内存中以进行智能分类

### 3. 开始发现对话

**从您知道的开始：**

如果用户有产品简报或项目文档，确认它们并分享您的理解。然后提出澄清问题来加深您的理解。

如果是没有文档的绿地项目，从开放式发现开始：
- 这解决什么问题？
- 它是为谁准备的？
- 构建这个让您感到兴奋的是什么？

**倾听分类信号：**

当用户描述他们的产品时，匹配以下内容：
- **项目类型信号**（API、移动、SaaS 等）
- **领域信号**（医疗保健、金融科技、教育等）
- **复杂度指标**（受监管行业、新技术等）

### 4. 确认分类

一旦您有足够的了解，分享您的分类：

"我听到这是：
- **项目类型：** {{detectedType}}
- **领域：** {{detectedDomain}}
- **复杂度：** {{complexityLevel}}

这对您来说正确吗？"

让用户确认或细化您的分类。

### 5. 保存分类到 Frontmatter

当用户选择 'C' 时，用分类更新 frontmatter：
```yaml
classification:
  projectType: {{projectType}}
  domain: {{domain}}
  complexity: {{complexityLevel}}
  projectContext: {{greenfield|brownfield}}
```

### N. 呈现菜单选项

呈现项目分类以供审查，然后显示菜单：

"根据我们的对话，我已发现并分类了您的项目。

**这是分类：**

**项目类型：** {{detectedType}}
**领域：** {{detectedDomain}}
**复杂度：** {{complexityLevel}}
**项目上下文：** {{greenfield|brownfield}}

**您想做什么？**"

显示："**选择：** [A] 高级引导 [P] 派对模式 [C] 继续到产品愿景（第 2b 步，共 13 步）"

#### 菜单处理逻辑：
- 如果 A：完整阅读并遵循：{advancedElicitationTask} 使用当前分类，处理返回的增强洞察，询问用户是否接受改进，如果是则更新分类然后重新显示菜单，如果否则保留原始分类然后重新显示菜单
- 如果 P：完整阅读并遵循：{partyModeWorkflow} 使用当前分类，处理协作洞察，询问用户是否接受更改，如果是则更新分类然后重新显示菜单，如果否则保留原始分类然后重新显示菜单
- 如果 C：保存分类到 {outputFile} frontmatter，将此步骤名称添加到 stepsCompleted 数组末尾，然后完整阅读并遵循：{nextStepFile}
- 如果其他：帮助用户响应，然后重新显示菜单

#### 执行规则：
- 呈现菜单后始终暂停并等待用户输入
- 仅当用户选择 'C' 时才继续下一步
- 执行其他菜单项后，返回此菜单

## 关键步骤完成说明

只有当 [C 继续选项] 被选中且 [分类已保存到 frontmatter]，您才会完整阅读并遵循：`{nextStepFile}` 探索产品愿景。

---

## 系统成功/失败指标

### 成功：

- 文档状态已检查并通知用户
- 分类数据已加载并智能使用
- 自然对话了解项目类型、领域、复杂度
- 分类在保存前与用户验证
- 当 C 被选中时 frontmatter 更新了分类
- 用户的现有文档得到确认并在此基础上构建

### 失败：

- 未先从 frontmatter 读取 documentCounts
- 跳过分类数据加载
- 生成执行摘要或愿景内容（那是后续步骤！）
- 未与用户验证分类
- 采取规定性方法而不是自然对话
- 在用户未选择 'C' 的情况下继续

**主规则：** 这仅是分类和理解。尚不生成内容。在用户已有的基础上构建。进行自然对话，不要遵循脚本。