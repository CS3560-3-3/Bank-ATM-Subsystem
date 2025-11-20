#include "../headers/BankAccount.h"
#include <iostream>
#include <stdexcept>
using namespace std;

const string error = "Error: Invalid input provided.";

//Parameterized Constructor
BankAccount::BankAccount(const string& pin, const string& routing) : accountPin(pin), routingNumber(routing) {}

//Failed Constructor : Cannot be blank
BankAccount::BankAccount() {
	throw std::invalid_argument(error);
}

/*-------Getters-------*/
virtual double BankAccount::getBalance() const {
	//logic for getting balance goes here
}

/*-------Setters-------*/
virtual void BankAccount::setBalance(double newBalance) {
	//logic for setting balance goes here
}

/*-------Display Info-------*/
virtual void BankAccount::showInfo() const {
	//logic for displaying account info goes here
}s