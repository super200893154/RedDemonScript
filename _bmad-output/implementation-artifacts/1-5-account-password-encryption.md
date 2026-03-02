# Story 1.5: 账号密码加密

Status: ready-for-dev

<!-- 注意：验证是可选的。在 dev-story 之前运行 validate-create-story 进行质量检查。 -->

## Story

作为一个 **用户**，
我想要 **账号密码被安全加密存储**，
以便 **我的游戏账号信息安全**。

## 验收标准

1. **假设** 配置管理器已实现（Story 1.4 完成）
2. **当** 用户保存账号信息
3. **那么** 密码使用 Windows DPAPI 加密
4. **当** 用户读取账号信息
5. **那么** 密码自动解密显示
6. **并且** 加密后的密码存储在配置文件中
7. **并且** 只有同一 Windows 用户账户可以解密密码

**覆盖的 FR：** 安全相关（架构需求）

**架构来源：** architecture.md#认证与安全 - 账号密码加密使用 Windows DPAPI

## 任务 / 子任务

- [ ] 任务 1：创建 DpapiHelper 加密工具类（AC：#2, #3）
  - [ ] 1.1 创建 DpapiHelper 类（src/helpers/DpapiHelper.h/cpp）
  - [ ] 1.2 实现 encrypt() 静态方法
  - [ ] 1.3 实现 decrypt() 静态方法
  - [ ] 1.4 添加错误处理和日志记录

- [ ] 任务 2：修改 ConfigManager 集成密码加密（AC：#2, #3, #4, #5）
  - [ ] 2.1 修改 addAccount() 方法，保存前加密密码
  - [ ] 2.2 修改 updateAccount() 方法，保存前加密密码
  - [ ] 2.3 修改 accounts() 方法，返回时解密密码
  - [ ] 2.4 添加密码字段检测逻辑（判断是否已加密）

- [ ] 任务 3：更新 CMakeLists.txt 添加依赖（AC：#2, #3）
  - [ ] 3.1 添加 Crypt32 库链接依赖
  - [ ] 3.2 添加 Windows 平台条件编译

- [ ] 任务 4：处理边界情况（AC：全部）
  - [ ] 4.1 处理加密/解密失败的情况
  - [ ] 4.2 处理配置文件迁移（未加密→加密）
  - [ ] 4.3 记录加密操作日志

- [ ] 任务 5：测试验证（AC：全部）
  - [ ] 5.1 验证密码加密后存储到配置文件
  - [ ] 5.2 验证读取时密码自动解密
  - [ ] 5.3 验证不同 Windows 用户无法解密
  - [ ] 5.4 验证加密失败时的错误处理

## Dev Notes

### 核心需求分析

**架构决策来源：architecture.md#认证与安全**

| 决策项 | 决策 | 详情 | 理由 |
|--------|------|-----------|------|
| 账号密码加密 | Windows DPAPI | 系统级加密 API | 仅 Windows 平台，安全性高，无需密钥管理 |
| 配置文件权限 | 用户目录存储 | Windows 用户权限 | 敏感信息保护 |

**DPAPI 特性：**
- 通常只有相同登录凭据的用户才能解密数据
- 加密和解密通常需要在同一台计算机上进行
- 使用漫游配置文件的用户可以从网络上的另一台计算机解密数据
- 无需管理加密密钥，由 Windows 系统自动管理

### 架构约束

**来源：architecture.md#项目结构与边界**

```
src/helpers/        # API 封装
├── imagehelper.h/cpp
├── inputhelper.h/cpp
└── dpapihelper.h/cpp   # 新增：DPAPI 加密封装
```

**来源：project-context.md#关键必知规则**

- 账号密码加密存储，不明文保存
- 敏感操作需记录日志审计
- 配置文件权限限制

### 项目结构说明

**新增文件位置：**
- 头文件：`src/helpers/DpapiHelper.h`
- 源文件：`src/helpers/DpapiHelper.cpp`

**修改文件：**
- `src/core/ConfigManager.h` - 添加加密/解密相关方法声明
- `src/core/ConfigManager.cpp` - 集成密码加密逻辑
- `CMakeLists.txt` - 添加 Crypt32 库依赖

