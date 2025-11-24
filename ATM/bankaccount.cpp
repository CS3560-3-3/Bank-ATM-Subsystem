// bankaccount.cpp
#include "bankaccount.h"
#include "ui_bankaccount.h"

#include <QString>
#include <QtSql>
#include <QComboBox>
#include <QStringList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
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
                             "WHERE accountNum = :accountNum AND userID = :userID");
    accountTypeQuery.bindValue(":accountNum", accountNum);
    accountTypeQuery.bindValue(":userID", userID);

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
        long long accountNum = accounts[i];

        accountTypeQuery.prepare("SELECT accountType "
                                 "FROM accounts "
                                 "WHERE accountNum = :accountNum AND userID = :userID");
        accountTypeQuery.bindValue(":accountNum", accountNum);
        accountTypeQuery.bindValue(":userID", userID);

        if(!accountTypeQuery.exec()) {
            qWarning() << "Query failed: " << accountTypeQuery.lastError().text();
            return;
        }
        if(accountTypeQuery.next()) {
            typeLabels[i]->setText(accountTypeQuery.value(0).toString());
        }

        accountBalanceQuery.prepare("SELECT accountBal "
                                    "FROM accounts "
                                    "WHERE accountNum = :accountNum AND userID = :userID");
        accountBalanceQuery.bindValue(":accountNum", accountNum);
        accountBalanceQuery.bindValue(":userID", userID);

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
    QSqlQuery accountTypeQuery;
    int i = 0;

    while(i < accounts.size()) {
        long long accountNum = accounts[i];

        accountTypeQuery.prepare("SELECT accountType "
                                 "FROM accounts "
                                 "WHERE accountNum = :accountNum AND userID = :userID");
        accountTypeQuery.bindValue(":accountNum", accountNum);
        accountTypeQuery.bindValue(":userID", userID);

        if(!accountTypeQuery.exec()) {
            qWarning() << "Query failed: " << accountTypeQuery.lastError().text();
            return;
        }
        if(accountTypeQuery.next()) {
            const QString t = accountTypeQuery.value(0).toString();
            cbFrom->addItem(t);
            cbTo->addItem(t);
            cbDeposit->addItem(t);
            cbWithdraw->addItem(t);
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

    Q_UNUSED(cbFrom);
    // disable the same index in the 'to' combo to prevent same-account transfers
    cbTo->setItemData(index, Qt::NoItemFlags, Qt::UserRole - 1);
}

void BankAccount::onToAccountChanged(int index) {
    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;

    Q_UNUSED(cbTo);
    cbFrom->setItemData(index, Qt::NoItemFlags, Qt::UserRole - 1);
}

static bool ensurePositiveAmount(int amount, QWidget* parent, const QString& action) {
    if (amount <= 0) {
        QMessageBox::warning(parent, action, "Amount must be greater than zero.");
        return false;
    }
    return true;
}

void BankAccount::on_submitTransfer_clicked()
{
    QComboBox *cbFrom = ui->cbFrom;
    QComboBox *cbTo = ui->cbTo;

    const QString accountFrom = cbFrom->currentText().trimmed();
    const QString accountTo   = cbTo->currentText().trimmed();
    const int userAmt = ui->txtAmt->toPlainText().trimmed().toInt();

    if (accountFrom.isEmpty() || accountTo.isEmpty()) {
        QMessageBox::warning(this, "Transfer", "Select both source and destination accounts.");
        return;
    }
    if (accountFrom == accountTo) {
        QMessageBox::warning(this, "Transfer", "Source and destination cannot be the same.");
        return;
    }
    if (!ensurePositiveAmount(userAmt, this, "Transfer")) return;

    qint64 fromBalance = 0;
    qint64 toBalance = 0;

    QSqlQuery fromQuery;
    fromQuery.prepare("SELECT accountBal "
                      "FROM accounts "
                      "WHERE accountType = :accountFrom AND userID = :userID");
    fromQuery.bindValue(":accountFrom", accountFrom);
    fromQuery.bindValue(":userID", userID);

    if(!fromQuery.exec() || !fromQuery.next()) {
        qWarning() << "From query failed:" << fromQuery.lastError().text();
        QMessageBox::critical(this, "Transfer", "Failed to read source account.");
        return;
    }
    fromBalance = fromQuery.value(0).toLongLong();

    QSqlQuery toQuery;
    toQuery.prepare("SELECT accountBal "
                    "FROM accounts "
                    "WHERE accountType = :accountTo AND userID = :userID");
    toQuery.bindValue(":accountTo", accountTo);
    toQuery.bindValue(":userID", userID);

    if(!toQuery.exec() || !toQuery.next()) {
        qWarning() << "To query failed:" << toQuery.lastError().text();
        QMessageBox::critical(this, "Transfer", "Failed to read destination account.");
        return;
    }
    toBalance = toQuery.value(0).toLongLong();

    if (fromBalance < userAmt) { // why: prevent negative balances
        QMessageBox::warning(this, "Transfer", "Insufficient funds in the source account.");
        return;
    }

    const qint64 newFrom = fromBalance - userAmt;
    const qint64 newTo   = toBalance + userAmt;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "Transfer", "Failed to start transaction.");
        return;
    }

    QSqlQuery updateFromQuery;
    updateFromQuery.prepare("UPDATE accounts "
                            "SET accountBal = :fromBalance "
                            "WHERE accountType = :accountFrom AND userID = :userID");
    updateFromQuery.bindValue(":fromBalance", newFrom);
    updateFromQuery.bindValue(":accountFrom", accountFrom);
    updateFromQuery.bindValue(":userID", userID);

    QSqlQuery updateToQuery;
    updateToQuery.prepare("UPDATE accounts "
                          "SET accountBal = :toBalance "
                          "WHERE accountType = :accountTo AND userID = :userID");
    updateToQuery.bindValue(":toBalance", newTo);
    updateToQuery.bindValue(":accountTo", accountTo);
    updateToQuery.bindValue(":userID", userID);

    if (!updateFromQuery.exec() || !updateToQuery.exec()) {
        db.rollback();
        qWarning() << "Update failed:" << updateFromQuery.lastError().text() << updateToQuery.lastError().text();
        QMessageBox::critical(this, "Transfer", "Failed to update balances.");
        return;
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "Transfer", "Failed to commit transaction.");
        return;
    }

    setActBalance();
    ui->txtAmt->clear();
    ui->cbFrom->setCurrentIndex(-1);
    ui->cbTo->setCurrentIndex(-1);
    QMessageBox::information(this, "Transfer", "Transfer completed.");
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
    QString accountTypeInput = ui->cbDeposit->currentText();
    int depositAmount = ui->txtDeposit->toPlainText().trimmed().toInt();

    if (accountTypeInput.isEmpty() || !ensurePositiveAmount(depositAmount, this, "Deposit")) return;

    int accountBalance = 0;

    balanceQuery.prepare("SELECT accountBal "
                         "FROM accounts "
                         "WHERE accountType = :accountTypeInput AND userID = :userID");
    balanceQuery.bindValue(":accountTypeInput", accountTypeInput);
    balanceQuery.bindValue(":userID", userID);

    if(!balanceQuery.exec()) {
        qWarning() << "Query failed: " << balanceQuery.lastError().text();
        return;
    }
    if(balanceQuery.next()) {
        accountBalance = balanceQuery.value(0).toInt();
    }

    accountBalance += depositAmount;

    updateDB.prepare("UPDATE accounts "
                     "SET accountBal = :accountBalance "
                     "WHERE accountType = :accountTypeInput AND userID = :userID ");
    updateDB.bindValue(":accountBalance", accountBalance);
    updateDB.bindValue(":accountTypeInput", accountTypeInput);
    updateDB.bindValue(":userID", userID);

    updateDB.exec();
    setActBalance();
}


