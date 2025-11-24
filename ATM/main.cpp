#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qInfo() << QCoreApplication::applicationDirPath();
    QString appDir = QCoreApplication::applicationDirPath();
    QString dbPath = appDir + "/atm.db";

    // This is where your real DB lives in the repo
    QString sourceDb = QDir::cleanPath(appDir + "/../../../../ATM/atm.db");

    // ✅ If no DB exists beside exe, copy it
    if (!QFile::exists(dbPath)) {
        qDebug() << "Database not found in build. Copying from source...";
        if (!QFile::copy(sourceDb, dbPath)) {
            qCritical() << "FAILED TO COPY DB from:" << sourceDb;
            return 1;
        }
        QFile::setPermissions(dbPath, QFile::ReadOwner | QFile::WriteOwner);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Failed to open DB:" << db.lastError().text();
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
