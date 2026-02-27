---
name: 'step-05-adversarial-review'
description: '构建差异并调用对抗性审查任务'

nextStepFile: './step-06-resolve-findings.md'
---

# 步骤 5：对抗性代码审查

**目标：** 构建所有更改的差异，调用对抗性审查任务，呈现发现。

---

## 可用状态

来自之前的步骤：

- `{baseline_commit}` - 工作流开始时的 Git HEAD（对差异至关重要）
- `{execution_mode}` - "tech-spec" 或 "direct"
- `{tech_spec_path}` - 技术规格文件（如果为模式 A）

---

### 1. 构建差异

构建工作流开始以来所有更改的完整差异。

### 如果 `{baseline_commit}` 是 Git 提交哈希：

**已跟踪文件更改：**

```bash
git diff {baseline_commit}
```

**新的未跟踪文件：**
仅包含你在本工作流期间（步骤 2-4）创建的未跟踪文件。
不要包含预先存在的未跟踪文件。
对于每个创建的新文件，将其完整内容作为"新文件"添加内容包含。

### 如果 `{baseline_commit}` 为 "NO_GIT"：

使用尽力而为的差异构建：

- 列出你在步骤 2-4 期间修改的所有文件
- 对于每个文件，显示你做的更改（如果你记得前后状态，或仅当前状态）
- 包含你创建的任何新文件及其完整内容
- 注意：这不如 Git 差异精确，但仍能进行有意义的审查

### 捕获为 {diff_output}

将所有更改合并到 `{diff_output}`。

**注意：** 不要执行 `git add` - 这是只读检查。

---

### 2. 调用对抗性审查

构建 `{diff_output}` 后，加载并遵循审查任务。如果可能，使用信息不对称：在单独的子 agent 或进程中仅加载此步骤，只有项目的读取权限，但没有除 `{diff_output}` 以外的任何上下文。

```xml
<invoke-task>使用 {project-root}/_bmad/core/tasks/review-adversarial-general.xml 审查 {diff_output}</invoke-task>
```

**平台回退：** 如果任务调用不可用，加载任务文件并按其指示内联执行，传递 `{diff_output}` 作为内容。

任务应：审查 `{diff_output}` 并返回发现列表。

---

### 3. 处理发现

从任务输出捕获发现。
**如果零发现：** 暂停 - 这很可疑。重新分析或请求用户指导。
评估严重性（严重、高、中、低）和有效性（真实、噪音、未定）。
除非明确要求，否则不要根据严重性或有效性排除发现。
按严重性排序发现。
为排序后的发现编号（F1、F2、F3 等）。
如果 TodoWrite 或类似工具可用，将每个发现转换为 TODO，在 TODO 中包含 ID、严重性、有效性和描述；否则将发现呈现为表格，列包括：ID、严重性、有效性、描述

---

## 下一步

准备好发现后，完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-06-resolve-findings.md` 让用户选择解决方式。

---

## 成功指标

- 从 baseline_commit 构建差异
- 新文件已包含在差异中
- 已调用任务并传入差异作为输入
- 已接收发现
- 发现已处理为 TODO 或表格并呈现给用户

## 失败模式

- 缺失 baseline_commit（无法构建准确差异）
- 未在差异中包含新的未跟踪文件
- 未提供差异输入就调用任务
- 不经质疑就接受零发现
- 未经明确指示呈现比审查任务返回更少的发现