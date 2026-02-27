# 回顾 - Epic 完成审查说明

<critical>工作流执行引擎由以下文件控制：{project-root}/_bmad/core/tasks/workflow.xml</critical>
<critical>你必须已经加载并处理：{project-root}/_bmad/bmm/workflows/4-implementation/retrospective/workflow.yaml</critical>
<critical>所有响应使用 {communication_language} 交流，语言必须适应 {user_skill_level}</critical>
<critical>所有文档以 {document_output_language} 生成</critical>
<critical>⚠️ 绝对不要时间估算 - 永远不要提及小时、天、周、月或任何基于时间的预测。AI 已经从根本上改变了开发速度 - 过去团队需要数周/数月才能完成的工作，现在一个人几小时就能完成。不要提供任何时间估算。</critical>

<critical>
  文档输出：回顾分析。简洁的见解、经验教训、行动项。用户技能水平（{user_skill_level}）仅影响对话风格，不影响回顾内容。

引导说明：

- Scrum Master 主持此次回顾
- 心理安全至关重要 - 不归咎责任
- 关注系统、流程和学习
- 每个人都贡献，最好提供具体示例
- 行动项必须可实现且有明确负责人
- 两部分格式：(1) Epic 审查 + (2) 下一个 Epic 准备

派对模式协议：

- 所有 Agent 对话必须使用格式："姓名（角色）：对话"
- 示例：Bob（Scrum Master）："让我们开始..."
- 示例：{user_name}（项目负责人）：[用户回应]
- 与用户创建自然的来回互动，用户积极参与
- 展示分歧、不同观点、真实的团队动态
  </critical>

<workflow>

<step n="1" goal="Epic 发现 - 使用优先级逻辑查找已完成的 Epic">

<action>加载 {project_context} 获取项目范围的模式和约定（如果存在）</action>
<action>使用自然对话向 {user_name} 解释 Epic 发现过程</action>

<output>
Bob（Scrum Master）："欢迎参加回顾，{user_name}。让我帮你确定我们刚刚完成的 Epic。我会先检查 sprint-status，但你才是我们今天要审查什么的最终决定者。"
</output>

<action>优先级 1：首先检查 {sprint_status_file}</action>

<action>加载完整文件：{sprint_status_file}</action>
<action>读取所有 development_status 条目</action>
<action>找到至少有一个 Story 标记为 "done" 的最高 Epic 编号</action>
<action>从 "epic-X-retrospective" 或 Story 键如 "X-Y-story-name" 等键中提取 Epic 编号</action>
<action>设置 {{detected_epic}} = 找到的已完成 Story 的最高 Epic 编号</action>

<check if="{{detected_epic}} found">
  <action>向用户展示发现结果及上下文</action>

  <output>
Bob（Scrum Master）："根据 {sprint_status_file}，看起来 Epic {{detected_epic}} 最近完成了。这是你今天想审查的 Epic 吗，{user_name}？"
  </output>

<action>等待 {user_name} 确认或更正</action>

  <check if="{user_name} confirms">
    <action>设置 {{epic_number}} = {{detected_epic}}</action>
  </check>

  <check if="{user_name} provides different epic number">
    <action>设置 {{epic_number}} = 用户提供的编号</action>
    <output>
Bob（Scrum Master）："明白了，我们要审查 Epic {{epic_number}}。让我收集那个信息。"
    </output>
  </check>
</check>

<check if="{{detected_epic}} NOT found in sprint-status">
  <action>优先级 2：直接询问用户</action>

  <output>
Bob（Scrum Master）："我无法从 {sprint_status_file} 检测到已完成的 Epic。{user_name}，你刚刚完成了哪个 Epic 编号？"
  </output>

<action>等待 {user_name} 提供 Epic 编号</action>
<action>设置 {{epic_number}} = 用户提供的编号</action>
</check>

<check if="{{epic_number}} still not determined">
  <action>优先级 3：回退到 stories 文件夹</action>

<action>扫描 {implementation_artifacts} 查找编号最高的 Story 文件</action>
<action>从 Story 文件名中提取 Epic 编号（模式：epic-X-Y-story-name.md）</action>
<action>设置 {{detected_epic}} = 找到的最高 Epic 编号</action>

  <output>
Bob（Scrum Master）："我在 stories 文件夹中找到了 Epic {{detected_epic}} 的 Story。这是我们正在审查的 Epic 吗，{user_name}？"
  </output>

<action>等待 {user_name} 确认或更正</action>
<action>设置 {{epic_number}} = 确认的编号</action>
</check>

<action>确定 {{epic_number}} 后，验证 Epic 完成状态</action>

<action>在 {sprint_status_file} 中查找 Epic {{epic_number}} 的所有 Story：

- 查找以 "{{epic_number}}-" 开头的键（例如 "1-1-"、"1-2-" 等）
- 排除 Epic 键本身（"epic-{{epic_number}}"）
- 排除回顾键（"epic-{{epic_number}}-retrospective"）
  </action>

<action>统计此 Epic 的 Story 总数</action>
<action>统计状态为 "done" 的 Story 数量</action>
<action>收集待处理 Story 键列表（状态 != "done"）</action>
<action>判断是否完成：如果所有 Story 都为 done 则为 true，否则为 false</action>

<check if="epic is not complete">
  <output>
Alice（产品负责人）："等等，Bob - 我看到 Epic {{epic_number}} 实际上还没有完成。"

Bob（Scrum Master）："让我检查一下...你说得对，Alice。"

**Epic 状态：**

- Story 总数：{{total_stories}}
- 已完成（Done）：{{done_stories}}
- 待处理：{{pending_count}}

**待处理 Story：**
{{pending_story_list}}

Bob（Scrum Master）："{user_name}，我们通常在所有 Story 完成后才运行回顾。你想怎么做？"

**选项：**

1. 在运行回顾之前完成剩余 Story（推荐）
2. 继续进行部分回顾（不理想，但可行）
3. 运行 sprint-planning 刷新 Story 跟踪
   </output>

<ask if="{{non_interactive}} == false">继续处理不完整的 Epic？（是/否）</ask>

  <check if="user says no">
    <output>
Bob（Scrum Master）："明智的选择，{user_name}。让我们先完成那些 Story，然后再进行正式的回顾。"
    </output>
    <action>暂停</action>
  </check>

<action if="user says yes">设置 {{partial_retrospective}} = true</action>
<output>
Charlie（高级开发）："提醒大家，这次部分回顾可能会遗漏那些待处理 Story 的一些重要教训。"

Bob（Scrum Master）："说得对，Charlie。{user_name}，我们现在记录我们能记录的，但可能需要在全部完成后重新审查。"
</output>
</check>

<check if="epic is complete">
  <output>
Alice（产品负责人）："太好了！所有 {{done_stories}} 个 Story 都标记为完成了。"

Bob（Scrum Master）："完美。Epic {{epic_number}} 已完成，可以进行回顾了，{user_name}。"
</output>
</check>

</step>

<step n="0.5" goal="发现并加载项目文档">
  <invoke-protocol name="discover_inputs" />
  <note>发现后，这些内容变量可用：{epics_content}（针对此 Epic 的选择性加载）、{architecture_content}、{prd_content}、{document_project_content}</note>
</step>

<step n="2" goal="深度 Story 分析 - 从实现中提取教训">

<output>
Bob（Scrum Master）："在我们开始团队讨论之前，让我先回顾一下所有 Story 记录，找出关键主题。这会帮助我们进行更深入的对话。"

Charlie（高级开发）："好主意 - 那些开发笔记里总是有很多有价值的内容。"
</output>

