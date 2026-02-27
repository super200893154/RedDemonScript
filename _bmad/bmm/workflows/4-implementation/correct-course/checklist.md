# 变更导航清单

<critical>此清单作为以下工作流的一部分执行：{project-root}/_bmad/bmm/workflows/4-implementation/correct-course/workflow.yaml</critical>
<critical>与用户系统地完成每个部分，记录发现和影响</critical>

<checklist>

<section n="1" title="理解触发因素和上下文">

<check-item id="1.1">
<prompt>识别揭示此问题的触发 Story</prompt>
<action>记录 Story ID 和简要描述</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="1.2">
<prompt>精确定义核心问题</prompt>
<action>分类问题类型：</action>
  - 实现过程中发现的技术限制
  - 利益相关者提出的新需求
  - 对原始需求的误解
  - 战略转向或市场变化
  - 需要不同解决方案的失败方法
<action>写出清晰的问题陈述</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="1.3">
<prompt>评估初始影响并收集支持证据</prompt>
<action>收集具体示例、错误消息、利益相关者反馈或技术约束</action>
<action>记录证据以供后续参考</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<halt-condition>
<action if="trigger is unclear">暂停："无法在不知道变更原因的情况下继续"</action>
<action if="no evidence provided">暂停："在分析影响之前需要具体证据或问题示例"</action>
</halt-condition>

</section>

<section n="2" title="Epic 影响评估">

<check-item id="2.1">
<prompt>评估包含触发 Story 的当前 Epic</prompt>
<action>此 Epic 是否仍可按原计划完成？</action>
<action>如果不能，需要什么修改？</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="2.2">
<prompt>确定需要的 Epic 级别变更</prompt>
<action>检查每种情况：</action>
  - 修改现有 Epic 范围或验收标准
  - 添加新 Epic 来解决问题
  - 移除或推迟不再可行的 Epic
  - 基于新理解完全重新定义 Epic
<action>记录需要的具体 Epic 变更</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="2.3">
<prompt>审查所有剩余计划 Epic 是否需要变更</prompt>
<action>检查每个未来 Epic 的影响</action>
<action>识别可能受影响的依赖</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="2.4">
<prompt>检查问题是否使未来 Epic 无效或需要新 Epic</prompt>
<action>此变更是否使任何计划中的 Epic 过时？</action>
<action>是否需要新 Epic 来填补此变更造成的缺口？</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="2.5">
<prompt>考虑 Epic 顺序或优先级是否应该改变</prompt>
<action>是否应该根据此问题重新排序 Epic？</action>
<action>优先级是否需要调整？</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

</section>

<section n="3" title="工件冲突和影响分析">

<check-item id="3.1">
<prompt>检查 PRD 是否有冲突</prompt>
<action>问题是否与核心 PRD 目标或目的冲突？</action>
<action>需求是否需要修改、添加或移除？</action>
<action>定义的 MVP 是否仍可实现或范围是否需要调整？</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="3.2">
<prompt>审查架构文档是否有冲突</prompt>
<action>检查每个领域的影响：</action>
  - 系统组件及其交互
  - 架构模式和设计决策
  - 技术栈选择
  - 数据模型和架构
  - API 设计和契约
  - 集成点
<action>记录需要更新的具体架构部分</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="3.3">
<prompt>检查 UI/UX 规范是否有冲突</prompt>
<action>检查对以下方面的影响：</action>
  - 用户界面组件
  - 用户流程和旅程
  - 线框图或原型
  - 交互模式
  - 可访问性考虑
<action>注意需要修订的具体 UI/UX 部分</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="3.4">
<prompt>考虑对其他工件的影响</prompt>
<action>审查其他工件的影响：</action>
  - 部署脚本
  - 基础设施即代码（IaC）
  - 监控和可观察性设置
  - 测试策略
  - 文档
  - CI/CD 流水线
<action>记录需要更新的任何次要工件</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

</section>

<section n="4" title="前进路径评估">

<check-item id="4.1">
<prompt>评估选项 1：直接调整</prompt>
<action>问题是否可以通过修改现有 Story 来解决？</action>
<action>是否可以在当前 Epic 结构内添加新 Story？</action>
<action>此方法是否可以保持项目时间线和范围？</action>
<action>工作量估算：[高/中/低]</action>
<action>风险级别：[高/中/低]</action>
<status>[ ] 可行 / [ ] 不可行</status>
</check-item>

<check-item id="4.2">
<prompt>评估选项 2：潜在回滚</prompt>
<action>恢复最近完成的 Story 是否会简化解决此问题？</action>
<action>哪些 Story 需要回滚？</action>
<action>回滚工作量是否因获得的简化而合理？</action>
<action>工作量估算：[高/中/低]</action>
<action>风险级别：[高/中/低]</action>
<status>[ ] 可行 / [ ] 不可行</status>
</check-item>

