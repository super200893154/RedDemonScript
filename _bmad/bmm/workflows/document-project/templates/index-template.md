# {{project_name}} 文档索引

**类型：** {{repository_type}}{{#if is_multi_part}}，包含 {{parts_count}} 个部分{{/if}}
**主要语言：** {{primary_language}}
**架构：** {{architecture_type}}
**最后更新：** {{date}}

## 项目概述

{{project_description}}

{{#if is_multi_part}}

## 项目结构

本项目由 {{parts_count}} 个部分组成：

{{#each project_parts}}

### {{part_name}}（{{part_id}}）

- **类型：** {{project_type}}
- **位置：** `{{root_path}}`
- **技术栈：** {{tech_stack_summary}}
- **入口点：** {{entry_point}}
  {{/each}}

## 跨部分集成

{{integration_summary}}

{{/if}}

## 快速参考

{{#if is_single_part}}

- **技术栈：** {{tech_stack_summary}}
- **入口点：** {{entry_point}}
- **架构模式：** {{architecture_pattern}}
- **数据库：** {{database}}
- **部署：** {{deployment_platform}}
  {{else}}
  {{#each project_parts}}

### {{part_name}} 快速参考

- **技术栈：** {{tech_stack_summary}}
- **入口：** {{entry_point}}
- **模式：** {{architecture_pattern}}
  {{/each}}
  {{/if}}

## 生成的文档

### 核心文档

- [项目概述](./project-overview.md) - 执行摘要和高层架构
- [源码树分析](./source-tree-analysis.md) - 带注释的目录结构

{{#if is_single_part}}

- [架构](./architecture.md) - 详细技术架构
- [组件清单](./component-inventory.md) - 主要组件目录{{#if has_ui_components}}和 UI 元素{{/if}}
- [开发指南](./development-guide.md) - 本地设置和开发工作流
  {{#if has_api_docs}}- [API 契约](./api-contracts.md) - API 端点和模式{{/if}}
  {{#if has_data_models}}- [数据模型](./data-models.md) - 数据库模式和模型{{/if}}
  {{else}}

### 各部分文档

{{#each project_parts}}

#### {{part_name}}（{{part_id}}）

- [架构](./architecture-{{part_id}}.md) - {{part_name}} 的技术架构
  {{#if has_components}}- [组件](./component-inventory-{{part_id}}.md) - 组件目录{{/if}}
- [开发指南](./development-guide-{{part_id}}.md) - 设置和开发工作流
  {{#if has_api}}- [API 契约](./api-contracts-{{part_id}}.md) - API 文档{{/if}}
  {{#if has_data}}- [数据模型](./data-models-{{part_id}}.md) - 数据架构{{/if}}
  {{/each}}

### 集成

- [集成架构](./integration-architecture.md) - 各部分如何通信
- [项目部分元数据](./project-parts.json) - 机器可读结构
  {{/if}}

### 可选文档

{{#if has_deployment_guide}}- [部署指南](./deployment-guide.md) - 部署流程和基础设施{{/if}}
{{#if has_contribution_guide}}- [贡献指南](./contribution-guide.md) - 贡献指南和标准{{/if}}

## 现有文档

{{#if has_existing_docs}}
{{#each existing_docs}}

- [{{title}}]({{path}}) - {{description}}
  {{/each}}
  {{else}}
  项目中未找到现有文档文件。
  {{/if}}

## 入门

{{#if is_single_part}}

### 先决条件

{{prerequisites}}

### 设置

```bash
{{setup_commands}}
```

### 本地运行

```bash
{{run_commands}}
```

### 运行测试

```bash
{{test_commands}}
```

{{else}}
{{#each project_parts}}

### {{part_name}} 设置

**先决条件：** {{prerequisites}}

**安装并运行：**

```bash
cd {{root_path}}
{{setup_command}}
{{run_command}}
```

{{/each}}
{{/if}}

## AI 辅助开发

本文档专门为使 AI agent 能够理解和扩展此代码库而生成。

### 规划新功能时：

**仅 UI 功能：**
{{#if is_multi_part}}→ 参考：`architecture-{{ui_part_id}}.md`、`component-inventory-{{ui_part_id}}.md`{{else}}→ 参考：`architecture.md`、`component-inventory.md`{{/if}}

**API/后端功能：**
{{#if is_multi_part}}→ 参考：`architecture-{{api_part_id}}.md`、`api-contracts-{{api_part_id}}.md`、`data-models-{{api_part_id}}.md`{{else}}→ 参考：`architecture.md`{{#if has_api_docs}}、`api-contracts.md`{{/if}}{{#if has_data_models}}、`data-models.md`{{/if}}{{/if}}

**全栈功能：**
→ 参考：所有架构文档{{#if is_multi_part}} + `integration-architecture.md`{{/if}}

**部署变更：**
{{#if has_deployment_guide}}→ 参考：`deployment-guide.md`{{else}}→ 审查项目中的 CI/CD 配置{{/if}}

---

_文档由 BMAD 方法 `document-project` 工作流生成_