### 技术要点

#### 1. CMake 配置 - 添加 Crypt32 库依赖

```cmake
# CMakeLists.txt
if(WIN32)
    target_link_libraries(appRedDemonScript PRIVATE Crypt32)
endif()
```

#### 2. DpapiHelper 类定义

```cpp
// src/helpers/DpapiHelper.h
#ifndef DPAPIHELPER_H
#define DPAPIHELPER_H

#include <QObject>
#include <QByteArray>
#include <QString>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dpapi.h>
#endif

/**
 * @brief Windows DPAPI 加密工具类
 * 
 * 提供基于 Windows Data Protection API 的数据加密和解密功能。
 * 仅在 Windows 平台可用。
 */
class DpapiHelper
{
public:
    /**
     * @brief 加密数据
     * @param plaintext 明文数据
     * @param ciphertext 输出：加密后的数据（Base64 编码）
     * @param description 可选的描述字符串
     * @return 加密成功返回 true
     */
    static bool encrypt(const QByteArray &plaintext, QByteArray &ciphertext, const QString &description = QString());
    
    /**
     * @brief 解密数据
     * @param ciphertext 加密的数据（Base64 编码）
     * @param plaintext 输出：解密后的明文数据
     * @param description 输出：加密时的描述字符串
     * @return 解密成功返回 true
     */
    static bool decrypt(const QByteArray &ciphertext, QByteArray &plaintext, QString &description);
    
    /**
     * @brief 检查数据是否已加密
     * @param data 要检查的数据
     * @return 如果数据看起来像是加密的返回 true
     */
    static bool isEncrypted(const QByteArray &data);
    
    /**
     * @brief 获取最后的错误信息
     * @return 错误描述字符串
     */
    static QString lastError();

private:
    static QString s_lastError;
};

#endif // DPAPIHELPER_H
```

#### 3. DpapiHelper 实现

```cpp
// src/helpers/DpapiHelper.cpp
#include "DpapiHelper.h"
#include "LogManager.h"
#include <QDebug>

#ifdef Q_OS_WIN
#include <QRegularExpression>
#endif

QString DpapiHelper::s_lastError;

bool DpapiHelper::encrypt(const QByteArray &plaintext, QByteArray &ciphertext, const QString &description)
{
#ifdef Q_OS_WIN
    DATA_BLOB dataIn;
    DATA_BLOB dataOut;
    
    dataIn.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(plaintext.data()));
    dataIn.cbData = static_cast<DWORD>(plaintext.length());
    
    LPWSTR descriptionPtr = description.isEmpty() ? NULL : 
        const_cast<LPWSTR>(reinterpret_cast<const wchar_t*>(description.utf16()));
    
    if (CryptProtectData(&dataIn, descriptionPtr, NULL, NULL, NULL, 0, &dataOut)) {
        // 将加密数据转换为 Base64 便于 JSON 存储
        ciphertext = QByteArray(reinterpret_cast<char*>(dataOut.pbData), dataOut.cbData).toBase64();
        LocalFree(dataOut.pbData);
        
        if (LogManager::instance()) {
            LogManager::instance()->logDebug("system", "数据加密成功");
        }
        return true;
    }
    
    DWORD error = GetLastError();
    s_lastError = QString("CryptProtectData 失败，错误码: %1").arg(error);
    if (LogManager::instance()) {
        LogManager::instance()->logError("system", s_lastError);
    }
    return false;
#else
    s_lastError = "DPAPI 仅在 Windows 平台可用";
    qWarning() << s_lastError;
    return false;
#endif
}

bool DpapiHelper::decrypt(const QByteArray &ciphertext, QByteArray &plaintext, QString &description)
{
#ifdef Q_OS_WIN
    // 从 Base64 解码
    QByteArray encryptedData = QByteArray::fromBase64(ciphertext);
    if (encryptedData.isEmpty()) {
        s_lastError = "Base64 解码失败";
        return false;
    }
    
    DATA_BLOB dataIn;
    DATA_BLOB dataOut;
    LPWSTR descriptionPtr = NULL;
    
    dataIn.pbData = reinterpret_cast<BYTE*>(encryptedData.data());
    dataIn.cbData = static_cast<DWORD>(encryptedData.length());
    
    if (CryptUnprotectData(&dataIn, &descriptionPtr, NULL, NULL, NULL, 0, &dataOut)) {
        plaintext = QByteArray(reinterpret_cast<char*>(dataOut.pbData), dataOut.cbData);
        LocalFree(dataOut.pbData);
        
        if (descriptionPtr) {
            description = QString::fromWCharArray(descriptionPtr);
            LocalFree(descriptionPtr);
        }
        
        if (LogManager::instance()) {
            LogManager::instance()->logDebug("system", "数据解密成功");
        }
        return true;
    }
    
    DWORD error = GetLastError();
    s_lastError = QString("CryptUnprotectData 失败，错误码: %1").arg(error);
    if (LogManager::instance()) {
        LogManager::instance()->logError("system", s_lastError);
    }
    return false;
#else
    s_lastError = "DPAPI 仅在 Windows 平台可用";
    qWarning() << s_lastError;
    return false;
#endif
}

bool DpapiHelper::isEncrypted(const QByteArray &data)
{
    // 检查是否是有效的 Base64 编码数据
    // 加密后的数据存储为 Base64 格式
    if (data.isEmpty()) {
        return false;
    }
    
#ifdef Q_OS_WIN
    // 尝试 Base64 解码
    QByteArray decoded = QByteArray::fromBase64(data);
    if (decoded.isEmpty()) {
        return false;
    }
    
    // DPAPI 加密数据通常有最小长度（包含头部信息）
    // 这里使用一个简单的启发式方法
    return decoded.length() >= 16;
#else
    return false;
#endif
}

QString DpapiHelper::lastError()
{
    return s_lastError;
}
```

