# 步骤 6：项目结构与边界

## 强制执行规则（首先阅读）：

- 🛑 绝不在没有用户输入的情况下生成内容

- 📖 关键：始终在采取任何行动之前阅读完整的步骤文件 - 部分理解会导致不完整的决策
- 🔄 关键：当使用 'C' 加载下一步时，确保完整阅读并理解文件后再继续
- ✅ 始终将此视为架构同伴之间的协作探索
- 📋 你是协调者，而非内容生成者
- 💬 专注于定义完整的项目结构和清晰的边界
- 🗺️ 将需求/epic 映射到架构组件
- ⚠️ 绝对禁止时间估算 - AI 开发速度已经发生了根本性变化
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示你的分析
- 🗺️ 创建完整的项目树，而非通用占位符
- ⚠️ 生成项目结构后呈现 A/P/C 菜单
- 💾 仅当用户选择 C（继续）时保存
- 📖 在加载下一步之前更新 frontmatter `stepsCompleted: [1, 2, 3, 4, 5, 6]`
- 🚫 禁止在选择 C 之前加载下一步

## 协作菜单（A/P/C）：

此步骤将生成内容并呈现选择：

- **A（高级引导）**：使用发现协议探索创新的项目组织方法
- **P（多角色模式）**：引入多个视角评估项目结构权衡
- **C（继续）**：保存项目结构并进入验证

## 协议集成：

