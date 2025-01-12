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
    MainWindow w;
    w.show();
    return a.exec();
}
bool createConnection()
{
    QString createTableLaucnher = R"(
    CREATE TABLE IF NOT EXISTS launcher (
    id int(11) NOT NULL auto_increment PRIMARY KEY,
    email varchar(50) NOT NULL default '' UNIQUE KEY,
    name varchar(40) NOT NULL default '' UNIQUE KEY,
    password varchar(255) NOT NULL default '',
    uuid CHAR(36) UNIQUE DEFAULT NULL,
    hwidId varchar(255) DEFAULT NULL,
    created_account_date date DEFAULT NULL,
    last_date_entry date DEFAULT NULL,
    image_data LONGBLOB,
    user_group int(11) NOT NULL DEFAULT '3'))";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("31.31.196.183"); // Название хоста
    db.setDatabaseName("u1878082_test"); // название база данных u1878082_test
    db.setUserName("u1878082_root"); // Логни u1878082_root
    db.setPassword("nL7yP9pF7ssU6rA6");    // пароль nL7yP9pF7ssU6rA6
    //bool connected = db.open();
    if (!db.open())
    {
        QMessageBox::warning(0, QObject::tr("Ошибка"),
        QObject::tr("Ошибка подключения к базе!!!"));
        return true;
    }
    else
    {
        //QMessageBox::warning(0, QObject::tr("Все хорошо"),
        //QObject::tr("Подключился к базе!!!"));
        QSqlQuery query;
        if (!(db.tables().contains(QLatin1String("launcher")))) {
            query.exec(createTableLaucnher);
            query.exec("CREATE TRIGGER setUUID BEFORE INSERT ON launcher ""FOR EACH ROW BEGIN IF NEW.uuid IS NULL THEN SET NEW.uuid = UUID(); END IF; END; // DELIMITER ;" );
            qDebug() << "База данных успешно инициализирована.";
        }
        QSqlQuery Ex;
        Ex.exec("Pragma foreign_keys=on");
        return true;
    }
    db.close();
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
