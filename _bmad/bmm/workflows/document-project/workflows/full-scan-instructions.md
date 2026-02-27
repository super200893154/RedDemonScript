# 完整项目扫描说明

<workflow>

<critical>此工作流执行完整项目文档生成（步骤 1-12）</critical>
<critical>调用者：document-project/instructions.md 路由器</critical>
<critical>处理：initial_scan 和 full_rescan 模式</critical>

<step n="0.5" goal="为新开始加载文档需求数据（恢复时不需要）" if="resume_mode == false">
<critical>数据加载策略 - 理解文档需求系统：</critical>

<action>向用户显示说明：

**项目类型检测如何工作：**

此工作流使用单个综合 CSV 文件智能地记录您的项目：

**documentation-requirements.csv**（{documentation_requirements_csv}）

- 包含 12 种项目类型（web、mobile、backend、cli、library、desktop、game、data、extension、infra、embedded）
- 24 列架构，结合项目类型检测和文档需求
- **检测列**：project_type_id、key_file_patterns（用于从代码库识别项目类型）
- **需求列**：requires_api_scan、requires_data_models、requires_ui_components 等
- **模式列**：critical_directories、test_file_patterns、config_patterns 等
- 充当"扫描指南" - 告诉工作流在哪里查找和记录什么
- 示例：对于 project_type_id="web"，key_file_patterns 包括 "package.json;tsconfig.json;\*.config.js"，requires_api_scan=true

**何时加载文档需求：**

- **全新开始（initial_scan）**：加载所有 12 行 → 使用 key_file_patterns 检测类型 → 使用该行的需求
- **恢复**：仅加载缓存 project_type_id 的文档需求行
- **完全重新扫描**：与全新开始相同（可能重新检测项目类型）
- **Deep Dive**：仅加载正在深入的部分的文档需求
  </action>

<action>现在为新开始加载文档需求数据...</action>

<action>从以下位置加载 documentation-requirements.csv：{documentation_requirements_csv}</action>
<action>存储所有 12 行，按 project_type_id 索引用于项目检测和需求查找</action>
<action>显示："已加载 12 种项目类型的文档需求（web、mobile、backend、cli、library、desktop、game、data、extension、infra、embedded）"</action>

<action>显示："✓ 文档需求加载成功。准备开始项目分析。"</action>
</step>

<step n="0.6" goal="检查现有文档并确定工作流模式">
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
    <action>继续到下方扫描级别选择</action>
  </check>

  <check if="用户选择 2">
    <action>设置 workflow_mode = "deep_dive"</action>
    <action>设置 scan_level = "exhaustive"</action>
    <action>用 mode=deep_dive、scan_level=exhaustive 初始化状态文件</action>
    <action>跳转到步骤 13</action>
  </check>

  <check if="用户选择 3">
    <action>显示消息："保持现有文档。退出工作流。"</action>
    <action>退出工作流</action>
  </check>
</check>

<check if="index.md 不存在">
  <action>设置 workflow_mode = "initial_scan"</action>
  <action>继续到下方扫描级别选择</action>
</check>

<action if="workflow_mode != deep_dive">选择扫描级别</action>

<check if="workflow_mode == initial_scan OR workflow_mode == full_rescan">
  <ask>选择您的扫描深度级别：

**1. Quick Scan**（2-5 分钟）[默认]

- 基于模式的分析，不读取源文件
- 扫描：配置文件、包清单、目录结构
- 最适合：快速项目概览、初步理解
- 文件读取：最少（配置、README、package.json 等）

**2. Deep Scan**（10-30 分钟）

- 根据项目类型读取关键目录中的文件
- 扫描：文档需求中的所有关键路径
- 最适合：棕地 PRD 的全面文档
- 文件读取：选择性（关键目录中的关键文件）

**3. Exhaustive Scan**（30-120 分钟）

- 读取项目中的所有源文件
- 扫描：每个源文件（排除 node_modules、dist、build）
- 最适合：完整分析、迁移规划、详细审计
- 文件读取：完整（所有源文件）

您的选择 [1/2/3]（默认：1）：
</ask>

  <action if="用户选择 1 或用户按回车">
    <action>设置 scan_level = "quick"</action>
    <action>显示："使用 Quick Scan（基于模式，不读取源文件）"</action>
  </action>

  <action if="用户选择 2">
    <action>设置 scan_level = "deep"</action>
    <action>显示："使用 Deep Scan（按项目类型读取关键文件）"</action>
  </action>

  <action if="用户选择 3">
    <action>设置 scan_level = "exhaustive"</action>
    <action>显示："使用 Exhaustive Scan（读取所有源文件）"</action>
  </action>

<action>初始化状态文件：{project_knowledge}/project-scan-report.json</action>
<critical>每次操作状态文件时，记录：步骤 ID、人类可读摘要（您实际做了什么）、精确时间戳、任何写入的输出。模糊短语是不可接受的。</critical>
<action>写入初始状态：
{
"workflow_version": "1.2.0",
"timestamps": {"started": "{{current_timestamp}}", "last_updated": "{{current_timestamp}}"},
"mode": "{{workflow_mode}}",
"scan_level": "{{scan_level}}",
"project_root": "{{project_root_path}}",
"project_knowledge": "{{project_knowledge}}",
"completed_steps": [],
"current_step": "step_1",
"findings": {},
"outputs_generated": ["project-scan-report.json"],
"resume_instructions": "从步骤 1 开始"
}
</action>
<action>从步骤 1 继续标准工作流</action>
</check>
</step>

