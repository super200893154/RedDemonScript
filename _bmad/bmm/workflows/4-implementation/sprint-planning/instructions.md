# Sprint 规划 - Sprint 状态生成器

<critical>工作流执行引擎由以下文件控制：{project-root}/_bmad/core/tasks/workflow.xml</critical>
<critical>你必须已经加载并处理：{project-root}/_bmad/bmm/workflows/4-implementation/sprint-planning/workflow.yaml</critical>

## 📚 文档发现 - 完整 Epic 加载

**策略**：Sprint 规划需要所有 Epic 和 Story 来构建完整的状态跟踪。

**Epic 发现流程：**

1. **首先搜索完整文档** - 查找 `epics.md`、`bmm-epics.md` 或任何 `*epic*.md` 文件
2. **检查分片版本** - 如果未找到完整文档，查找 `epics/index.md`
3. **如果找到分片版本**：
   - 读取 `index.md` 了解文档结构
   - 读取索引中列出的所有 Epic 部分文件（例如 `epic-1.md`、`epic-2.md` 等）
   - 从合并的内容中处理所有 Epic 及其 Story
   - 这确保完整的 Sprint 状态覆盖
4. **优先级**：如果完整版和分片版同时存在，使用完整文档

**模糊匹配**：对文档名称保持灵活 - 用户可能使用变体如 `epics.md`、`bmm-epics.md`、`user-stories.md` 等

<workflow>

<step n="1" goal="解析 Epic 文件并提取所有工作项">
<action>加载 {project_context} 获取项目范围的模式和约定（如果存在）</action>
<action>使用 {communication_language} 与 {user_name} 沟通</action>
<action>在 {epics_location} 中查找匹配 `{epics_pattern}` 的所有文件</action>
<action>可能是单个 `epics.md` 文件或多个 `epic-1.md`、`epic-2.md` 文件</action>

<action>对于找到的每个 Epic 文件，提取：</action>

- 来自标题的 Epic 编号，如 `## Epic 1:` 或 `## Epic 2:`
- 来自模式的 Story ID 和标题，如 `### Story 1.1: User Authentication`
- 将 Story 格式从 `Epic.Story: Title` 转换为 kebab-case 键：`epic-story-title`

**Story ID 转换规则：**

- 原始格式：`### Story 1.1: User Authentication`
- 将句点替换为连字符：`1-1`
- 将标题转换为 kebab-case：`user-authentication`
- 最终键：`1-1-user-authentication`

<action>从所有 Epic 文件构建所有 Epic 和 Story 的完整清单</action>
</step>

  <step n="0.5" goal="发现并加载项目文档">
    <invoke-protocol name="discover_inputs" />
    <note>发现后，这些内容变量可用：{epics_content}（所有 Epic 已加载 - 使用 FULL_LOAD 策略）</note>
  </step>

<step n="2" goal="构建 Sprint 状态结构">
<action>对于找到的每个 Epic，按以下顺序创建条目：</action>

1. **Epic 条目** - 键：`epic-{num}`，默认状态：`backlog`
2. **Story 条目** - 键：`{epic}-{story}-{title}`，默认状态：`backlog`
3. **回顾条目** - 键：`epic-{num}-retrospective`，默认状态：`optional`

**示例结构：**

```yaml
development_status:
  epic-1: backlog
  1-1-user-authentication: backlog
  1-2-account-management: backlog
  epic-1-retrospective: optional
```

</step>

<step n="3" goal="应用智能状态检测">
<action>对于每个 Story，通过检查文件检测当前状态：</action>

**Story 文件检测：**

- 检查：`{story_location_absolute}/{story-key}.md`（例如 `stories/1-1-user-authentication.md`）
- 如果存在 → 将状态至少升级为 `ready-for-dev`

**保留规则：**

- 如果存在 `{status_file}` 且有更高级的状态，则保留它
- 永远不要降级状态（例如不要将 `done` 改为 `ready-for-dev`）

**状态流转参考：**

- Epic：`backlog` → `in-progress` → `done`
- Story：`backlog` → `ready-for-dev` → `in-progress` → `review` → `done`
- 回顾：`optional` ↔ `done`
  </step>

<step n="4" goal="生成 Sprint 状态文件">
<action>使用以下内容创建或更新 {status_file}：</action>

**文件结构：**

