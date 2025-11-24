#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include "security/qaesencryption.h"

/**
 * Generate a random 16-byte salt
 */
inline QByteArray generateSalt()
{
    QByteArray salt(16, 0);
    for (int i = 0; i < salt.size(); ++i) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->generate() & 0xFF);
    }
    return salt;
}

/**
 * Hash a PIN using SHA-256 + salt
 */
inline QByteArray hashPin(const QString &pin, const QByteArray &salt)
{
    QByteArray data = pin.toUtf8() + salt;
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256);
}

const QByteArray CARD_AES_KEY = QByteArray::fromHex(
    "00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF"
    );


// Encrypt card number using AES-256-CBC
inline QByteArray encryptCardNum(const QString &cardNum, QByteArray &outIV)
{
    outIV = generateSalt(); // reuse for 16-byte IV

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    return encryption.encode(
        cardNum.toUtf8(),
        CARD_AES_KEY,
        outIV
        );
}

// Decrypt card number
inline QString decryptCardNum(const QByteArray &encrypted,
                              const QByteArray &iv)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QByteArray decrypted =
        encryption.decode(encrypted, CARD_AES_KEY, iv);

    return QString::fromUtf8(decrypted);
}

#endif