#### 4. ConfigManager 集成密码加密

```cpp
// ConfigManager.cpp 中修改账号相关方法

bool ConfigManager::addAccount(const QJsonObject &account)
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    
    // 复制账号对象以便加密密码
    QJsonObject encryptedAccount = account;
    
    // 加密密码字段
    if (account.contains("password") && !account["password"].toString().isEmpty()) {
        QString password = account["password"].toString();
        QByteArray encryptedPassword;
        
        if (!DpapiHelper::encrypt(password.toUtf8(), encryptedPassword, "RedDemonScript Password")) {
            LogManager::instance()->logError("system", "账号密码加密失败");
            return false;
        }
        
        encryptedAccount["password"] = QString(encryptedPassword);
        encryptedAccount["passwordEncrypted"] = true;  // 标记已加密
    }
    
    accounts.append(encryptedAccount);
    m_config["accounts"] = accounts;
    
    bool saved = saveConfigInternal();
    if (saved) {
        emit accountsChanged();
    }
    return saved;
}

bool ConfigManager::updateAccount(int index, const QJsonObject &account)
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    if (index < 0 || index >= accounts.size()) {
        return false;
    }
    
    // 复制账号对象以便加密密码
    QJsonObject encryptedAccount = account;
    
    // 加密密码字段
    if (account.contains("password") && !account["password"].toString().isEmpty()) {
        QString password = account["password"].toString();
        QByteArray encryptedPassword;
        
        if (!DpapiHelper::encrypt(password.toUtf8(), encryptedPassword, "RedDemonScript Password")) {
            LogManager::instance()->logError("system", "账号密码加密失败");
            return false;
        }
        
        encryptedAccount["password"] = QString(encryptedPassword);
        encryptedAccount["passwordEncrypted"] = true;
    }
    
    accounts[index] = encryptedAccount;
    m_config["accounts"] = accounts;
    
    bool saved = saveConfigInternal();
    if (saved) {
        emit accountsChanged();
    }
    return saved;
}

QJsonArray ConfigManager::accounts() const
{
    QMutexLocker locker(&m_mutex);
    QJsonArray accounts = m_config["accounts"].toArray();
    QJsonArray decryptedAccounts;
    
    for (const QJsonValue &value : accounts) {
        QJsonObject account = value.toObject();
        
        // 解密密码字段
        if (account.contains("passwordEncrypted") && account["passwordEncrypted"].toBool()) {
            QByteArray encryptedPassword = account["password"].toString().toUtf8();
            QByteArray decryptedPassword;
            QString description;
            
            if (DpapiHelper::decrypt(encryptedPassword, decryptedPassword, description)) {
                account["password"] = QString::fromUtf8(decryptedPassword);
            } else {
                // 解密失败，记录日志但返回加密值
                LogManager::instance()->logWarn("system", 
                    QString("账号 %1 密码解密失败").arg(account["username"].toString()));
            }
        }
        
        decryptedAccounts.append(account);
    }
    
    return decryptedAccounts;
}
```

