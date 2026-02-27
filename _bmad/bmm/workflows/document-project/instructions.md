# 文档项目工作流路由器

<critical>工作流执行引擎由以下文件控制：{project-root}/_bmad/core/tasks/workflow.xml</critical>
<critical>你必须已加载并处理：{project-root}/_bmad/bmm/workflows/document-project/workflow.yaml</critical>
<critical>所有响应使用 {communication_language} 交流</critical>

<workflow>

<critical>此路由器确定工作流模式并委托给专门的子工作流</critical>

<step n="1" goal="检查恢复能力并确定工作流模式">
<action>检查现有状态文件：{project_knowledge}/project-scan-report.json</action>

<check if="project-scan-report.json 存在">
  <action>读取状态文件并提取：timestamps、mode、scan_level、current_step、completed_steps、project_classification</action>
  <action>如果状态文件中存在，提取缓存的项目类型 ID</action>
  <action>计算状态文件年龄（当前时间 - last_updated）</action>

<ask>我发现来自 {{last_updated}} 的进行中工作流状态。

    **当前进度：**

    - 模式：{{mode}}
    - 扫描级别：{{scan_level}}
    - 已完成步骤：{{completed_steps_count}}/{{total_steps}}
    - 最后步骤：{{current_step}}
    - 项目类型：{{cached_project_types}}

    您想：

    1. **从上次离开的地方继续** - 从步骤 {{current_step}} 继续
    2. **重新开始** - 归档旧状态并开始新扫描
    3. **取消** - 退出不做更改

    您的选择 [1/2/3]：
</ask>

  <check if="用户选择 1">
    <action>设置 resume_mode = true</action>
    <action>设置 workflow_mode = {{mode}}</action>
    <action>从状态文件加载发现摘要</action>
    <action>从状态文件加载缓存的 project_type_id</action>

    <critical>恢复的条件性 CSV 加载：</critical>
    <action>对于每个缓存的 project_type_id，仅从 {documentation_requirements_csv} 加载对应行</action>
    <action>跳过加载 project-types.csv 和 architecture_registry.csv（恢复时不需要）</action>
    <action>存储加载的文档要求以供剩余步骤使用</action>

    <action>显示："正在从 {{current_step}} 恢复 {{workflow_mode}}，缓存的项目类型：{{cached_project_types}}"</action>

    <check if="workflow_mode == deep_dive">
      <action>完整阅读并遵循：{installed_path}/workflows/deep-dive-instructions.md，带恢复上下文</action>
    </check>

    <check if="workflow_mode == initial_scan OR workflow_mode == full_rescan">
      <action>完整阅读并遵循：{installed_path}/workflows/full-scan-instructions.md，带恢复上下文</action>
    </check>

  </check>

  <check if="用户选择 2">
    <action>创建归档目录：{project_knowledge}/.archive/</action>
    <action>将旧状态文件移动到：{project_knowledge}/.archive/project-scan-report-{{timestamp}}.json</action>
    <action>设置 resume_mode = false</action>
    <action>继续到步骤 0.5</action>
  </check>

  <check if="用户选择 3">
    <action>显示："退出工作流不做更改。"</action>
    <action>退出工作流</action>
  </check>

  <check if="状态文件年龄 >= 24 小时">
    <action>显示："发现旧状态文件（>24小时）。开始全新扫描。"</action>
    <action>归档旧状态文件到：{project_knowledge}/.archive/project-scan-report-{{timestamp}}.json</action>
    <action>设置 resume_mode = false</action>
    <action>继续到步骤 0.5</action>
  </check>

</step>

<step n="3" goal="检查现有文档并确定工作流模式" if="resume_mode == false">
<action>检查 {project_knowledge}/index.md 是否存在</action>

<check if="index.md 存在">
  <action>读取现有 index.md 提取元数据（日期、项目结构、部分数量）</action>
  <action>存储为 {{existing_doc_date}}、{{existing_structure}}</action>

<ask>我发现在 {{existing_doc_date}} 生成的现有文档。

您想做什么？

1. **重新扫描整个项目** - 用最新更改更新所有文档
2. **深入特定区域** - 为特定功能/模块/文件夹生成详细文档
3. **取消** - 保持现有文档不变

您的选择 [1/2/3]：
</ask>

  <check if="用户选择 1">
    <action>设置 workflow_mode = "full_rescan"</action>
    <action>显示："开始完整项目重新扫描..."</action>
    <action>完整阅读并遵循：{installed_path}/workflows/full-scan-instructions.md</action>
    <action>子工作流完成后，继续到步骤 4</action>
  </check>

  <check if="用户选择 2">
    <action>设置 workflow_mode = "deep_dive"</action>
    <action>设置 scan_level = "exhaustive"</action>
    <action>显示："开始深入文档模式..."</action>
    <action>完整阅读并遵循：{installed_path}/workflows/deep-dive-instructions.md</action>
    <action>子工作流完成后，继续到步骤 4</action>
  </check>

  <check if="用户选择 3">
    <action>显示消息："保持现有文档。退出工作流。"</action>
    <action>退出工作流</action>
  </check>
</check>

<check if="index.md 不存在">
  <action>设置 workflow_mode = "initial_scan"</action>
  <action>显示："未找到现有文档。开始初始项目扫描..."</action>
  <action>完整阅读并遵循：{installed_path}/workflows/full-scan-instructions.md</action>
  <action>子工作流完成后，继续到步骤 4</action>
</check>

</step>

</workflow>