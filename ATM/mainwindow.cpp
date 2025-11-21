#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int cardNum;

void MainWindow::on_submit_clicked()
{
    cardNum = ui->txtCard->toPlainText().toInt();
    ui->lblText->setText(QString::number(cardNum));
}
