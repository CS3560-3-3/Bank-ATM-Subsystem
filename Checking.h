#ifndef CHECKING_H
#define CHECKING_H

#include "BankAccount.h"
using namespace std;

class Checking : public BankAccount {
private:
    string accountNumber;
    double accountBalance;

public:
    Checking(const string& accountNumber, double balance,
             const string& pin, const string& routing);

    double getBalance() const override;
    void setBalance(double newBalance);

    void showInfo() const override;
};

#endif
