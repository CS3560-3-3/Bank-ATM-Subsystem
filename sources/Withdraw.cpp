#include "../headers/Withdraw.h"

/*--------Parameterized Constructor--------*/
Withdraw::Withdraw(double amount) : withdrawalAmount(amount) {}

/*--------Getters---------*/
double Withdraw::getAmount() const {
	return withdrawalAmount;
}

/*--------Processing Logic--------*/
void Withdraw::process() {
	//Process logic here
}