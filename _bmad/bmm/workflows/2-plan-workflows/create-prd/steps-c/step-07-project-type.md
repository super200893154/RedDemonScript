---
name: 'step-07-project-type'
description: '使用 CSV 驱动指导进行项目类型特定发现'

# 文件引用
nextStepFile: '{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-prd/steps-c/step-08-scoping.md'
outputFile: '{planning_artifacts}/prd.md'

# 数据文件
projectTypesCSV: '../data/project-types.csv'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'
---

# 步骤 7：项目类型深度探索

**进度：第 7 步，共 11 步** - 下一步：范围确定

## 强制执行规则（首先阅读）：

- 🛑 永远不要在没有用户输入的情况下生成内容

- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件 - 部分理解会导致不完整的决策
- 🔄 关键：当使用 'C' 加载下一步时，确保整个文件已被阅读和理解
- ✅ 始终将此视为 PM 同行之间的协作发现
- 📋 您是引导者，不是内容生成器
- 💬 专注于项目类型特定需求和技术考虑
- 🎯 数据驱动：使用 CSV 配置指导发现
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示您的分析
- ⚠️ 生成项目类型内容后呈现 A/P/C 菜单
- 💾 仅当用户选择 C（继续）时保存
- 📖 更新输出文件 frontmatter，将此步骤名称添加到 stepsCompleted 列表末尾
- 🚫 禁止在 C 被选中之前加载下一步

## 上下文边界：

- 来自前面步骤的当前文档和 frontmatter 可用
- 步骤 02 的项目类型可用于配置加载
- 项目类型 CSV 数据将在此步骤中加载
- 专注于特定于该项目类型的技术和功能需求

## 您的任务：

使用 CSV 驱动指导进行项目类型特定发现，定义技术需求。

## 项目类型发现序列：

### 1. 加载项目类型配置数据

**尝试子流程数据查找：**

"您的任务：在 {projectTypesCSV} 中查找数据

**搜索条件：**
- 找到 project_type 匹配 {{步骤 02 的项目类型}} 的行

**返回格式：**
仅返回匹配的行作为 YAML 格式对象，包含以下字段：
project_type, key_questions, required_sections, skip_sections, innovation_signals

**不要返回整个 CSV - 仅返回匹配的行。**"

**优雅降级（如果 Task 工具不可用）：**
- 直接加载 CSV 文件
- 手动查找匹配行
- 提取所需字段：
  - `key_questions`（分号分隔的发现问题列表）
  - `required_sections`（分号分隔的要记录的章节列表）
  - `skip_sections`（分号分隔的要跳过的章节列表）
  - `innovation_signals`（已在步骤 6 中探索）

### 2. 使用关键问题进行引导发现

解析 CSV 中的 `key_questions` 并探索每个：

#### 基于问题的发现：

对于 CSV 中 `key_questions` 的每个问题：

- 以对话风格自然询问用户
- 倾听他们的回答并询问澄清性后续问题
- 将答案连接到产品价值主张

**示例流程：**
如果 key_questions = "需要的端点？;认证方法？;数据格式？;速率限制？;版本控制？;需要 SDK？"

自然地询问：

- "您的 API 需要暴露哪些主要端点？"
- "您将如何处理认证和授权？"
- "您将支持什么数据格式用于请求和响应？"

### 3. 记录项目类型特定需求

根据用户对 key_questions 的回答，综合全面的需求：

#### 需求类别：

覆盖 CSV 中 `required_sections` 指示的领域：

- 综合每个必需章节的发现内容
- 记录具体需求、约束和决策
- 相关时连接到产品差异化因素

#### 跳过不相关的章节：

跳过 CSV 中 `skip_sections` 指示的领域，避免在不相关的方面浪费时间。

### 4. 生成动态内容章节

解析匹配 CSV 行中的 `required_sections` 列表。对于每个章节名称，生成相应内容：

#### 常见 CSV 章节映射：

- "endpoint_specs" 或 "endpoint_specification" → API 端点文档
- "auth_model" 或 "authentication_model" → 认证方法
- "platform_reqs" 或 "platform_requirements" → 平台支持需求
- "device_permissions" 或 "device_features" → 设备能力
- "tenant_model" → 多租户方法
- "rbac_matrix" 或 "permission_matrix" → 权限结构