### 命名约定

**来源：project-context.md#代码质量与风格规则**

| 类型 | 约定 | 示例 |
|------|------|------|
| 类名 | PascalCase | `DpapiHelper` |
| 方法 | camelCase | `encrypt()`, `decrypt()` |
| 静态成员 | s_ 前缀 | `s_lastError` |
| 信号 | 动词过去式 | `passwordEncrypted()` |

### 与前序 Story 的关系

**Story 1.4 已完成内容：**
- ✅ ConfigManager 单例类完整实现
- ✅ JSON 配置加载/保存功能
- ✅ 账号 CRUD 操作（addAccount, updateAccount, removeAccount, accounts）
- ✅ 线程安全（QMutex）
- ✅ 原子写入和备份机制

**承接要点：**
- 复用 ConfigManager 现有的账号管理接口
- 在账号保存前调用 DpapiHelper::encrypt()
- 在账号读取后调用 DpapiHelper::decrypt()
- 复用 LogManager 记录加密操作日志

**代码模式参考：**
- 单例模式：参考 ConfigManager::instance() 实现
- 日志记录：参考 ConfigManager 使用 LogManager::instance()->logXxx() 的模式
- 错误处理：参考 ConfigManager 的错误处理模式（返回 bool + 记录日志）

### 与后续 Story 的关系

**Epic 2+ 依赖：**
- ScriptController 在启动游戏时需要读取账号密码
- AccountManager 将依赖 ConfigManager 获取解密后的账号信息
- 沙箱启动游戏时需要密码自动登录

**重要：所有账号密码读取都必须通过 ConfigManager::accounts() 方法，该方法会自动解密密码。**

### 检测到的潜在风险

1. **平台兼容性**：DPAPI 仅在 Windows 平台可用
   - 解决方案：使用 `#ifdef Q_OS_WIN` 条件编译，非 Windows 平台返回错误

2. **用户账户变更**：Windows 用户账户变更后无法解密
   - 解决方案：文档说明，建议用户备份明文密码

3. **解密失败**：配置文件被篡改或损坏导致解密失败
   - 解决方案：解密失败时记录日志，返回加密值并提示用户重新输入

4. **配置迁移**：旧配置文件中的密码可能是明文
   - 解决方案：检测 passwordEncrypted 字段，明文密码在保存时自动加密

5. **内存安全**：解密后的密码在内存中可能被读取
   - 解决方案：这是已知限制，DPAPI 主要保护存储安全

### QML 集成要求

ConfigManager 已提供账号访问接口，无需额外 QML 集成。密码加密/解密对 QML 透明。

**注意事项：**
- QML 通过 ConfigManager.accounts() 获取的账号密码已解密
- QML 通过 ConfigManager.addAccount(account) 保存时密码会自动加密

### 参考资料

- [来源：epics.md#Story 1.5] - Story 定义和验收标准
- [来源：architecture.md#认证与安全] - DPAPI 加密决策
- [来源：architecture.md#项目结构与边界] - 文件位置
- [来源：project-context.md#关键必知规则] - 安全注意事项
- [来源：1-4-configuration-manager-implementation.md] - 前序 Story 完成状态
- [来源：Microsoft Learn - CryptProtectData] - DPAPI API 文档
- [来源：amin-ahmadi.com - DPAPI with Qt] - Qt 集成示例

## Dev Agent Record

### 使用的 Agent 模型

{{agent_model_name_version}}

### 调试日志引用

### 完成笔记列表

### 文件列表
