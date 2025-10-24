#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <string>
using namespace std;

class Withdraw {
private:
    double withdrawalAmount;

public:
    Withdraw(double amount = 0.0);

    void process();
    double getAmount() const;
};

#endif
