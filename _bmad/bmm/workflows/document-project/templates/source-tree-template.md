# {{project_name}} - 源码树分析

**日期：** {{date}}

## 概述

{{source_tree_overview}}

{{#if is_multi_part}}

## 多部分结构

本项目组织为 {{parts_count}} 个独立部分：

{{#each project_parts}}

- **{{part_name}}**（`{{root_path}}`）：{{purpose}}
  {{/each}}
  {{/if}}

## 完整目录结构

```
{{complete_source_tree}}
```

## 关键目录

{{#each critical_folders}}

### `{{folder_path}}`

{{description}}

**用途：** {{purpose}}
**包含：** {{contents_summary}}
{{#if entry_points}}**入口点：** {{entry_points}}{{/if}}
{{#if integration_note}}**集成：** {{integration_note}}{{/if}}

{{/each}}

{{#if is_multi_part}}

## 各部分源码树

{{#each project_parts}}

### {{part_name}} 结构

```
{{source_tree}}
```

**关键目录：**
{{#each critical_directories}}

- **`{{path}}`**：{{description}}
  {{/each}}

{{/each}}

## 集成点

{{#each integration_points}}

### {{from_part}} → {{to_part}}

- **位置：** `{{integration_path}}`
- **类型：** {{integration_type}}
- **详情：** {{details}}
  {{/each}}

{{/if}}

## 入口点

{{#if is_single_part}}

- **主入口：** `{{main_entry_point}}`
  {{#if additional_entry_points}}
- **其他：**
  {{#each additional_entry_points}}
  - `{{path}}`：{{description}}
    {{/each}}
    {{/if}}
    {{else}}
    {{#each project_parts}}

### {{part_name}}

- **入口点：** `{{entry_point}}`
- **启动：** {{bootstrap_description}}
  {{/each}}
  {{/if}}

## 文件组织模式

{{file_organization_patterns}}

## 关键文件类型

{{#each file_type_patterns}}

### {{file_type}}

- **模式：** `{{pattern}}`
- **用途：** {{purpose}}
- **示例：** {{examples}}
  {{/each}}

## 资产位置

{{#if has_assets}}
{{#each asset_locations}}

- **{{asset_type}}**：`{{location}}`（{{file_count}} 个文件，{{total_size}}）
  {{/each}}
  {{else}}
  未检测到重要资产。
  {{/if}}

## 配置文件

{{#each config_files}}

- **`{{path}}`**：{{description}}
  {{/each}}

## 开发备注

{{development_notes}}

---

_使用 BMAD 方法 `document-project` 工作流生成_