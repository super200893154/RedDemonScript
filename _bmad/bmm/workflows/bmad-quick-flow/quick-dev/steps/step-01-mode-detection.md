---
name: 'step-01-mode-detection'
description: '确定执行模式（技术规格 vs 直接），处理升级，设置状态变量'

nextStepFile_modeA: './step-03-execute.md'
nextStepFile_modeB: './step-02-context-gathering.md'
---

# 步骤 1：模式检测

**目标：** 确定执行模式，捕获基线，如需要处理升级。

---

## 状态变量（立即捕获，全程持久化）

这些变量必须在此步骤中设置，并可用于所有后续步骤：

- `{baseline_commit}` - 工作流开始时的 Git HEAD（如果不是 git 仓库则为 "NO_GIT"）
- `{execution_mode}` - "tech-spec" 或 "direct"
- `{tech_spec_path}` - 技术规格文件路径（如果为模式 A）

---

## 执行序列

### 1. 捕获基线

首先，检查项目是否使用 Git 版本控制：

**如果 Git 仓库存在**（`.git` 目录存在或 `git rev-parse --is-inside-work-tree` 成功）：

- 运行 `git rev-parse HEAD` 并将结果存储为 `{baseline_commit}`

**如果不是 Git 仓库：**

- 设置 `{baseline_commit}` = "NO_GIT"

### 2. 加载项目上下文

检查 `{project_context}` 是否存在（`**/project-context.md`）。如果找到，将其作为所有实现决策的基础参考加载。

### 3. 解析用户输入

分析用户的输入以确定模式：

**模式 A：技术规格**

- 用户提供了技术规格文件路径（例如 `quick-dev tech-spec-auth.md`）
- 加载规格，提取任务/上下文/验收标准
- 设置 `{execution_mode}` = "tech-spec"
- 设置 `{tech_spec_path}` = 提供的路径
- **下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-03-execute.md`

**模式 B：直接指令**

- 用户直接提供任务描述（例如 `refactor src/foo.ts...`）
- 设置 `{execution_mode}` = "direct"
- **下一步：** 评估升级阈值，然后继续

---

## 升级阈值（仅模式 B）

以最少的 token 使用评估用户输入（不加载文件）：

**触发升级（如果存在 2+ 信号）：**

- 提到多个组件（dashboard + api + database）
- 系统级语言（平台、集成、架构）
- 方法不确定（"我应该如何"、"最佳方式"）
- 多层范围（UI + 后端 + 数据一起）
- 延长时间框架（"本周"、"接下来的几天"）

**降低信号：**

- 简单性标记（"只是"、"快速"、"修复"、"bug"、"拼写错误"、"简单"）
- 单一文件/组件关注
- 自信、具体的请求

使用整体判断，而非机械的关键词匹配。

---

## 升级处理

### 无升级（简单请求）

显示："**选择：** [P] 先规划（技术规格） [E] 直接执行"

#### 菜单处理逻辑：

- 如果 P：引导用户至 `{quick_spec_workflow}`。**退出 Quick Dev。**
- 如果 E：询问任何额外指导，然后**下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-02-context-gathering.md`

#### 执行规则：

- 始终在显示菜单后暂停并等待用户输入
- 只有当用户做出选择后才继续

---

### 触发升级 - 级别 0-2

呈现："这看起来像是一个涉及多个组件的专注功能。"

显示：

**[P] 先规划（技术规格）**（推荐）
**[W] 看起来比 quick-dev 更大** - 推荐完整的 BMad Flow PRD 流程
**[E] 直接执行**

#### 菜单处理逻辑：

- 如果 P：引导至 `{quick_spec_workflow}`。**退出 Quick Dev。**
- 如果 W：引导用户改为运行 PRD 工作流。**退出 Quick Dev。**
- 如果 E：询问指导，然后**下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-02-context-gathering.md`

#### 执行规则：

- 始终在显示菜单后暂停并等待用户输入
- 只有当用户做出选择后才继续

---

### 触发升级 - 级别 3+

呈现："这听起来像是平台/系统级别的工作。"

显示：

**[W] 启动 BMad 方法**（推荐）
**[P] 先规划（技术规格）**（较轻量的规划）
**[E] 直接执行** - 试试运气

#### 菜单处理逻辑：

- 如果 P：引导至 `{quick_spec_workflow}`。**退出 Quick Dev。**
- 如果 W：引导用户改为运行 PRD 工作流。**退出 Quick Dev。**
- 如果 E：询问指导，然后**下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-02-context-gathering.md`

#### 执行规则：

- 始终在显示菜单后暂停并等待用户输入
- 只有当用户做出选择后才继续

---

## 下一步指令

**关键：** 当此步骤完成时，明确说明要加载哪个步骤：

- 模式 A（技术规格）："**下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-03-execute.md`"
- 模式 B（直接，已选择 [E]）："**下一步：** 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-02-context-gathering.md`"
- 升级（[P] 或 [W]）："**退出 Quick Dev。** 遵循引导的工作流。"

---

## 成功指标

- `{baseline_commit}` 已捕获并存储
- `{execution_mode}` 已确定（"tech-spec" 或 "direct"）
- 如果为模式 A，`{tech_spec_path}` 已设置
- 项目上下文如存在已加载
- 适当地评估了升级（模式 B）
- 提供了明确的下一步指令

## 失败模式

- 未捕获 baseline commit 就继续
- 未设置 execution_mode 变量
- 在模式 A（提供了技术规格）时加载步骤 2
- 升级后尝试"返回"而不是退出
- 步骤完成时没有明确的下一步指令