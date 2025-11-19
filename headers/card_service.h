#pragma once
#include <string>
#include <vector>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "../headers/sqlite3.h"

using namespace std;

class CardService {
public:
	CardService();
    bool addCard(sqlite3* db, int userID, const string& cardNumber, const string& pin);
    bool verifyCardLogin(sqlite3* db, const string& cardNumber, const string& pin, int& outUserID);
    
private:
    vector<unsigned char> encryptCardNumber(const string& cardNumber, const vector<unsigned char>& key, vector<unsigned char>& iv);
    vector<unsigned char> hashPin(const string& pin, vector<unsigned char>& salt);
    vector<unsigned char> aesKey;
};