```yaml
# generated: {date}
# project: {project_name}
# project_key: {project_key}
# tracking_system: {tracking_system}
# story_location: {story_location}

# 状态定义：
# ==================
# Epic 状态：
#   - backlog：Epic 尚未开始
#   - in-progress：Epic 正在进行中
#   - done：Epic 中的所有 Story 已完成
#
# Epic 状态转换：
#   - backlog → in-progress：创建第一个 Story 时自动转换（通过 create-story）
#   - in-progress → done：当所有 Story 达到 'done' 状态时手动转换
#
# Story 状态：
#   - backlog：Story 仅存在于 Epic 文件中
#   - ready-for-dev：Story 文件已在 stories 文件夹中创建
#   - in-progress：开发人员正在积极进行实现
#   - review：准备进行代码审查（通过 Dev 的 code-review 工作流）
#   - done：Story 已完成
#
# 回顾状态：
#   - optional：可以完成但不是必需的
#   - done：回顾已完成
#
# 工作流说明：
# ===============
# - 当创建第一个 Story 时，Epic 自动转换为 'in-progress'
# - 如果团队容量允许，Story 可以并行进行
# - SM 通常在上一个 Story 'done' 后创建下一个 Story，以融入学习成果
# - Dev 将 Story 移至 'review'，然后运行 code-review（建议使用新上下文、不同的 LLM）

generated: { date }
project: { project_name }
project_key: { project_key }
tracking_system: { tracking_system }
story_location: { story_location }

development_status:
  # 所有 Epic、Story 和回顾按顺序排列
```

<action>将完整的 Sprint 状态 YAML 写入 {status_file}</action>
<action>关键：元数据出现两次 - 一次作为注释（#）用于文档，一次作为 YAML key:value 字段用于解析</action>
<action>确保所有项目按顺序排列：epic、其 story、其回顾、下一个 epic...</action>
</step>

<step n="5" goal="验证并报告">
<action>执行验证检查：</action>

- [ ] Epic 文件中的每个 Epic 都出现在 {status_file} 中
- [ ] Epic 文件中的每个 Story 都出现在 {status_file} 中
- [ ] 每个 Epic 都有对应的回顾条目
- [ ] {status_file} 中没有不存在于 Epic 文件中的项目
- [ ] 所有状态值合法（匹配状态机定义）
- [ ] 文件是有效的 YAML 语法

<action>统计总数：</action>

- Epic 总数：{{epic_count}}
- Story 总数：{{story_count}}
- 进行中的 Epic：{{in_progress_count}}
- 已完成的 Story：{{done_count}}

<action>使用 {communication_language} 向 {user_name} 显示完成摘要：</action>

**Sprint 状态生成成功**

- **文件位置：** {status_file}
- **Epic 总数：** {{epic_count}}
- **Story 总数：** {{story_count}}
- **进行中的 Epic：** {{epics_in_progress_count}}
- **已完成的 Story：** {{done_count}}

**下一步：**

1. 审查生成的 {status_file}
2. 使用此文件跟踪开发进度
3. Agent 在工作时会更新状态
4. 重新运行此工作流以刷新自动检测的状态

</step>

</workflow>

## 附加文档

### 状态状态机

**Epic 状态流程：**

```
backlog → in-progress → done
```

- **backlog**：Epic 尚未开始
- **in-progress**：Epic 正在进行中（正在创建/实现 Story）
- **done**：Epic 中的所有 Story 已完成

**Story 状态流程：**

```
backlog → ready-for-dev → in-progress → review → done
```

- **backlog**：Story 仅存在于 Epic 文件中
- **ready-for-dev**：Story 文件已创建（例如 `stories/1-3-plant-naming.md`）
- **in-progress**：开发人员正在积极工作
- **review**：准备进行代码审查（通过 Dev 的 code-review 工作流）
- **done**：已完成

**回顾状态：**

```
optional ↔ done
```

- **optional**：可以进行但不是必需的
- **done**：已完成

### 指南

1. **Epic 激活**：开始处理其第一个 Story 时将 Epic 标记为 `in-progress`
2. **顺序默认**：Story 通常按顺序进行，但支持并行工作
3. **支持并行工作**：如果团队容量允许，多个 Story 可以同时处于 `in-progress`
4. **完成后审查**：Story 在 `done` 之前应经过 `review`
5. **学习传递**：SM 通常在上一个 Story `done` 后创建下一个 Story，以融入学习成果