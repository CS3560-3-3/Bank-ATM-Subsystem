#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QWidget>

namespace Ui {
class BankAccount;
}

class BankAccount : public QWidget
{
    Q_OBJECT

public:
    explicit BankAccount(int cardPin, long long cardNum, std::vector<long long> accounts, int userID, QWidget *parent = nullptr);
    ~BankAccount();

    void setCardNum(qint64 num);
    void setCardPin(int pin);
    void getAccounts();
    void getTypes();
    void setActBalance();
    void setComboBox();

private slots:
    void on_submitTransfer_clicked();

private:
    Ui::BankAccount *ui;
    int cardPin;
    long long cardNum;
    std::vector<long long> accounts;
    int userID;
    void onFromAccountChanged(int index);
    void onToAccountChanged(int index);
};

#endif // BANKACCOUNT_H
