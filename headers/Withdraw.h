#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <string>
using namespace std;

class Withdraw {
private:
    double withdrawalAmount;

public:
    Withdraw(double amount = 0.0);

    // Process the withdrawal and then return the amount withdrawn
    void process();
    double getAmount() const;
};

#endif
