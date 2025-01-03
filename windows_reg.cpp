#include "windows_reg.h"
#include "ui_windows_reg.h"
#include "qtbcrypt.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
windows_reg::windows_reg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windows_reg)
{
    ui->setupUi(this);
}

windows_reg::~windows_reg()
{
    delete ui;
}
void windows_reg::on_reg_clicked()
{
    //QString input = "@"; // Введенная строка
    QString allowedChars = "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "0123456789._-+@"; // Разрешенные символы
    QSqlQuery query;
    QString salt = QtBCrypt::generateSalt();
    QString hashedPassword = QtBCrypt::hashPassword(ui->user_password->text(), salt);
    QString name = ui->user_login->text();
    QString email = ui->user_email->text();
    QSqlQueryModel model;
    bool Sql_no_User = true;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString email = model.record(i).value("email").toString();
        QString name = model.record(i).value("name").toString();
        //QString pass = model.record(i).value("password").toString();
        if(ui->user_login->text() == name || ui->user_email->text() == email){
            Sql_no_User = false;
            break;
        }
    }
    while (Sql_no_User) {
        int end_symbol = email.indexOf('@');
        int last_symbol = email.lastIndexOf('@');
        int last_symbol_1 = email.lastIndexOf('.');
        qDebug() << end_symbol << last_symbol;
        qDebug() << last_symbol_1;
        bool ind = (last_symbol < email.length() - 1 && last_symbol_1 < email.length() - 1);
        qDebug() << ind;
        auto it = std::find_if(email.begin(), email.end(), [&allowedChars](QChar c) {
            return !allowedChars.contains(c);
        });

        if (it == email.end()) {
            qDebug() << "Все символы подходят";
        }
        if(name.size() < 5){
            statusBar()->showMessage("Логин меньше 5 символов", 3000);
            break;
        }
        if(ui->user_login->text().size() == 0 || ui->user_email->text().size() == 0 || ui->user_password->text().size() == 0){
            statusBar()->showMessage("Полня не заполнены", 3000);
        }else{
            query.prepare("INSERT INTO launcher (email, name, password) VALUES (:email, :name, :password)");
            query.bindValue(":email", email);
            query.bindValue(":name", name);
            query.bindValue(":password", hashedPassword);
            query.exec();
            qDebug("Регаем");
        }
        statusBar()->showMessage("Логин или Email занят", 3000);
    }
}


void windows_reg::on_exit_reg_clicked()
{
    this->close();
    emit firstWindow();
}

