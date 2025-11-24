// bankaccount.h
#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QWidget>
#include <QtGlobal>      // qint64
#include <vector>

namespace Ui {
class BankAccount;
}

class BankAccount : public QWidget
{
    Q_OBJECT

public:
    explicit BankAccount(int cardPin,
                         qint64 cardNum,
                         std::vector<qint64> accounts,
                         int userID,
                         QWidget *parent = nullptr);
    ~BankAccount();

    void setCardNum(qint64 num);
    void setCardPin(int pin);

    void setActBalance();
    void setComboBox();
    void getAccounts();
    void getTypes();
    void getUserName();
    void getActNum();

private slots:
    // Buttons
    void on_submitTransfer_clicked();
    void on_cancelTransfer_clicked();
    void on_btnSubmitDeposit_clicked();
    void on_btnWithdraw_clicked();

    // Combobox guards (disable same-account selection)
    void onFromAccountChanged(int index);
    void onToAccountChanged(int index);

private:
    Ui::BankAccount *ui = nullptr;

    int cardPin = 0;
    qint64 cardNum = 0;
    std::vector<qint64> accounts;
    int userID = 0;
};

#endif // BANKACCOUNT_H
