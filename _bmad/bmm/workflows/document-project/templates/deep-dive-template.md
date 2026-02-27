# {{target_name}} - 深入文档

**生成时间：** {{date}}
**范围：** {{target_path}}
**分析文件：** {{file_count}}
**代码行数：** {{total_loc}}
**工作流模式：** Exhaustive Deep-Dive

## 概述

{{target_description}}

**用途：** {{target_purpose}}
**关键职责：** {{responsibilities}}
**集成点：** {{integration_summary}}

## 完整文件清单

{{#each files_in_inventory}}

### {{file_path}}

**用途：** {{purpose}}
**代码行数：** {{loc}}
**文件类型：** {{file_type}}

**未来贡献者必知：** {{contributor_note}}

**导出：**
{{#each exports}}

- `{{signature}}` - {{description}}
  {{/each}}

**依赖：**
{{#each imports}}

- `{{import_path}}` - {{reason}}
  {{/each}}

**被使用于：**
{{#each dependents}}

- `{{dependent_path}}`
  {{/each}}

**关键实现细节：**

```{{language}}
{{key_code_snippet}}
```

{{implementation_notes}}

**使用的模式：**
{{#each patterns}}

- {{pattern_name}}：{{pattern_description}}
  {{/each}}

**状态管理：** {{state_approach}}

**副作用：**
{{#each side_effects}}

- {{effect_type}}：{{effect_description}}
  {{/each}}

**错误处理：** {{error_handling_approach}}

**测试：**

- 测试文件：{{test_file_path}}
- 覆盖率：{{coverage_percentage}}%
- 测试方法：{{test_approach}}

**注释/TODO：**
{{#each todos}}

- 行 {{line_number}}：{{todo_text}}
  {{/each}}

---

{{/each}}

## 贡献者清单

- **风险和陷阱：** {{risks_notes}}
- **更改前验证步骤：** {{verification_steps}}
- **PR 前建议测试：** {{suggested_tests}}

## 架构与设计模式

### 代码组织

{{organization_approach}}

### 设计模式

{{#each design_patterns}}

- **{{pattern_name}}**：{{usage_description}}
  {{/each}}

### 状态管理策略

{{state_management_details}}

### 错误处理理念

{{error_handling_philosophy}}

### 测试策略

{{testing_strategy}}

## 数据流

{{data_flow_diagram}}

### 数据入口点

{{#each entry_points}}

- **{{entry_name}}**：{{entry_description}}
  {{/each}}

### 数据转换

{{#each transformations}}

- **{{transformation_name}}**：{{transformation_description}}
  {{/each}}

### 数据出口点

{{#each exit_points}}

- **{{exit_name}}**：{{exit_description}}
  {{/each}}

## 集成点

### 消费的 API

{{#each apis_consumed}}

- **{{api_endpoint}}**：{{api_description}}
  - 方法：{{method}}
  - 认证：{{auth_requirement}}
  - 响应：{{response_schema}}
    {{/each}}

### 暴露的 API

{{#each apis_exposed}}

- **{{api_endpoint}}**：{{api_description}}
  - 方法：{{method}}
  - 请求：{{request_schema}}
  - 响应：{{response_schema}}
    {{/each}}

### 共享状态

{{#each shared_state}}

- **{{state_name}}**：{{state_description}}
  - 类型：{{state_type}}
  - 访问者：{{accessors}}
    {{/each}}

### 事件

{{#each events}}

- **{{event_name}}**：{{event_description}}
  - 类型：{{publish_or_subscribe}}
  - 载荷：{{payload_schema}}
    {{/each}}

### 数据库访问

{{#each database_operations}}

- **{{table_name}}**：{{operation_type}}
  - 查询：{{query_patterns}}
  - 使用的索引：{{indexes}}
    {{/each}}

## 依赖图

{{dependency_graph_visualization}}

### 入口点（范围内不被其他文件导入）

{{#each entry_point_files}}

- {{file_path}}
  {{/each}}

### 叶节点（不导入范围内其他文件）

{{#each leaf_files}}

- {{file_path}}
  {{/each}}

### 循环依赖

{{#if has_circular_dependencies}}
⚠️ 检测到循环依赖：
{{#each circular_deps}}

- {{cycle_description}}
  {{/each}}
  {{else}}
  ✓ 未检测到循环依赖
  {{/if}}

## 测试分析

### 测试覆盖率摘要

- **语句：** {{statements_coverage}}%
- **分支：** {{branches_coverage}}%
- **函数：** {{functions_coverage}}%
- **行：** {{lines_coverage}}%

### 测试文件

{{#each test_files}}

- **{{test_file_path}}**
  - 测试：{{test_count}}
  - 方法：{{test_approach}}
  - Mock 策略：{{mocking_strategy}}
    {{/each}}

### 可用测试工具

{{#each test_utilities}}

- `{{utility_name}}`：{{utility_description}}
  {{/each}}

### 测试缺口

{{#each testing_gaps}}

- {{gap_description}}
  {{/each}}

## 相关代码和复用机会

### 其他地方的类似功能

{{#each similar_features}}

- **{{feature_name}}**（`{{feature_path}}`）
  - 相似性：{{similarity_description}}
  - 可参考用途：{{reference_use_case}}
    {{/each}}

### 可复用的工具

{{#each reusable_utilities}}

- **{{utility_name}}**（`{{utility_path}}`）
  - 用途：{{utility_purpose}}
  - 使用方法：{{usage_example}}
    {{/each}}

### 遵循的模式

{{#each patterns_to_follow}}

- **{{pattern_name}}**：参考 `{{reference_file}}` 了解实现
  {{/each}}

## 实现备注

### 代码质量观察

{{#each quality_observations}}

- {{observation}}
  {{/each}}

### TODO 和未来工作

{{#each all_todos}}

- **{{file_path}}:{{line_number}}**：{{todo_text}}
  {{/each}}

### 已知问题

{{#each known_issues}}

- {{issue_description}}
  {{/each}}

### 优化机会

{{#each optimizations}}

- {{optimization_suggestion}}
  {{/each}}

### 技术债务

{{#each tech_debt_items}}

- {{debt_description}}
  {{/each}}

## 修改指导

### 添加新功能

{{modification_guidance_add}}

### 修改现有功能

{{modification_guidance_modify}}

### 移除/废弃

{{modification_guidance_remove}}

### 更改测试清单

{{#each testing_checklist_items}}

- [ ] {{checklist_item}}
      {{/each}}

---

_由 `document-project` 工作流（deep-dive 模式）生成_
_基础文档：docs/index.md_
_扫描日期：{{date}}_
_分析模式：Exhaustive_