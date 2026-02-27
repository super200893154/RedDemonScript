# 步骤 3：起始模板评估

## 强制执行规则（首先阅读）：

- 🛑 绝不在没有用户输入的情况下生成内容
- ✅ 始终将此视为架构同伴之间的协作探索
- 📋 你是协调者，而非内容生成者
- 💬 专注于评估当前版本的起始模板选项
- 🌐 始终搜索网络验证当前版本 - 绝不信任硬编码版本
- ⚠️ 绝对禁止时间估算 - AI 开发速度已经发生了根本性变化
- 📖 关键：始终在采取任何行动之前阅读完整的步骤文件 - 部分理解会导致不完整的架构
- 🔄 关键：当使用 'C' 加载下一步时，确保完整阅读并理解文件后再继续
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

## 执行协议：

- 🎯 在采取任何行动之前展示你的分析
- 🌐 搜索网络验证当前版本和选项
- ⚠️ 生成起始模板分析后呈现 A/P/C 菜单
- 💾 仅当用户选择 C（继续）时保存
- 📖 在加载下一步之前更新 frontmatter `stepsCompleted: [1, 2, 3]`
- 🚫 禁止在选择 C 之前加载下一步

## 协作菜单（A/P/C）：

此步骤将生成内容并呈现选择：

- **A（高级引导）**：使用发现协议探索非传统起始选项或自定义方法
- **P（多角色模式）**：引入多个视角评估不同用例的起始权衡
- **C（继续）**：将内容保存到文档并进入下一步

## 协议集成：

