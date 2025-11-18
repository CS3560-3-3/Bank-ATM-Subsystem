// App.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sqlite3.h>

using namespace std;

static int createDB(const char* s);
static int createTable(const char* s);
static int openDB(const char* s);

int main()
{
    const char* dir = "../atm.db";
    sqlite3* DB;

    openDB(dir);

    return 0;
}

static int openDB(const char* s) {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(s, &DB);

    if (exit == SQLITE_OK) {
                cout << "Opened Database Successfully!" << endl;
    }
    else {
        cout << "Error Open DB " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_close(DB);
	return 0;
}

static int createDB(const char* s) {
    sqlite3* DB;
    int exit = 0;

	exit = sqlite3_open(s, &DB);
    sqlite3_close(DB);

    return 0;
}

static int createTable(const char* s) {
    sqlite3* DB;

    string sql = "CREATE TABLE IF NOT EXISTS User("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "userID           INT    NOT NULL, "
        "name            TEXT    NOT NULL, "
        "phone		     TEXT    NOT NULL, "
        "email           TEXT    NOT NULL, "
        "ssn			 TEXT    NOT NULL"
        ");";

    try {
        int exit = 0;
		exit = sqlite3_open(s, &DB);

        char* messaggeError;
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
        if (exit != SQLITE_OK) {
            cerr << "Error Create Table" << endl;
            sqlite3_free(messaggeError);
        }
        else
            cout << "Table created Successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception& e) {
        cerr << e.what();
    }

    return 0;
}