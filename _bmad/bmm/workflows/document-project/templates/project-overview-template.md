# {{project_name}} - 项目概述

**日期：** {{date}}
**类型：** {{project_type}}
**架构：** {{architecture_type}}

## 执行摘要

{{executive_summary}}

## 项目分类

- **仓库类型：** {{repository_type}}
- **项目类型：** {{project_types_list}}
- **主要语言：** {{primary_languages}}
- **架构模式：** {{architecture_pattern}}

{{#if is_multi_part}}

## 多部分结构

本项目由 {{parts_count}} 个独立部分组成：

{{#each project_parts}}

### {{part_name}}

- **类型：** {{project_type}}
- **位置：** `{{root_path}}`
- **用途：** {{purpose}}
- **技术栈：** {{tech_stack}}
  {{/each}}

### 部分如何集成

{{integration_description}}
{{/if}}

## 技术栈摘要

{{#if is_single_part}}
{{technology_table}}
{{else}}
{{#each project_parts}}

### {{part_name}} 技术栈

{{technology_table}}
{{/each}}
{{/if}}

## 关键功能

{{key_features}}

## 架构亮点

{{architecture_highlights}}

## 开发概述

### 先决条件

{{prerequisites}}

### 入门

{{getting_started_summary}}

### 关键命令

{{#if is_single_part}}

- **安装：** `{{install_command}}`
- **开发：** `{{dev_command}}`
- **构建：** `{{build_command}}`
- **测试：** `{{test_command}}`
  {{else}}
  {{#each project_parts}}

#### {{part_name}}

- **安装：** `{{install_command}}`
- **开发：** `{{dev_command}}`
  {{/each}}
  {{/if}}

## 仓库结构

{{repository_structure_summary}}

## 文档导航

详细信息请参阅：

- [index.md](./index.md) - 主文档索引
- [architecture.md](./architecture{{#if is_multi_part}}-{part_id}{{/if}}.md) - 详细架构
- [source-tree-analysis.md](./source-tree-analysis.md) - 目录结构
- [development-guide.md](./development-guide{{#if is_multi_part}}-{part_id}{{/if}}.md) - 开发工作流

---

_使用 BMAD 方法 `document-project` 工作流生成_