<check-item id="4.3">
<prompt>评估选项 3：PRD MVP 审查</prompt>
<action>原始 PRD MVP 在有此问题的情况下是否仍可实现？</action>
<action>MVP 范围是否需要减少或重新定义？</action>
<action>核心目标是否需要根据新约束进行修改？</action>
<action>如果范围减少，什么将被推迟到 MVP 后？</action>
<action>工作量估算：[高/中/低]</action>
<action>风险级别：[高/中/低]</action>
<status>[ ] 可行 / [ ] 不可行</status>
</check-item>

<check-item id="4.4">
<prompt>选择推荐的前进路径</prompt>
<action>基于所有选项的分析，选择最佳路径</action>
<action>提供清晰的理由，考虑：</action>
  - 实现工作量和时间线影响
  - 技术风险和复杂性
  - 对团队士气和动力的影响
  - 长期可持续性和可维护性
  - 利益相关者期望和业务价值
<action>选定的方法：[选项 1 / 选项 2 / 选项 3 / 混合]</action>
<action>理由：[记录推理]</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

</section>

<section n="5" title="Sprint 变更提案组件">

<check-item id="5.1">
<prompt>创建识别的问题摘要</prompt>
<action>写出清晰、简洁的问题陈述</action>
<action>包括关于发现和影响的上下文</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="5.2">
<prompt>记录 Epic 影响和工件调整需求</prompt>
<action>总结 Epic 影响评估（第 2 部分）的发现</action>
<action>总结工件冲突分析（第 3 部分）的发现</action>
<action>具体说明需要什么变更以及为什么</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="5.3">
<prompt>展示推荐的前进路径及理由</prompt>
<action>包括第 4 部分选定的方法</action>
<action>为推荐提供完整的理由</action>
<action>解决权衡和考虑的替代方案</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="5.4">
<prompt>定义 PRD MVP 影响和高级行动计划</prompt>
<action>清楚说明 MVP 是否受影响</action>
<action>概述实现所需的主要行动项</action>
<action>识别依赖和顺序</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="5.5">
<prompt>建立 Agent 交接计划</prompt>
<action>识别哪些角色/Agent 将执行变更：</action>
  - 开发团队（用于实现）
  - 产品负责人 / Scrum Master（用于待办事项变更）
  - 产品经理 / 架构师（用于战略变更）
<action>定义每个角色的职责</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

</section>

<section n="6" title="最终审查和交接">

<check-item id="6.1">
<prompt>审查清单完成情况</prompt>
<action>验证所有适用部分已处理</action>
<action>确认所有 [需要行动] 项目已记录</action>
<action>确保分析全面且可操作</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="6.2">
<prompt>验证 Sprint 变更提案的准确性</prompt>
<action>审查完整提案的一致性和清晰度</action>
<action>确保所有推荐都有分析支持</action>
<action>检查提案可操作且具体</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="6.3">
<prompt>获取用户明确批准</prompt>
<action>向用户展示完整提案</action>
<action>获得明确的批准/不批准</action>
<action>记录批准和任何条件</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="6.4">
<prompt>更新 sprint-status.yaml 以反映批准的 Epic 变更</prompt>
<action>如果添加了 Epic：添加新 Epic 条目，状态为 'backlog'</action>
<action>如果移除了 Epic：移除相应的条目</action>
<action>如果重新编号了 Epic：更新 Epic ID 和 Story 引用</action>
<action>如果添加/移除了 Story：更新受影响 Epic 内的 Story 条目</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<check-item id="6.5">
<prompt>确认下一步和交接计划</prompt>
<action>与用户审查交接职责</action>
<action>确保所有利益相关者理解他们的角色</action>
<action>确认时间线和成功标准</action>
<status>[ ] 完成 / [ ] N/A / [ ] 需要行动</status>
</check-item>

<halt-condition>
<action if="any critical section cannot be completed">暂停："没有完整的影响分析无法继续到提案阶段"</action>
<action if="user approval not obtained">暂停："实现变更前必须有明确批准"</action>
<action if="handoff responsibilities unclear">暂停："必须清楚定义谁将执行提议的变更"</action>
</halt-condition>

</section>

</checklist>

<execution-notes>
<note>此清单用于影响项目方向的重大变更</note>
<note>与用户交互式地工作 - 他们做最终决定</note>
<note>在分析问题时要实事求是，不要归咎</note>
<note>专业地处理变更为改进项目的机会</note>
<note>在整个过程中保持对话上下文 - 这是协作工作</note>
</execution-notes>