<step n="1" goal="检测项目结构并分类项目类型" if="workflow_mode != deep_dive">
<action>询问用户："要记录的项目根目录是什么？"（默认：当前工作目录）</action>
<action>存储为 {{project_root_path}}</action>

<action>扫描 {{project_root_path}} 寻找关键指标：

- 目录结构（是否存在 client/、server/、api/、src/、app/ 等）
- 关键文件（package.json、go.mod、requirements.txt 等）
- 匹配 project-types.csv 中 detection_keywords 的技术标记
  </action>

<action>检测项目是否为：

- **Monolith**：单一内聚代码库
- **Monorepo**：一个仓库中多个部分
- **Multi-part**：独立的 client/server 或类似架构
  </action>

<check if="检测到多个独立部分（例如 client/ 和 server/ 文件夹）">
  <action>列出检测到的部分及其路径</action>
  <ask>我检测到此项目有多个部分：
  {{detected_parts_list}}

这正确吗？我应该分别记录每个部分吗？[y/n]
</ask>

<action if="用户确认">设置 repository_type = "monorepo" 或 "multi-part"</action>
<action if="用户确认">对于每个检测到的部分：- 识别根路径 - 使用 documentation-requirements.csv 中的 key_file_patterns 运行项目类型检测 - 存储为 project_parts 数组中的部分
</action>

<action if="用户否认或更正">询问用户指定正确的部分及其路径</action>
</check>

<check if="检测到单一内聚项目">
  <action>设置 repository_type = "monolith"</action>
  <action>在 project_parts 数组中创建单一部分，root_path = {{project_root_path}}</action>
  <action>使用 documentation-requirements.csv 中的 key_file_patterns 运行项目类型检测</action>
</check>

<action>对于每个部分，将检测到的技术和文件模式与 documentation-requirements.csv 中的 key_file_patterns 列匹配</action>
<action>为每个部分分配 project_type_id</action>
<action>为每个部分加载对应的 documentation_requirements 行</action>

<ask>我已对此项目分类：
{{project_classification_summary}}

这看起来正确吗？[y/n/edit]
</ask>

<template-output>project_structure</template-output>
<template-output>project_parts_metadata</template-output>

<action>立即用步骤完成更新状态文件：

- 添加到 completed_steps：{"step": "step_1", "status": "completed", "timestamp": "{{now}}", "summary": "分类为 {{repository_type}}，包含 {{parts_count}} 个部分"}
- 更新 current_step = "step_2"
- 用高级摘要更新 findings.project_classification
- **缓存 project_type_id**：添加 project_types 数组：[{"part_id": "{{part_id}}", "project_type_id": "{{project_type_id}}", "display_name": "{{display_name}}"}]
- 此缓存数据防止恢复时重新加载所有 CSV 文件 - 我们可以仅加载所需的 documentation_requirements 行
- 更新 last_updated 时间戳
- 写入状态文件
  </action>

<action>从内存中清除详细扫描结果，仅保留摘要："{{repository_type}}、{{parts_count}} 个部分、{{primary_tech}}"</action>
</step>

<step n="2" goal="发现现有文档并收集用户上下文" if="workflow_mode != deep_dive">
<action>对于每个部分，使用以下模式扫描现有文档：
- README.md、README.rst、README.txt
- CONTRIBUTING.md、CONTRIBUTING.rst
- ARCHITECTURE.md、ARCHITECTURE.txt、docs/architecture/
- DEPLOYMENT.md、DEPLOY.md、docs/deployment/
- API.md、docs/api/
- docs/、documentation/、.github/ 文件夹中的任何文件
</action>

<action>创建 existing_docs 清单，包含：

- 文件路径
- 文件类型（readme、architecture、api 等）
- 属于哪个部分（如果是多部分）
  </action>

<ask>我找到这些现有文档文件：
{{existing_docs_list}}

在分析此项目时，还有其他重要文档或关键区域我应该关注吗？[提供路径或指导，或输入 'none']
</ask>

<action>存储用户指导为 {{user_context}}</action>

<template-output>existing_documentation_inventory</template-output>
<template-output>user_provided_context</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_2", "status": "completed", "timestamp": "{{now}}", "summary": "找到 {{existing_docs_count}} 个现有文档"}
- 更新 current_step = "step_3"
- 更新 last_updated 时间戳
  </action>

<action>从内存中清除详细文档内容，仅保留："找到 {{existing_docs_count}} 个文档"</action>
</step>

<step n="3" goal="分析每个部分的技术栈" if="workflow_mode != deep_dive">
<action>对于 project_parts 中的每个部分：
  - 从 documentation_requirements 加载 key_file_patterns
  - 扫描部分根目录查找这些模式
  - 解析技术清单文件（package.json、go.mod、requirements.txt 等）
  - 提取：框架、语言、版本、数据库、依赖项
  - 构建技术表，列包括：类别、技术、版本、理由
