#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
using namespace std;

class BankAccount {
protected:
    string accountPin;
    string routingNumber;

public:
    BankAccount();
    BankAccount(const string& pin, const string& routing);

    virtual ~BankAccount() = default;

    virtual void showInfo() const = 0;
    virtual double getBalance() const = 0; 
};

#endif