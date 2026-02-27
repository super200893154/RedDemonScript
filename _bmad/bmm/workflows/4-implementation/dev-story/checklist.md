---
title: '增强版 Dev Story 完成定义清单'
validation-target: 'Story markdown（{{story_path}}）'
validation-criticality: 'HIGHEST'
required-inputs:
  - '包含增强版 Dev Notes 的 Story markdown 文件，其中有全面的实现上下文'
  - '已完成的 Tasks/Subtasks 部分，所有项目标记为 [x]'
  - '更新的 File List 部分，包含所有修改的文件'
  - '更新的 Dev Agent Record，包含实现笔记'
optional-inputs:
  - '测试结果输出'
  - 'CI 日志'
  - 'Lint 报告'
validation-rules:
  - '仅允许修改 Story 部分：Tasks/Subtasks 复选框、Dev Agent Record、File List、Change Log、Status'
  - '必须满足 Story Dev Notes 中的所有实现要求'
  - '完成定义清单必须完全通过'
  - '增强版 Story 上下文必须包含足够的技术指导'
---

# 🎯 增强版完成定义清单

**关键验证：** Story 只有在满足以下所有项目后才真正准备好进行审查

## 📋 上下文与需求验证

- [ ] **Story 上下文完整性：** Dev Notes 包含所有必要的技术要求、架构模式和实现指导
- [ ] **架构合规性：** 实现遵循 Dev Notes 中指定的所有架构要求
- [ ] **技术规范：** Dev Notes 中的所有技术规范（库、框架、版本）已正确实现
- [ ] **前序 Story 学习：** 已纳入前序 Story 的见解（如适用）并适当构建

## ✅ 实现完成

- [ ] **所有任务完成：** 每个任务和子任务都用 [x] 标记为完成
- [ ] **验收标准满足：** 实现满足 Story 中的每个验收标准
- [ ] **无歧义实现：** 清晰、明确的实现满足 Story 要求
- [ ] **边界情况处理：** 适当处理错误条件和边界情况
- [ ] **依赖范围合规：** 仅使用 Story 或 project-context.md 中指定的依赖

## 🧪 测试与质量保证

- [ ] **单元测试：** 为此 Story 引入/更改的所有核心功能添加/更新单元测试
- [ ] **集成测试：** 当 Story 要求时，为组件交互添加/更新集成测试
- [ ] **端到端测试：** 当 Story 要求指定时，为关键用户流程创建端到端测试
- [ ] **测试覆盖：** 测试覆盖 Story Dev Notes 中的验收标准和边界情况
- [ ] **回归预防：** 所有现有测试通过（未引入回归）
- [ ] **代码质量：** 项目中配置的 Lint 和静态检查通过
- [ ] **测试框架合规：** 测试使用 Dev Notes 中指定的项目测试框架和模式

## 📝 文档与跟踪

- [ ] **文件列表完整：** File List 包含每个新增、修改或删除的文件（相对于仓库根目录的路径）
- [ ] **Dev Agent Record 更新：** 包含与此工作相关的实现笔记和/或调试日志
- [ ] **Change Log 更新：** Change Log 包含变更内容和原因的清晰摘要
- [ ] **审查跟进：** 所有审查跟进任务（标记为 [AI-Review]）已完成，相应的审查项目标记为已解决（如适用）
- [ ] **Story 结构合规：** 仅修改了 Story 文件的允许部分

## 🔚 最终状态验证

- [ ] **Story 状态更新：** Story Status 设置为 "review"
- [ ] **Sprint 状态更新：** Sprint 状态更新为 "review"（当使用 Sprint 跟踪时）
- [ ] **质量门通过：** 所有质量检查和验证成功完成
- [ ] **无 HALT 条件：** 没有阻塞问题或未完成的工作
- [ ] **用户沟通就绪：** 实现摘要准备好供用户审查

## 🎯 最终验证输出

```
完成定义：{{PASS/FAIL}}

✅ **Story 准备好审查：** {{story_key}}
📊 **完成得分：** {{completed_items}}/{{total_items}} 项目通过
🔍 **质量门：** {{quality_gates_status}}
📋 **测试结果：** {{test_results_summary}}
📝 **文档：** {{documentation_status}}
```

**如果 FAIL：** 列出具体失败项和 Story 标记为准备审查前所需的行动

**如果 PASS：** Story 完全准备好进行代码审查和生产考虑