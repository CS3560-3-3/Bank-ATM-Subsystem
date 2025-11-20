#include <iostream>
#include "../headers/sqlite3.h"
#include "../headers/card_service.h"
#include "../headers/crypto_key_manager.h"

using namespace std;

int main() {
/*--------Declarations--------*/
    long long cardNum;

/*--------Open Database--------*/
    // 1. Load database
    sqlite3* db = nullptr;
    if (sqlite3_open("./BankATM/atm.db", &db) != SQLITE_OK) {
        cout << "Failed to open DB: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    cout << "DB file actually opened: " << sqlite3_db_filename(db, "main") << endl;


    cout << "Database opened successfully.\n";

    // 2. Load AES key (testing CryptoKeyManager)
    /* CryptoKeyManager km;

    vector<unsigned char> key;
    if (!km.keyExists()) {
        cout << "AES key not found. Creating new one.\n";
        key = km.generateKey();
        km.saveKey(key);
    }
    else {
        key = km.loadKey();
        cout << "AES key loaded.\n";
    }

    // 3. Create CardService
    CardService cs;

    // 4. Test data
    int userID = 1;
    string cardNumber = "4444222266668888";
    string pin = "1234";

    cout << "\nAdding card...\n";

    if (cs.addCard(db, userID, cardNumber, pin)) {
        cout << "Card added successfully!\n";
    }
    else {
        cout << "Card insertion FAILED.\n";
    }

    sqlite3_close(db); */

    cout << "Please enter your card number: " << endl;
    cin >> cardNum;

    cout << cardNum;

    return 0;
}

