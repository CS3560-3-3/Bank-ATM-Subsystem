#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "BankAccount.h"
#include "Receipt.h"
#include "Withdraw.h"
#include "Deposit.h"
#include "Error.h"
using namespace std;

class Transaction {
private:
    double transactionAmount;
    string transactionID;
    string transactionType;  // "withdraw", "deposit", "transfer"
    string transactionDate;

    BankAccount* fromAccount;  // may be null for deposits
    BankAccount* toAccount;    // may be null for withdrawals

    Receipt* receipt;          // 0..1
    Withdraw* withdrawObj;     // 0..1
    Deposit* depositObj;       // 0..1
    Error* error;              // 0..1

public:
    Transaction(const string& id, const string& type, double amount,
                const string& date, BankAccount* from = nullptr, BankAccount* to = nullptr);

    bool execute(); // handles logic routing to Withdraw/Deposit/Transfer

    // Accessors
    double getAmount() const;
    string getID() const;
    string getType() const;
    string getDate() const;

    Receipt* getReceipt() const;
    Error* getError() const;
};

#endif
