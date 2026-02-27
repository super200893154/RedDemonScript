# Deep-Dive 文档说明

<workflow>

<critical>此工作流对特定区域执行详尽的深入文档记录</critical>
<critical>调用者：../document-project/instructions.md 路由器</critical>
<critical>处理：仅 deep_dive 模式</critical>

<step n="13" goal="特定区域的深入文档记录" if="workflow_mode == deep_dive">
<critical>Deep-dive 模式需要字面上的完整文件审查。采样、猜测或仅依赖工具输出是被禁止的。</critical>
<action>从 index.md 和 project-parts.json 加载现有项目结构（如果存在）</action>
<action>加载源码树分析以了解可用区域</action>

<step n="13a" goal="识别深入区域">
  <action>分析现有文档以建议深入选项</action>

<ask>您想深入哪个区域？

**基于项目结构的建议区域：**

{{#if has_api_routes}}

## API 路由（发现 {{api_route_count}} 个端点）

{{#each api_route_groups}}
{{group_index}}. {{group_name}} - {{endpoint_count}} 个端点位于 `{{path}}`
{{/each}}
{{/if}}

{{#if has_feature_modules}}

## 功能模块（{{feature_count}} 个功能）

{{#each feature_modules}}
{{module_index}}. {{module_name}} - {{file_count}} 个文件位于 `{{path}}`
{{/each}}
{{/if}}

{{#if has_ui_components}}

### UI 组件区域

{{#each component_groups}}
{{group_index}}. {{group_name}} - {{component_count}} 个组件位于 `{{path}}`
{{/each}}
{{/if}}

{{#if has_services}}

### 服务/业务逻辑

{{#each service_groups}}
{{service_index}}. {{service_name}} - `{{path}}`
{{/each}}
{{/if}}

**或指定自定义：**

- 文件夹路径（例如 "client/src/features/dashboard"）
- 文件路径（例如 "server/src/api/users.ts"）
- 功能名称（例如 "authentication system"）

输入您的选择（数字或自定义路径）：
</ask>

<action>解析用户输入以确定：- target_type："folder" | "file" | "feature" | "api_group" | "component_group" - target_path：要扫描的绝对路径 - target_name：文档的人类可读名称 - target_scope：要分析的所有文件列表
</action>

<action>存储为 {{deep_dive_target}}</action>

<action>显示确认：
目标：{{target_name}}
类型：{{target_type}}
路径：{{target_path}}
估计分析文件数：{{estimated_file_count}}

这将读取此区域的每个文件。继续？[y/n]
</action>

<action if="用户确认 'n'">返回步骤 13a（选择其他区域）</action>
</step>

<step n="13b" goal="目标区域的全面详尽扫描">
  <action>设置 scan_mode = "exhaustive"</action>
  <action>初始化 file_inventory = []</action>
  <critical>您必须读取范围内每个文件的每一行，并捕获未来开发者 agent 可以采取行动的自然语言解释（文件做什么、副作用、为什么重要）。不允许捷径。</critical>

  <check if="target_type == folder">
    <action>从 {{target_path}} 获取完整递归文件列表</action>
    <action>过滤掉：node_modules/、.git/、dist/、build/、coverage/、*.min.js、*.map</action>
    <action>对于文件夹中剩余的每个文件：
      - 读取完整文件内容（所有行）
      - 提取所有导出（函数、类、类型、接口、常量）
      - 提取所有导入（依赖项）
      - 从注释和代码结构识别用途
      - 用自然语言写 1-2 句话（最少）描述行为、副作用、假设和开发者在修改文件前必须知道的任何内容
      - 提取带参数类型和返回类型的函数签名
      - 注意任何 TODO、FIXME 或注释
      - 识别模式（hooks、components、services、controllers 等）
      - 捕获每个文件的贡献者指导：`contributor_note`、`risks`、`verification_steps`、`suggested_tests`
      - 存储在 file_inventory
    </action>
  </check>

  <check if="target_type == file">
    <action>读取 {{target_path}} 的完整文件</action>
    <action>提取所有信息如上</action>
    <action>读取它导入的所有文件（跟随导入链 1 层深度）</action>
    <action>查找导入此文件的所有文件（通过 grep 查找依赖方）</action>
    <action>将所有内容存储在 file_inventory</action>
  </check>

  <check if="target_type == api_group">
    <action>识别 API 组中的所有路由/控制器文件</action>
    <action>完整读取所有路由处理器</action>
    <action>读取关联的中间件、控制器、服务</action>
    <action>读取使用的数据模型和模式</action>
    <action>提取完整的请求/响应模式</action>
    <action>记录认证和授权要求</action>
    <action>将所有内容存储在 file_inventory</action>
  </check>

  <check if="target_type == feature">
    <action>在代码库中搜索与功能名称相关的所有文件</action>
    <action>包括：UI 组件、API 端点、模型、服务、测试</action>
    <action>完整读取每个文件</action>
    <action>将所有内容存储在 file_inventory</action>
  </check>

  <check if="target_type == component_group">
    <action>获取组中的所有组件文件</action>
    <action>完整读取每个组件</action>
    <action>提取：Props 接口、使用的 hooks、子组件、状态管理</action>
    <action>将所有内容存储在 file_inventory</action>
  </check>

<action>对于 file_inventory 中的每个文件，记录：- **文件路径：** 完整路径 - **用途：** 此文件做什么（1-2 句话） - **代码行数：** 总 LOC - **导出：** 带签名的完整列表

- 函数：`functionName(param: Type): ReturnType` - 描述
  - 类：`ClassName` - 描述及关键方法
  - 类型/接口：`TypeName` - 描述
  - 常量：`CONSTANT_NAME: Type` - 描述 - **导入/依赖：** 它使用什么及原因 - **被使用于：** 导入此文件的文件（依赖方） - **关键实现细节：** 重要逻辑、算法、模式 - **状态管理：** 如适用（Redux、Context、本地状态） - **副作用：** API 调用、数据库查询、文件 I/O、外部服务 - **错误处理：** Try/catch 块、错误边界、验证 - **测试：** 关联的测试文件和覆盖率 - **注释/TODO：** 任何内联文档或计划工作
    </action>

<template-output>comprehensive_file_inventory</template-output>
</step>

<step n="13c" goal="分析关系和数据流">
  <action>为扫描区域构建依赖图：
    - 创建以文件为节点的图
    - 为导入关系添加边
    - 识别循环依赖（如有）
    - 查找入口点（范围内不被其他文件导入的文件）
    - 查找叶节点（不导入范围内其他文件的文件）
  </action>

<action>跟踪系统中的数据流：- 跟随函数调用和数据转换 - 跟踪 API 调用及其响应 - 记录状态更新和传播 - 映射数据库查询和变更
</action>

<action>识别集成点：- 消费的外部 API - 调用的内部 API/服务 - 访问的共享状态 - 发布/订阅的事件 - 访问的数据库表
</action>

<template-output>dependency_graph</template-output>
<template-output>data_flow_analysis</template-output>
<template-output>integration_points</template-output>
</step>

<step n="13d" goal="查找相关代码和类似模式">
  <action>在扫描区域外的代码库中搜索：
    - 类似的文件/文件夹命名模式
    - 类似的函数签名
    - 类似的组件结构
    - 类似的 API 模式
    - 可用的可复用工具
  </action>

<action>识别代码复用机会：- 可用的共享工具 - 其他地方使用的设计模式 - 可用的组件库 - 可应用的辅助函数
</action>

<action>查找参考实现：- 代码库其他部分的类似功能 - 要遵循的既定模式 - 其他地方使用的测试方法
</action>

<template-output>related_code_references</template-output>
<template-output>reuse_opportunities</template-output>
</step>

<step n="13e" goal="生成全面的深入文档">
  <action>创建文档文件名：deep-dive-{{sanitized_target_name}}.md</action>
  <action>跨文件聚合贡献者见解：
    - 将独特的风险/陷阱备注合并到 {{risks_notes}}
    - 将开发者更改前应运行的验证步骤合并到 {{verification_steps}}
    - 将推荐的测试命令合并到 {{suggested_tests}}
  </action>

<action>从以下位置加载完整的深入模板：{installed_path}/templates/deep-dive-template.md</action>
<action>用步骤 13b-13d 收集的所有数据填写模板</action>
<action>将填写好的模板写入：{project_knowledge}/deep-dive-{{sanitized_target_name}}.md</action>
<action>验证深入文档完整性</action>

<template-output>deep_dive_documentation</template-output>

<action>更新状态文件：- 添加到 deep_dive_targets 数组：{"target_name": "{{target_name}}", "target_path": "{{target_path}}", "files_analyzed": {{file_count}}, "output_file": "deep-dive-{{sanitized_target_name}}.md", "timestamp": "{{now}}"} - 添加输出到 outputs_generated - 更新 last_updated 时间戳
</action>
</step>

<step n="13f" goal="用深入链接更新主索引">
  <action>读取现有 index.md</action>

<action>检查是否存在"Deep-Dive 文档"部分</action>

  <check if="部分不存在">
    <action>在"生成的文档"后添加新部分：

## Deep-Dive 文档

特定区域的详细详尽分析：

    </action>

  </check>

<action>添加新深入文档链接：

- [{{target_name}} 深入](./deep-dive-{{sanitized_target_name}}.md) - {{target_description}} 的全面分析（{{file_count}} 个文件，{{total_loc}} LOC） - 生成于 {{date}}
  </action>

  <action>更新索引元数据：
  最后更新：{{date}}
  Deep-Dives：{{deep_dive_count}}
  </action>

  <action>保存更新后的 index.md</action>

  <template-output>updated_index</template-output>
  </step>

<step n="13g" goal="提供继续或完成选项">
  <action>显示摘要：

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

## Deep-Dive 文档完成！✓

**生成于：** {project_knowledge}/deep-dive-{{target_name}}.md
**分析文件：** {{file_count}}
**扫描代码行数：** {{total_loc}}
**耗时：** ~{{duration}}

**文档包含：**

- 包含所有导出的完整文件清单
- 依赖图和数据流
- 集成点和 API 契约
- 测试分析和覆盖率
- 相关代码和复用机会
- 实现指导

**索引已更新：** {project_knowledge}/index.md 现在包含此深入的链接

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
</action>

<ask>您想：

1. **深入另一个区域** - 分析另一个功能/模块/文件夹
2. **完成** - 完成工作流

您的选择 [1/2]：
</ask>

  <action if="用户选择 1">
    <action>清除当前 deep_dive_target</action>
    <action>转到步骤 13a（选择新区域）</action>
  </action>

  <action if="用户选择 2">
    <action>显示最终消息：

所有深入文档完成！

**主索引：** {project_knowledge}/index.md
**生成的深入文档：** {{deep_dive_count}}

这些全面的文档现已可用于：

- 架构审查
- 实现规划
- 代码理解
- 棕地 PRD 创建

感谢使用 document-project 工作流！
</action>
<action>退出工作流</action>
</action>
</step>
</step>

</workflow>