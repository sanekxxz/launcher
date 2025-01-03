#include "mainwindow.h"
#include "main.h"
#include <QApplication>
#include "QMessageBox"
bool createConnection();
QString generateHWID();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    //QString hwid = generateHWID();
    //qDebug() << "HWID: в main" << hwid;
    QCoreApplication::setOrganizationName("Monsters, Inc");
    QCoreApplication::setOrganizationDomain("Monsters.com");
    QCoreApplication::setApplicationName("Monsters");
    MainWindow w;
    w.show();
    return a.exec();
}
bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("31.31.196.183"); // Название хоста
    db.setDatabaseName("u1878082_test"); // название база данных u1878082_test
    db.setUserName("u1878082_root"); // Логни u1878082_root
    db.setPassword("nL7yP9pF7ssU6rA6");    // пароль nL7yP9pF7ssU6rA6
    //bool connected = db.open();
    if (!db.open())
    {
        //QMessageBox::warning(0, QObject::tr("Ошибка"),
        //QObject::tr("Ошибка подключения к базе!!!"));
        return true;
    }
    else
    {
        //QMessageBox::warning(0, QObject::tr("Все хорошо"),
        //QObject::tr("Подключился к базе!!!"));
        QSqlQuery query;
        if (!(db.tables().contains(QLatin1String("launcher")))) {
            qDebug("db not, created...");
            query.exec("CREATE TABLE IF NOT EXISTS launcher (id int(11) NOT NULL auto_increment PRIMARY KEY, email varchar(50) NOT NULL default '' UNIQUE KEY, name varchar(40) NOT NULL default '' UNIQUE KEY, password varchar(255) NOT NULL default '')");
        }
        //QSqlQuery Ex;
        //Ex.exec("Pragma foreign_keys=on");
        return true;
    }
    //db.close();
}
QString generateHWID()
{
    QString hwidInput;
    hwidInput += QSysInfo::machineHostName();
    hwidInput += QSysInfo::machineUniqueId();
    hwidInput += QSysInfo::kernelType();
    hwidInput += QSysInfo::kernelVersion();
    hwidInput += QSysInfo::productType();
    hwidInput += QSysInfo::productVersion();

    QByteArray hash = QCryptographicHash::hash(hwidInput.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}
