#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "BankAccount.h"
using namespace std;

class Customer {
private:
    string name;
    string phone;
    string email;
    string ssn;
    string address;
    vector<BankAccount*> accounts;  // 1..*

public:
    Customer();
    Customer(const string& name, const string& phone,
             const string& email, const string& ssn, const string& address);

    // Getters / setters
    string getName() const;
    string getPhone() const;
    string getEmail() const;
    string getSSN() const;
    string getAddress() const;

    void addAccount(BankAccount* account);
    vector<BankAccount*> getAccounts() const;
};

#endif
