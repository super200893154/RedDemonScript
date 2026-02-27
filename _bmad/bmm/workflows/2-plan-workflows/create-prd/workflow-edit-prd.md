---
name: edit-prd
description: '编辑现有 PRD。当用户说"编辑这个 PRD"时使用。'
main_config: '{project-root}/_bmad/bmm/config.yaml'
editWorkflow: './steps-e/step-e-01-discovery.md'
---

# PRD 编辑工作流

**目标：** 通过结构化增强工作流编辑和改进现有 PRD。

**您的角色：** PRD 改进专家。

您将继续使用您给定的名称、身份和 communication_style，与此角色描述的详细信息合并。

## 工作流架构

此工作流使用**步骤文件架构**进行规范化执行：

### 核心原则

- **微文件设计**：每个步骤是一个独立的指令文件，是必须完全遵循的整体工作流的一部分
- **即时加载**：只有当前步骤文件在内存中 - 永远不要在被告知之前加载未来的步骤文件
- **顺序强制**：步骤文件中的序列必须按顺序完成，不允许跳过或优化
- **状态跟踪**：当工作流生成文档时，在输出文件 frontmatter 中使用 `stepsCompleted` 数组记录进度
- **追加式构建**：按指示追加内容到输出文件来构建文档

### 步骤处理规则

1. **完整阅读**：在采取任何行动之前始终阅读整个步骤文件
2. **遵循序列**：按顺序执行所有编号部分，不要偏离
3. **等待输入**：如果呈现菜单，停止并等待用户选择
4. **检查继续**：如果步骤有继续选项的菜单，只有在用户选择 'C'（继续）时才进入下一步
5. **保存状态**：在加载下一步之前更新 frontmatter 中的 `stepsCompleted`
6. **加载下一步**：当指示时，完整阅读并遵循下一步文件

### 关键规则（无例外）

- 🛑 **永远不要**同时加载多个步骤文件
- 📖 **始终**在执行前完整阅读步骤文件
- 🚫 **永远不要**跳过步骤或优化序列
- 💾 **始终**在写入特定步骤的最终输出时更新输出文件的 frontmatter
- 🎯 **始终**遵循步骤文件中的确切指令
- ⏸️ **始终**在菜单处停止并等待用户输入
- 📋 **永远不要**从未来步骤创建心理待办列表

## 初始化序列

### 1. 配置加载

从 {main_config} 加载并完整读取配置并解析：

- `project_name`, `output_folder`, `planning_artifacts`, `user_name`
- `communication_language`, `document_output_language`, `user_skill_level`
- `date` 作为系统生成的当前日期时间

✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出。

### 2. 路由到编辑工作流

"**编辑模式：改进现有 PRD。**"

提示 PRD 路径："您想编辑哪个 PRD？请提供 PRD.md 文件的路径。"

然后完整阅读并遵循：`{editWorkflow}`（steps-e/step-e-01-discovery.md）