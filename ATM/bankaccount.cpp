#include "bankaccount.h"
#include "ui_bankaccount.h"
#include <QString>
#include <QtSql>
#include <QComboBox>
#include <QStringList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <vector>

BankAccount::BankAccount(int cardPin, long long cardNum, std::vector<long long> accountNums, int userID, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BankAccount)
    , cardPin(cardPin)
    , cardNum(cardNum)
    , accounts(accountNums)
    , userID(userID)
{
    ui->setupUi(this);
    getTypes();
    setActBalance();
    setComboBox();
    getUserName();
}

BankAccount::~BankAccount()
{
    delete ui;
}

void BankAccount::setCardNum(long long num) {
    cardNum = num;
}

void BankAccount::setCardPin(int pin) {
    cardPin = pin;
}

void BankAccount::getAccounts() {
    ui->lblType0->setText(QString::number(accounts[1]));
}

void BankAccount::getTypes() {
    QSqlQuery accountTypeQuery;
    long long accountNum = accounts[0];

    accountTypeQuery.prepare("SELECT accountType "
                             "FROM accounts "
                             "WHERE accountNum = :accountNum");
    accountTypeQuery.bindValue(":accountNum", accountNum);

    if(!accountTypeQuery.exec()) {
        qWarning() << "Query failed:" << accountTypeQuery.lastError().text();
        return;
    }

    if(accountTypeQuery.next()) {
        ui->lblType0->setText(accountTypeQuery.value(0).toString());
    }
}

void BankAccount::setActBalance() {
    QSqlQuery accountBalanceQuery;
    QSqlQuery accountTypeQuery;
    QVector<QString> accountTypes = {};
    QVector<QLabel*> typeLabels = { ui->lblType0, ui->lblType1 };
    QVector<QLabel*> balLabels = { ui->lblBalance0, ui->lblBalance1};

    int i = 0;

    while(i < accounts.size()) {
        //Grab accountNum from vector
        long long accountNum = accounts[i];

        //Get the accountType based on the accountNum
        accountTypeQuery.prepare("SELECT accountType "
                                 "FROM accounts "
                                 "WHERE accountNum = :accountNum ");
        accountTypeQuery.bindValue(":accountNum", accountNum);

        //if executes assign the first label to the value (account type)
        if(!accountTypeQuery.exec()) {
            qWarning() << "Query failed: " << accountTypeQuery.lastError().text();
            return;
        }
        if(accountTypeQuery.next()) {
            typeLabels[i]->setText(accountTypeQuery.value(0).toString());
        }

        //Now we get the balance
        accountBalanceQuery.prepare("SELECT accountBal "
                                    "FROM accounts "
                                    "WHERE accountNum = :accountNum ");
        accountBalanceQuery.bindValue(":accountNum", accountNum);

        //And assign it to the corresponding balance label
        if(!accountBalanceQuery.exec()) {
            qWarning() << "Query failed: " << accountBalanceQuery.lastError().text();
            return;
        }
        if(accountBalanceQuery.next()) {
            balLabels[i]->setText("$ " + accountBalanceQuery.value(0).toString());
        }

        i++;
    }
}

void BankAccount::setComboBox() {
    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;
    QComboBox *cbDeposit = ui->cbDeposit;
    QComboBox *cbWithdraw = ui->cbWithdraw;
    QString accountType;
    QSqlQuery accountTypeQuery;
    int i = 0;

    while(i < accounts.size()) {
        long long accountNum = accounts[i];

        accountTypeQuery.prepare("SELECT accountType "
                                 "FROM accounts "
                                 "WHERE accountNum = :accountNum ");
        accountTypeQuery.bindValue(":accountNum", accountNum);

        if(!accountTypeQuery.exec()) {
            qWarning() << "Query failed: " << accountTypeQuery.lastError().text();
            return;
        }
        if(accountTypeQuery.next()) {
            cbFrom->addItem(accountTypeQuery.value(0).toString());
            cbTo->addItem(accountTypeQuery.value(0).toString());
            cbDeposit->addItem(accountTypeQuery.value(0).toString());
            cbWithdraw->addItem(accountTypeQuery.value(0).toString());
        }

        i++;
    }

    cbFrom->setCurrentIndex(-1);
    cbTo->setCurrentIndex(-1);
    cbDeposit->setCurrentIndex(-1);
    cbWithdraw->setCurrentIndex(-1);

    connect(ui->cbFrom, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &BankAccount::onFromAccountChanged);
    connect(ui->cbTo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BankAccount::onToAccountChanged);
}

void BankAccount::onFromAccountChanged(int index) {
    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;

    QString selected = cbFrom->itemText(index);

    cbTo->setItemData(index, Qt::NoItemFlags, Qt::UserRole - 1);
}

void BankAccount::onToAccountChanged(int index) {
    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;

    QString selected = cbTo->itemText(index);
    cbFrom->setItemData(index, Qt::NoItemFlags, Qt::UserRole - 1);
}

