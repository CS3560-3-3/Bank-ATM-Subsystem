#ifndef SAVINGS_H
#define SAVINGS_H

#include "BankAccount.h"
using namespace std;

class Savings : public BankAccount {
private:
    string accountNumber;
    double accountBalance;
    double interestPercentage;

public:
    Savings(const string& accountNumber, double balance, double interest,
            const string& pin, const string& routing);

    void applyInterest();
    double getBalance() const override;
    void showInfo() const override;
};

#endif
