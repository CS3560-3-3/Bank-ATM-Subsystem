#include <iostream>
#include "headers"
#include "sqlite3.h"

using namespace std;

int main() {
	sqlite3* db;
	int rc;

	rc = sqlite3_open("my_database.db", &db)
	return 0;
}