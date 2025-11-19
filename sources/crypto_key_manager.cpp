#include "../headers/crypto_key_manager.h"
#include <openssl/rand.h>
#include <fstream>
#include <iostream>

using namespace std;

CryptoKeyManager::CryptoKeyManager(const string& filePath)
    : keyFilePath(filePath) {}

bool CryptoKeyManager::keyExists() {
    ifstream file(keyFilePath, ios::binary);
    return file.good();
}

vector<unsigned char> CryptoKeyManager::generateKey() {
    vector<unsigned char> key(32); // AES-256
    RAND_bytes(key.data(), 32);
    return key;
}

void CryptoKeyManager::saveKey(const vector<unsigned char>& key) {
    ofstream file(keyFilePath, ios::binary);
    file.write((char*)key.data(), key.size());
    file.close();
}

vector<unsigned char> CryptoKeyManager::loadKey() {
    vector<unsigned char> key(32);

    ifstream file(keyFilePath, ios::binary);
    if (!file.good()) {
        cout << "ERROR: AES key file not found." << endl;
        return {};
    }

    file.read((char*)key.data(), 32);
    file.close();
    return key;
}
