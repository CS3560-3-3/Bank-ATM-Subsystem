#ifndef SAVINGS_H
#define SAVINGS_H

#include "BankAccount.h"
#include <iostream>
using namespace std;

class Savings : public BankAccount {
private:
    string accountNumber;
    double accountBalance;
    double interestPercentage;

public:
    Savings(const string& accountNumber, double balance, double interest,
            const string& pin, const string& routing);

    double getBalance() const override;
    void setBalance(double newBalance) override;
    void applyInterest();
    void showInfo() const override;
};

#endif