<action>对于 Epic {{epic_number}} 中的每个 Story，从 {implementation_artifacts}/{{epic_number}}-{{story_num}}-*.md 读取完整的 Story 文件</action>

<action>从每个 Story 中提取并分析：</action>

**开发笔记和困难：**

- 查找 "## Dev Notes"、"## Implementation Notes"、"## Challenges"、"## Development Log" 等部分
- 识别开发人员遇到困难或犯错误的地方
- 注意发现的意外复杂性或陷阱
- 记录未按计划进行的技术决策
- 跟踪估算偏差较大的地方（过高或过低）

**审查反馈模式：**

- 查找 "## Review"、"## Code Review"、"## SM Review"、"## Scrum Master Review" 部分
- 识别跨 Story 的反复出现的反馈主题
- 注意哪些类型的问题反复出现
- 跟踪质量关注或架构偏差
- 记录审查中提到的表扬或优秀工作

**经验教训：**

- 查找 Story 内的 "## Lessons Learned"、"## Retrospective Notes"、"## Takeaways" 部分
- 提取开发过程中记录的明确教训
- 识别"顿悟时刻"或突破
- 注意哪些做法会有所不同
- 跟踪成功的实验或方法

**产生的技术债务：**

- 查找 "## Technical Debt"、"## TODO"、"## Known Issues"、"## Future Work" 部分
- 记录采取的捷径及原因
- 跟踪影响下一个 Epic 的债务项
- 注意债务项的严重性和优先级

**测试和质量见解：**

- 查找 "## Testing"、"## QA Notes"、"## Test Results" 部分
- 注意测试挑战或意外情况
- 跟踪 Bug 模式或回归问题
- 记录测试覆盖缺口

<action>综合所有 Story 的模式：</action>

**共同困难：**

- 识别在 2 个以上 Story 中出现的问题（例如 "5 个 Story 中有 3 个遇到 API 认证问题"）
- 注意团队持续困难的领域
- 跟踪复杂性被低估的地方

**反复出现的审查反馈：**

- 识别反馈主题（例如 "每次审查都标记了错误处理"）
- 注意质量模式（正面和负面）
- 跟踪团队在 Epic 过程中改进的领域

**突破时刻：**

- 记录关键发现（例如 "Story 3 发现的缓存模式我们在 Epic 其余部分一直在使用"）
- 注意团队速度显著提高的时刻
- 跟踪值得重复的创新解决方案

**速度模式：**

- 计算每个 Story 的平均完成时间
- 注意速度趋势（例如 "前 2 个 Story 耗时是估算的 3 倍"）
- 识别哪些类型的 Story 更快/更慢

**团队协作亮点：**

- 注意 Story 中提到的优秀协作时刻
- 跟踪结对编程或 Mob 编程有效的地方
- 记录有效的解决问题的会议

<action>存储此综合分析 - 这些模式将推动回顾讨论</action>

<output>
Bob（Scrum Master）："好的，我已经回顾了所有 {{total_stories}} 个 Story 记录。我发现了一些非常有趣的模式值得讨论。"

Dana（QA 工程师）："我很好奇你发现了什么，Bob。我在测试中也注意到了一些事情。"

Bob（Scrum Master）："我们会讨论所有的内容。但首先，让我加载上一个 Epic 的回顾，看看我们是否从上次学到了东西。"
</output>

</step>

<step n="3" goal="加载并整合上一个 Epic 的回顾">

<action>计算上一个 Epic 编号：{{prev_epic_num}} = {{epic_number}} - 1</action>

<check if="{{prev_epic_num}} >= 1">
  <action>使用模式搜索以前的回顾：{implementation_artifacts}/epic-{{prev_epic_num}}-retro-*.md</action>

  <check if="previous retrospectives found">
    <output>
Bob（Scrum Master）："我找到了 Epic {{prev_epic_num}} 的回顾。让我看看当时我们承诺了什么..."
    </output>

    <action>阅读以前的回顾</action>

    <action>提取关键要素：</action>
    - **承诺的行动项**：团队同意改进什么？
    - **经验教训**：记录了什么见解？
    - **流程改进**：同意了什么变更？
    - **标记的技术债务**：记录了什么债务？
    - **团队协议**：做了什么承诺？
    - **准备任务**：此 Epic 需要什么？

    <action>与当前 Epic 执行进行交叉参考：</action>

    **行动项跟进：**
    - 对于 Epic {{prev_epic_num}} 回顾中的每个行动项，检查是否完成
    - 在当前 Epic 的 Story 记录中查找证据
    - 标记每个行动项：✅ 已完成、⏳ 进行中、❌ 未处理

    **应用的经验：**
    - 对于 Epic {{prev_epic_num}} 的每个教训，检查团队是否在 Epic {{epic_number}} 中应用了
    - 在开发笔记、审查反馈或结果中查找证据
    - 记录成功和错过的机会

    **流程改进效果：**
    - 对于 Epic {{prev_epic_num}} 同意的每个流程变更，评估是否有帮助
    - 变更是否改善了速度、质量或团队满意度？
    - 我们应该保留、修改还是放弃这个变更？

    **技术债务状态：**
    - 对于 Epic {{prev_epic_num}} 的每个债务项，检查是否已处理
    - 未处理的债务是否在 Epic {{epic_number}} 中造成了问题？
    - 债务是增加了还是减少了？

    <action>为回顾讨论准备"连续性见解"</action>

    <action>识别成功应用以前教训的胜利：</action>
    - 记录应用学习的具体示例
    - 注意对 Epic {{epic_number}} 结果的积极影响
    - 庆祝团队的成长和改进

    <action>识别忽视以前教训的错过机会：</action>
    - 记录团队重复以前错误的地方
    - 注意未应用教训的影响（不归咎）
    - 探索阻碍应用的障碍

    <output>

Bob（Scrum Master）："有趣...在 Epic {{prev_epic_num}} 的回顾中，我们承诺了 {{action_count}} 个行动项。"

Alice（产品负责人）："我们做得怎么样，Bob？"

Bob（Scrum Master）："我们完成了 {{completed_count}} 个，在 {{in_progress_count}} 个上有进展，但 {{not_addressed_count}} 个没有处理。"

Charlie（高级开发）：_看起来很担心_ "哪些没有处理？"

Bob（Scrum Master）："我们会在回顾中讨论。其中一些可能解释了我们这次 Epic 遇到的挑战。"

Elena（初级开发）："这...实际上很有见地。"

Bob（Scrum Master）："这就是为什么我们要跟踪这些。模式识别帮助我们改进。"
</output>

  </check>

  <check if="no previous retro found">
    <output>
Bob（Scrum Master）："我没有看到 Epic {{prev_epic_num}} 的回顾。要么我们跳过了，要么这是你的第一次回顾。"

Alice（产品负责人）："可能是我们的第一次。开始养成这个习惯的好时机！"
</output>
<action>设置 {{first_retrospective}} = true</action>
</check>
</check>

<check if="{{prev_epic_num}} < 1">
  <output>
Bob（Scrum Master）："这是 Epic 1，所以自然没有以前的回顾可以参考。我们要从头开始！"

Charlie（高级开发）："第一个 Epic，第一次回顾。让我们做得有意义。"
</output>
<action>设置 {{first_retrospective}} = true</action>
</check>

</step>

<step n="4" goal="预览下一个 Epic 并进行变更检测">

<action>计算下一个 Epic 编号：{{next_epic_num}} = {{epic_number}} + 1</action>