#### 模板变量策略：

- 对于匹配常见模板变量的章节：生成具体内容
- 对于没有模板匹配的章节：包含在主 project_type_requirements 中
- 混合方法平衡模板结构与 CSV 驱动的灵活性

### 5. 生成项目类型内容

准备要追加到文档的内容：

#### 内容结构：

保存到文档时，追加这些二级和三级章节：

```markdown
## [项目类型] 特定需求

### 项目类型概述

[基于对话的项目类型摘要]

### 技术架构考虑

[基于对话的技术架构需求]

[基于 CSV 和对话的动态章节]

### 实现考虑

[基于对话的实现特定需求]
```

### 6. 呈现菜单选项

呈现项目类型内容以供审查，然后显示菜单：

"根据我们的对话和此产品类型的最佳实践，我已经记录了 {{project_name}} 的 {project_type} 特定需求。

**这是我要添加到文档的内容：**

[显示第 5 节的完整 markdown 内容]

**您想做什么？**"

显示："**选择：** [A] 高级引导 [P] 派对模式 [C] 继续到范围确定（第 8 步，共 11 步）"

#### 菜单处理逻辑：
- 如果 A：完整阅读并遵循：{advancedElicitationTask} 使用当前项目类型内容，处理返回的增强技术洞察，询问用户"接受这些对技术需求的改进？(y/n)"，如果是则用改进更新内容然后重新显示菜单，如果否则保留原始内容然后重新显示菜单
- 如果 P：完整阅读并遵循：{partyModeWorkflow} 使用当前项目类型需求，处理协作技术专业知识和验证，询问用户"接受这些对技术需求的更改？(y/n)"，如果是则用改进更新内容然后重新显示菜单，如果否则保留原始内容然后重新显示菜单
- 如果 C：将最终内容追加到 {outputFile}，更新 frontmatter 将此步骤名称添加到 stepsCompleted 数组末尾，然后完整阅读并遵循：{nextStepFile}
- 如果其他：帮助用户响应，然后重新显示菜单

#### 执行规则：
- 呈现菜单后始终暂停并等待用户输入
- 仅当用户选择 'C' 时才继续下一步
- 执行其他菜单项后，返回此菜单

## 追加到文档：

当用户选择 'C' 时，使用前面步骤的结构将内容直接追加到文档。

## 成功指标：

✅ 项目类型配置已加载并有效使用
✅ CSV 中的所有关键问题已通过用户输入探索
✅ 按 CSV 配置生成必需章节
✅ 正确跳过应跳过的章节以节省时间
✅ 技术需求连接到产品价值
✅ A/P/C 菜单已呈现并正确处理
✅ 当 C 被选中时内容正确追加到文档

## 失败模式：

❌ 未加载或使用项目类型 CSV 配置
❌ 在发现过程中遗漏 CSV 的关键问题
❌ 未按 CSV 配置生成必需章节
❌ 记录应按 CSV 跳过的章节
❌ 创建没有项目类型特异性的通用内容
❌ 内容生成后未呈现 A/P/C 菜单
❌ 在用户未选择 'C' 的情况下追加内容

❌ **关键**：仅阅读部分步骤文件 - 导致不完整的理解和错误的决策
❌ **关键**：在未完全阅读和理解下一步文件的情况下使用 'C' 继续
❌ **关键**：在没有完全理解步骤要求和协议的情况下做出决策

## 项目类型示例：

**对于 api_backend：**

- 专注于端点、认证、数据模式、速率限制
- 跳过视觉设计和用户旅程章节
- 生成 API 规格文档

**对于 mobile_app：**

- 专注于平台需求、设备权限、离线模式
- 除非需要，跳过 API 端点文档
- 生成移动特定技术需求

**对于 saas_b2b：**

- 专注于多租户、权限、集成
- 除非相关，跳过移动优先考虑
- 生成企业特定需求

## 下一步：

在用户选择 'C' 且内容保存到文档后，加载 `{nextStepFile}` 定义项目范围。

记住：在用户从 A/P/C 菜单明确选择 'C' 且内容已保存之前，不要继续到步骤 08（范围确定）！