</action>

<action>根据检测到的技术栈确定架构模式：

- 使用 project_type_id 作为主要指示器（例如，"web" → 分层/基于组件，"backend" → 服务/API 中心）
- 考虑框架模式（例如，React → 组件层次结构，Express → 中间件管道）
- 在技术表中记录架构风格
- 存储为每个部分的 {{architecture_pattern}}
  </action>

<template-output>technology_stack</template-output>
<template-output>architecture_patterns</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_3", "status": "completed", "timestamp": "{{now}}", "summary": "技术栈：{{primary_framework}}"}
- 更新 current_step = "step_4"
- 用每个部分的摘要更新 findings.technology_stack
- 更新 last_updated 时间戳
  </action>

<action>从内存中清除详细技术分析，仅保留："{{framework}} 于 {{language}}"</action>
</step>

<step n="4" goal="根据项目类型需求执行条件分析" if="workflow_mode != deep_dive">

<critical>DEEP/EXHAUSTIVE 扫描的批处理策略</critical>

<check if="scan_level == deep OR scan_level == exhaustive">
  <action>此步骤需要文件读取。应用批处理策略：</action>

<action>根据以下识别要处理的子文件夹：- scan_level == "deep"：使用 documentation_requirements 中的 critical_directories - scan_level == "exhaustive"：递归获取所有子文件夹（排除 node_modules、.git、dist、build、coverage）
</action>

<action>对于要扫描的每个子文件夹：1. 读取子文件夹中的所有文件（考虑文件大小 - 对 >5000 LOC 的文件使用判断） 2. 根据下方条件标志提取所需信息 3. 立即将发现写入适当的输出文件 4. 验证写入的文档（部分级别验证） 5. 用批次完成更新状态文件 6. 从上下文中清除详细发现，仅保留 1-2 句话摘要 7. 进入下一个子文件夹
</action>

<action>在状态文件中跟踪批次：
findings.batches_completed: [
{"path": "{{subfolder_path}}", "files_scanned": {{count}}, "summary": "{{brief_summary}}"}
]
</action>
</check>

<check if="scan_level == quick">
  <action>仅使用模式匹配 - 不读取源文件</action>
  <action>使用 glob/grep 识别文件位置和模式</action>
  <action>仅从文件名、目录结构和配置文件提取信息</action>
</check>

<action>对于每个部分，检查 documentation_requirements 布尔标志并执行相应的扫描：</action>

<check if="requires_api_scan == true">
  <action>使用 integration_scan_patterns 扫描 API 路由和端点</action>
  <action>查找：controllers/、routes/、api/、handlers/、endpoints/</action>

  <check if="scan_level == quick">
    <action>使用 glob 查找路由文件，从文件名和文件夹结构提取模式</action>
  </check>

  <check if="scan_level == deep OR scan_level == exhaustive">
    <action>分批读取文件（一次一个子文件夹）</action>
    <action>从实际代码提取：HTTP 方法、路径、请求/响应类型</action>
  </check>

<action>构建 API 契约目录</action>
<action>立即写入：{project_knowledge}/api-contracts-{part_id}.md</action>
<action>验证文档包含所有必需部分</action>
<action>用生成的输出更新状态文件</action>
<action>清除详细 API 数据，仅保留："已记录 {{api_count}} 个端点"</action>
<template-output>api_contracts\*{part_id}</template-output>
</check>

<check if="requires_data_models == true">
  <action>使用 schema_migration_patterns 扫描数据模型</action>
  <action>查找：models/、schemas/、entities/、migrations/、prisma/、ORM 配置</action>

  <check if="scan_level == quick">
    <action>通过 glob 识别模式文件，解析迁移文件名发现表</action>
  </check>

  <check if="scan_level == deep OR scan_level == exhaustive">
    <action>分批读取模型文件（一次一个子文件夹）</action>
    <action>从实际代码提取：表名、字段、关系、约束</action>
  </check>

<action>构建数据库模式文档</action>
<action>立即写入：{project_knowledge}/data-models-{part_id}.md</action>
<action>验证文档完整性</action>
<action>用生成的输出更新状态文件</action>
<action>清除详细模式数据，仅保留："已记录 {{table_count}} 个表"</action>
<template-output>data_models\*{part_id}</template-output>
</check>

<check if="requires_state_management == true">
  <action>分析状态管理模式</action>
  <action>查找：Redux、Context API、MobX、Vuex、Pinia、Provider 模式</action>
  <action>识别：stores、reducers、actions、state 结构</action>
  <template-output>state_management_patterns_{part_id}</template-output>
</check>

<check if="requires_ui_components == true">
  <action>清单记录 UI 组件库</action>
  <action>扫描：components/、ui/、widgets/、views/ 文件夹</action>
  <action>分类：Layout、Form、Display、Navigation 等</action>
  <action>识别：设计系统、组件模式、可复用元素</action>
  <template-output>ui_component_inventory_{part_id}</template-output>
