#ifndef DPAPIHELPER_H
#define DPAPIHELPER_H

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
