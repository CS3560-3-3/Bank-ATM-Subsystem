#include "../headers/Checking.h"

//Constructor
Checking::Checking(const string& accountNumber, double balance, const string &pin, const string &routing) : accountNumber(accountNumber), accountBalance(balance), BankAccount(pin, routing) {
	// validation
}

double Checking::getBalance() const {
	return accountBalance;
}

void Checking::setBalance(double newBalance) {
	accountBalance = newBalance;
}

void Checking::showInfo() const {
	cout << "Account Number: " << accountNumber << endl;
	cout << "Balance: $" << accountBalance << endl;
	BankAccount::showInfo();
}