void BankAccount::on_btnWithdraw_clicked()
{
    QSqlQuery balanceQuery;
    QSqlQuery updateDB;
    QString accountTypeInput = ui->cbWithdraw->currentText();
    int withdrawAmount = ui->txtWithdraw->toPlainText().trimmed().toInt();

    if (accountTypeInput.isEmpty() || !ensurePositiveAmount(withdrawAmount, this, "Withdraw")) return;

    int accountBalance = 0;

    balanceQuery.prepare("SELECT accountBal "
                         "FROM accounts "
                         "WHERE accountType = :accountTypeInput AND userID = :userID");
    balanceQuery.bindValue(":accountTypeInput", accountTypeInput);
    balanceQuery.bindValue(":userID", userID);

    if(!balanceQuery.exec()) {
        qWarning() << "Query failed: " << balanceQuery.lastError().text();
        return;
    }
    if(balanceQuery.next()) {
        accountBalance = balanceQuery.value(0).toInt();
    }

    if (accountBalance < withdrawAmount) { // why: prevent overdraft by withdraw path, too
        QMessageBox::warning(this, "Withdraw", "Insufficient funds.");
        return;
    }

    accountBalance -= withdrawAmount;

    updateDB.prepare("UPDATE accounts "
                     "SET accountBal = :accountBalance "
                     "WHERE accountType = :accountTypeInput AND userID = :userID");
    updateDB.bindValue(":accountBalance", accountBalance);
    updateDB.bindValue(":accountTypeInput", accountTypeInput);
    updateDB.bindValue(":userID", userID);

    updateDB.exec();
    setActBalance();
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