- 当选择 'A'：完整阅读并遵循：{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml
- 当选择 'P'：完整阅读并遵循：{project-root}/_bmad/core/workflows/party-mode/workflow.md
- 协议在 A 或 P 完成后总是返回显示此步骤的 A/P/C 菜单
- 用户在继续前接受/拒绝协议更改

## 上下文边界：

- 所有之前的架构决策已完成
- 实施模式和一致性规则已定义
- 专注于物理项目结构和组件边界
- 将需求映射到具体文件和目录

## 你的任务：

根据所有已做出的决策定义完整的项目结构和架构边界，为 AI Agent 创建具体的实施指南。

## 项目结构序列：

### 1. 分析需求映射

将项目需求映射到架构组件：

**从 Epic（如可用）：**
"Epic：{{epic_name}} → 位于 {{module/directory/service}}"

- Epic 内的用户故事
- 跨 epic 依赖
- 所需的共享组件

**从 FR 类别（如无 epic）：**
"FR 类别：{{fr_category_name}} → 位于 {{module/directory/service}}"

- 相关的功能需求
- 跨类别的共享功能
- 类别间的集成点

### 2. 定义项目目录结构

根据技术栈和模式，创建完整的项目结构：

**根配置文件：**

- 包管理文件（package.json、requirements.txt 等）
- 构建和开发配置
- 环境配置文件
- CI/CD 管道文件
- 文档文件

**源代码组织：**

- 应用程序入口点
- 核心应用程序结构
- 功能/模块组织
- 共享工具和库
- 配置和环境文件

**测试组织：**

- 单元测试位置和结构
- 集成测试组织
- 端到端测试结构
- 测试工具和 fixtures

**构建和分发：**

- 构建输出目录
- 分发文件
- 静态资源
- 文档构建

### 3. 定义集成边界

映射组件如何通信以及边界在哪里：

**API 边界：**

- 外部 API 端点
- 内部服务边界
- 认证和授权边界
- 数据访问层边界

**组件边界：**

- 前端组件通信模式
- 状态管理边界
- 服务通信模式
- 事件驱动集成点

**数据边界：**

- 数据库架构边界
- 数据访问模式
- 缓存边界
- 外部数据集成点

### 4. 创建完整的项目树

生成包含所有文件和目录的综合目录结构：

**技术特定的结构示例：**

**Next.js 全栈：**

```
project-name/
├── README.md
├── package.json
├── next.config.js
├── tailwind.config.js
├── tsconfig.json
├── .env.local
├── .env.example
├── .gitignore
├── .github/
│   └── workflows/
│       └── ci.yml
├── src/
│   ├── app/
│   │   ├── globals.css
│   │   ├── layout.tsx
│   │   └── page.tsx
│   ├── components/
│   │   ├── ui/
│   │   ├── forms/
│   │   └── features/
│   ├── lib/
│   │   ├── db.ts
│   │   ├── auth.ts
│   │   └── utils.ts
│   ├── types/
│   └── middleware.ts
├── prisma/
│   ├── schema.prisma
│   └── migrations/
├── tests/
│   ├── __mocks__/
│   ├── components/
│   └── e2e/
└── public/
    └── assets/
```

**API 后端（NestJS）：**

```
project-name/
├── package.json
├── nest-cli.json
├── tsconfig.json
├── .env
├── .env.example
├── .gitignore
├── README.md
├── src/
│   ├── main.ts
│   ├── app.module.ts
│   ├── config/
│   ├── modules/
│   │   ├── auth/
│   │   ├── users/
│   │   └── common/
│   ├── services/
│   ├── repositories/
│   ├── decorators/
│   ├── pipes/
│   ├── guards/
│   └── interceptors/
├── test/
│   ├── unit/
│   ├── integration/
│   └── e2e/
├── prisma/
│   ├── schema.prisma
│   └── migrations/
└── docker-compose.yml
```

### 5. 将需求映射到结构

创建从项目需求到具体文件/目录的显式映射：

**Epic/功能映射：**
"Epic：用户管理

- 组件：src/components/features/users/
- 服务：src/services/users/
- API 路由：src/app/api/users/
- 数据库：prisma/migrations/_*users*_
- 测试：tests/features/users/"

**横切关注点：**
"认证系统

- 组件：src/components/auth/
- 服务：src/services/auth/
- 中间件：src/middleware/auth.ts
- 守卫：src/guards/auth.guard.ts
- 测试：tests/auth/"

### 6. 生成结构内容

准备追加到文档的内容：

#### 内容结构：

```markdown
## 项目结构与边界

### 完整项目目录结构
```

{{带所有文件和目录的完整项目树}

```

### 架构边界

**API 边界：**
{{API 边界定义和端点}}

**组件边界：**
{{组件通信模式和边界}}

**服务边界：**
{{服务集成模式和边界}}

**数据边界：**
{{数据访问模式和边界}}

### 需求到结构映射

**功能/Epic 映射：**
{{epic 或功能到具体目录的映射}}

**横切关注点：**
{{共享功能到位置的映射}}

### 集成点

**内部通信：**
{{项目内组件如何通信}}

**外部集成：**
{{第三方服务集成点}}

**数据流：**
{{数据如何流经架构}}

### 文件组织模式

**配置文件：**
{{配置文件的组织位置和方式}}

**源代码组织：**
{{源代码的结构和组织方式}}

**测试组织：**
{{测试的结构和组织方式}}

**资源组织：**
{{静态和动态资源的组织方式}}

### 开发工作流集成

**开发服务器结构：**
{{项目如何为开发组织}}

**构建流程结构：**
{{构建流程如何使用项目结构}}

**部署结构：**
{{项目结构如何支持部署}}
```

### 7. 呈现内容和菜单

显示生成的项目结构内容并呈现选择：

"我已经根据所有架构决策创建了完整的项目结构。

**这是我将添加到文档的内容：**

[显示第6步的完整 markdown 内容]

**您想做什么？**
[A] 高级引导 - 探索创新的项目组织方法
[P] 多角色模式 - 从不同开发视角审查结构
[C] 继续 - 保存此结构并进入架构验证"

### 8. 处理菜单选择

#### 如果 'A'（高级引导）：

- 完整阅读并遵循：{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml，使用当前项目结构
- 处理返回的增强组织洞察
- 询问用户："接受这些对项目结构的更改吗？(y/n)"
- 如果是：更新内容，然后返回 A/P/C 菜单
- 如果否：保留原始内容，然后返回 A/P/C 菜单

#### 如果 'P'（多角色模式）：

- 完整阅读并遵循：{project-root}/_bmad/core/workflows/party-mode/workflow.md，使用项目结构上下文
- 处理关于组织权衡的协作洞察
- 询问用户："接受这些对项目结构的更改吗？(y/n)"
- 如果是：更新内容，然后返回 A/P/C 菜单
- 如果否：保留原始内容，然后返回 A/P/C 菜单

#### 如果 'C'（继续）：

- 将最终内容追加到 `{planning_artifacts}/architecture.md`
- 更新 frontmatter：`stepsCompleted: [1, 2, 3, 4, 5, 6]`
- 加载 `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-07-validation.md`

## 追加到文档：

当用户选择 'C' 时，使用第6步的结构将内容直接追加到文档。

## 成功指标：

✅ 定义完整的项目树，包含所有文件和目录
✅ 清晰记录所有架构边界
✅ 将需求/epic 映射到具体位置
✅ 定义集成点和通信模式
✅ 项目结构与选择的技术栈对齐
✅ 正确呈现和处理 A/P/C 菜单
✅ 选择 C 时正确追加内容到文档

## 失败模式：

❌ 创建通用占位符结构而非具体、完整的树
❌ 未将需求映射到具体文件和目录
❌ 遗漏重要的集成边界
❌ 结构设计时未考虑选择的技术栈
❌ 未定义组件如何跨边界通信
❌ 内容生成后未呈现 A/P/C 菜单

❌ **关键**：仅读取部分步骤文件 - 导致理解不完整和决策不当
❌ **关键**：未完整阅读并理解下一步文件就使用 'C' 继续
❌ **关键**：未完整理解步骤要求和协议就做出决策

## 下一步：

用户选择 'C' 且内容保存到文档后，加载 `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-07-validation.md` 验证架构一致性和完整性。

记住：在用户明确从 A/P/C 菜单选择 'C' 且内容保存之前，不要进入 step-07！