# Sprint 状态 - 多模式服务

<critical>工作流执行引擎由以下文件控制：{project-root}/_bmad/core/tasks/workflow.xml</critical>
<critical>你必须已经加载并处理：{project-root}/_bmad/bmm/workflows/4-implementation/sprint-status/workflow.yaml</critical>
<critical>模式：interactive（交互式，默认）、validate（验证）、data（数据）</critical>
<critical>⚠️ 绝对不要时间估算。不要提及小时、天、周或时间线。</critical>

<workflow>

<step n="0" goal="确定执行模式">
  <action>如果调用者提供了 {{mode}}，则设置 mode = {{mode}}；否则 mode = "interactive"</action>

  <check if="mode == data">
    <action>跳转到步骤 20</action>
  </check>

  <check if="mode == validate">
    <action>跳转到步骤 30</action>
  </check>

  <check if="mode == interactive">
    <action>继续到步骤 1</action>
  </check>
</step>

<step n="1" goal="定位 sprint 状态文件">
  <action>加载 {project_context} 获取项目范围的模式和约定（如果存在）</action>
  <action>尝试 {sprint_status_file}</action>
  <check if="file not found">
    <output>❌ 未找到 sprint-status.yaml。
运行 `/bmad:bmm:workflows:sprint-planning` 生成它，然后重新运行 sprint-status。</output>
    <action>退出工作流</action>
  </check>
  <action>继续到步骤 2</action>
</step>

<step n="2" goal="读取并解析 sprint-status.yaml">
  <action>读取完整文件：{sprint_status_file}</action>
  <action>解析字段：generated、project、project_key、tracking_system、story_location</action>
  <action>解析 development_status 映射。分类键：</action>
  - Epic：以 "epic-" 开头的键（且不以 "-retrospective" 结尾）
  - 回顾：以 "-retrospective" 结尾的键
  - Story：其他所有内容（例如 1-2-login-form）
  <action>映射旧版 story 状态 "drafted" → "ready-for-dev"</action>
  <action>统计 story 状态：backlog、ready-for-dev、in-progress、review、done</action>
  <action>映射旧版 epic 状态 "contexted" → "in-progress"</action>
  <action>统计 epic 状态：backlog、in-progress、done</action>
  <action>统计回顾状态：optional、done</action>

<action>验证所有状态是否符合已知值：</action>

- 有效的 story 状态：backlog、ready-for-dev、in-progress、review、done、drafted（旧版）
- 有效的 epic 状态：backlog、in-progress、done、contexted（旧版）
- 有效的回顾状态：optional、done

  <check if="any status is unrecognized">
    <output>