</check>

<check if="requires_hardware_docs == true">
  <action>使用 hardware_interface_patterns 查找硬件原理图</action>
  <ask>这似乎是一个嵌入式/硬件项目。您有：
  - 引脚图
  - 硬件原理图
  - PCB 布局
  - 硬件文档

如有，请提供路径或链接。[提供路径或输入 'none']
</ask>
<action>存储硬件文档引用</action>
<template-output>hardware*documentation*{part_id}</template-output>
</check>

<check if="requires_asset_inventory == true">
  <action>使用 asset_patterns 扫描和清单记录资产</action>
  <action>分类：Images、Audio、3D Models、Sprites、Textures 等</action>
  <action>计算：总大小、文件数、使用的格式</action>
  <template-output>asset_inventory_{part_id}</template-output>
</check>

<action>根据文档需求扫描其他模式：

- config_patterns → 配置管理
- auth_security_patterns → 认证/授权方法
- entry_point_patterns → 应用入口点和引导
- shared_code_patterns → 共享库和工具
- async_event_patterns → 事件驱动架构
- ci_cd_patterns → CI/CD 管道详情
- localization_patterns → i18n/l10n 支持
  </action>

<action>对每个模式扫描应用 scan_level 策略（quick=仅 glob，deep/exhaustive=读取文件）</action>

<template-output>comprehensive*analysis*{part_id}</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_4", "status": "completed", "timestamp": "{{now}}", "summary": "条件分析完成，写入 {{files_generated}} 个文件"}
- 更新 current_step = "step_5"
- 更新 last_updated 时间戳
- 列出所有 outputs_generated
  </action>

<action>从上下文中清除所有详细扫描结果。仅保留摘要：

- "API：{{api_count}} 个端点"
- "数据：{{table_count}} 个表"
- "组件：{{component_count}} 个组件"
  </action>
  </step>

<step n="5" goal="生成带注释的源码树分析" if="workflow_mode != deep_dive">
<action>对于每个部分，使用文档需求中的 critical_directories 生成完整目录树</action>

<action>为树添加注释：

- 每个关键目录的用途
- 标记入口点
- 突出显示关键文件位置
- 注明集成点（多部分项目）
  </action>

<action if="多部分项目">显示各部分如何组织及其接口位置</action>

<action>创建带描述的格式化源码树：

```
project-root/
├── client/          # React 前端（部分：client）
│   ├── src/
│   │   ├── components/  # 可复用 UI 组件
│   │   ├── pages/       # 基于路由的页面
│   │   └── api/         # API 客户端层 → 调用 server/
├── server/          # Express API 后端（部分：api）
│   ├── src/
│   │   ├── routes/      # REST API 端点
│   │   ├── models/      # 数据库模型
│   │   └── services/    # 业务逻辑
```

</action>

<template-output>source_tree_analysis</template-output>
<template-output>critical_folders_summary</template-output>

<action>立即将 source-tree-analysis.md 写入磁盘</action>
<action>验证文档结构</action>
<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_5", "status": "completed", "timestamp": "{{now}}", "summary": "源码树已记录"}
- 更新 current_step = "step_6"
- 添加输出："source-tree-analysis.md"
  </action>
  <action>从上下文中清除详细树，仅保留："源码树包含 {{folder_count}} 个关键文件夹"</action>
  </step>

<step n="6" goal="提取开发和运维信息" if="workflow_mode != deep_dive">
<action>使用 key_file_patterns 和现有文档扫描开发设置：
- 先决条件（Node 版本、Python 版本等）
- 安装步骤（npm install 等）
- 环境设置（.env 文件、配置）
- 构建命令（npm run build、make 等）
- 运行命令（npm start、go run 等）
- 使用 test_file_patterns 的测试命令
</action>

<action>使用 ci_cd_patterns 查找部署配置：

- Dockerfile、docker-compose.yml
- Kubernetes 配置（k8s/、helm/）
- CI/CD 管道（.github/workflows/、.gitlab-ci.yml）
- 部署脚本
- 基础设施即代码（terraform/、pulumi/）
  </action>

<action if="找到 CONTRIBUTING.md 或类似文件">
  <action>提取贡献指南：
    - 代码风格规则
    - PR 流程
    - 提交约定
    - 测试要求
  </action>
</action>

<template-output>development_instructions</template-output>
<template-output>deployment_configuration</template-output>
<template-output>contribution_guidelines</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_6", "status": "completed", "timestamp": "{{now}}", "summary": "开发/部署指南已写入"}
- 更新 current_step = "step_7"
- 添加生成的输出到列表
  </action>
  <action>清除详细说明，仅保留："开发设置和部署已记录"</action>
  </step>

<step n="7" goal="检测多部分集成架构" if="workflow_mode != deep_dive 且项目有多个部分">
<action>分析各部分如何通信：
- 跨部分扫描 integration_scan_patterns
- 识别：REST 调用、GraphQL 查询、gRPC、消息队列、共享数据库
- 记录：部分间的 API 契约、数据流、认证流程
</action>

<action>创建 integration_points 数组，包含：

