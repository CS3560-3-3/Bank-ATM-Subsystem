#include "../headers/Checking.h"
#include <iostream>

Checking::Checking(const string& accountNum, double balance, const int& pin, const int& routing) :
	accountNumber(accountNum),
	accountBalance(balance),
	accountPin(pin),
	routingNumber(routing)
	{}

double Checking::getBalance() const {
	return accountBalance;
}

void Checking::setBalance(double newBalance) {
	accountBalance = newBalance;
}

void Checking::showInfo() const {

}