void BankAccount::on_submitTransfer_clicked()
{
    QSqlQuery updateFromQuery;
    QSqlQuery updateToQuery;
    QSqlQuery fromQuery;
    QSqlQuery toQuery;

    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;
    int userAmt = ui->txtAmt->toPlainText().trimmed().toInt();

    QString accountFrom = cbFrom->currentText();
    QString accountTo = cbTo->currentText();

    qint64 fromBalance = 0;
    qint64 toBalance = 0;

    //SET THE FROM BALANCE
    fromQuery.prepare("SELECT accountBal "
                    "FROM accounts "
                      "WHERE accountType = :accountFrom");
    fromQuery.bindValue(":accountFrom", accountFrom);

    if(!fromQuery.exec()) {
        qWarning() << "Query failed: " << fromQuery.lastError().text();
        return;
    }
    if(fromQuery.next()) {
        fromBalance = fromQuery.value(0).toInt();
    }

    //SET THE TO BALANCE
    toQuery.prepare("SELECT accountBal "
                    "FROM accounts "
                    "WHERE accountType = :accountTo");
    toQuery.bindValue(":accountTo", accountTo);

    if(!toQuery.exec()) {
        qWarning() << "Query failed: " << toQuery.lastError().text();
        return;
    }
    if(toQuery.next()) {
        toBalance = toQuery.value(0).toInt();
    }

    //DO CALCULATIONS
    fromBalance -= userAmt;
    toBalance += userAmt;

    //UPDATE THE DATABASE
    //NEED TO CHANGE ACCOUNTTYPE TO ACCESS ACCOUNTNUM
    updateFromQuery.prepare("UPDATE accounts "
                        "SET accountBal = :fromBalance "
                        "WHERE accountType = :accountFrom "
                        "AND userID = :userID");
    updateFromQuery.bindValue(":fromBalance", fromBalance);
    updateFromQuery.bindValue(":accountFrom", accountFrom);
    updateFromQuery.bindValue(":userID", userID);
    updateFromQuery.exec();

    updateToQuery.prepare("UPDATE accounts "
                          "SET accountBal = :toBalance "
                          "WHERE accountType = :accountTo "
                          "AND userID = :userID");
    updateToQuery.bindValue(":toBalance", toBalance);
    updateToQuery.bindValue(":accountTo", accountTo);
    updateFromQuery.bindValue(":userId", userID);
    updateToQuery.exec();
}//onSubmitTransferClicked


void BankAccount::on_cancelTransfer_clicked()
{
    ui->cbFrom->setCurrentIndex(-1);
    ui->cbTo->setCurrentIndex(-1);

    auto *fromModel = qobject_cast<QStandardItemModel*>(ui->cbFrom->model());
    auto *toModel   = qobject_cast<QStandardItemModel*>(ui->cbTo->model());

    if (fromModel) {
        for (int i = 0; i < fromModel->rowCount(); ++i) {
            QStandardItem *item = fromModel->item(i);
            if (item) item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
    if (toModel) {
        for (int i = 0; i < toModel->rowCount(); ++i) {
            QStandardItem *item = toModel->item(i);
            if (item) item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
}


void BankAccount::on_btnSubmitDeposit_clicked()
{
    QSqlQuery balanceQuery;
    QSqlQuery updateDB;
    QString accountTypeInput;
    int depositAmount;
    int accountBalance;

    depositAmount = ui->txtDeposit->toPlainText().trimmed().toInt();
    accountTypeInput = ui->cbDeposit->currentText();

    balanceQuery.prepare("SELECT accountBal "
                         "FROM accounts "
                         "WHERE accountType = :accountTypeInput "
                         "AND userID = :userID");
    balanceQuery.bindValue(":accountTypeInput", accountTypeInput);
    balanceQuery.bindValue(":userID", userID);

    if(!balanceQuery.exec()) {
        qWarning() << "Query failed: " << balanceQuery.lastError().text();
        return;
    }
    if(balanceQuery.next()) {
        accountBalance = balanceQuery.value(0).toInt();
    }

    //UPDATE AMOUNT
    accountBalance += depositAmount;

    //UPDATE DATABASE
    updateDB.prepare("UPDATE accounts "
                     "SET accountBal = :accountBalance "
                     "WHERE accountType = :accountTypeInput "
                     "AND userID = :userID ");
    updateDB.bindValue(":accountBalance", accountBalance);
    updateDB.bindValue(":accountTypeInput", accountTypeInput);
    updateDB.bindValue(":userID", userID);

    updateDB.exec();
}


void BankAccount::on_btnWithdraw_clicked()
{
    QSqlQuery balanceQuery;
    QSqlQuery updateDB;
    QString accountTypeInput;
    int withdrawAmount;
    int accountBalance;

    withdrawAmount = ui->txtWithdraw->toPlainText().trimmed().toInt();
    accountTypeInput = ui->cbWithdraw->currentText();

    //GET ACCOUNT BALANCE
    balanceQuery.prepare("SELECT accountBal "
                         "FROM accounts "
                         "WHERE accountType = :accountTypeInput "
                         "AND userID = :userID");
    balanceQuery.bindValue(":accountTypeInput", accountTypeInput);
    balanceQuery.bindValue(":userID", userID);

    if(!balanceQuery.exec()) {
        qWarning() << "Query failed: " << balanceQuery.lastError().text();
        return;
    }
    if(balanceQuery.next()) {
        accountBalance = balanceQuery.value(0).toInt();
    }

    accountBalance -= withdrawAmount;

    updateDB.prepare("UPDATE accounts "
                     "SET accountBal = :accountBalance "
                     "WHERE accountType = :accountTypeInput "
                     "AND userID = :userID");
    updateDB.bindValue(":accountBalance", accountBalance);
    updateDB.bindValue(":accountTypeInput", accountTypeInput);
    updateDB.bindValue(":userID", userID);

    updateDB.exec();
}

void BankAccount::getUserName() {
    QSqlQuery nameQuery;
    QString firstName;
    QString lastName;
    QString greeting = "Hello, ";
    QLabel *nameLabel = ui->lblName;

    nameQuery.prepare("SELECT firstName, lastName "
                      "FROM users "
                      "WHERE userID = :userID ");
    nameQuery.bindValue(":userID", userID);

    if(!nameQuery.exec()) {
        qWarning() << "Query failed: " << nameQuery.lastError().text();
        return;
    }
    if(nameQuery.next()) {
        firstName = nameQuery.value(0).toString();
        lastName = nameQuery.value(1).toString();
    }

    nameLabel->setText(greeting + firstName + " " + lastName);
}
