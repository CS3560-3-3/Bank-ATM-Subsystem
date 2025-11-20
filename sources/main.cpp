#include <iostream>
#include "../headers/sqlite3.h"
#include "../headers/card_service.h"
#include "../headers/crypto_key_manager.h"

using namespace std;

/*--------Function Declarations--------*/
static int selectData(sqlite3* db, string phonNum, string userID);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int accounts(void* NotUsed, int argc, char** argv, char** azColName);


//MOST OF THIS CODE WILL BE DELETED. THIS IS JUST TO TEST THE DATABASE CONNECTION AND QUERIES.

/*----------Functions----------*/
static int selectData(sqlite3* db, string phoneNum, string userID) {
	string sql = "SELECT firstName, lastName FROM User WHERE phone = ";
    string sql2 = "SELECT accountType, accountBal FROM Accounts WHERE userID = ";
    sql += phoneNum;
	sql += ";";
    sql2 += userID;
    sql2 += ";";
    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    int rc2 = sqlite3_exec(db, sql2.c_str(), accounts, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << endl;
        sqlite3_free(errMsg);
    }

    return rc;
}

static int callback(void* Notused, int argc, char** argv, char** azColName) {
    cout << "Hello there, " << argv[0] << " " << argv[1] << ". " << endl;

    cout << endl;
    return 0;
}

static int accounts(void* Notused, int argc, char** argv, char** azColName) {
    cout << argv[0] << "\nBalance: " << argv[1] << endl;
    cout << endl;
    return 0;
}

int main() {
/*--------Declarations--------*/
    string phoneNum = "";
    string userID = "";
	const char* dbPath = "./BankATM/atm.db";
    sqlite3* db = nullptr;
    
/*--------Open Database--------*/
    int rc = sqlite3_open(dbPath, &db);
    if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }
    cout << "Database opened successfully." << endl;

/*--------GET CARD NUM--------*/
    cout << "Please enter your phone number: ";
    cin >> phoneNum;

	cout << "Please enter your user ID: ";
    cin >> userID;

/*--------SELECT DATA--------*/
    selectData(db, phoneNum, userID);
    // 1. Load database

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

    return 0;
}