# 步骤 1：上下文发现与初始化

## 强制执行规则（首先阅读）：

- 🛑 绝不在没有用户输入的情况下生成内容
- ✅ 始终将其视为技术同伴之间的协作发现
- 📋 你是引导者，而非内容生成器
- 💬 专注于发现现有项目上下文和技术栈
- 🎯 识别 AI agent 需要的关键实现规则
- ⚠️ 绝对不要时间估计 - AI 开发速度已根本改变
- ✅ 你必须始终使用配置中的 `{communication_language}` 以你的 Agent 交流风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示你的分析
- 📖 阅读现有项目文件以了解当前上下文
- 💾 初始化文档并更新 frontmatter
- 🚫 禁止在发现完成前加载下一步

## 上下文边界：

- workflow.md 中的变量已在内存中可用
- 专注于现有项目文件和架构决策
- 寻找模式、约定和独特要求
- 优先处理防止实现错误的规则

## 你的任务：

发现项目的技术栈、现有模式和 AI agent 在编写代码时必须遵循的关键实现规则。

## 发现序列：

### 1. 检查现有项目上下文

首先，检查项目上下文是否已存在：

- 在 `{project_knowledge}/project-context.md 或 {project-root}/**/project-context.md` 查找文件
- 如果存在：读取完整文件以了解现有规则
- 向用户呈现："发现现有项目上下文，包含 {number_of_sections} 个部分。您想更新这个还是创建新的？"

### 2. 发现项目技术栈

加载并分析项目文件以识别技术：

**架构文档：**

- 查找 `{planning_artifacts}/architecture.md`
- 提取带有特定版本的技术选择
- 记录影响实现的架构决策

**包文件：**

- 检查 `package.json`、`requirements.txt`、`Cargo.toml` 等
- 提取所有依赖的确切版本
- 注意开发依赖 vs 生产依赖

**配置文件：**

- 查找项目语言特定配置（例如：`tsconfig.json`）
- 构建工具配置（webpack、vite、next.config.js 等）
- Linting 和格式化配置（.eslintrc、.prettierrc 等）
- 测试配置（jest.config.js、vitest.config.ts 等）

### 3. 识别现有代码模式

搜索现有代码库以发现模式：

**命名约定：**

- 文件命名模式（PascalCase、kebab-case 等）
- 组件/函数命名约定
- 变量命名模式
- 测试文件命名模式

**代码组织：**

- 组件如何构建
- 工具和辅助函数放在哪里
- 服务如何组织
- 测试组织模式

**文档模式：**

- 注释风格和约定
- 文档要求
- README 和 API 文档模式

### 4. 提取关键实现规则

寻找 AI agent 可能遗漏的规则：

**语言特定规则：**

- TypeScript 严格模式要求
- 导入/导出约定
- Async/await vs Promise 使用模式
- 语言特定的错误处理模式

**框架特定规则：**

- React hooks 使用模式
- API 路由约定
- 中间件使用模式
- 状态管理模式

**测试规则：**

- 测试结构要求
- Mock 使用约定
- 集成测试 vs 单元测试边界
- 覆盖率要求

**开发工作流规则：**

- 分支命名约定
- 提交消息模式
- PR 审查要求
- 部署流程

### 5. 初始化项目上下文文档

根据发现，创建或更新上下文文档：

#### A. 新文档设置（如果没有现有上下文）

从 `{installed_path}/project-context-template.md` 复制模板到 `{output_folder}/project-context.md`
初始化 frontmatter 字段。

#### B. 现有文档更新

加载现有上下文并准备更新
设置 frontmatter `sections_completed` 以跟踪将要更新的内容

### 6. 呈现发现摘要

向用户报告发现：

"欢迎 {{user_name}}！我已经分析了您的项目 {{project_name}}，发现了 AI agent 需要的上下文。

**发现的技术栈：**
{{带版本的技术列表}}

**发现的现有模式：**

- {{number_of_patterns}} 个实现模式
- {{number_of_conventions}} 个编码约定
- {{number_of_rules}} 条关键规则

**上下文规则的关键领域：**

- {{area_1}}（例如，TypeScript 配置）
- {{area_2}}（例如，测试模式）
- {{area_3}}（例如，代码组织）

{if_existing_context}
**现有上下文：** 发现已定义 {{sections}} 个部分。我们可以更新或添加这些内容。
{/if_existing_context}

准备创建/更新您的项目上下文。这将帮助 AI agent 按照您项目的标准一致地实现代码。

[C] 继续生成上下文"

## 成功指标：

✅ 现有项目上下文正确检测和处理
✅ 技术栈及其版本准确识别
✅ 关键实现模式已发现
✅ 项目上下文文档正确文档正确初始化
✅ 发现结果清晰呈现给用户
✅ 用户准备好继续上下文生成

## 失败模式：

❌ 在创建新上下文前未检查现有项目上下文
❌ 遗漏关键技术版本或配置
❌ 忽视重要编码模式或约定
❌ 未正确初始化 frontmatter
❌ 未向用户呈现清晰的发现摘要

## 下一步：

用户选择 [C] 继续后，加载 `{project-root}/_bmad/bmm/workflows/generate-project-context/steps/step-02-generate.md` 协作生成具体的项目上下文规则。

记住：在用户从菜单明确选择 [C] 且发现已确认且初始文件已按此发现步骤指示写入之前，不要进入步骤 2！