#include "../headers/BankAccount.h"
#include <iostream>
#include <stdexcept>
using namespace std;

const string error = "Error: Invalid input provided.";
//Constructor
BankAccount::BankAccount(const string& pin, const string& routing) : accountPin(pin), routingNumber(routing) {
	// validation
}

//Failed Constructor
BankAccount::BankAccount() {
	throw std::invalid_argument(error);
}