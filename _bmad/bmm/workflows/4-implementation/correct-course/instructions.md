# 航向修正 - Sprint 变更管理说明

<critical>工作流执行引擎由以下文件控制：{project-root}/_bmad/core/tasks/workflow.xml</critical>
<critical>你必须已经加载并处理：{project-root}/_bmad/bmm/workflows/4-implementation/correct-course/workflow.yaml</critical>
<critical>所有响应使用 {communication_language} 交流，语言必须适应 {user_skill_level}</critical>
<critical>所有文档以 {document_output_language} 生成</critical>

<critical>文档输出：更新的 Epics、Stories 或 PRD 部分。清晰、可操作的变更。用户技能水平（{user_skill_level}）仅影响对话风格，不影响文档更新。</critical>

<workflow>

<step n="1" goal="初始化变更导航">
  <action>加载 {project_context} 获取编码标准和项目范围模式（如果存在）</action>
  <action>确认变更触发因素并收集用户对问题的描述</action>
  <action>询问："发现了什么具体问题或变更需要进行导航？"</action>
  <action>验证是否可以访问所需的项目文档：</action>
    - PRD（产品需求文档）
    - 当前的 Epics 和 Stories
    - 架构文档
    - UI/UX 规范
  <action>询问用户偏好的模式：</action>
    - **增量**（推荐）：协作完善每个编辑
    - **批量**：一次性展示所有变更供审查
  <action>存储模式选择以在整个工作流中使用</action>

<action if="change trigger is unclear">暂停："无法在没有清楚了解触发问题的情况下导航变更。请提供关于需要改变什么以及为什么的具体细节。"</action>

<action if="core documents are unavailable">暂停："需要访问项目文档（PRD、Epics、架构、UI/UX）来评估变更影响。请确保这些文档可访问。"</action>
</step>

<step n="0.5" goal="发现并加载项目文档">
  <invoke-protocol name="discover_inputs" />
  <note>发现后，这些内容变量可用：{prd_content}、{epics_content}、{architecture_content}、{ux_design_content}、{tech_spec_content}、{document_project_content}</note>
</step>

<step n="2" goal="执行变更分析清单">
  <action>完整阅读并遵循以下文件的系统分析：{checklist}</action>
  <action>与用户交互式地完成每个清单部分</action>
  <action>记录每个清单项目的状态：</action>
    - [x] 完成 - 项目成功完成
    - [N/A] 跳过 - 项目不适用于此变更
    - [!] 需要行动 - 项目需要关注或后续
  <action>维护发现和影响的运行笔记</action>
  <action>在每个主要部分后展示清单进展</action>

<action if="checklist cannot be completed">识别阻塞问题并与用户一起解决后再继续</action>
</step>

<step n="3" goal="起草具体变更提案">
<action>基于清单发现，为每个识别的工件创建明确的编辑提案</action>

<action>对于 Story 变更：</action>

- 显示旧 → 新文本格式
- 包含 Story ID 和正在修改的部分
- 为每个变更提供理由
- 示例格式：

  ```
  Story：[STORY-123] 用户认证
  部分：验收标准

  旧：
  - 用户可以使用邮箱/密码登录

  新：
  - 用户可以使用邮箱/密码登录
  - 用户可以通过认证器应用启用双因素认证

  理由：实现过程中识别的安全要求
  ```

<action>对于 PRD 修改：</action>

- 指定要更新的具体部分
- 显示当前内容和提议的变更
- 解释对 MVP 范围和要求的影响

<action>对于架构变更：</action>

- 识别受影响的组件、模式或技术选择
- 描述需要的图表更新
- 注意对其他组件的任何连锁效应

<action>对于 UI/UX 规范更新：</action>

- 引用具体的屏幕或组件
- 显示需要的线框图或流程变更
- 将变更与用户体验影响联系起来

<check if="mode is Incremental">
  <action>单独展示每个编辑提案</action>
  <ask>审查并完善此变更？选项：批准 [a]、编辑 [e]、跳过 [s]</ask>
  <action>根据用户反馈迭代每个提案</action>
