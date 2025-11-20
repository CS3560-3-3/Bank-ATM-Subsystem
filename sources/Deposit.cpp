#include "../headers/Deposit.h"

/*-----------Setters-----------*/
Deposit::Deposit(double amount) : depositAmount(amount) {}

void Deposit::process() {
	//logic for processing goes here
}

double Deposit::getAmount() const {
	return depositAmount;
}