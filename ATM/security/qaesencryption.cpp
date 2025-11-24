#include "qaesencryption.h"
#include <QCryptographicHash>

QAESEncryption::QAESEncryption(Aes level, Mode mode)
{
    m_level = level;
    m_mode = mode;
}

int QAESEncryption::keyLength()
{
    switch (m_level) {
    case AES_128: return 16;
    case AES_192: return 24;
    case AES_256: return 32;
    default: return 32;
    }
}

int QAESEncryption::blockSize()
{
    return 16;
}

QByteArray QAESEncryption::encode(const QByteArray &rawText,
                                  const QByteArray &key,
                                  const QByteArray &iv)
{
    QByteArray keyHash = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    QByteArray paddedKey = keyHash.left(keyLength());

    QByteArray input = rawText;
    int padding = blockSize() - (input.size() % blockSize());

    for(int i = 0; i < padding; ++i)
        input.append((char)padding);

    QByteArray output = input;

    for(int i = 0; i < output.size(); i++) {
        output[i] = output[i] ^ paddedKey.at(i % paddedKey.size())
        ^ iv.at(i % iv.size());
    }

    return output;
}

QByteArray QAESEncryption::decode(const QByteArray &rawText,
                                  const QByteArray &key,
                                  const QByteArray &iv)
{
    QByteArray keyHash = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    QByteArray paddedKey = keyHash.left(keyLength());

    QByteArray output = rawText;

    for(int i = 0; i < output.size(); i++) {
        output[i] = output[i] ^ paddedKey.at(i % paddedKey.size())
        ^ iv.at(i % iv.size());
    }

    if(!output.isEmpty()) {
        int padding = output.at(output.size() - 1);
        output.chop(padding);
    }

    return output;
}
