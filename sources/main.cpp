#include <iostream>
#include "../ATM/mainwindow.h">
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
    string sql = "SELECT firstName, lastName FROM User WHERE phone = '" + phoneNum + "';";
    string sql2 = "SELECT accountType, accountBal FROM Accounts WHERE userID = '" + userID + "';";
    char* errMsg = nullptr;
    char* errMsg2 = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    int rc2 = sqlite3_exec(db, sql2.c_str(), accounts, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << endl;
        sqlite3_free(errMsg);
    }
    if (rc2 != SQLITE_OK) {
        cerr << "SQL error (Accounts): " << (errMsg2 ? errMsg2 : "Unknown error") << endl;
        sqlite3_free(errMsg2);
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

    return 0;
}