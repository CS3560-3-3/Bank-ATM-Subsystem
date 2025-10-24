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

    virtual double getBalance() const = 0;
    virtual void setBalance(double newBalance) = 0;
    virtual void showInfo() const = 0;
};

#endif
<<<<<<< HEAD
=======

//Testing
>>>>>>> 3233457106b366b8074b9dc32a2d808d7277e0bf
