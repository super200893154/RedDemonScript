#include "DpapiHelper.h"
#include "../core/LogManager.h"

#ifdef Q_OS_WIN
#include <QRegularExpression>
#endif

QString DpapiHelper::s_lastError;

bool DpapiHelper::encrypt(const QByteArray &plaintext, QByteArray &ciphertext, const QString &description)
{
#ifdef Q_OS_WIN
    if (plaintext.isEmpty()) {
        s_lastError = "明文数据为空";
        return false;
    }

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

        LogManager *logger = LogManager::instance();
        if (logger) {
            logger->logInfo("system", "账号密码加密成功");
        }
        return true;
    }

    DWORD error = GetLastError();
    s_lastError = QString("CryptProtectData 失败，错误码: %1").arg(error);

    LogManager *logger = LogManager::instance();
    if (logger) {
        logger->logError("system", s_lastError);
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

        LogManager *logger = LogManager::instance();
        if (logger) {
            logger->logInfo("system", "账号密码解密成功");
        }
        return true;
    }

    DWORD error = GetLastError();
    s_lastError = QString("CryptUnprotectData 失败，错误码: %1").arg(error);

    LogManager *logger = LogManager::instance();
    if (logger) {
        logger->logError("system", s_lastError);
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