- from：源部分
- to：目标部分
- type：REST API、GraphQL、gRPC、Event Bus 等
- details：端点、协议、数据格式
  </action>

<action>立即将 integration-architecture.md 写入磁盘</action>
<action>验证文档完整性</action>

<template-output>integration_architecture</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_7", "status": "completed", "timestamp": "{{now}}", "summary": "集成架构已记录"}
- 更新 current_step = "step_8"
  </action>
  <action>清除集成详情，仅保留："{{integration_count}} 个集成点"</action>
  </step>

<step n="8" goal="为每个部分生成架构文档" if="workflow_mode != deep_dive">
<action>对于 project_parts 中的每个部分：
  - 使用步骤 3 匹配的架构模板作为基础结构
  - 用发现的信息填写所有部分：
    * 执行摘要
    * 技术栈（来自步骤 3）
    * 架构模式（来自注册表匹配）
    * 数据架构（来自步骤 4 数据模型扫描）
    * API 设计（来自步骤 4 API 扫描，如适用）
    * 组件概述（来自步骤 4 组件扫描，如适用）
    * 源码树（来自步骤 5）
    * 开发工作流（来自步骤 6）
    * 部署架构（来自步骤 6）
    * 测试策略（来自测试模式）
</action>

<action if="单部分项目">
  - 生成：architecture.md（无部分后缀）
</action>

<action if="多部分项目">
  - 为每个部分生成：architecture-{part_id}.md
</action>

<action>对于每个生成的架构文件：

- 立即将架构文件写入磁盘
- 对照架构模板模式验证
- 用输出更新状态文件
- 从上下文中清除详细架构，仅保留："{{part_id}} 的架构已写入"
  </action>

<template-output>architecture_document</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_8", "status": "completed", "timestamp": "{{now}}", "summary": "已为 {{parts_count}} 个部分写入架构文档"}
- 更新 current_step = "step_9"
  </action>
  </step>

<step n="9" goal="生成支持文档文件" if="workflow_mode != deep_dive">
<action>生成 project-overview.md，包含：
- 项目名称和用途（来自 README 或用户输入）
- 执行摘要
- 技术栈摘要表
- 架构类型分类
- 仓库结构（monolith/monorepo/multi-part）
- 详细文档链接
</action>

<action>生成 source-tree-analysis.md，包含：

- 步骤 5 的完整带注释目录树
- 关键文件夹解释
- 入口点记录
- 多部分结构（如适用）
  </action>

<action>立即将 project-overview.md 写入磁盘</action>
<action>验证文档部分</action>

<action>生成 source-tree-analysis.md（如果步骤 5 中尚未写入）</action>
<action>立即写入磁盘并验证</action>

<action>生成 component-inventory.md（或各部分版本），包含：

- 步骤 4 发现的所有组件
- 按类型分类
- 可复用 vs 特定组件
- 设计系统元素（如找到）
  </action>
  <action>立即将每个组件清单写入磁盘并验证</action>

<action>生成 development-guide.md（或各部分版本），包含：

- 先决条件和依赖项
- 环境设置说明
- 本地开发命令
- 构建过程
- 测试方法和命令
- 常见开发任务
  </action>
  <action>立即将每个开发指南写入磁盘并验证</action>

<action if="找到部署配置">
  <action>生成 deployment-guide.md，包含：
    - 基础设施需求
    - 部署过程
    - 环境配置
    - CI/CD 管道详情
  </action>
  <action>立即写入磁盘并验证</action>
</action>

<action if="找到贡献指南">
  <action>生成 contribution-guide.md，包含：
    - 代码风格和约定
    - PR 流程
    - 测试要求
    - 文档标准
  </action>
  <action>立即写入磁盘并验证</action>
</action>

<action if="记录了 API 契约">
  <action>生成 api-contracts.md（或各部分），包含：
    - 所有 API 端点
    - 请求/响应模式
    - 认证要求
    - 示例请求
  </action>
  <action>立即写入磁盘并验证</action>
</action>

<action if="记录了数据模型">
  <action>生成 data-models.md（或各部分），包含：
    - 数据库模式
    - 表关系
    - 数据模型和实体
    - 迁移策略
  </action>
  <action>立即写入磁盘并验证</action>
</action>

<action if="多部分项目">
  <action>生成 integration-architecture.md，包含：
    - 各部分如何通信
    - 集成点图/描述
    - 部分间数据流
    - 共享依赖
  </action>
  <action>立即写入磁盘并验证</action>

<action>生成 project-parts.json 元数据文件：
`json
    {
      "repository_type": "monorepo",
      "parts": [ ... ],
      "integration_points": [ ... ]
    }
    `
</action>
<action>立即写入磁盘</action>
</action>

<template-output>supporting_documentation</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_9", "status": "completed", "timestamp": "{{now}}", "summary": "所有支持文档已写入"}
- 更新 current_step = "step_10"
- 列出所有新生成的输出
  </action>

<action>从上下文中清除所有文档内容，仅保留生成的文件列表</action>
</step>

<step n="10" goal="生成主索引作为主要 AI 检索源" if="workflow_mode != deep_dive">

