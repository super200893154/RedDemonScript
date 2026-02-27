---
name: generate-project-context
description: '创建包含 AI 规则的 project-context.md。当用户说"生成项目上下文"或"创建项目上下文"时使用'
---

# 生成项目上下文工作流

**目标：** 创建一个简洁、优化的 `project-context.md` 文件，包含 AI agent 在实现代码时必须遵循的关键规则、模式和指南。此文件专注于 LLM 需要被提醒的不明显的细节。

**你的角色：** 你是一名技术引导者，与同伴合作捕获确保所有在此项目上工作的 AI agent 生成一致、高质量代码的关键实现规则。

---

## 工作流架构

本工作流采用**微文件架构**进行规范化执行：

- 每个步骤是一个独立的文件，内嵌规则
- 通过用户控制逐步推进
- 文档状态在 frontmatter 中跟踪
- 专注于精简、针对 LLM 优化的内容生成
- 如果当前步骤文件指示用户必须批准并指示继续，你绝不进入下一步骤文件

---

## 初始化

### 配置加载

从 `{project-root}/_bmad/bmm/config.yaml` 加载配置并解析：

- `project_name`, `output_folder`, `user_name`
- `communication_language`, `document_output_language`, `user_skill_level`
- `date` 为系统生成的当前日期时间
- ✅ 你必须始终使用配置中的 `{communication_language}` 以你的 Agent 交流风格输出

### 路径

- `installed_path` = `{project-root}/_bmad/bmm/workflows/generate-project-context`
- `template_path` = `{installed_path}/project-context-template.md`
- `output_file` = `{output_folder}/project-context.md`

---

## 执行

加载并执行 `{project-root}/_bmad/bmm/workflows/generate-project-context/steps/step-01-discover.md` 开始工作流。

**注意：** 输入文档发现和初始化协议在 step-01-discover.md 中处理。