#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bankaccount.h"
#include <QString>
#include <QtSql>
#include <vector>
#include "crypto_utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #5A6A85");
    ui->pinTxt->setVisible(false);
    ui->pinSubmit->setVisible(false);
    QFont appFont("Segoe UI", 20);
    this->setFont(appFont);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString rawText;
QString greeting = "Hello, ";

void MainWindow::on_submit_clicked()
{
    QString userInput = ui->txtCard->text().trimmed();
    cardNum = userInput.toLongLong();

    QSqlQuery query;
    query.prepare("SELECT pinHash, pinSalt FROM cards WHERE cardNum = :cardNum");
    query.bindValue(":cardNum", cardNum);

    if (!query.exec() || !query.next()) {
        ui->lblText->setText("CARD NOT FOUND");
        return;
    }

    ui->label->setText("Enter your PIN:");
    ui->txtCard->hide();
    ui->pinTxt->setVisible(true);
    ui->pinSubmit->setVisible(true);
    ui->submit->setVisible(false);
}


void MainWindow::on_pinSubmit_clicked()
{
    QString enteredPin = ui->pinTxt->toPlainText().trimmed();

    QSqlQuery query;
    query.prepare("SELECT pinHash, pinSalt, userID FROM cards WHERE cardNum = :cardNum");
    query.bindValue(":cardNum", cardNum);

    if (!query.exec() || !query.next()) {
        ui->lblText->setText("DATABASE ERROR");
        return;
    }

    QByteArray storedHash = query.value("pinHash").toByteArray();
    QByteArray storedSalt = query.value("pinSalt").toByteArray();
    int userNum = query.value("userID").toInt();

    QByteArray inputHash = hashPin(enteredPin, storedSalt);

    if (inputHash != storedHash) {
        ui->lblText->setText("INCORRECT PIN");
        return;
    }

    // ✅ PIN correct, continue normally
    std::vector<long long> accountNums;

    QSqlQuery accountQuery;
    accountQuery.prepare("SELECT accountNum FROM accounts WHERE userID = :userNum");
    accountQuery.bindValue(":userNum", userNum);

    if(accountQuery.exec()) {
        while(accountQuery.next()) {
            accountNums.push_back(accountQuery.value(0).toLongLong());
        }
    }

    BankAccount *accountForm = new BankAccount(0, cardNum, accountNums, userNum);
    accountForm->show();
    this->hide();
}