</check>

<action if="mode is Batch">收集所有编辑提案并在步骤结束时一起展示</action>

</step>

<step n="4" goal="生成 Sprint 变更提案">
<action>编制包含以下部分的综合 Sprint 变更提案文档：</action>

<action>第 1 部分：问题摘要</action>

- 描述触发变更的清晰问题陈述
- 关于何时/如何发现问题的上下文
- 演示问题的证据或示例

<action>第 2 部分：影响分析</action>

- Epic 影响：哪些 Epic 受影响以及如何
- Story 影响：需要变更的当前和未来 Story
- 工件冲突：需要更新的 PRD、架构、UI/UX 文档
- 技术影响：代码、基础设施或部署影响

<action>第 3 部分：推荐方法</action>

- 从清单评估中展示选定的前进路径：
  - 直接调整：在现有计划内修改/添加 Story
  - 潜在回滚：恢复已完成的工作以简化解决
  - MVP 审查：减少范围或修改目标
- 为推荐提供清晰的理由
- 包括工作量估算、风险评估和时间线影响

<action>第 4 部分：详细变更提案</action>

- 包括来自步骤 3 的所有完善的编辑提案
- 按工件类型分组（Stories、PRD、架构、UI/UX）
- 确保每个变更包含前后对比和理由

<action>第 5 部分：实现交接</action>

- 分类变更范围：
  - 轻微：由开发团队直接实现
  - 中等：需要待办事项重组（PO/SM）
  - 重大：需要根本性重新规划（PM/架构师）
- 指定交接接收者及其职责
- 定义实现的成功标准

<action>向用户展示完整的 Sprint 变更提案</action>
<action>将 Sprint 变更提案文档写入 {default_output_file}</action>
<ask>审查完整提案。继续 [c] 或编辑 [e]？</ask>
</step>

<step n="5" goal="最终确定并路由实现">
<action>获取用户对完整提案的明确批准</action>
<ask>你批准此 Sprint 变更提案进行实现吗？（是/否/修订）</ask>

<check if="no or revise">
  <action>收集关于需要调整的具体反馈</action>
  <action>返回适当的步骤解决问题</action>
  <goto step="3">如果需要更改编辑提案</goto>
  <goto step="4">如果需要更改整体提案结构</goto>

</check>

<check if="yes the proposal is approved by the user">
  <action>最终确定 Sprint 变更提案文档</action>
  <action>确定变更范围分类：</action>

- **轻微**：可由开发团队直接实现
- **中等**：需要待办事项重组和 PO/SM 协调
- **重大**：需要 PM/架构师参与的根本性重新规划

<action>根据范围提供适当的交接：</action>

</check>

<check if="Minor scope">
  <action>路由到：开发团队直接实现</action>
  <action>交付物：最终确定的编辑提案和实现任务</action>
</check>

<check if="Moderate scope">
  <action>路由到：产品负责人 / Scrum Master Agent</action>
  <action>交付物：Sprint 变更提案 + 待办事项重组计划</action>
</check>

<check if="Major scope">
  <action>路由到：产品经理 / 解决方案架构师</action>
  <action>交付物：完整的 Sprint 变更提案 + 升级通知</action>

<action>与用户确认交接完成和下一步</action>
<action>在工作流执行日志中记录交接</action>
</check>

</step>

<step n="6" goal="工作流完成">
<action>总结工作流执行：</action>
  - 已解决的问题：{{change_trigger}}
  - 变更范围：{{scope_classification}}
  - 修改的工件：{{list_of_artifacts}}
  - 路由到：{{handoff_recipients}}

<action>确认所有交付物已生成：</action>

- Sprint 变更提案文档
- 带有前后对比的具体编辑提案
- 实现交接计划

<action>向用户报告工作流完成情况并提供个性化消息："✅ 航向修正工作流完成，{user_name}！"</action>
<action>提醒用户成功标准和实现团队的下一步</action>
</step>

</workflow>