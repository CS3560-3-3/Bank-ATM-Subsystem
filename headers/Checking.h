#ifndef CHECKING_H
#define CHECKING_H

#include "BankAccount.h"
#include <iostream>
using namespace std;

class Checking : public BankAccount {
private:
    string accountNumber;
    double accountBalance;

public:
    Checking(const string& accountNumber, double balance,
             const string& pin, const string& routing);

    // Getter and setters
    double getBalance() const override;
    void setBalance(double newBalance) override;
    void showInfo() const override;
};

#endif
