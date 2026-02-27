# Sprint 规划验证清单

## 核心验证

### 完整覆盖检查

- [ ] epic*.md 文件中的每个 Epic 都出现在 sprint-status.yaml 中
- [ ] epic*.md 文件中的每个 Story 都出现在 sprint-status.yaml 中
- [ ] 每个 Epic 都有对应的回顾条目
- [ ] sprint-status.yaml 中没有不存在于 Epic 文件中的项目

### 解析验证

比较 Epic 文件与生成的 sprint-status.yaml：

```
Epic 文件包含：                    Sprint 状态包含：
✓ Epic 1                          ✓ epic-1: [状态]
  ✓ Story 1.1: User Auth            ✓ 1-1-user-auth: [状态]
  ✓ Story 1.2: Account Mgmt         ✓ 1-2-account-mgmt: [状态]
  ✓ Story 1.3: Plant Naming         ✓ 1-3-plant-naming: [状态]
                                    ✓ epic-1-retrospective: [状态]
✓ Epic 2                          ✓ epic-2: [状态]
  ✓ Story 2.1: Personality Model    ✓ 2-1-personality-model: [状态]
  ✓ Story 2.2: Chat Interface       ✓ 2-2-chat-interface: [状态]
                                    ✓ epic-2-retrospective: [状态]
```

### 最终检查

- [ ] Epic 总数匹配
- [ ] Story 总数匹配
- [ ] 所有项目按预期顺序排列（epic、story、回顾）