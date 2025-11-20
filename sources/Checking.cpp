#include "../headers/Checking.h"
#include <iostream>

/*-------Parameterized Constructor-------*/
Checking::Checking(const string& accountNum, double balance, const int& pin, const int& routing) :
	accountNumber(accountNum),
	accountBalance(balance),
	accountPin(pin),
	routingNumber(routing)
	{}

/*-------Getters-------*/ /
double Checking::getBalance() const {
	return accountBalance;
}

void Checking::showInfo() const {
	//logic for displaying checking account info goes here
}

//-------Setters-------*/
void Checking::setBalance(double newBalance) {
	accountBalance = newBalance;
}