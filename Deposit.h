#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <string>
using namespace std;

class Deposit {
private:
    double depositAmount;

public:
    Deposit(double amount = 0.0);

    // Process the deposit and then return the amount deposited
    void process();
    double getAmount() const;
};

#endif
