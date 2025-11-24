#ifndef QAESENCRYPTION_H
#define QAESENCRYPTION_H

#include <QObject>
#include <QByteArray>

class QAESEncryption
{
public:
    enum Aes {
        AES_128,
        AES_192,
        AES_256
    };

    enum Mode {
        ECB,
        CBC,
        CFB,
        OFB
    };

    QAESEncryption(Aes level, Mode mode);

    QByteArray encode(const QByteArray &rawText,
                      const QByteArray &key,
                      const QByteArray &iv);

    QByteArray decode(const QByteArray &rawText,
                      const QByteArray &key,
                      const QByteArray &iv);

private:
    Aes m_level;
    Mode m_mode;

    int keyLength();
    int blockSize();
};

#endif // QAESENCRYPTION_H
