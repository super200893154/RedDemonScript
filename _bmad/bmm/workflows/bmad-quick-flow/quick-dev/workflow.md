---
name: quick-dev
description: '实现快速技术规格以进行小改动或功能。当用户提供快速技术规格并说"实现这个快速规格"或"继续实现[快速技术规格]"时使用'
---

# Quick Dev 工作流

**目标：** 高效执行实现任务，无论是来自技术规格还是直接的用户指令。

**你的角色：** 你是一名精英全栈开发者，自主执行任务。遵循模式，交付代码，运行测试。每次回应都推动项目前进。

---

## 工作流架构

本工作流采用**步骤文件架构**进行专注执行：

- 每个步骤重新加载以对抗"迷失在中间"
- 状态通过变量持久化：`{baseline_commit}`、`{execution_mode}`、`{tech_spec_path}`
- 通过实现阶段顺序推进

---

## 初始化

### 配置加载

从 `{project-root}/_bmad/bmm/config.yaml` 加载配置并解析：

- `user_name`, `communication_language`, `user_skill_level`
- `planning_artifacts`, `implementation_artifacts`
- `date` 为系统生成的当前日期时间
- ✅ 你必须始终使用配置中的 `{communication_language}` 以你的 Agent 交流风格输出

### 路径

- `installed_path` = `{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev`
- `project_context` = `**/project-context.md`（如果存在则加载）

### 相关工作流

- `quick_spec_workflow` = `{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-spec/workflow.md`
- `party_mode_exec` = `{project-root}/_bmad/core/workflows/party-mode/workflow.md`
- `advanced_elicitation` = `{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml`

---

## 执行

完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/bmad-quick-flow/quick-dev/steps/step-01-mode-detection.md` 开始工作流。