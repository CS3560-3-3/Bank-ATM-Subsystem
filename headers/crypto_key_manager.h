#ifndef CRYPTO_KEY_MANAGER_H
#define CRYPTO_KEY_MANAGER_H

#include <string>
#include <vector>

using namespace std;

class CryptoKeyManager {
private:
    string keyFilePath;

public:
    CryptoKeyManager(const string& filePath = "security/aes_key.bin");

    bool keyExists();
    vector<unsigned char> generateKey();
    void saveKey(const vector<unsigned char>& key);
    vector<unsigned char> loadKey();
};

#endif
