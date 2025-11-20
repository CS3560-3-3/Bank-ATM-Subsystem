#include "../headers/Transaction.h"

/*--------Parameterized Constructor--------*/
Transaction::Transaction(const string& id, const string& type, double amount,
						 const string& date, BankAccount* from, BankAccount* to) :
	transactionID(id),
	transactionType(type),
	transactionAmount(amount),
	transactionDate(date),
	fromAccount(from),
	toAccount(to)
{}

/*--------Getters--------*/
double Transaction::getAmount() const {
	return transactionAmount;
}

string Transaction::getID() const {
	return transactionID;
}

string Transaction::getType() const {
	return transactionType;
}

string Transaction::getDate() const {
	return transactionDate;
}

/*--------Logic & Errors--------*/
bool Transaction::execute() {
	//execute logic here
}

Receipt* Transaction::getReceipt() const {
	//receipt logic here
}

Error* getError() const {
	//error logic here
}