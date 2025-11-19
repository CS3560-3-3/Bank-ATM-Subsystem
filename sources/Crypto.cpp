#include "../headers/Crypto.h"

#include <stdexcept>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>

using namespace std;

// Helper: convert OpenSSL error into C++ exception
[[noreturn]] void throwOpenSslError(const char* msg) {
    unsigned long err = ERR_get_error();
    char buf[256];
    ERR_error_string_n(err, buf, sizeof(buf));
    throw runtime_error(string(msg) + " (OpenSSL: " + buf + ")");
}

namespace Crypto {

    vector<unsigned char> generateRandomBytes(size_t length) {
        vector<unsigned char> buf(length);
        if (RAND_bytes(buf.data(), (int)buf.size()) != 1) {
            throwOpenSslError("RAND_bytes failed");
        }
        return buf;
    }

    PinHashResult hashPin(const string& pin,
                          int iterations,
                          size_t saltLen,
                          size_t hashLen)
    {
        PinHashResult result;
        result.salt = generateRandomBytes(saltLen);
        result.hash.resize(hashLen);

        if (PKCS5_PBKDF2_HMAC(
                pin.c_str(), (int)pin.size(),
                result.salt.data(), (int)result.salt.size(),
                iterations,
                EVP_sha256(),
                (int)result.hash.size(),
                result.hash.data()) != 1)
        {
            throwOpenSslError("PBKDF2 failed");
        }

        return result;
    }

    bool verifyPin(const string& pin,
                   const vector<unsigned char>& salt,
                   const vector<unsigned char>& expectedHash,
                   int iterations)
    {
        vector<unsigned char> computed(expectedHash.size());

        if (PKCS5_PBKDF2_HMAC(
                pin.c_str(), (int)pin.size(),
                salt.data(), (int)salt.size(),
                iterations,
                EVP_sha256(),
                (int)computed.size(),
                computed.data()) != 1)
        {
            throwOpenSslError("PBKDF2 verify failed");
        }

        // constant-time compare
        unsigned char diff = 0;
        for (size_t i = 0; i < expectedHash.size(); ++i) {
            diff |= (computed[i] ^ expectedHash[i]);
        }

        return diff == 0;
    }

    vector<unsigned char> encryptCardNumber(
        const string& cardNumber,
        const vector<unsigned char>& key,
        vector<unsigned char>& outIv)
    {
        if (key.size() != 32)
            throw invalid_argument("AES-256 key must be 32 bytes");

        outIv = generateRandomBytes(16); // IV size

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw runtime_error("Failed to create AES context");

        vector<unsigned char> ciphertext;
        try {
            if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                   key.data(), outIv.data()) != 1)
            {
                throwOpenSslError("EncryptInit failed");
            }

            ciphertext.resize(cardNumber.size() + 16);
            int len1 = 0;

            if (EVP_EncryptUpdate(ctx,
                                  ciphertext.data(), &len1,
                                  (unsigned char*)cardNumber.data(),
                                  (int)cardNumber.size()) != 1)
            {
                throwOpenSslError("EncryptUpdate failed");
            }

            int len2 = 0;
            if (EVP_EncryptFinal_ex(ctx,
                                    ciphertext.data() + len1,
                                    &len2) != 1)
            {
                throwOpenSslError("EncryptFinal failed");
            }

            ciphertext.resize(len1 + len2);

            EVP_CIPHER_CTX_free(ctx);
            return ciphertext;
        }
        catch (...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }
    }

    string decryptCardNumber(
        const vector<unsigned char>& ciphertext,
        const vector<unsigned char>& key,
        const vector<unsigned char>& iv)
    {
        if (key.size() != 32)
            throw invalid_argument("AES-256 key must be 32 bytes");

        if (iv.size() != 16)
            throw invalid_argument("IV must be 16 bytes");

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw runtime_error("Failed to create AES context");

        string plaintext;
        plaintext.resize(ciphertext.size());

        try {
            if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                                   key.data(), iv.data()) != 1)
            {
                throwOpenSslError("DecryptInit failed");
            }

            int len1 = 0;
            if (EVP_DecryptUpdate(ctx,
                                  (unsigned char*)&plaintext[0], &len1,
                                  ciphertext.data(), (int)ciphertext.size()) != 1)
            {
                throwOpenSslError("DecryptUpdate failed");
            }

            int len2 = 0;
            if (EVP_DecryptFinal_ex(ctx,
                                    (unsigned char*)&plaintext[0] + len1,
                                    &len2) != 1)
            {
                throwOpenSslError("DecryptFinal failed");
            }

            plaintext.resize(len1 + len2);

            EVP_CIPHER_CTX_free(ctx);
            return plaintext;
        }
        catch (...) {
            EVP_CIPHER_CTX_free(ctx);
            throw;
        }
    }

} // namespace Cryp
