#include "../headers/BankAccount.h"
#include "../headers/Savings.h"


/*--------Paramaterized Constructor--------*/
Savings::Savings(const string& accountNumber, double balance, double interest,
				 const string& pin, const string& routing) :
	accountNumber(accountNumber),
	accountBalance(balance),
	interestPercentage(interest),
	accountPin(pin),
	routingNumber(routing)
{}

/*--------Getters--------*/
double Savings::getBalance() const {
	return accountBalance;
}

void Savings::showInfo() const {
	//show info logic here
}

/*--------Setters--------*/
void setBalance(double newBalance) {
	accountBalance = newBalance;
}

void applyInterest() {
	// Apply interest to the account balance
}