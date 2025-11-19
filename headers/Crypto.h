#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>

using namespace std;

struct PinHashResult {
    vector<unsigned char> salt;
    vector<unsigned char> hash;
};

namespace Crypto
{
    // Generate secure random bytes
    vector<unsigned char> generateRandomBytes(size_t length);

    // PIN hashing with PBKDF2-HMAC-SHA256
    PinHashResult hashPin(const string& pin,
                          int iterations = 100'000,
                          size_t saltLen = 16,
                          size_t hashLen = 32);

    bool verifyPin(const string& pin,
                   const vector<unsigned char>& salt,
                   const vector<unsigned char>& expectedHash,
                   int iterations = 100'000);

    // AES-256-CBC card encryption
    vector<unsigned char> encryptCardNumber(
        const string& cardNumber,
        const vector<unsigned char>& key,
        vector<unsigned char>& outIv);

    string decryptCardNumber(
        const vector<unsigned char>& ciphertext,
        const vector<unsigned char>& key,
        const vector<unsigned char>& iv);
}

#endif
