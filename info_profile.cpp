#include "info_profile.h"
#include "main.h"
#include "ui_info_profile.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

info_profile::info_profile(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::info_profile)
{
    ui->setupUi(this);
}

info_profile::~info_profile()
{
    delete ui;
}

void info_profile::slot(QString name)
{
    ui->profil_login->setText(name);
    QString slot_name = name;
    QSqlQueryModel model;
    QString hwid = generateHWID();
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString id = model.record(i).value("id").toString();
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        QString pass = model.record(i).value("password").toString();
        if(slot_name == name){
            ui->profil_id->setText(id);
            ui->profil_login->setText(name);
            ui->profil_email->setText(email);
            ui->profil_hwid->setText(hwid);
            //qDebug() << id << email << name << pass;
        }
    }
}

