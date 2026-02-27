---
name: 'step-01-understand'
description: '分析当前状态与用户想要构建的目标之间的需求差异'

templateFile: '../tech-spec-template.md'
wipFile: '{implementation_artifacts}/tech-spec-wip.md'
---

# 步骤 1：分析需求差异

**进度：第 1 步，共 4 步** - 下一步：深入调查

## 规则：

- 不得跳过步骤。
- 不得优化序列。
- 必须遵循确切指令。
- 不得提前查看后续步骤。
- ✅ 你必须始终使用配置中的 `{communication_language}` 以你的 Agent 交流风格输出

## 上下文：

- `workflow.md` 中的变量已在内存中可用。
- 重点：定义技术需求差异和范围。
- 调查：仅执行表面级代码扫描以验证差异。将实现后果的深入分析保留到步骤 2。
- 目标：在当前状态和目标状态之间建立可验证的差异。

## 指令序列

### 0. 检查进行中的工作

a) **在任何操作之前，检查 `{wipFile}` 是否存在：**

b) **如果 WIP 文件存在：**

1. 读取 frontmatter 并提取：`title`、`slug`、`stepsCompleted`
2. 计算进度：`lastStep = max(stepsCompleted)`
3. 向用户呈现：

```
嘿 {user_name}！发现一个进行中的技术规格：

**{title}** - 已完成第 {lastStep} 步，共 4 步

这是您要继续的内容吗？

[Y] 是的，从上次离开的地方继续
[N] 不，归档它并开始新的
```

4. **暂停并等待用户选择。**

a) **菜单处理：**

- **[Y] 继续现有的：**
  - 根据 `stepsCompleted` 直接跳转到相应步骤：
    - `[1]` → 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-spec/steps/step-02-investigate.md`（步骤 2）
    - `[1, 2]` → 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-spec/steps/step-03-generate.md`（步骤 3）
    - `[1, 2, 3]` → 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-spec/steps/step-04-review.md`（步骤 4）
- **[N] 归档并重新开始：**
  - 将 `{wipFile}` 重命名为 `{implementation_artifacts}/tech-spec-{slug}-archived-{date}.md`

### 1. 问候并询问初始请求

a) **简短问候用户：**

"嘿 {user_name}！今天我们要构建什么？"

b) **获取他们的初始描述。** 暂时不要问详细问题 - 只需了解足够的内容以知道从哪里查看。

### 2. 快速定向扫描

a) **在询问详细问题之前，进行快速扫描以了解情况：**

b) **检查现有的上下文文档：**

- 检查 `{implementation_artifacts}` 和 `{planning_artifacts}` 中的规划文档（PRD、架构、Epic、研究）
- 检查 `**/project-context.md` - 如果存在，浏览模式和约定
- 检查任何与用户请求相关的现有故事或规格

c) **如果用户提到具体代码/功能，进行快速扫描：**

- 搜索他们提到的相关文件/类/函数
- 浏览结构（暂不深入分析 - 那是步骤 2）
- 记录：技术栈、明显模式、文件位置

d) **构建心智模型：**

- 此功能的可能情况是什么？
- 根据发现的内容，可能的范围是什么？
- 基于代码，你现在有什么问题？

**此扫描应耗时 < 30 秒。仅足以提出明智的问题。**

### 3. 提出有依据的问题

a) **现在提出澄清性问题 - 但这些问题应基于发现的内容：**

不要问"范围是什么？"这样的通用问题，而是问具体的问题，如：
- "`AuthService` 在控制器中处理验证 — 新字段应该遵循该模式还是移至专用验证器？"
- "`NavigationSidebar` 组件使用本地状态管理 'collapsed' 切换 — 我们应该保持这种方式还是移至全局存储？"
- "Epic 文档提到 X - 这与之相关吗？"

**根据 {user_skill_level} 调整。** 技术用户需要技术问题。非技术用户需要翻译。

b) **如果未找到现有代码：**

- 询问预期的架构、模式、约束
- 询问他们想要模拟的类似系统

### 4. 捕获核心理解

a) **从对话中提取并确认：**

- **标题**：此项工作的清晰简洁名称
- **Slug**：标题的 URL 安全版本（小写、连字符、无空格）
- **问题陈述**：我们要解决什么问题？
- **解决方案**：高层方法（1-2 句话）
- **范围内**：包含什么
- **范围外**：明确不包含什么

b) **要求用户在继续之前确认捕获的理解。**

### 5. 初始化 WIP 文件

a) **创建技术规格 WIP 文件：**

1. 从 `{templateFile}` 复制模板
2. 写入 `{wipFile}`
3. 用捕获的值更新 frontmatter：
   ```yaml
   ---
   title: '{title}'
   slug: '{slug}'
   created: '{date}'
   status: 'in-progress'
   stepsCompleted: [1]
   tech_stack: []
   files_to_modify: []
   code_patterns: []
   test_patterns: []
   ---
   ```
4. 用问题陈述、解决方案和范围填写概述部分
5. 用有依据发现期间收集的任何技术偏好或约束填写开发上下文部分
6. 写入文件

b) **向用户报告：**

"已创建：`{wipFile}`

**已捕获：**

- 标题：{title}
- 问题：{problem_statement_summary}
- 范围：{scope_summary}"

### 6. 呈现检查点菜单

a) **显示菜单：**

显示："**选择：** [A] 高级引导 [P] Party Mode [C] 继续深入调查（第 2 步，共 4 步）"

b) **暂停并等待用户选择。**

#### 菜单处理逻辑：

- 如果 A：完整阅读并遵循：`{advanced_elicitation}` 处理当前技术规格内容，处理增强的见解，询问用户"接受改进？(y/n)"，如果接受则更新 WIP 文件然后重新显示菜单，如果不接受则保留原样然后重新显示菜单
- 如果 P：完整阅读并遵循：`{party_mode_exec}` 处理当前技术规格内容，处理协作见解，询问用户"接受更改？(y/n)"，如果接受则更新 WIP 文件然后重新显示菜单，如果不接受则保留原样然后重新显示菜单
- 如果 C：验证 `{wipFile}` 具有 `stepsCompleted: [1]`，然后完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-spec/steps/step-02-investigate.md`
- 如果有其他评论或查询：提供有帮助的回复然后重新显示菜单

#### 执行规则：

- 始终在显示菜单后暂停并等待用户输入
- 只有当用户选择 'C' 时才继续下一步
- 执行 A 或 P 后，返回此菜单

---

## 必需输出：

- 必须用捕获的元数据初始化 WIP 文件。

## 验证清单：

- [ ] 在任何问候之前首先执行 WIP 检查。
- [ ] `{wipFile}` 已创建，具有正确的 frontmatter、概述、开发上下文和 `stepsCompleted: [1]`。
- [ ] 用户选择了 [C] 继续。