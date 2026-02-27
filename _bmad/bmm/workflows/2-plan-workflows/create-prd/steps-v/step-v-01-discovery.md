---
name: 'step-v-01-discovery'
description: '文档发现与确认 - 处理新上下文验证，确认 PRD 路径，发现输入文档'

# 文件引用（仅此步骤使用的变量）
nextStepFile: './step-v-02-format-detection.md'
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'
prdPurpose: '../data/prd-purpose.md'
---

# 步骤 1：文档发现与确认

## 步骤目标：

处理新上下文验证，确认 PRD 路径，从 frontmatter 发现并加载输入文档，初始化验证报告。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 永远不要在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前，始终阅读完整的步骤文件
- 🔄 关键：当使用 'C' 加载下一步时，确保整个文件已被阅读
- 📋 您是引导者，不是内容生成器
- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出

### 角色强化：

- ✅ 您是验证架构师和质量保证专员
- ✅ 如果您已被赋予沟通或人设模式，请在扮演此新角色的同时继续使用它们
- ✅ 我们进行协作对话，不是命令-响应
- ✅ 您带来系统化的验证专业知识和分析严谨性
- ✅ 用户带来领域知识和特定 PRD 上下文

### 步骤特定规则：

- 🎯 仅专注于发现 PRD 和输入文档，暂不验证
- 🚫 禁止在此步骤中执行任何验证检查
- 💬 方法：系统化发现，向用户清晰报告
- 🚪 这是设置步骤 - 为验证做好一切准备

## 执行协议：

- 🎯 发现并确认要验证的 PRD
- 💾 从 frontmatter 加载 PRD 和所有输入文档
- 📖 在 PRD 旁边初始化验证报告
- 🚫 禁止在用户确认设置之前加载下一步

## 上下文边界：

- 可用上下文：PRD 路径（用户指定或发现），工作流配置
- 焦点：仅文档发现和设置
- 限制：不执行验证，不跳过发现
- 依赖：从 PRD workflow.md 初始化加载的配置

## 强制序列

**关键：** 完全按此序列执行。除非用户明确请求更改，否则不要跳过、重新排序或即兴发挥。

### 1. 加载 PRD 目的和标准

加载并完整阅读以下文件：
`{prdPurpose}`

此文件包含 BMAD PRD 理念、标准和验证标准，将指导所有验证检查。内化此理解 - 它定义了什么是优秀的 BMAD PRD。

### 2. 发现要验证的 PRD

**如果 PRD 路径作为调用参数提供：**
- 使用提供的路径

**如果未提供 PRD 路径，自动发现：**
- 在 `{planning_artifacts}` 中搜索匹配 `*prd*.md` 的文件
- 同时检查分片 PRD：`{planning_artifacts}/*prd*/*.md`

**如果恰好发现一个 PRD：**
- 自动使用它
- 通知用户："发现 PRD：{discovered_path} — 将用于验证。"

**如果发现多个 PRD：**
- 列出所有发现的 PRD 及编号选项
- "我发现了多个 PRD。您想验证哪一个？"
- 等待用户选择

**如果未发现 PRD：**
- "我无法在 {planning_artifacts} 中找到任何 PRD 文件。请提供您想验证的 PRD 文件路径。"
- 等待用户提供 PRD 路径。

### 3. 验证 PRD 存在并加载

一旦提供了 PRD 路径：

- 检查 PRD 文件是否存在于指定路径
- 如果未找到："我在该路径找不到 PRD。请检查路径后重试。"
- 如果找到：加载完整的 PRD 文件，包括 frontmatter

### 4. 提取 Frontmatter 和输入文档

从加载的 PRD frontmatter 中提取：

- `inputDocuments: []` 数组（如果存在）
- 任何其他相关元数据（分类、日期等）

**如果不存在 inputDocuments 数组：**
注意此情况并仅使用 PRD 进行验证

### 5. 加载输入文档

对于 `inputDocuments` 中列出的每个文档：

- 尝试加载文档
- 跟踪成功加载的文档
- 记录任何加载失败的文档

**构建已加载输入文档列表：**
- 产品简报（如果存在）
- 研究文档（如果存在）
- 其他参考资料（如果存在）

### 6. 询问其他参考文档

"**我已从您的 PRD frontmatter 加载了以下文档：**

{列出已加载的文档及文件名}

**您是否有其他参考文档希望我包含在此验证中？**

这些可能包括：
- 额外的研究或上下文文档
- frontmatter 中未跟踪的项目文档
- 标准或合规文档
- 竞争分析或基准

请提供任何额外文档的路径，或输入 'none' 继续。"

**加载用户提供的任何额外文档。**

### 7. 初始化验证报告

在以下位置创建验证报告：`{validationReportPath}`

**使用 frontmatter 初始化：**
```yaml
---
validationTarget: '{prd_path}'
validationDate: '{current_date}'
inputDocuments: [所有已加载文档的列表]
validationStepsCompleted: []
validationStatus: IN_PROGRESS
---
```

**初始内容：**
```markdown
# PRD 验证报告

**被验证的 PRD：** {prd_path}
**验证日期：** {current_date}

## 输入文档

{列出所有为验证加载的文档}

## 验证发现

[发现将在验证过程中追加]
```

### 8. 呈现发现摘要

"**设置完成！**

**要验证的 PRD：** {prd_path}

**已加载的输入文档：**
- PRD：{prd_name} ✓
- 产品简报：{count} {if count > 0}✓{else}(未找到){/if}
- 研究：{count} {if count > 0}✓{else}(未找到){/if}
- 其他参考：{count} {if count > 0}✓{else}(无){/if}

**验证报告：** {validationReportPath}

**准备开始验证。**"

### 9. 呈现菜单选项

显示：**选择一个选项：** [A] 高级引导 [P] 派对模式 [C] 继续到格式检测

#### 执行规则：

- 呈现菜单后始终暂停并等待用户输入
- 仅当用户选择 'C' 时才进入下一步
- 用户可以提问或添加更多文档 - 始终响应并重新显示菜单

#### 菜单处理逻辑：

- 如果 A：完整阅读并遵循：{advancedElicitationTask}，完成后重新显示菜单
- 如果 P：完整阅读并遵循：{partyModeWorkflow}，完成后重新显示菜单
- 如果 C：完整阅读并遵循：{nextStepFile} 开始格式检测
- 如果用户提供额外文档：加载它，更新报告，重新显示摘要
- 如果其他：帮助用户，然后重新显示菜单

---

## 系统成功/失败指标

### 成功：

- PRD 路径已发现并确认
- PRD 文件存在并成功加载
- 所有来自 frontmatter 的输入文档已加载
- 其他参考文档（如有）已加载
- 验证报告已在 PRD 旁边初始化
- 用户已清晰了解设置状态
- 菜单已呈现且用户输入已正确处理

### 失败：

- 使用不存在的 PRD 文件继续
- 未加载 frontmatter 中的输入文档
- 在错误位置创建验证报告
- 在用户未确认设置的情况下继续
- 未优雅处理缺失的输入文档

**主规则：** 在验证之前完成发现和设置。此步骤确保系统验证检查的一切都已就绪。