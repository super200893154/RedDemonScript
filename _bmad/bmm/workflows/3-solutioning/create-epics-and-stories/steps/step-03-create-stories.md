---
name: 'step-03-create-stories'
description: '按照模板结构生成所有 epic 及其 story'

# 路径定义
workflow_path: '{project-root}/_bmad/bmm/workflows/3-solutioning/create-epics-and-stories'

# 文件引用
thisStepFile: './step-03-create-stories.md'
nextStepFile: './step-04-final-validation.md'
workflowFile: '{workflow_path}/workflow.md'
outputFile: '{planning_artifacts}/epics.md'

# 任务引用
advancedElicitationTask: '{project-root}/_bmad/core/workflows/advanced-elicitation/workflow.xml'
partyModeWorkflow: '{project-root}/_bmad/core/workflows/party-mode/workflow.md'

# 模板引用
epicsTemplate: '{workflow_path}/templates/epics-template.md'
---

# 步骤 3：生成 Epic 和 Story

## 步骤目标：

根据批准的 epics_list 生成所有 epic 及其 story，完全遵循模板结构。

## 强制执行规则（首先阅读）：

### 通用规则：

- 🛑 绝不在没有用户输入的情况下生成内容
- 📖 关键：在采取任何行动之前阅读完整的步骤文件
- 🔄 关键：按顺序处理 epic
- 📋 你是协调者，而非内容生成者
- ✅ 你必须始终以配置中 `{communication_language}` 指定的 Agent 沟通风格输出

### 角色强化：

- ✅ 你是产品策略师和技术规格文档撰写者
- ✅ 如果你已被赋予沟通或人设模式，在扮演此新角色时继续使用
- ✅ 我们进行协作对话，而非命令-响应
- ✅ 你带来 story 创建和验收标准专业知识
- ✅ 用户带来他们的实施优先级和约束

### 步骤特定规则：

- 🎯 完全按照模板为每个 epic 生成 story
- 🚫 禁止偏离模板结构
- 💬 每个 story 必须有清晰的验收标准
- 🚪 确保每个 story 可由单个开发 Agent 完成
- 🔗 **关键：Story 必须不依赖于同一 epic 内的未来 story**

## 执行协议：

- 🎯 与用户输入协作生成 story
- 💾 按照模板将 epic 和 story 追加到 {outputFile}
- 📖 按顺序一次处理一个 epic
- 🚫 禁止跳过任何 epic 或匆忙处理 story

## Story 生成流程：

### 1. 加载批准的 Epic 结构

加载 {outputFile} 并审查：

- 步骤2批准的 epics_list
- FR 覆盖图
- 所有需求（FR、NFR、额外）
- 文档末尾的模板结构

### 2. 解释 Story 创建方法

**Story 创建指南：**

对于每个 epic，创建 story 应：

- 遵循确切的模板结构
- 规模适合单个开发 Agent 完成
- 有清晰的用户价值
- 包含具体的验收标准
- 引用正在履行的需求

**🚨 数据库/实体创建原则：**
仅在 story 需要时创建表/实体：

- ❌ 错误：Epic 1 Story 1 创建所有 50 个数据库表
- ✅ 正确：每个 story 仅创建/修改它需要的表

**🔗 Story 依赖原则：**
Story 必须可按顺序独立完成：

- ❌ 错误：Story 1.2 需要先完成 Story 1.3
- ✅ 正确：每个 story 可以仅基于之前的 story 完成
- ❌ 错误："等待 Story 1.4 实施后才能工作"
- ✅ 正确："此 story 独立工作并启用未来 story"

**Story 格式（来自模板）：**

```
### Story {N}.{M}: {story_title}

作为一个 {用户类型}，
我想要 {能力}，
以便 {价值收益}。

**验收标准：**

**假设** {前置条件}
**当** {操作}
**那么** {预期结果}
**并且** {额外标准}
```

**✅ 良好 Story 示例：**

_Epic 1：用户认证_

- Story 1.1：使用邮箱注册用户
- Story 1.2：使用密码登录用户
- Story 1.3：通过邮件重置密码

_Epic 2：内容创建_

- Story 2.1：创建新博客文章
- Story 2.2：编辑现有博客文章
- Story 2.3：发布博客文章

**❌ 错误 Story 示例：**