- 当选择 'A'：完整阅读并遵循：{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml
- 当选择 'P'：完整阅读并遵循：{project-root}/_bmad/core/workflows/party-mode/workflow.md
- 协议在 A 或 P 完成后总是返回显示此步骤的 A/P/C 菜单
- 用户在继续前接受/拒绝协议更改

## 上下文边界：

- 步骤2的项目上下文可用且完整
- step-01 的项目上下文文件可能包含技术偏好
- 尚未做出架构决策 - 正在评估基础
- 专注于技术偏好发现和起始评估
- 评估选项时考虑项目需求和现有偏好

## 你的任务：

发现技术偏好并评估起始模板选项，利用现有技术偏好并建立坚实的架构基础。

## 起始评估序列：

### 0. 检查技术偏好和上下文

**检查项目上下文中的现有技术偏好：**
"在我们深入起始模板之前，让我检查您是否已有记录的技术偏好。

{{if_project_context_exists}}
我在您的项目上下文文件中发现了一些技术规则：
{{从项目上下文提取的技术偏好}

**发现的项目上下文技术规则：**

- 语言/框架：{{从上下文获取的语言框架}}
- 工具和库：{{从上下文获取的工具}}
- 开发模式：{{从上下文获取的模式}}
- 平台偏好：{{从上下文获取的平台}}

{{else}}
项目上下文文件中未发现现有技术偏好。我们将现在建立您的技术偏好。
{{/if_project_context}}"

**发现用户技术偏好：**
"根据您的项目上下文，让我们讨论您的技术偏好：

{{主要技术类别}} 偏好：

- **语言**：您对 TypeScript/JavaScript、Python、Go、Rust 等有偏好吗？
- **框架**：是否有现有熟悉或偏好（React、Vue、Angular、Next.js 等）？
- **数据库**：是否有偏好或现有基础设施（PostgreSQL、MongoDB、MySQL 等）？

**开发经验：**

- 您团队对不同技术的经验水平如何？
- 有没有您想学习的技术 vs 您熟悉的技术？

**平台/部署偏好：**

- 云服务商偏好（AWS、Vercel、Railway 等）？
- 容器偏好（Docker、Serverless、传统）？

**集成：**

- 需要与任何现有系统或 API 集成吗？
- 计划使用的第三方服务（支付、认证、分析等）？

这些偏好将帮助我推荐最适合的起始模板并指导我们的架构决策。"

### 1. 识别主要技术领域

根据项目上下文分析和技术偏好，识别主要技术栈：

- **Web 应用程序** → 寻找 Next.js、Vite、Remix、SvelteKit 起始模板
- **移动应用** → 寻找 React Native、Expo、Flutter 起始模板
- **API/后端** → 寻找 NestJS、Express、Fastify、Supabase 起始模板
- **CLI 工具** → 寻找 CLI 框架起始模板（oclif、commander 等）
- **全栈** → 寻找 T3、RedwoodJS、Blitz、Next.js 起始模板
- **桌面应用** → 寻找 Electron、Tauri 起始模板

### 2. UX 需求考虑

如果加载了 UX 规格，在选择起始时考虑 UX 需求：

- **丰富动画** → Framer Motion 兼容起始模板
- **复杂表单** → 包含 React Hook Form 的起始模板
- **实时功能** → Socket.io 或 WebSocket 就绪起始模板
- **设计系统** → 启用 Storybook 的起始模板
- **离线能力** → Service worker 或 PWA 配置的起始模板

### 3. 研究当前起始选项

搜索网络查找当前、维护中的起始模板：

```
搜索网络："{{primary_technology}} starter template CLI create command latest"
搜索网络："{{primary_technology}} boilerplate generator latest options"
搜索网络："{{primary_technology}} production-ready starter best practices"
```

### 4. 调查热门起始选项

对于每个有前景的起始模板，调查详情：

```
搜索网络："{{starter_name}} default setup technologies included latest"
搜索网络："{{starter_name}} project structure file organization"
搜索网络："{{starter_name}} production deployment capabilities"
搜索网络："{{starter_name}} recent updates maintenance status"
```

### 5. 分析每个起始模板提供的内容

对于每个可行的起始选项，记录：

**已做出的技术决策：**

- 语言/TypeScript 配置
- 样式解决方案（CSS、Tailwind、Styled Components 等）
- 测试框架设置
- Linting/格式化配置
- 构建工具和优化
- 项目结构和组织

**已建立的架构模式：**

- 代码组织模式
- 组件结构约定
- API 分层方法
- 状态管理设置
- 路由模式
- 环境配置

**开发体验功能：**

- 热重载和开发服务器
- TypeScript 配置
- 调试设置
- 测试基础设施
- 文档生成

### 6. 呈现起始选项

根据用户技能水平和项目需求：

**专家用户：**
"发现 {{starter_name}}，它提供：
{{关键决策快速列表}

这将用这些已做出的技术决策建立我们的基础架构。使用它？"

**中级用户：**
"我发现了 {{starter_name}}，这是 {{project_type}} 项目的一个维护良好的起始模板。

它为我们做出以下架构决策：
{{带解释的决策列表}

这为我们提供了遵循当前最佳实践的坚实基础。我们应该使用它吗？"

**初级用户：**
"我发现了 {{starter_name}}，这就像您项目的预构建基础。

把它想象成购买预制房屋框架，而不是自己切割每块木板。

它为我们做出以下决策：
{{友好的决策解释}

这是一个遵循最佳实践的绝佳起点，帮助我们省去几十个小技术选择。我们应该使用它吗？"

### 7. 获取当前 CLI 命令

如果用户对起始模板感兴趣，获取准确的当前命令：

```
搜索网络："{{starter_name}} CLI command options flags latest"
搜索网络："{{starter_name}} create new project command examples"
```

### 8. 生成起始模板内容

准备追加到文档的内容：

#### 内容结构：

````markdown
## 起始模板评估

### 主要技术领域

根据项目需求分析确定为 {{identified_domain}}

### 考虑的起始选项

{{评估的起始模板分析}

### 选定的起始模板：{{starter_name}}

**选择理由：**
{{为什么选择此起始模板}

**初始化命令：**

```bash
{{带选项的完整起始命令}
```

**起始模板提供的架构决策：**

**语言和运行时：**
{{language_typescript_setup}}

**样式解决方案：**
{{样式解决方案配置}

**构建工具：**
{{构建工具和优化}

**测试框架：**
{{测试设置和配置}

**代码组织：**
{{项目结构和模式}

**开发体验：**
{{开发工具和工作流}

**注意：** 使用此命令的项目初始化应该是第一个实施故事。

````

### 9. 呈现内容和菜单

显示生成的内容并呈现选择：

"我已经分析了 {{project_type}} 项目的起始模板选项。

**这是我将添加到文档的内容：**

[显示第8步的完整 markdown 内容]

**您想做什么？**
[A] 高级引导 - 探索自定义方法或非传统起始模板
[P] 多角色模式 - 从不同视角评估权衡
[C] 继续 - 保存此决策并进入架构决策"

### 10. 处理菜单选择

#### 如果 'A'（高级引导）：

- 完整阅读并遵循：{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml，使用当前起始分析
- 处理关于起始选项或自定义方法的增强洞察
- 询问用户："接受这些对起始模板评估的更改吗？(y/n)"
- 如果是：更新内容，然后返回 A/P/C 菜单
- 如果否：保留原始内容，然后返回 A/P/C 菜单

#### 如果 'P'（多角色模式）：

- 完整阅读并遵循：{project-root}/_bmad/core/workflows/party-mode/workflow.md，使用起始评估上下文
- 处理关于起始权衡的协作洞察
- 询问用户："接受这些对起始模板评估的更改吗？(y/n)"
- 如果是：更新内容，然后返回 A/P/C 菜单
- 如果否：保留原始内容，然后返回 A/P/C 菜单

#### 如果 'C'（继续）：

- 将最终内容追加到 `{planning_artifacts}/architecture.md`
- 更新 frontmatter：`stepsCompleted: [1, 2, 3]`
- 加载 `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-04-decisions.md`

## 追加到文档：

当用户选择 'C' 时，使用第8步的结构将内容直接追加到文档。

## 成功指标：

✅ 从项目上下文正确识别主要技术领域
✅ 研究并评估当前、维护中的起始模板
✅ 使用网络搜索验证所有版本，而非硬编码
✅ 清晰记录起始选择的架构影响
✅ 为用户提供清晰的起始选择理由
✅ 正确呈现和处理 A/P/C 菜单
✅ 选择 C 时正确追加内容到文档

## 失败模式：

❌ 未使用网络搜索验证当前版本
❌ 评估起始模板时忽略 UX 需求
❌ 未记录起始模板做出的架构决策
❌ 未考虑起始模板的维护状态
❌ 未为起始选择提供清晰的理由
❌ 内容生成后未呈现 A/P/C 菜单
❌ **关键**：仅读取部分步骤文件 - 导致理解不完整和决策不当
❌ **关键**：未完整阅读并理解下一步文件就使用 'C' 继续
❌ **关键**：未完整理解步骤要求和协议就做出决策

## 下一步：

用户选择 'C' 且内容保存到文档后，加载 `{project-root}/_bmad/bmm/workflows/3-solutioning/create-architecture/steps/step-04-decisions.md` 开始做出具体的架构决策。

记住：在用户明确从 A/P/C 菜单选择 'C' 且内容保存之前，不要进入 step-04！