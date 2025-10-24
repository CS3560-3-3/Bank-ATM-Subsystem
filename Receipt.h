#ifndef RECEIPT_H
#define RECEIPT_H

#include <string>
using namespace std;

class Receipt {
private:
    double transferAmount;

public:
    Receipt(double transferAmount = 0.0);

    void printReceipt() const;
};

#endif