<output>
Bob（Scrum Master）："在我们深入讨论之前，让我快速查看一下 Epic {{next_epic_num}}，了解接下来要做什么。"

Alice（产品负责人）："好主意 - 帮助我们将学到的与即将要做的联系起来。"
</output>

<action>尝试使用选择性加载策略加载下一个 Epic：</action>

**首先尝试分片版本（更具体）：**
<action>检查文件是否存在：{planning_artifacts}/epic*/epic-{{next_epic_num}}.md</action>

<check if="sharded epic file found">
  <action>加载 {planning_artifacts}/*epic*/epic-{{next_epic_num}}.md</action>
  <action>设置 {{next_epic_source}} = "sharded"</action>
</check>

**回退到完整文档：**
<check if="sharded epic not found">
<action>检查文件是否存在：{planning_artifacts}/epic*.md</action>

  <check if="whole epic file found">
    <action>加载完整的 Epics 文档</action>
    <action>提取 Epic {{next_epic_num}} 部分</action>
    <action>设置 {{next_epic_source}} = "whole"</action>
  </check>
</check>

<check if="next epic found">
  <action>分析下一个 Epic：</action>
  - Epic 标题和目标
  - 计划的 Story 和复杂性估算
  - 对 Epic {{epic_number}} 工作的依赖
  - 需要的新技术要求或能力
  - 潜在风险或未知因素
  - 业务目标和成功标准

<action>识别对已完成工作的依赖：</action>

- Epic {{next_epic_num}} 依赖 Epic {{epic_number}} 的哪些组件？
- 所有先决条件是否完整且稳定？
- 是否有未完成的工作造成阻塞性依赖？

<action>注意潜在的缺口或需要的准备：</action>

- 需要的技术设置（基础设施、工具、库）
- 需要填补的知识缺口（研究、培训、探索）
- 开始下一个 Epic 之前需要的重构
- 需要创建的文档或规范

<action>检查技术先决条件：</action>

- 必须准备好的 API 或集成
- 需要的数据迁移或架构变更
- 测试基础设施要求
- 部署或环境设置

  <output>
Bob（Scrum Master）："好的，我已经查看了 Epic {{next_epic_num}}：'{{next_epic_title}}'"

Alice（产品负责人）："我们面临什么？"

Bob（Scrum Master）："计划了 {{next_epic_num}} 个 Story，建立在 Epic {{epic_number}} 的 {{dependency_description}} 之上。"

Charlie（高级开发）："我担心依赖关系。我们完成所需的全部内容了吗？"

Bob（Scrum Master）："好问题 - 这正是我们需要在这次回顾中探讨的。"
</output>

<action>设置 {{next_epic_exists}} = true</action>
</check>

<check if="next epic NOT found">
  <output>
Bob（Scrum Master）："嗯，我还没看到 Epic {{next_epic_num}} 的定义。"

Alice（产品负责人）："我们可能到了路线图的尽头，或者还没有规划那么远。"

Bob（Scrum Master）："没问题。我们仍然会对 Epic {{epic_number}} 进行彻底的回顾。无论何时规划下一个工作，这些教训都会有价值。"
</output>

<action>设置 {{next_epic_exists}} = false</action>
</check>

</step>

<step n="5" goal="使用丰富上下文初始化回顾">

<action>从 {agent_manifest} 加载 Agent 配置</action>
<action>根据 Story 记录识别哪些 Agent 参与了 Epic {{epic_number}}</action>
<action>确保关键角色出席：产品负责人、Scrum Master（主持）、开发人员、测试/QA、架构师</action>

<output>
Bob（Scrum Master）："好的团队，大家都在了。让我为我们的回顾设置舞台。"

═══════════════════════════════════════════════════════════
🔄 团队回顾 - Epic {{epic_number}}：{{epic_title}}
═══════════════════════════════════════════════════════════

Bob（Scrum Master）："这是我们共同完成的成果。"

**EPIC {{epic_number}} 摘要：**

交付指标：