<critical>不完整文档标记约定：
当文档应该生成但未生成时（由于 quick 扫描、数据缺失、条件需求未满足）：

- 精确使用此标记：_(待生成)_
- 将其放在 markdown 链接行末尾
- 示例：- [API 契约 - Server](./api-contracts-server.md) _(待生成)_
- 这允许步骤 11 检测并提供完成这些项目
- 始终使用此精确格式以保持一致性和自动检测
  </critical>

<action>根据项目结构创建带智能导航的 index.md</action>

<action if="单部分项目">
  <action>生成简单索引，包含：
    - 项目名称和类型
    - 快速参考（技术栈、架构类型）
    - 所有生成文档的链接
    - 发现的现有文档链接
    - 入门部分
  </action>
</action>

<action if="多部分项目">
  <action>生成综合索引，包含：
    - 项目概述和结构摘要
    - 基于部分的导航部分
    - 各部分的快速参考
    - 跨部分集成链接
    - 所有生成和现有文档的链接
    - 各部分的入门指南
  </action>
</action>

<action>在 index.md 中包含：

## 项目文档索引

### 项目概述

- **类型：** {{repository_type}}{{#if multi-part}}，包含 {{parts.length}} 个部分{{/if}}
- **主要语言：** {{primary_language}}
- **架构：** {{architecture_type}}

### 快速参考

{{#if single_part}}

- **技术栈：** {{tech_stack_summary}}
- **入口点：** {{entry_point}}
- **架构模式：** {{architecture_pattern}}
  {{else}}
  {{#each parts}}

#### {{part_name}}（{{part_id}}）

- **类型：** {{project_type}}
- **技术栈：** {{tech_stack}}
- **根目录：** {{root_path}}
  {{/each}}
  {{/if}}

### 生成的文档

- [项目概述](./project-overview.md)
- [架构](./architecture{{#if multi-part}}-{part\*id}{{/if}}.md){{#unless architecture_file_exists}} (待生成) {{/unless}}
- [源码树分析](./source-tree-analysis.md)
- [组件清单](./component-inventory{{#if multi-part}}-{part\*id}{{/if}}.md){{#unless component_inventory_exists}} (待生成) {{/unless}}
- [开发指南](./development-guide{{#if multi-part}}-{part\*id}{{/if}}.md){{#unless dev_guide_exists}} (待生成) {{/unless}}
  {{#if deployment_found}}- [部署指南](./deployment-guide.md){{#unless deployment_guide_exists}} (待生成) {{/unless}}{{/if}}
  {{#if contribution_found}}- [贡献指南](./contribution-guide.md){{/if}}
  {{#if api_documented}}- [API 契约](./api-contracts{{#if multi-part}}-{part_id}{{/if}}.md){{#unless api_contracts_exists}} (待生成) {{/unless}}{{/if}}
  {{#if data_models_documented}}- [数据模型](./data-models{{#if multi-part}}-{part_id}{{/if}}.md){{#unless data_models_exists}} (待生成) {{/unless}}{{/if}}
  {{#if multi-part}}- [集成架构](./integration-architecture.md){{#unless integration_arch_exists}} (待生成) {{/unless}}{{/if}}

### 现有文档

{{#each existing_docs}}

- [{{title}}]({{relative_path}}) - {{description}}
  {{/each}}

### 入门

{{getting_started_instructions}}
</action>

<action>写入 index.md 前，检查哪些预期文件实际存在：

- 对于每个应该生成的文档，检查磁盘上文件是否存在
- 设置存在标志：architecture_file_exists、component_inventory_exists、dev_guide_exists 等
- 这些标志决定是否添加 _(待生成)_ 标记
- 在 {{missing_docs_list}} 中跟踪哪些文件缺失以供报告
  </action>

<action>立即将 index.md 写入磁盘，对缺失文件使用适当的 _(待生成)_ 标记</action>
<action>验证索引包含所有必需部分且链接有效</action>

<template-output>index</template-output>

<action>更新状态文件：

- 添加到 completed_steps：{"step": "step_10", "status": "completed", "timestamp": "{{now}}", "summary": "主索引已生成"}
- 更新 current_step = "step_11"
- 添加输出："index.md"
  </action>

<action>从上下文中清除索引内容</action>
</step>

<step n="11" goal="验证和审查生成的文档" if="workflow_mode != deep_dive">
<action>显示所有生成文件的摘要：
已生成于 {{project_knowledge}}/：
{{file_list_with_sizes}}
</action>

<action>从 {validation} 运行验证清单</action>

<critical>不完整文档检测：

1. 主要扫描：查找精确标记：_(待生成)_
2. 备用扫描：查找模糊模式（以防 agent 懒惰）：
   - _(TBD)_
   - _(TODO)_
   - _(即将推出)_
   - _(尚未生成)_
   - _(待定)_
3. 从每个匹配提取文档元数据供用户选择
   </critical>

<action>读取 {project_knowledge}/index.md</action>

<action>扫描不完整文档标记：
步骤 1：搜索精确模式 "_(待生成)_"（区分大小写）
步骤 2：对于每个匹配，提取整行
步骤 3：解析行以提取：

- 文档标题（[方括号] 或 **粗体** 内的文本）
- 文件路径（从 markdown 链接或从标题推断）
- 文档类型（从文件名推断：architecture、api-contracts、data-models、component-inventory、development-guide、deployment-guide、integration-architecture）
- 部分 ID（如适用，从文件名提取如 "architecture-server.md" → part_id: "server"）
  步骤 4：添加到 {{incomplete_docs_strict}} 数组
  </action>

<action>备用模糊扫描其他标记：
搜索模式：_(TBD)_、_(TODO)_、_(即将推出)_、_(尚未生成)_、_(待定)_
对于每个模糊匹配：

- 提取与精确扫描相同的元数据
- 添加到 {{incomplete_docs_fuzzy}} 数组并带 fuzzy_match 标志
  </action>

<action>合并结果：
设置 {{incomplete_docs_list}} = {{incomplete_docs_strict}} + {{incomplete_docs_fuzzy}}
对于每个项目存储结构：
{
"title": "架构 – Server",
"file\*path": "./architecture-server.md",
"doc_type": "architecture",
"part_id": "server",
"line_text": "- [架构 – Server](./architecture-server.md) (待生成)",
"fuzzy_match": false
}
</action>

<ask>文档生成完成！

摘要：

- 项目类型：{{project_type_summary}}
- 已记录部分：{{parts_count}}
- 生成文件：{{files_count}}
- 总行数：{{total_lines}}

{{#if incomplete_docs_list.length > 0}}
⚠️ **检测到不完整文档：**

我发现 {{incomplete_docs_list.length}} 项标记为不完整：

{{#each incomplete_docs_list}}
{{@index + 1}}. **{{title}}**（{{doc_type}}{{#if part_id}} - {{part_id}}{{/if}}）{{#if fuzzy_match}} ⚠️ [非标准标记]{{/if}}
{{/each}}

{{/if}}

您想：

{{#if incomplete_docs_list.length > 0}}

1. **生成不完整文档** - 完成上述 {{incomplete_docs_list.length}} 项中的任意项
2. 审查特定部分 [输入部分名称]
3. 为任意区域添加更多详情 [输入区域名称]
4. 生成其他自定义文档 [描述内容]
5. 完成并结束 [输入 'done']
   {{else}}
6. 审查特定部分 [输入部分名称]
7. 为任意区域添加更多详情 [输入区域名称]
8. 生成其他文档 [描述内容]
9. 完成并结束 [输入 'done']
   {{/if}}

您的选择：
</ask>

<check if="用户选择选项 1（生成不完整项）">
  <ask>您想生成哪些不完整项？

{{#each incomplete_docs_list}}
{{@index + 1}}. {{title}}（{{doc_type}}{{#if part_id}} - {{part_id}}{{/if}}）
{{/each}}
{{incomplete_docs_list.length + 1}}. 全部

输入用逗号分隔的数字（例如 "1,3,5"），或输入 'all'：
</ask>

<action>解析用户选择：

- 如果 "all"，设置 {{selected_items}} = {{incomplete_docs_list}} 中的所有项目
- 如果逗号分隔的数字，按索引提取选中的项目
- 存储结果到 {{selected_items}} 数组
  </action>

  <action>显示："正在生成 {{selected_items.length}} 个文档..."</action>

  <action>对于 {{selected_items}} 中的每个项目：

1. **识别部分和需求：**
   - 从项目提取 part_id（如果存在）
   - 在状态文件的 project_parts 数组中查找部分数据
   - 加载该部分 project_type_id 的 documentation_requirements

2. **根据 doc_type 路由到适当的生成子步骤：**

   **如果 doc_type == "architecture"：**
   - 显示："正在为 {{part_id}} 生成架构文档..."
   - 从状态文件（步骤 3 缓存）加载此部分的 architecture_match
   - 仅对此特定部分重新运行步骤 8 架构生成逻辑
   - 使用匹配的模板并用状态文件缓存的数据填写
   - 将 architecture-{{part_id}}.md 写入磁盘
   - 验证完整性

   **如果 doc_type == "api-contracts"：**
   - 显示："正在为 {{part_id}} 生成 API 契约..."
   - 加载部分数据和 documentation_requirements
   - 仅针对此部分重新运行步骤 4 API 扫描子步骤
   - 使用状态文件中的 scan_level（quick/deep/exhaustive）
   - 生成 api-contracts-{{part_id}}.md
   - 验证文档结构

   **如果 doc_type == "data-models"：**
   - 显示："正在为 {{part_id}} 生成数据模型文档..."
   - 仅针对此部分重新运行步骤 4 数据模型扫描子步骤
   - 使用 documentation_requirements 中的 schema_migration_patterns
   - 生成 data-models-{{part_id}}.md
   - 验证完整性

   **如果 doc_type == "component-inventory"：**
   - 显示："正在为 {{part_id}} 生成组件清单..."
   - 对此特定部分重新运行步骤 9 组件清单生成
   - 扫描 components/、ui/、widgets/ 文件夹
   - 生成 component-inventory-{{part_id}}.md
   - 验证结构

   **如果 doc_type == "development-guide"：**
   - 显示："正在为 {{part_id}} 生成开发指南..."
   - 对此特定部分重新运行步骤 9 开发指南生成
   - 使用 documentation_requirements 中的 key_file_patterns 和 test_file_patterns
   - 生成 development-guide-{{part_id}}.md
   - 验证完整性

   **如果 doc_type == "deployment-guide"：**
   - 显示："正在生成部署指南..."
   - 重新运行步骤 6 部署配置扫描
   - 重新运行步骤 9 部署指南生成
   - 生成 deployment-guide.md
   - 验证结构

   **如果 doc_type == "integration-architecture"：**
   - 显示："正在生成集成架构..."
   - 对所有部分重新运行步骤 7 集成分析
   - 生成 integration-architecture.md
   - 验证完整性

3. **生成后操作：**
   - 确认文件已成功写入
   - 用新生成的输出更新状态文件
   - 添加到 {{newly_generated_docs}} 跟踪列表
   - 显示："✓ 已生成：{{file_path}}"

4. **处理错误：**
   - 如果生成失败，记录错误并继续下一项
   - 在 {{failed_generations}} 列表中跟踪失败项
     </action>

<action>所有选中项处理完成后：

**更新 index.md 移除标记：**

1. 读取当前 index.md 内容
2. 对于 {{newly_generated_docs}} 中的每个项目：
   - 找到包含文件链接和标记的行
   - 移除 _(待生成)_ 或模糊标记文本
   - 保留 markdown 链接完整
3. 将更新后的 index.md 写回磁盘
4. 更新状态文件记录 index.md 修改
   </action>

<action>显示生成摘要：

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✓ **文档生成完成！**

**成功生成：**
{{#each newly_generated_docs}}

- {{title}} → {{file_path}}
  {{/each}}

{{#if failed_generations.length > 0}}
**生成失败：**
{{#each failed_generations}}

- {{title}}（{{error_message}}）
  {{/each}}
  {{/if}}

**已更新：** index.md（已移除不完整标记）

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
</action>

<action>用所有生成活动更新状态文件</action>

<action>返回步骤 11 菜单（循环检查是否有任何剩余不完整项）</action>
</check>

<action if="用户请求其他更改（选项 2-3）">进行请求的修改并重新生成受影响的文件</action>
<action if="用户选择完成（选项 4 或 5）">进入步骤 12 完成</action>

<check if="未完成">
  <action>更新状态文件：
- 添加到 completed_steps：{"step": "step_11_iteration", "status": "completed", "timestamp": "{{now}}", "summary": "审查迭代完成"}
- 保持 current_step = "step_11"（用于循环回）
- 更新 last_updated 时间戳
  </action>
  <action>循环回到步骤 11 开头（重新扫描剩余不完整文档）</action>
</check>

<check if="完成">
  <action>更新状态文件：
- 添加到 completed_steps：{"step": "step_11", "status": "completed", "timestamp": "{{now}}", "summary": "验证和审查完成"}
- 更新 current_step = "step_12"
  </action>
  <action>进入步骤 12</action>
</check>
</step>

<step n="12" goal="完成并提供后续步骤" if="workflow_mode != deep_dive">
<action>创建最终摘要报告</action>
<action>编译验证回顾变量：
  - 设置 {{verification_summary}} 为您执行的具体测试、验证或脚本（或"未运行"）。
  - 设置 {{open_risks}} 为任何剩余风险或待办跟进（或"无"）。
  - 设置 {{next_checks}} 为合并/部署前建议的操作（或"无"）。
</action>

<action>显示完成消息：

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

## 项目文档完成！✓

**位置：** {{project_knowledge}}/

**主索引：** {{project_knowledge}}/index.md
👆 这是您 AI 辅助开发的主要入口点

**生成的文档：**
{{generated_files_list}}

**后续步骤：**

1. 审查 index.md 以熟悉文档结构
2. 创建棕地 PRD 时，将 PRD 工作流指向：{{project_knowledge}}/index.md
3. 对于仅 UI 功能：参考 {{project_knowledge}}/architecture-{{ui_part_id}}.md
4. 对于仅 API 功能：参考 {{project_knowledge}}/architecture-{{api_part_id}}.md
5. 对于全栈功能：参考两部分架构 + integration-architecture.md

**验证回顾：**

- 执行的测试/提取：{{verification_summary}}
- 未解决的风险或跟进：{{open_risks}}
- PR 前建议检查：{{next_checks}}

**棕地 PRD 命令：**
准备规划新功能时，运行 PRD 工作流并提供此索引作为输入。

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
</action>

<action>完成状态文件：

- 添加到 completed_steps：{"step": "step_12", "status": "completed", "timestamp": "{{now}}", "summary": "工作流完成"}
- 更新 timestamps.completed = "{{now}}"
- 更新 current_step = "completed"
- 写入最终状态文件
  </action>

<action>显示："状态文件已保存：{{project_knowledge}}/project-scan-report.json"</action>

</workflow>