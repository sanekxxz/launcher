#include "info_profile.h"
#include "ui_info_profile.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>

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
    QSqlQuery query;
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString id = model.record(i).value("id").toString();
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        QString pass = model.record(i).value("password").toString();
        QString uuid = model.record(i).value("uuid").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        if(slot_name == name){
            ui->profil_id->setText(id);
            ui->profil_login->setText(name);
            ui->profil_email->setText(email);
            ui->profil_hwid->setText(hwid);
            ui->profil_uuid->setText(uuid);
        }
    }
}

