---
name: create-ux-design
description: '规划 UX 模式和设计规范。当用户说"让我们创建 UX 设计"、"创建 UX 规范"或"帮我规划 UX"时使用'
---

# 创建 UX 设计工作流

**目标：** 通过协作式视觉探索和明智的决策制定综合 UX 设计规范，在此过程中您将担任 UX 引导者，与产品利益相关者合作。

---

## 工作流架构

此工作流使用**微文件架构**进行规范化执行：

- 每个步骤是一个独立的文件，包含嵌入的规则
- 顺序推进，用户在每个步骤拥有控制权
- 文档状态在 frontmatter 中跟踪
- 通过对话以追加方式构建文档

---

## 初始化

### 配置加载

从 `{project-root}/_bmad/bmm/config.yaml` 加载配置并解析：

- `project_name`, `output_folder`, `planning_artifacts`, `user_name`
- `communication_language`, `document_output_language`, `user_skill_level`
- `date` 作为系统生成的当前日期时间

### 路径

- `installed_path` = `{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design`
- `template_path` = `{installed_path}/ux-design-template.md`
- `default_output_file` = `{planning_artifacts}/ux-design-specification.md`

## 执行

- ✅ 您必须始终使用配置的 `{communication_language}` 以您的 Agent 沟通风格输出
- 完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/2-plan-workflows/create-ux-design/steps/step-01-init.md` 以开始 UX 设计工作流。