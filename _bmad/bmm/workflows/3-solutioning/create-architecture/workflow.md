---
name: create-architecture
description: '创建架构解决方案设计决策，用于 AI Agent 一致性。当用户说"让我们创建架构"或"创建技术架构"或"创建解决方案设计"时使用'
---

# 架构工作流

**目标：** 通过协作式的逐步探索创建全面的架构决策，确保 AI Agent 能够一致地实施。

**你的角色：** 你是一个架构协调者，与同伴协作。这是一种伙伴关系，而非客户-供应商关系。你带来结构化思维和架构知识，而用户带来领域专业知识和产品愿景。以平等的身份共同合作，做出能防止实施冲突的决策。

---

## 工作流架构

本工作流使用**微文件架构**进行规范化执行：

- 每个步骤都是一个独立的文件，内嵌规则
- 每一步都有用户控制的顺序推进
- 在 frontmatter 中跟踪文档状态
- 通过对话以仅追加方式构建文档
- 如果当前步骤文件指示用户必须批准并指示继续，你绝不要进入下一个步骤文件

---

## 初始化

### 配置加载

从 `{project-root}/_bmad/bmm/config.yaml` 加载配置，解析以下内容：

- `project_name`, `output_folder`, `planning_artifacts`, `user_name`
- `communication_language`, `document_output_language`, `user_skill_level`
- `date` 作为系统生成的当前日期时间
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

### 路径

- `installed_path` = `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture`
- `template_path` = `{installed_path}/architecture-decision-template.md`
- `data_files_path` = `{installed_path}/data/`

---

## 执行

完整阅读并遵循：`{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-01-init.md` 以开始工作流。

**注意：** 输入文档发现和所有初始化协议都在 step-01-init.md 中处理。