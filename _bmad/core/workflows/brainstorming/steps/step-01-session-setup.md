# 步骤 1：会话设置和继续检测

## 强制执行规则（请先阅读）：

- 🛑 没有用户输入绝不生成内容
- ✅ 始终将此视为协作引导
- 📋 你是引导者，不是内容生成器
- 💬 仅专注于会话设置和继续检测
- 🚪 检测现有工作流状态并正确处理继续
- ✅ 你必须始终以你的 Agent 沟通风格和 `communication_language` 输出

## 执行协议：

- 🎯 在采取任何行动之前展示你的分析
- 💾 初始化文档并更新 frontmatter
- 📖 在加载下一步之前设置 frontmatter `stepsCompleted: [1]`
- 🚫 禁止在设置完成前加载下一步

## 上下文边界：

- 来自 workflow.md 的变量在内存中可用
- 先前上下文 = 输出文档 + frontmatter 中的内容
- 不要假设来自其他步骤的知识
- 需要时从 CSV 按需加载脑力技巧

## 你的任务：

通过检测继续状态并设置会话上下文来初始化头脑风暴工作流。

## 初始化序列：

### 1. 检查现有工作流

首先，检查输出文档是否已存在：

- 查找 `{output_folder}/brainstorming/brainstorming-session-{{date}}.md` 文件
- 如果存在，读取包括 frontmatter 在内的完整文件
- 如果不存在，这是一个全新的工作流

### 2. 处理继续（如果文档存在）

如果文档存在且有包含 `stepsCompleted` 的 frontmatter：

- **在此停止**并立即加载 `./step-01b-continue.md`
- 不要继续任何初始化任务
- 让 step-01b 处理继续逻辑

### 3. 全新工作流设置（如果无文档）

如果没有文档或 frontmatter 中没有 `stepsCompleted`：

#### A. 初始化文档

创建头脑风暴会话文档：

```bash
# 如需要创建目录
mkdir -p "$(dirname "{output_folder}/brainstorming/brainstorming-session-{{date}}.md")"

# 从模板初始化
cp "{template_path}" "{output_folder}/brainstorming/brainstorming-session-{{date}}.md"
```

#### B. 上下文文件检查和加载

**检查上下文文件：**

- 检查工作流调用中是否提供 `context_file`
- 如果上下文文件存在且可读，加载它
- 解析上下文内容获取项目特定指导
- 使用上下文指导会话设置和方法推荐

#### C. 会话上下文收集

"欢迎 {{user_name}}！我很高兴引导您的头脑风暴会话。我将引导您通过经过验证的创意技巧产生创新想法和突破性解决方案。

**上下文加载：** [如提供 context_file，指示上下文已加载]
**基于上下文的指导：** [如有上下文，简要提及焦点领域]

**让我们为最大的创意和生产力设置您的会话：**

**会话发现问题：**

1. **我们在头脑风暴什么？**（中心话题或挑战）
2. **您希望获得什么具体成果？**（想法类型、解决方案或洞察）"

#### D. 处理用户响应

等待用户响应，然后：

**会话分析：**
"根据您的响应，我理解我们将聚焦于 **[摘要话题]**，目标围绕 **[摘要目标]**。

**会话参数：**

- **话题焦点：** [清晰话题表述]
- **主要目标：** [具体成果目标]

**这准确捕捉了您想实现的目标吗？**"

#### E. 更新 Frontmatter 和文档

更新文档 frontmatter：

```yaml
---
stepsCompleted: [1]
inputDocuments: []
session_topic: '[session_topic]'
session_goals: '[session_goals]'
selected_approach: ''
techniques_used: []
ideas_generated: []
context_file: '[context_file if provided]'
---
```

追加到文档：

```markdown
## 会话概述

**话题：** [session_topic]
**目标：** [session_goals]

### 上下文指导

_[如提供上下文文件，摘要关键上下文和焦点领域]_

### 会话设置

_[基于关于会话参数和引导者方法的对话内容]_
```

## 追加到文档：

当用户选择方法时，使用上述结构直接将初始会话概述内容追加到 `{output_folder}/brainstorming/brainstorming-session-{{date}}.md`。

### E. 继续到技巧选择

"**会话设置完成！** 我清楚理解您的目标，可以为您的头脑风暴需求选择完美的技巧。

**准备好探索技巧方法了吗？**
[1] 用户选择技巧 - 浏览我们完整的技巧库
[2] AI 推荐技巧 - 根据您的目标获取定制建议
[3] 随机技巧选择 - 发现意外的创意方法
[4] 渐进式技巧流程 - 从广泛开始，然后系统聚焦

哪种方法最吸引您？（输入 1-4）"

### 4. 处理用户选择和初始文档追加

#### 当用户选择方法编号时：

- **将初始会话概述追加到 `{output_folder}/brainstorming/brainstorming-session-{{date}}.md`**
- **更新 frontmatter：** `stepsCompleted: [1]`，`selected_approach: '[选择的方法]'`
- **根据选择加载适当的 step-02 文件**

### 5. 处理用户选择

用户选择方法编号后：

- **如果选择 1：** 加载 `./step-02a-user-selected.md`
- **如果选择 2：** 加载 `./step-02b-ai-recommended.md`
- **如果选择 3：** 加载 `./step-02c-random-selection.md`
- **如果选择 4：** 加载 `./step-02d-progressive-flow.md`

## 成功指标：

✅ 正确检测现有工作流并处理继续
✅ 以正确的文档结构初始化全新工作流
✅ 清晰收集和理解会话上下文
✅ 正确捕获和路由用户的方法选择
✅ 正确更新会话状态的 frontmatter
✅ 用会话概述部分初始化文档

## 失败模式：

❌ 创建新文档前未检查现有文档
❌ 缺少继续检测导致重复工作
❌ 不充分的会话上下文收集
❌ 未正确路由用户的方法选择
❌ 未用会话参数更新 frontmatter

## 会话设置协议：

- 初始化前始终验证文档存在性
- 仅在需要技巧呈现时加载脑力技巧 CSV
- 全程使用协作引导语言
- 为创意探索维护心理安全感
- 根据用户偏好清晰路由下一步

## 下一步：

根据用户的方法选择，加载适当的 step-02 文件进行技巧选择和引导。

记住：仅专注于设置和路由——不要预加载技巧信息或提前查看执行步骤！