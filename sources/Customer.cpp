#include "../headers/Customer.h"
#include <iostream>

//Default constructor
Customer::Customer() : name(""), phone(""), email(""), ssn(""), address("") {}

//Parameterized constructor
Customer::Customer(const string& name, const string& phone, const string& email, const string& ssn, const string& address) :
	name(name),
	phone(phone),
	email(email),
	ssn(ssn),
	address(address) 
{}

/*--------------Getters----------------*/
string Customer::getName() const {
	return name;
}

string Customer::getPhone() const {
	return phone;
}

string  Customer::getEmail() const {
	return email;
}

string Customer::getSSN() const {
	return ssn;
}

string Customer::getAddress() const {
	return address;
}

vector<BankAccount*> Customer::getAccounts() const {
	return accounts;
}

/*--------------Methods----------------*/
void Customer::addAccount(BankAccount* account) {
	accounts.push_back(account);
}