- Story："设置数据库"（无用户价值）
- Story："创建所有模型"（太大，无用户价值）
- Story："构建认证系统"（太大）
- Story："登录 UI（依赖于 Story 1.3 API 端点）"（未来依赖！）
- Story："编辑文章（需要先实施 Story 1.4）"（错误顺序！）

### 3. 按顺序处理 Epic

对于批准的 epics_list 中的每个 epic：

#### A. Epic 概览

显示：

- Epic 编号和标题
- Epic 目标声明
- 此 epic 涵盖的 FR
- 任何相关的 NFR 或额外需求

#### B. Story 分解

与用户合作将 epic 分解为 story：

- 识别不同的用户能力
- 确保 epic 内的逻辑流程
- 适当调整 story 规模

#### C. 生成每个 Story

对于 epic 中的每个 story：

1. **Story 标题**：清晰、行动导向
2. **用户故事**：完成"作为一个/我想要/以便"格式
3. **验收标准**：编写具体、可测试的标准

**AC 编写指南：**

- 使用 Given/When/Then 格式
- 每个 AC 应独立可测试
- 包括边缘情况和错误条件
- 适用时引用具体需求

#### D. 协作审查

编写每个 story 后：

- 向用户呈现 story
- 询问："此 story 是否正确捕获了需求？"
- "范围是否适合单个开发会话？"
- "验收标准是否完整且可测试？"

#### E. 追加到文档

Story 批准后：

- 按照模板结构追加到 {outputFile}
- 使用正确的编号（Epic N, Story M）
- 保持正确的 markdown 格式

### 4. Epic 完成

epic 的所有 story 完成后：

- 显示 epic 摘要
- 显示创建的 story 数量
- 验证 epic 的所有 FR 都被覆盖
- 获取用户确认继续下一个 epic

### 5. 对所有 Epic 重复

继续对批准列表中的每个 epic 进行处理，按顺序处理它们（Epic 1、Epic 2 等）。

### 6. 最终文档完成

所有 epic 和 story 生成后：

- 验证文档完全遵循模板结构
- 确保所有占位符已替换
- 确认所有 FR 被覆盖
- 检查格式一致性

## 模板结构合规性：

最终 {outputFile} 必须完全遵循此结构：

1. **概览** 部分，包含项目名称
2. **需求清单**，所有三个子部分已填充
3. **FR 覆盖图**，显示需求到 epic 的映射
4. **Epic 列表**，包含批准的 epic 结构
5. **Epic 部分**，每个 epic（N = 1, 2, 3...）
   - Epic 标题和目标
   - 该 epic 的所有 story（M = 1, 2, 3...）
     - Story 标题和用户故事
     - 验收标准使用 Given/When/Then 格式

### 7. 呈现最终菜单选项

所有 epic 和 story 完成后：

显示：`**选择一个选项：** [A] 高级引导 [P] 多角色模式 [C] 继续`

#### 菜单处理逻辑：

- 如果 A：完整阅读并遵循：{advancedElicitationTask}
- 如果 P：完整阅读并遵循：{partyModeWorkflow}
- 如果 C：将内容保存到 {outputFile}，更新 frontmatter，然后完整阅读并遵循：{nextStepFile}
- 如果有任何其他评论或查询：帮助用户响应，然后[重新显示菜单选项](#7-present-final-menu-options)

#### 执行规则：

- 始终在呈现菜单后暂停并等待用户输入
- 仅当用户选择 'C' 时进入下一步
- 其他菜单项执行后，返回此菜单
- 用户可以聊天或提问 - 始终响应，然后再次显示菜单选项

## 关键步骤完成说明

只有当选择 [C 继续选项] 且 [所有 epic 和 story 已完全按照模板结构保存到文档] 后，你才会完整阅读并遵循：`{nextStepFile}` 开始最终验证阶段。

---

## 🚨 系统成功/失败指标

### ✅ 成功：

- 按顺序处理所有 epic
- 为每个 epic 创建 story
- 完全遵循模板结构
- 所有 FR 被 story 覆盖
- Story 规模适当
- 验收标准具体可测试
- 文档完整且准备开发

### ❌ 系统失败：

- 偏离模板结构
- 缺少 epic 或 story
- Story 太大或不清楚
- 缺少验收标准
- 未遵循正确格式

**主规则：** 跳过步骤、优化序列或不遵循确切指令是被禁止的，构成系统失败。