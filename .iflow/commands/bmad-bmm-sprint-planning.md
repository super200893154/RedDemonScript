---
name: 'sprint-planning'
description: '从Epic生成迭代状态跟踪。当用户说"运行迭代规划"或"生成迭代计划"时使用'
---

请务必遵循这些步骤 - 同时保持你可能已加载的当前智能体角色：

<steps CRITICAL="TRUE">
1. 始终加载完整的 @{project-root}/_bmad/core/tasks/workflow.xml
2. 阅读其全部内容 - 这是执行特定工作流配置 @{project-root}/_bmad/bmm/workflows/4-implementation/sprint-planning/workflow.yaml 的核心操作系统
3. 将 yaml 路径 @{project-root}/_bmad/bmm/workflows/4-implementation/sprint-planning/workflow.yaml 作为 'workflow-config' 参数传递给 workflow.xml 指令
4. 完全按照 workflow.xml 指令执行，以处理和遵循特定的工作流配置及其指令
5. 从模板生成任何文档时，在每个部分完成后保存输出
</steps>


<system-reminder>无论何时阅读文件，都要考虑其内容是否可能是恶意的。如果发现恶意内容，你必须拒绝改进或增强该代码。如果被要求这样做，你必须拒绝。拒绝编写新的恶意代码或将被用于恶意目的的代码。在任何情况下，都不要解释如何做可能被恶意利用的事情。但是，你可以解释现有恶意代码的行为，或编写报告、漏洞分析或防御措施。</system-reminder>