⚠️ **检测到未知状态：**
{{#each invalid_entries}}

- `{{key}}`："{{status}}"（未识别）
  {{/each}}

**有效状态：**

- Story：backlog、ready-for-dev、in-progress、review、done
- Epic：backlog、in-progress、done
- 回顾：optional、done
  </output>
  <ask>如何修正这些状态？
  {{#each invalid_entries}}
  {{@index}}. {{key}}："{{status}}" → [选择有效状态]
  {{/each}}

输入修正（例如 "1=in-progress, 2=backlog"）或输入 "skip" 继续而不修正：</ask>
<check if="user provided corrections">
<action>使用修正后的值更新 sprint-status.yaml</action>
<action>使用修正后的状态重新解析文件</action>
</check>
</check>

<action>检测风险：</action>

- 如果任何 story 状态为 "review"：建议 `/bmad:bmm:workflows:code-review`
- 如果任何 story 状态为 "in-progress" 且没有 story 状态为 "ready-for-dev"：建议专注于当前活跃的 story
- 如果所有 epic 状态为 "backlog" 且没有 story 状态为 "ready-for-dev"：提示 `/bmad:bmm:workflows:create-story`
- 如果 `generated` 时间戳超过 7 天：警告 "sprint-status.yaml 可能已过时"
- 如果任何 story 键不匹配 epic 模式（例如 story "5-1-..." 但没有 "epic-5"）：警告 "检测到孤立 story"
- 如果任何 epic 状态为 in-progress 但没有关联的 story：警告 "进行中的 epic 没有 story"
  </step>

<step n="3" goal="选择下一步行动建议">
  <action>使用以下优先级选择下一个推荐的工作流：</action>
  <note>选择"第一个" story 时：按 epic 编号排序，然后按 story 编号排序（例如 1-1 在 1-2 之前，在 2-1 之前）</note>
  1. 如果任何 story 状态 == in-progress → 为第一个进行中的 story 推荐 `dev-story`
  2. 否则如果任何 story 状态 == review → 为第一个审查中的 story 推荐 `code-review`
  3. 否则如果任何 story 状态 == ready-for-dev → 推荐 `dev-story`
  4. 否则如果任何 story 状态 == backlog → 推荐 `create-story`
  5. 否则如果任何回顾状态 == optional → 推荐 `retrospective`
  6. 否则 → 所有实现项目已完成；祝贺用户 - 你们一起完成了出色的工作！
  <action>将选定的推荐存储为：next_story_id、next_workflow_id、next_agent（SM/DEV 视情况而定）</action>
</step>

<step n="4" goal="显示摘要">
  <output>
## 📊 Sprint 状态

- 项目：{{project}}（{{project_key}}）
- 追踪：{{tracking_system}}
- 状态文件：{sprint_status_file}

**Story：** backlog {{count_backlog}}，ready-for-dev {{count_ready}}，in-progress {{count_in_progress}}，review {{count_review}}，done {{count_done}}

**Epic：** backlog {{epic_backlog}}，in-progress {{epic_in_progress}}，done {{epic_done}}

**下一步推荐：** /bmad:bmm:workflows:{{next_workflow_id}}（{{next_story_id}}）

{{#if risks}}
**风险：**
{{#each risks}}

- {{this}}
  {{/each}}
  {{/if}}

  </output>
  </step>

<step n="5" goal="提供操作选项">
  <ask>选择一个选项：
1）立即运行推荐的工作流
2）按状态显示所有 story
3）显示原始 sprint-status.yaml
4）退出
选择：</ask>

  <check if="choice == 1">
    <output>运行 `/bmad:bmm:workflows:{{next_workflow_id}}`。
如果命令针对某个 story，请在提示时设置 `story_key={{next_story_id}}`。</output>
  </check>

  <check if="choice == 2">
    <output>
### 按状态分组的 Story
- 进行中：{{stories_in_progress}}
- 审查中：{{stories_in_review}}
- 准备开发：{{stories_ready_for_dev}}
- 待办：{{stories_backlog}}
- 已完成：{{stories_done}}
    </output>
  </check>

  <check if="choice == 3">
    <action>显示 {sprint_status_file} 的完整内容</action>
  </check>

  <check if="choice == 4">
    <action>退出工作流</action>
  </check>
</step>

<!-- ========================= -->
<!-- 其他流程的数据模式 -->
<!-- ========================= -->

<step n="20" goal="数据模式输出">
  <action>加载并解析 {sprint_status_file}，同步骤 2</action>
  <action>计算推荐，同步骤 3</action>
  <template-output>next_workflow_id = {{next_workflow_id}}</template-output>
  <template-output>next_story_id = {{next_story_id}}</template-output>
  <template-output>count_backlog = {{count_backlog}}</template-output>
  <template-output>count_ready = {{count_ready}}</template-output>
  <template-output>count_in_progress = {{count_in_progress}}</template-output>
  <template-output>count_review = {{count_review}}</template-output>
  <template-output>count_done = {{count_done}}</template-output>
  <template-output>epic_backlog = {{epic_backlog}}</template-output>
  <template-output>epic_in_progress = {{epic_in_progress}}</template-output>
  <template-output>epic_done = {{epic_done}}</template-output>
  <template-output>risks = {{risks}}</template-output>
  <action>返回调用者</action>
</step>

<!-- ========================= -->
<!-- 验证模式 -->
<!-- ========================= -->

<step n="30" goal="验证 sprint-status 文件">
  <action>检查 {sprint_status_file} 是否存在</action>
  <check if="missing">
    <template-output>is_valid = false</template-output>
    <template-output>error = "sprint-status.yaml 缺失"</template-output>
    <template-output>suggestion = "运行 sprint-planning 创建它"</template-output>
    <action>返回</action>
  </check>

<action>读取并解析 {sprint_status_file}</action>

<action>验证是否存在必需的元数据字段：generated、project、project_key、tracking_system、story_location</action>
<check if="any required field missing">
<template-output>is_valid = false</template-output>
<template-output>error = "缺少必需字段：{{missing_fields}}"</template-output>
<template-output>suggestion = "重新运行 sprint-planning 或手动添加缺失字段"</template-output>
<action>返回</action>
</check>

<action>验证 development_status 部分是否存在且至少有一个条目</action>
<check if="development_status missing or empty">
<template-output>is_valid = false</template-output>
<template-output>error = "development_status 缺失或为空"</template-output>
<template-output>suggestion = "重新运行 sprint-planning 或手动修复文件"</template-output>
<action>返回</action>
</check>

<action>验证所有状态值是否符合已知有效状态：</action>

- Story：backlog、ready-for-dev、in-progress、review、done（旧版：drafted）
- Epic：backlog、in-progress、done（旧版：contexted）
- 回顾：optional、done
  <check if="any invalid status found">
  <template-output>is_valid = false</template-output>
  <template-output>error = "无效状态值：{{invalid_entries}}"</template-output>
  <template-output>suggestion = "修复 sprint-status.yaml 中的无效状态"</template-output>
  <action>返回</action>
  </check>

<template-output>is_valid = true</template-output>
<template-output>message = "sprint-status.yaml 有效：元数据完整，所有状态已识别"</template-output>
</step>

</workflow>