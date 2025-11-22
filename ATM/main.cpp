#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Path to the database in the BankATM folder
    QString dbPath = QCoreApplication::applicationDirPath() + "/atm.db";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Failed to open DB:" << db.lastError().text();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        return 1;
    }
    qDebug() << "Database opened at" << dbPath;

    MainWindow w;
    w.show();

    int ret = app.exec();

    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    return ret;
}