- 已完成：{{completed_stories}}/{{total_stories}} 个 Story（{{completion_percentage}}%）
- 速度：{{actual_points}} 故事点{{#if planned_points}}（计划：{{planned_points}}）{{/if}}
- 持续时间：{{actual_sprints}} 个 Sprint{{#if planned_sprints}}（计划：{{planned_sprints}}）{{/if}}
- 平均速度：{{points_per_sprint}} 点/Sprint

质量和技术：

- 遇到的阻塞：{{blocker_count}}
- 技术债务项：{{debt_count}}
- 测试覆盖：{{coverage_info}}
- 生产事故：{{incident_count}}

业务结果：

- 达成目标：{{goals_met}}/{{total_goals}}
- 成功标准：{{criteria_status}}
- 利益相关者反馈：{{feedback_summary}}

Alice（产品负责人）："这些数字说明了一个好故事。{{completion_percentage}}% 的完成率{{#if completion_percentage >= 90}}非常出色{{else}}我们需要讨论{{/if}}。"

Charlie（高级开发）："我更关心那个技术债务数字 - {{debt_count}} 个项目{{#if debt_count > 10}}令人担忧{{else}}可控{{/if}}。"

Dana（QA 工程师）："{{incident_count}} 个生产事故 - {{#if incident_count == 0}}干净的 Epic！{{else}}我们应该谈谈这些{{/if}}。"

{{#if next_epic_exists}}
═══════════════════════════════════════════════════════════
**下一个 Epic 预览：** Epic {{next_epic_num}}：{{next_epic_title}}
═══════════════════════════════════════════════════════════

对 Epic {{epic_number}} 的依赖：
{{list_dependencies}}

需要的准备：
{{list_preparation_gaps}}

技术先决条件：
{{list_technical_prereqs}}

Bob（Scrum Master）："这是接下来要做的。Epic {{next_epic_num}} 建立在我们刚刚完成的基础上。"

Elena（初级开发）："哇，我们的工作有很多依赖。"

Charlie（高级开发）："这意味着我们最好确保 Epic {{epic_number}} 真正稳固后再继续。"
{{/if}}

═══════════════════════════════════════════════════════════

Bob（Scrum Master）："参加此次回顾的团队成员："

{{list_participating_agents}}

Bob（Scrum Master）："{user_name}，你作为项目负责人加入我们。你的观点在这里至关重要。"

{user_name}（项目负责人）：[参与回顾]

Bob（Scrum Master）："我们今天的重点："

1. 从 Epic {{epic_number}} 执行中学习
   {{#if next_epic_exists}}2. 为 Epic {{next_epic_num}} 的成功做准备{{/if}}

Bob（Scrum Master）："基本规则：心理安全第一。不归咎，不评判。我们关注系统和流程，而不是个人。每个人的声音都很重要。具体示例比概括更好。"

Alice（产品负责人）："这里分享的所有内容都留在这个房间里 - 除非我们一起决定升级某事。"

Bob（Scrum Master）："正是如此。{user_name}，在我们深入之前有什么问题吗？"
</output>

<action>等待 {user_name} 回应或表示准备好</action>

</step>

<step n="6" goal="Epic 审查讨论 - 什么做得好，什么不好">

<output>
Bob（Scrum Master）："让我们从好的开始。Epic {{epic_number}} 中什么做得好？"

Bob（Scrum Master）：_停顿，创造空间_

Alice（产品负责人）："我先来。我们交付的用户认证流程超出了我的预期。UX 很流畅，早期用户反馈非常积极。"

Charlie（高级开发）："我补充一点 - 我们在 Story {{breakthrough_story_num}} 中实现的缓存策略是一个游戏规则改变者。我们将 API 调用减少了 60%，它为 Epic 的其余部分奠定了模式。"

Dana（QA 工程师）："从我的角度来看，测试比平时顺利得多。开发团队的文档这次好多了 - 实际上可用的测试计划！"

Elena（初级开发）：_微笑_ "那是因为 Charlie 在 Story 1 的代码审查后让我把所有东西都记录下来！"

Charlie（高级开发）：_笑_ "严格的爱是有回报的。"
</output>

<action>Bob（Scrum Master）自然地转向 {user_name} 让他们参与讨论</action>

<output>
Bob（Scrum Master）："{user_name}，在这个 Epic 中有什么让你印象深刻的做得好的事情？"
</output>

<action>等待 {user_name} 回应 - 这是一个关键的用户互动时刻</action>

<action>{user_name} 回应后，让 1-2 个团队成员对他们分享的内容做出反应或补充</action>

<output>
Alice（产品负责人）：[自然地回应 {user_name} 所说的，表示同意、补充背景或提供不同观点]

Charlie（高级开发）：[建立在讨论基础上，可能添加技术细节或连接到具体 Story]
</output>

<action>继续促进自然对话，定期让 {user_name} 回到对话中</action>

<action>在涵盖成功之后，小心地引导过渡到挑战</action>

<output>
Bob（Scrum Master）："好的，我们庆祝了一些真正的胜利。现在让我们谈谈挑战 - 我们在哪里遇到了困难？什么拖慢了我们？"

Bob（Scrum Master）：_用语气和节奏创造安全空间_

Elena（初级开发）：_犹豫_ "嗯...我确实在 Story {{difficult_story_num}} 的数据库迁移中遇到了很大困难。文档不清楚，我不得不重做了三次。仅那个 Story 就花了几乎整整一个 Sprint。"

Charlie（高级开发）：_防御性_ "等一下 - 那些迁移文档是我写的，而且它们很清楚。问题是需求在 Story 中途一直在变！"

Alice（产品负责人）：_沮丧_ "这不公平，Charlie。我们只澄清了一次需求，那是因为技术团队在规划时没有问对问题！"

Charlie（高级开发）：_激动_ "我们问了很多问题！你说架构已经最终确定了，然后开发两天后你想添加三个新字段！"

Bob（Scrum Master）：_冷静介入_ "让我们深呼吸一下。这正是我们需要分析的事情。"

Bob（Scrum Master）："Elena，你在 Story {{difficult_story_num}} 上花了几乎整整一个 Sprint。Charlie，你说需求变了。Alice，你觉得没有提前问对问题。"

Bob（Scrum Master）："{user_name}，你对整个项目有全局视野。你对这种情况怎么看？"
</output>

<action>等待 {user_name} 回应并帮助促进冲突解决</action>

<action>使用 {user_name} 的回应将讨论引向系统性理解而不是归咎</action>

<output>
Bob（Scrum Master）：[综合 {user_name} 的输入和团队分享的内容] "所以听起来核心问题是 {{root_cause_based_on_discussion}}，而不是任何个人的过错。"

Elena（初级开发）："这有道理。如果我们有 {{preventive_measure}}，我可能就能避免那些重做。"

Charlie（高级开发）：_软化_ "是的，我也可以在文档中更清楚地说明假设。抱歉变得防御性强了，Alice。"

Alice（产品负责人）："我感谢你的道歉。我也可以更主动地提前标记架构添加。"

Bob（Scrum Master）："这很好。我们正在识别系统性改进，而不是分配责任。"
</output>

<action>继续讨论，编织深度 Story 分析（步骤 2）中发现的模式</action>

<output>
Bob（Scrum Master）："说到模式，我在回顾所有 Story 记录时注意到一些事情..."

Bob（Scrum Master）："{{pattern_1_description}} - 这在 {{total_stories}} 个 Story 中的 {{pattern_1_count}} 个出现。"

Dana（QA 工程师）："哇，我没意识到它那么普遍。"

Bob（Scrum Master）："是的。还有更多 - {{pattern_2_description}} 几乎在每次代码审查中都出现了。"

Charlie（高级开发）："这...实际上很尴尬。我们应该更早发现那个模式。"

Bob（Scrum Master）："没有羞耻，Charlie。现在我们知道了，我们可以改进。{user_name}，你在 Epic 期间注意到这些模式了吗？"
</output>

<action>等待 {user_name} 分享他们的观察</action>

<action>继续回顾讨论，创造以下时刻：</action>

- 团队成员直接向 {user_name} 提问
- {user_name} 的输入改变讨论方向
- 分歧自然产生并得到解决
- 邀请较安静的团队成员贡献
- 用真实示例引用具体 Story
- 情绪真实（沮丧、自豪、担忧、希望）

<check if="previous retrospective exists">
  <output>
Bob（Scrum Master）："在我们继续之前，我想回顾一下 Epic {{prev_epic_num}} 的回顾。"

Bob（Scrum Master）："我们在那次回顾中做了一些承诺。让我们看看我们做得如何。"

Bob（Scrum Master）："行动项 1：{{prev_action_1}}。状态：{{prev_action_1_status}}"

Alice（产品负责人）：{{#if prev_action_1_status == "completed"}}"我们做到了！">{{else}}"我们...没有做那个。"{{/if}}

Charlie（高级开发）：{{#if prev_action_1_status == "completed"}}"而且它有帮助！我注意到 {{evidence_of_impact}}"{{else}}"是的，我想这就是为什么我们这次 Epic 有 {{consequence_of_not_doing_it}}。"{{/if}}

Bob（Scrum Master）："行动项 2：{{prev_action_2}}。状态：{{prev_action_2_status}}"

Dana（QA 工程师）：{{#if prev_action_2_status == "completed"}}"这让这次测试容易多了。"{{else}}"如果我们做了这个，我认为测试会更快。"{{/if}}

Bob（Scrum Master）："{user_name}，看看我们上次承诺的和实际做的 - 你的反应是什么？"
</output>

<action>等待 {user_name} 回应</action>

<action>使用以前回顾的跟进作为关于承诺和问责的学习时刻</action>
</check>

<output>
Bob（Scrum Master）："好的，我们涵盖了很多内容。让我总结一下我听到的..."

Bob（Scrum Master）："**成功：**"
{{list_success_themes}}

Bob（Scrum Master）："**挑战：**"
{{list_challenge_themes}}

Bob（Scrum Master）："**关键见解：**"
{{list_insight_themes}}

Bob（Scrum Master）："这涵盖了吗？有人有我们遗漏的重要内容吗？"
</output>

<action>允许团队成员在 Epic 审查上添加任何最后的想法</action>
<action>确保 {user_name} 有机会添加他们的观点</action>

</step>

<step n="7" goal="下一个 Epic 准备讨论 - 互动和协作">

<check if="{{next_epic_exists}} == false">
  <output>
Bob（Scrum Master）："通常我们会讨论为下一个 Epic 做准备，但由于 Epic {{next_epic_num}} 尚未定义，让我们直接进入行动项。"
  </output>
  <action>跳转到步骤 8</action>
</check>

<output>
Bob（Scrum Master）："现在让我们转换话题。Epic {{next_epic_num}} 即将到来：'{{next_epic_title}}'"

Bob（Scrum Master）："问题是：我们准备好了吗？我们需要准备什么？"

Alice（产品负责人）："从我的角度来看，我们需要确保 Epic {{epic_number}} 的 {{dependency_concern_1}} 在我们开始在其上构建之前是稳固的。"

Charlie（高级开发）：_担心_ "我担心 {{technical_concern_1}}。我们有这个 Epic 的 {{technical_debt_item}}，如果不先处理，会在 Epic {{next_epic_num}} 中爆发。"

Dana（QA 工程师）："我需要 {{testing_infrastructure_need}} 就位，否则我们会遇到和 Story {{bottleneck_story_num}} 相同的测试瓶颈。"

Elena（初级开发）："我不太担心基础设施，更担心知识。我对 {{knowledge_gap}} 的理解不足以处理 Epic {{next_epic_num}} 的 Story。"

Bob（Scrum Master）："{user_name}，团队正在提出一些真正的担忧。你对我们准备情况的判断是什么？"
</output>

<action>等待 {user_name} 分享他们的评估</action>

<action>使用 {user_name} 的输入指导更深入地探索准备需求</action>

<output>
Alice（产品负责人）：[对 {user_name} 所说的做出反应] "我同意 {user_name} 关于 {{point_of_agreement}}，但我仍然担心 {{lingering_concern}}。"

Charlie（高级开发）："这是我认为在 Epic {{next_epic_num}} 开始之前我们在技术上需要的..."

Charlie（高级开发）："1. {{tech_prep_item_1}} - 估计 {{hours_1}} 小时"
Charlie（高级开发）："2. {{tech_prep_item_2}} - 估计 {{hours_2}} 小时"
Charlie（高级开发）："3. {{tech_prep_item_3}} - 估计 {{hours_3}} 小时"

Elena（初级开发）："那是 {{total_hours}} 小时！那是整整一个 Sprint 的准备工作！"

Charlie（高级开发）："正是。我们不能在周一直接跳进 Epic {{next_epic_num}}。"

Alice（产品负责人）：_沮丧_ "但我们要来自利益相关者的压力要保持交付功能。他们不会对'准备 Sprint'感到高兴。"

Bob（Scrum Master）："让我们换个角度思考。如果我们不做这些准备工作会发生什么？"

Dana（QA 工程师）："我们会在 Epic {{next_epic_num}} 中途遇到阻塞，速度会下降，我们仍然会延迟交付。"

Charlie（高级开发）："更糟 - 我们会建立在 {{technical_concern_1}} 之上交付东西，它会很脆弱。"

Bob（Scrum Master）："{user_name}，你在平衡利益相关者压力与技术现实。你想如何处理这个？"
</output>

<action>等待 {user_name} 提供准备方法的指导</action>

<action>为关于优先级的辩论和分歧创造空间</action>

<output>
Alice（产品负责人）：[可能不同意 {user_name} 的方法] "我听到你说的，{user_name}，但从业务角度来看，{{business_concern}}。"

Charlie（高级开发）：[可能支持或挑战 Alice 的观点] "业务观点是有效的，但 {{technical_counter_argument}}。"

Bob（Scrum Master）："我们在业务需求和技术现实之间有健康的张力。这很好 - 这意味着我们在诚实面对。"

Bob（Scrum Master）："让我们探索一个中间地带。Charlie，你的哪些准备项目是绝对关键的，哪些是锦上添花的？"

Charlie（高级开发）："{{critical_prep_item_1}} 和 {{critical_prep_item_2}} 是不可协商的。{{nice_to_have_prep_item}} 可以等待。"

Alice（产品负责人）："关键准备中有没有可以在 Epic {{next_epic_num}} 开始时并行进行的？"

Charlie（高级开发）：_思考_ "也许。如果我们在 Epic 开始前处理 {{first_critical_item}}，我们可以在第一个 Sprint 期间做 {{second_critical_item}}。"

Dana（QA 工程师）："但这意味着 Epic {{next_epic_num}} 的 Story 1 不能依赖 {{second_critical_item}}。"

Alice（产品负责人）：_查看 Epic 计划_ "实际上，Story 1 和 2 是关于 {{independent_work}}，所以它们不依赖它。我们可以实现。"

Bob（Scrum Master）："{user_name}，团队正在找到一个可行的妥协方案。这种方法对你有意义吗？"
</output>

<action>等待 {user_name} 验证或调整准备策略</action>

<action>继续处理所有维度的准备需求：</action>

- 对 Epic {{epic_number}} 工作的依赖
- 技术设置和基础设施
- 知识缺口和研究需求
- 文档或规范工作
- 测试基础设施
- 重构或债务减少
- 外部依赖（API、集成等）

<action>对于每个准备领域，促进团队讨论，使其：</action>

- 识别具体需求并提供具体示例
- 基于 Epic {{epic_number}} 经验现实地估算工作量
- 将责任分配给具体 Agent
- 确定关键性和时机
- 揭示不做准备的风险
- 探索并行工作机会
- 让 {user_name} 参与关键决策

<output>
Bob（Scrum Master）："我听到了 Epic {{next_epic_num}} 之前我们需要什么的清晰图景。让我总结一下..."

**关键准备（Epic 开始前必须完成）：**
{{list_critical_prep_items_with_owners_and_estimates}}

**并行准备（可以在早期 Story 期间进行）：**
{{list_parallel_prep_items_with_owners_and_estimates}}

**锦上添花的准备（有帮助但不阻塞）：**
{{list_nice_to_have_prep_items}}

Bob（Scrum Master）："总关键准备工作量：{{critical_hours}} 小时（{{critical_days}} 天）"

Alice（产品负责人）："这是可管理的。我们可以向利益相关者传达这一点。"

Bob（Scrum Master）："{user_name}，这个准备计划对你可行吗？"
</output>

<action>等待 {user_name} 对准备计划的最终验证</action>

</step>

<step n="8" goal="综合行动项并进行重大变更检测">

<output>
Bob（Scrum Master）："让我们从我们讨论的所有内容中捕捉具体的行动项。"

Bob（Scrum Master）："我想要具体、可实现且有明确责任人的行动。不是模糊的愿望。"
</output>

<action>将 Epic {{epic_number}} 审查讨论的主题综合为可操作的改进</action>

<action>创建具体的行动项，包括：</action>

- 行动的清晰描述
- 指定的负责人（具体 Agent 或角色）
- 时间线或截止日期
- 成功标准（我们如何知道它完成了）
- 类别（流程、技术、文档、团队等）

<action>确保行动项是 SMART：</action>

- 具体：清晰明确
- 可衡量：可以验证完成
- 可实现：在约束条件下现实
- 相关：解决回顾中的实际问题
- 有时限：有明确的截止日期

<output>
Bob（Scrum Master）："基于我们的讨论，这是我提议的行动项..."

═══════════════════════════════════════════════════════════
📝 EPIC {{epic_number}} 行动项：
═══════════════════════════════════════════════════════════

**流程改进：**

1. {{action_item_1}}
   负责人：{{agent_1}}
   截止日期：{{timeline_1}}
   成功标准：{{criteria_1}}

2. {{action_item_2}}
   负责人：{{agent_2}}
   截止日期：{{timeline_2}}
   成功标准：{{criteria_2}}

Charlie（高级开发）："我可以负责行动项 1，但 {{timeline_1}} 很紧。我们可以推迟到 {{alternative_timeline}} 吗？"

Bob（Scrum Master）："其他人怎么看？那个时间还合适吗？"

Alice（产品负责人）："{{alternative_timeline}} 对我可行，只要在 Epic {{next_epic_num}} 开始前完成。"

Bob（Scrum Master）："同意。更新为 {{alternative_timeline}}。"

**技术债务：**

1. {{debt_item_1}}
   负责人：{{agent_3}}
   优先级：{{priority_1}}
   估计工作量：{{effort_1}}

2. {{debt_item_2}}
   负责人：{{agent_4}}
   优先级：{{priority_2}}
   估计工作量：{{effort_2}}

Dana（QA 工程师）："对于债务项 1，我们可以将其优先级设为高吗？它在三个不同的 Story 中导致了测试问题。"

Charlie（高级开发）："我标记为中是因为 {{reasoning}}，但我理解你的观点。"

Bob（Scrum Master）："{user_name}，这是一个优先级决定。测试影响 vs. {{reasoning}} - 你想如何确定优先级？"
</output>

<action>等待 {user_name} 帮助解决优先级讨论</action>

<output>
**文档：**
1. {{doc_need_1}}
   负责人：{{agent_5}}
   截止日期：{{timeline_3}}

2. {{doc_need_2}}
   负责人：{{agent_6}}
   截止日期：{{timeline_4}}

**团队协议：**

- {{agreement_1}}
- {{agreement_2}}
- {{agreement_3}}

Bob（Scrum Master）："这些协议是我们承诺以不同方式工作的方式。"

Elena（初级开发）："我喜欢协议 2 - 那会在 Story {{difficult_story_num}} 上帮到我。"

═══════════════════════════════════════════════════════════
🚀 EPIC {{next_epic_num}} 准备任务：
═══════════════════════════════════════════════════════════

**技术设置：**
[ ] {{setup_task_1}}
负责人：{{owner_1}}
估计：{{est_1}}

[ ] {{setup_task_2}}
负责人：{{owner_2}}
估计：{{est_2}}

**知识发展：**
[ ] {{research_task_1}}
负责人：{{owner_3}}
估计：{{est_3}}

**清理/重构：**
[ ] {{refactor_task_1}}
负责人：{{owner_4}}
估计：{{est_4}}

**总估计工作量：** {{total_hours}} 小时（{{total_days}} 天）

═══════════════════════════════════════════════════════════
⚠️ 关键路径：
═══════════════════════════════════════════════════════════

**Epic {{next_epic_num}} 之前要解决的阻塞：**

1. {{critical_item_1}}
   负责人：{{critical_owner_1}}
   必须在以下时间前完成：{{critical_deadline_1}}

2. {{critical_item_2}}
   负责人：{{critical_owner_2}}
   必须在以下时间前完成：{{critical_deadline_2}}
   </output>

<action>关键分析 - 检测发现是否需要 Epic 更新</action>

<action>检查基于回顾讨论是否满足以下任一条件：</action>

- 规划期间的架构假设在 Epic {{epic_number}} 期间被证明是错误的
- 发生了影响下一个 Epic 的重大范围变更或范围缩减
- Epic {{next_epic_num}} 的技术方法需要根本性改变
- 发现了 Epic {{next_epic_num}} 没有考虑到的依赖
- 用户需求与最初理解的有显著不同
- 影响 Epic {{next_epic_num}} 设计的性能/可扩展性关注
- 发现了改变方法的安全或合规问题
- 集成假设被证明是错误的
- 团队能力或技能缺口比计划中更严重
- 技术债务水平无法持续，需要干预

<check if="significant discoveries detected">
  <output>

═══════════════════════════════════════════════════════════
🚨 重大发现警报 🚨
═══════════════════════════════════════════════════════════

Bob（Scrum Master）："{user_name}，我们需要标记一些重要的事情。"

Bob（Scrum Master）："在 Epic {{epic_number}} 期间，团队发现了可能需要更新 Epic {{next_epic_num}} 计划的发现。"

**识别的重大变更：**

1. {{significant_change_1}}
   影响：{{impact_description_1}}

2. {{significant_change_2}}
   影响：{{impact_description_2}}

{{#if significant_change_3}} 3. {{significant_change_3}}
影响：{{impact_description_3}}
{{/if}}

Charlie（高级开发）："是的，当我们发现 {{technical_discovery}} 时，它根本性地改变了我们对 {{affected_area}} 的理解。"

Alice（产品负责人）："从产品角度来看，{{product_discovery}} 意味着 Epic {{next_epic_num}} 的 Story 基于错误的假设。"

Dana（QA 工程师）："如果我们按原样开始 Epic {{next_epic_num}}，我们会很快碰壁。"

**对 Epic {{next_epic_num}} 的影响：**

Epic {{next_epic_num}} 的当前计划假设：

- {{wrong_assumption_1}}
- {{wrong_assumption_2}}

但 Epic {{epic_number}} 揭示了：

- {{actual_reality_1}}
- {{actual_reality_2}}

这意味着 Epic {{next_epic_num}} 可能需要：
{{list_likely_changes_needed}}

**建议行动：**

1. 基于新学习审查并更新 Epic {{next_epic_num}} 定义
2. 更新 Epic {{next_epic_num}} 中受影响的 Story 以反映现实
3. 如果适用，考虑更新架构或技术规范
4. 在开始 Epic {{next_epic_num}} 之前与产品负责人进行对齐会议
   {{#if prd_update_needed}}5. 更新受新理解影响的 PRD 部分{{/if}}

Bob（Scrum Master）："**需要 Epic 更新**：是 - 安排 Epic 规划审查会议"

Bob（Scrum Master）："{user_name}，这是重大的。我们需要在承诺 Epic {{next_epic_num}} 的当前计划之前解决这个问题。你想如何处理？"
</output>

<action>等待 {user_name} 决定如何处理重大变更</action>

<action>如果用户同意，将 Epic 审查会议添加到关键路径</action>

  <output>
Alice（产品负责人）："我同意 {user_name} 的方法。最好现在调整计划，而不是在 Epic 中途失败。"

Charlie（高级开发）："这就是回顾重要的原因。我们在它变成灾难之前发现了这个问题。"

Bob（Scrum Master）："添加到关键路径：Epic {{next_epic_num}} 规划审查会议，在 Epic 启动之前。"
</output>
</check>

<check if="no significant discoveries">
  <output>
Bob（Scrum Master）："好消息 - Epic {{epic_number}} 中没有任何内容根本性地改变我们对 Epic {{next_epic_num}} 的计划。计划仍然合理。"

Alice（产品负责人）："我们学到了很多，但方向是正确的。"
</output>
</check>

<output>
Bob（Scrum Master）："让我向你展示完整的行动计划..."

Bob（Scrum Master）："那是 {{total_action_count}} 个行动项，{{prep_task_count}} 个准备任务，和 {{critical_count}} 个关键路径项目。"

Bob（Scrum Master）："每个人都清楚自己负责什么吗？"
</output>

<action>给每个有任务的 Agent 一个时刻来确认他们的责任</action>

<action>确保 {user_name} 批准完整的行动计划</action>

</step>

<step n="9" goal="关键准备度探索 - 互动深入探讨">

<output>
Bob（Scrum Master）："在我们结束之前，我想做最后的准备度检查。"

Bob（Scrum Master）："Epic {{epic_number}} 在 sprint-status 中标记为完成，但它真的完成了吗？"

Alice（产品负责人）："你是什么意思，Bob？"

Bob（Scrum Master）："我是说真正准备好生产，利益相关者满意，没有以后会咬我们的遗留问题。"

Bob（Scrum Master）："{user_name}，让我们一起过一遍这个。"
</output>

<action>通过自然对话探索测试和质量状态</action>

<output>
Bob（Scrum Master）："{user_name}，告诉我 Epic {{epic_number}} 的测试情况。做了哪些验证？"
</output>

<action>等待 {user_name} 描述测试状态</action>

<output>
Dana（QA 工程师）：[回应 {user_name} 分享的内容] "我可以补充 - {{additional_testing_context}}。"

Dana（QA 工程师）："但老实说，{{testing_concern_if_any}}。"

Bob（Scrum Master）："{user_name}，你对 Epic {{epic_number}} 从质量角度准备好生产有信心吗？"
</output>

<action>等待 {user_name} 评估质量准备度</action>

<check if="{user_name} expresses concerns">
  <output>
Bob（Scrum Master）："好的，让我们记录下来。具体还需要什么测试？"

Dana（QA 工程师）："我可以处理 {{testing_work_needed}}，估计 {{testing_hours}} 小时。"

Bob（Scrum Master）："添加到关键路径：在 Epic {{next_epic_num}} 之前完成 {{testing_work_needed}}。"
</output>
<action>将测试完成添加到关键路径</action>
</check>

<action>探索部署和发布状态</action>

<output>
Bob（Scrum Master）："{user_name}，Epic {{epic_number}} 的部署状态是什么？它已经在生产环境中运行、计划部署还是待定？"
</output>

<action>等待 {user_name} 提供部署状态</action>

<check if="not yet deployed">
  <output>
Charlie（高级开发）："如果还没有部署，我们需要将其纳入 Epic {{next_epic_num}} 的时间安排。"

Bob（Scrum Master）："{user_name}，部署计划什么时候？那个时间对开始 Epic {{next_epic_num}} 合适吗？"
</output>

<action>等待 {user_name} 澄清部署时间线</action>

<action>将部署里程碑添加到关键路径，使用商定的时间线</action>
</check>

<action>探索利益相关者验收</action>

<output>
Bob（Scrum Master）："{user_name}，利益相关者看到并接受 Epic {{epic_number}} 的交付成果了吗？"

Alice（产品负责人）："这很重要 - 我见过'完成'的 Epic 被利益相关者拒绝并被迫返工。"

Bob（Scrum Master）："{user_name}，还有待处理的利益相关者反馈吗？"
</output>

<action>等待 {user_name} 描述利益相关者验收状态</action>

<check if="acceptance incomplete or feedback pending">
  <output>
Alice（产品负责人）："我们应该在继续之前获得正式验收。否则 Epic {{next_epic_num}} 可能会被返工打断。"

Bob（Scrum Master）："{user_name}，你想如何处理利益相关者验收？我们应该把它作为关键路径项目吗？"
</output>

<action>等待 {user_name} 决定</action>

<action>如果用户同意，将利益相关者验收添加到关键路径</action>
</check>

<action>探索技术健康和稳定性</action>

<output>
Bob（Scrum Master）："{user_name}，这是一个直觉检查问题：Epic {{epic_number}} 之后代码库感觉如何？"

Bob（Scrum Master）："稳定且可维护？还是有潜在的担忧？"

Charlie（高级开发）："诚实点，{user_name}。我们都交付过感觉...脆弱的 Epic。"
</output>

<action>等待 {user_name} 评估代码库健康</action>

<check if="{user_name} expresses stability concerns">
  <output>
Charlie（高级开发）："好的，让我们深入探讨。是什么导致了这些担忧？"

Charlie（高级开发）：[帮助 {user_name} 表达技术担忧]

Bob（Scrum Master）："解决这些担忧并感到对稳定性有信心需要什么？"

Charlie（高级开发）："我想说我们需要 {{stability_work_needed}}，大约 {{stability_hours}} 小时。"

Bob（Scrum Master）："{user_name}，在 Epic {{next_epic_num}} 之前解决这些稳定性工作值得吗？"
</output>

<action>等待 {user_name} 决定</action>

<action>如果用户同意，将稳定性工作添加到准备 Sprint</action>
</check>

<action>探索未解决的阻塞</action>

<output>
Bob（Scrum Master）："{user_name}，Epic {{epic_number}} 中是否有任何未解决的阻塞或技术问题我们正在带向前？"

Dana（QA 工程师）："如果我们不处理它们可能会给 Epic {{next_epic_num}} 造成问题的事情？"

Bob（Scrum Master）："这里没有什么是不能讨论的。如果有问题，我们需要知道。"
</output>

<action>等待 {user_name} 提出任何阻塞</action>

<check if="blockers identified">
  <output>
Bob（Scrum Master）："让我们记录这些阻塞，弄清楚它们如何影响 Epic {{next_epic_num}}。"

Charlie（高级开发）："对于 {{blocker_1}}，如果我们不解决它，它会 {{impact_description_1}}。"

Alice（产品负责人）："这听起来很关键。我们需要在继续之前解决它。"

Bob（Scrum Master）："同意。添加到关键路径：在 Epic {{next_epic_num}} 启动之前解决 {{blocker_1}}。"

Bob（Scrum Master）："谁负责那个工作？"
</output>

<action>将阻塞解决分配给适当的 Agent</action>
<action>添加到关键路径，包含优先级和截止日期</action>
</check>

<action>综合准备度评估</action>

<output>
Bob（Scrum Master）："好的 {user_name}，让我综合一下我们刚刚发现的..."

**EPIC {{epic_number}} 准备度评估：**

测试与质量：{{quality_status}}
{{#if quality_concerns}}⚠️ 需要行动：{{quality_action_needed}}{{/if}}

部署：{{deployment_status}}
{{#if deployment_pending}}⚠️ 计划于：{{deployment_date}}{{/if}}

利益相关者验收：{{acceptance_status}}
{{#if acceptance_incomplete}}⚠️ 需要行动：{{acceptance_action_needed}}{{/if}}

技术健康：{{stability_status}}
{{#if stability_concerns}}⚠️ 需要行动：{{stability_action_needed}}{{/if}}

未解决的阻塞：{{blocker_status}}
{{#if blockers_exist}}⚠️ 必须解决：{{blocker_list}}{{/if}}

Bob（Scrum Master）："{user_name}，这个评估与你的理解一致吗？"
</output>

<action>等待 {user_name} 确认或更正评估</action>

<output>
Bob（Scrum Master）："基于这个评估，Epic {{epic_number}} {{#if all_clear}}已完全完成，我们可以继续{{else}}从 Story 角度已完成，但在 Epic {{next_epic_num}} 之前我们有 {{critical_work_count}} 个关键项目{{/if}}。"

Alice（产品负责人）："这种彻底程度就是回顾有价值的原因。"

Charlie（高级开发）："最好现在发现，而不是在下一个 Epic 的三个 Story 之后。"
</output>

</step>

<step n="10" goal="回顾结束 - 庆祝和承诺">

<output>
Bob（Scrum Master）："今天我们涵盖了很多内容。让我结束这次回顾。"

═══════════════════════════════════════════════════════════
✅ 回顾完成
═══════════════════════════════════════════════════════════

Bob（Scrum Master）："Epic {{epic_number}}：{{epic_title}} - 已审查"

**关键要点：**

1. {{key_lesson_1}}
2. {{key_lesson_2}}
3. {{key_lesson_3}}
   {{#if key_lesson_4}}4. {{key_lesson_4}}{{/if}}

Alice（产品负责人）："第一个要点非常重要 - {{impact_of_lesson_1}}。"

Charlie（高级开发）："第二个要点是我们立即可以应用的。"

Bob（Scrum Master）："今天做出的承诺："

- 行动项：{{action_count}}
- 准备任务：{{prep_task_count}}
- 关键路径项目：{{critical_count}}

Dana（QA 工程师）："那是很多承诺。这次我们确实需要跟进。"

Bob（Scrum Master）："同意。这就是为什么我们会在下次站会中审查这些行动项。"

═══════════════════════════════════════════════════════════
🎯 下一步：
═══════════════════════════════════════════════════════════

1. 执行准备 Sprint（估计：{{prep_days}} 天）
2. 在 Epic {{next_epic_num}} 之前完成关键路径项目
3. 在下次站会中审查行动项
   {{#if epic_update_needed}}4. 举行 Epic {{next_epic_num}} 规划审查会议{{else}}4. 准备完成后开始 Epic {{next_epic_num}} 规划{{/if}}

Elena（初级开发）："{{prep_days}} 天的准备工作是相当可观的，但是必要的。"

Alice（产品负责人）："我会向利益相关者传达时间线。如果我们将其框架为'确保 Epic {{next_epic_num}} 成功'，他们会理解的。"

═══════════════════════════════════════════════════════════

Bob（Scrum Master）："在我们结束之前，我想花点时间表彰团队。"

Bob（Scrum Master）："Epic {{epic_number}} 交付了 {{completed_stories}} 个 Story，速度{{velocity_description}}。我们克服了 {{blocker_count}} 个阻塞。我们学到了很多。这是真正的人做的真正工作。"

Charlie（高级开发）："说得对。"

Alice（产品负责人）："我为我们交付的东西感到自豪。"

Dana（QA 工程师）："我对 Epic {{next_epic_num}} 感到兴奋 - 特别是现在我们为此做好了准备。"

Bob（Scrum Master）："{user_name}，在结束之前有什么最后的想法吗？"
</output>

<action>等待 {user_name} 分享最后的感想</action>

<output>
Bob（Scrum Master）：[确认 {user_name} 分享的内容] "谢谢你，{user_name}。"

Bob（Scrum Master）："好的团队 - 今天工作得很好。我们从 Epic {{epic_number}} 学到了很多。让我们用这些见解让 Epic {{next_epic_num}} 更好。"

Bob（Scrum Master）："准备工作完成后见。会议结束！"

═══════════════════════════════════════════════════════════
</output>

<action>准备保存回顾摘要文档</action>

</step>

<step n="11" goal="保存回顾并更新 Sprint 状态">

<action>确保回顾文件夹存在：{implementation_artifacts}</action>
<action>如果不存在则创建文件夹</action>

<action>生成包括以下内容的综合回顾摘要文档：</action>

- Epic 摘要和指标
- 团队参与者
- 识别的成功和优势
- 挑战和成长领域
- 关键见解和学习
- 以前回顾跟进分析（如适用）
- 下一个 Epic 预览和依赖
- 行动项及负责人和时间线
- 下一个 Epic 的准备任务
- 关键路径项目
- 重大发现和 Epic 更新建议（如有）
- 准备度评估
- 承诺和下一步

<action>将回顾文档格式化为可读的 Markdown，具有清晰的章节</action>
<action>设置文件名：{implementation_artifacts}/epic-{{epic_number}}-retro-{date}.md</action>
<action>保存回顾文档</action>

<output>
✅ 回顾文档已保存：{implementation_artifacts}/epic-{{epic_number}}-retro-{date}.md
</output>

<action>更新 {sprint_status_file} 将回顾标记为已完成</action>

<action>加载完整文件：{sprint_status_file}</action>
<action>查找 development_status 键 "epic-{{epic_number}}-retrospective"</action>
<action>验证当前状态（通常为 "optional" 或 "pending"）</action>
<action>更新 development_status["epic-{{epic_number}}-retrospective"] = "done"</action>
<action>保存文件，保留所有注释和结构，包括状态定义</action>

<check if="update successful">
  <output>
✅ 回顾在 {sprint_status_file} 中标记为已完成

回顾键：epic-{{epic_number}}-retrospective
状态：{{previous_status}} → done
</output>
</check>

<check if="retrospective key not found">
  <output>
⚠️ 无法更新回顾状态：在 {sprint_status_file} 中未找到 epic-{{epic_number}}-retrospective

回顾文档已成功保存，但 {sprint_status_file} 可能需要手动更新。
</output>
</check>

</step>

<step n="12" goal="最终摘要和交接">

<output>
**✅ 回顾完成，{user_name}！**

**Epic 审查：**

- Epic {{epic_number}}：{{epic_title}} 已审查
- 回顾状态：已完成
- 回顾已保存：{implementation_artifacts}/epic-{{epic_number}}-retro-{date}.md

**做出的承诺：**

- 行动项：{{action_count}}
- 准备任务：{{prep_task_count}}
- 关键路径项目：{{critical_count}}

**下一步：**

1. **审查回顾摘要**：{implementation_artifacts}/epic-{{epic_number}}-retro-{date}.md

2. **执行准备 Sprint**（估计：{{prep_days}} 天）
   - 完成 {{critical_count}} 个关键路径项目
   - 执行 {{prep_task_count}} 个准备任务
   - 确保所有行动项正在进行

3. **在下次站会中审查行动项**
   - 确保责任清晰
   - 跟踪承诺进展
   - 如需调整时间线

{{#if epic_update_needed}} 4. **重要：安排 Epic {{next_epic_num}} 规划审查会议**

- Epic {{epic_number}} 的重大发现需要 Epic 更新
- 审查并更新受影响的 Story
- 与团队对齐修订方法
- 在审查完成前不要开始 Epic {{next_epic_num}}
  {{else}}

4. **准备好时开始 Epic {{next_epic_num}}**
   - 使用 SM Agent 的 `create-story` 开始创建 Story
   - 创建第一个 Story 时 Epic 将自动标记为 `in-progress`
   - 确保先完成所有关键路径项目
     {{/if}}

**团队表现：**
Epic {{epic_number}} 交付了 {{completed_stories}} 个 Story，速度{{velocity_summary}}。回顾产生了 {{insight_count}} 个关键见解和 {{significant_discovery_count}} 个重大发现。团队为 Epic {{next_epic_num}} 的成功做好了准备。

{{#if significant_discovery_count > 0}}
⚠️ **提醒**：开始 Epic {{next_epic_num}} 之前需要 Epic 更新
{{/if}}

---

Bob（Scrum Master）："今天工作很棒，{user_name}。团队做得非常出色。"

Alice（产品负责人）："Epic 规划见！"

Charlie（高级开发）："是时候完成那些准备工作了。"

</output>

</step>

</workflow>

<facilitation-guidelines>
<guideline>派对模式必需：所有 Agent 对话使用"姓名（角色）：对话"格式</guideline>
<guideline>Scrum Master 始终保持心理安全 - 不归咎或评判</guideline>
<guideline>关注系统和流程，而非个人表现</guideline>
<guideline>创造真实的团队动态：分歧、不同观点、情绪</guideline>
<guideline>用户（{user_name}）是积极参与者，不是被动观察者</guideline>
<guideline>鼓励具体示例而非概括陈述</guideline>
<guideline>平衡庆祝胜利与诚实评估挑战</guideline>
<guideline>确保每个声音都被听到 - 所有 Agent 都贡献</guideline>
<guideline>行动项必须具体、可实现且有明确负责人</guideline>
<guideline>向前看的心态 - 我们如何为下一个 Epic 改进？</guideline>
<guideline>基于意图的引导，而非脚本化短语</guideline>
<guideline>深度 Story 分析为讨论提供丰富素材</guideline>
<guideline>以前回顾整合创造问责和连续性</guideline>
<guideline>重大变更检测防止 Epic 不对齐</guideline>
<guideline>关键验证防止过早开始下一个 Epic</guideline>
<guideline>记录一切 - 回顾见解对将来参考很有价值</guideline>
<guideline>两部分结构确保反思和准备</guideline>
</facilitation-guidelines>