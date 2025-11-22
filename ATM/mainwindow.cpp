#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bankaccount.h"
#include <QString>
#include <QtSql>
#include <vector>

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
    QString userInput = ui->txtCard->toPlainText().trimmed();
    cardNum = userInput.toLongLong();

    /* USER ENTERS THEIR CARD NUM */
    QSqlQuery query;
    query.prepare("SELECT cardPin "
                  "FROM cards "
                  "WHERE cardNum = :cardNum");
    query.bindValue(":cardNum", userInput);

    if(!query.exec()) {
        qWarning() << "DB query failed:" << query.lastError().text();
        ui->lblText->setText("Database error");
        return;
    }

    /* IF CARD NUM EXISTS, BRING OUT NEW FORM ELEMENTS*/
    if (query.next()) {

        ui->label->setText("Enter your PIN: ");

        ui->txtCard->hide();
        ui->pinTxt->setVisible(true);
        ui->pinSubmit->setVisible(true);
        ui->submit->setVisible(false);\

        cardPin = query.value(0).toInt();
    } else {
        ui->lblText->setText("userID not found");
    }
}

void MainWindow::on_pinSubmit_clicked()
{
    int userInputPin = ui->pinTxt->toPlainText().trimmed().toInt();
    int userNum = 0;
    std::vector<long long> accountNums;

    if (userInputPin == cardPin) {
        QSqlQuery userNumQuery;
        userNumQuery.prepare("SELECT userID "
                             "FROM cards "
                             "WHERE cardNum = :cardNum");
        userNumQuery.bindValue(":cardNum", cardNum);

        if(!userNumQuery.exec()) {
            qWarning() << "Query failed: " << userNumQuery.lastError().text();
        } else if (userNumQuery.next()) {
            userNum = userNumQuery.value(0).toInt();
        } else {
            qWarning() << "No user found for cardNum" << cardNum;
        }

        QSqlQuery accountQuery;
        accountQuery.prepare("SELECT accountNum "
                      "FROM accounts "
                      "WHERE userID = :userNum");
        accountQuery.bindValue(":userNum", userNum);

        if(!accountQuery.exec()) {
            qWarning() << "Account query failed:" << accountQuery.lastError().text();
        } else {
            while(accountQuery.next()) {
                long long accNum = accountQuery.value(0).toLongLong();
                accountNums.push_back(accNum);
            }
        }

        BankAccount *accountForm = new BankAccount(cardPin, cardNum, accountNums, userNum);

        accountForm->show();
        this->hide();
    } else {
        ui->lblText->setText("FAILURE");
    }

    QSqlQuery query;
}

