#include "../headers/card_service.h"
#include "../headers/crypto_key_manager.h"
#include <iostream>

vector<unsigned char> CardService::encryptCardNumber(
    const string& cardNumber,
    const vector<unsigned char>& key,
    vector<unsigned char>& iv
) {
    iv.resize(16);
    RAND_bytes(iv.data(), 16);

    vector<unsigned char> ciphertext(cardNumber.size() + EVP_MAX_BLOCK_LENGTH);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0, ciphertext_len = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data());

    EVP_EncryptUpdate(ctx,
        ciphertext.data(),
        &len,
        reinterpret_cast<const unsigned char*>(cardNumber.c_str()),
        cardNumber.size()
    );
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

vector<unsigned char> CardService::hashPin(const string& pin, vector<unsigned char>& salt) {
    salt.resize(16);
    RAND_bytes(salt.data(), 16);

    vector<unsigned char> hash(32);

    PKCS5_PBKDF2_HMAC(
        pin.c_str(),
        pin.length(),
        salt.data(),
        salt.size(),
        100000,
        EVP_sha256(),
        hash.size(),
        hash.data()
    );

    return hash;
}

bool CardService::addCard(sqlite3* db, int userID, const string& cardNumber, const string& pin) {\

    vector<unsigned char> iv;
    vector<unsigned char> encryptedCard = encryptCardNumber(cardNumber, aesKey, iv);

    vector<unsigned char> salt;
    vector<unsigned char> hashedPin = hashPin(pin, salt);

    sqlite3_stmt* stmt;
    string sql = "INSERT INTO Card (cardNumEncrypted, userID, pinSalt, pinHash, cardIV, status) VALUES (?, ?, ?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cout << "SQL Prepare Error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_blob(stmt, 1, encryptedCard.data(), encryptedCard.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, userID);
    sqlite3_bind_blob(stmt, 3, salt.data(), salt.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, hashedPin.data(), hashedPin.size(), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 5, iv.data(), iv.size(), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, 1);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Insert Error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool CardService::verifyCardLogin(sqlite3* db, const string& cardNumber, const string& pin, int& outUserID) {
    sqlite3_stmt* stmt;
    string sql = "SELECT cardNumEncrypted, pinSalt, pinHash, cardIV, userID FROM Card";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cout << "SQL Prepare Error: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    vector<unsigned char> aesKey(32);        // SAME SIZE as insert  
    RAND_bytes(aesKey.data(), 32);           // ⚠ TEMP KEY — REPLACE LATER WITH REAL KEY STORAGE

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* dbEncCard = (unsigned char*)sqlite3_column_blob(stmt, 0);
        int encCardSize = sqlite3_column_bytes(stmt, 0);

        const unsigned char* dbSalt = (unsigned char*)sqlite3_column_blob(stmt, 1);
        int saltSize = sqlite3_column_bytes(stmt, 1);

        const unsigned char* dbHash = (unsigned char*)sqlite3_column_blob(stmt, 2);
        int hashSize = sqlite3_column_bytes(stmt, 2);

        const unsigned char* dbIV = (unsigned char*)sqlite3_column_blob(stmt, 3);
        int ivSize = sqlite3_column_bytes(stmt, 3);

        int userID = sqlite3_column_int(stmt, 4);

        // --- Re-encrypt entered card number ---
        vector<unsigned char> iv(dbIV, dbIV + ivSize);
        vector<unsigned char> encryptedInput(cardNumber.size() + EVP_MAX_BLOCK_LENGTH);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        int len = 0, ciphertext_len = 0;

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey.data(), iv.data());

        EVP_EncryptUpdate(ctx,
            encryptedInput.data(),
            &len,
            reinterpret_cast<const unsigned char*>(cardNumber.c_str()),
            cardNumber.size()
        );
        ciphertext_len = len;

        EVP_EncryptFinal_ex(ctx, encryptedInput.data() + len, &len);
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        encryptedInput.resize(ciphertext_len);

        // Compare encrypted card numbers
        if (ciphertext_len != encCardSize) continue;
        if (!memcmp(encryptedInput.data(), dbEncCard, encCardSize)) {

            // --- Hash entered PIN using stored salt ---
            vector<unsigned char> computedHash(hashSize);
            
            PKCS5_PBKDF2_HMAC(
                pin.c_str(),
                pin.length(),
                dbSalt,
                saltSize,
                100000,
                EVP_sha256(),
                hashSize,
                computedHash.data()
            );

            // Compare PIN hash
            if (!memcmp(computedHash.data(), dbHash, hashSize)) {
                outUserID = userID;
                sqlite3_finalize(stmt);
                return true;
            }
        }
    }

    sqlite3_finalize(stmt);
    return false;
}

#include "../headers/crypto_key_manager.h"

CardService::CardService() {
    CryptoKeyManager keyManager;

    if (!keyManager.keyExists()) {
        cout << "AES key not found. Creating new key..." << endl;
        aesKey = keyManager.generateKey();
        keyManager.saveKey(aesKey);
    } else {
        aesKey = keyManager.loadKey();
    }
}
