#ifndef CHECKING_H
#define CHECKING_H

#include "BankAccount.h"
#include <iostream>
using namespace std;

class Checking : public BankAccount {
private:
    string accountNumber;
    double accountBalance;
    int accountPin;
    int routingNumber;

public:
    Checking(const string& accountNum, double balance,
             const int& pin, const int& routing);

    // Getter and setters
    double getBalance() const override;
    void setBalance(double newBalance) override;
    void showInfo() const override;
};

#endif
