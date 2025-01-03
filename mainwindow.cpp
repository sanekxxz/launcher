#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSettings>
#include "qtbcrypt.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->login->setStyleSheet("background-color: rgb(45, 45, 45);\nborder-style: solid;\nborder-width: 1px;\nborder-color: #ffffff;\nborder-radius: 5px;");
    Info_profile = new info_profile(); // Инициализируем второе окно
    connect(Info_profile, &info_profile::firstWindow, this, &MainWindow::show);    // подключаем к слоту запуска главного окна по кнопке во втором окне
    Windows_reg = new windows_reg(); // Инициализируем второе окнопоиск по базе данных sql qt
    connect(Windows_reg, &windows_reg::firstWindow, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_auto_profil_clicked()
{
    bool profil = false;
    QString salt = QtBCrypt::generateSalt();
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString name = model.record(i).value("name").toString();
        QString pass = model.record(i).value("password").toString();
        if(ui->login->text() == name && QtBCrypt::hashPassword(ui->pass->text(),pass) == pass)
        {
            profil = true;
        }
    }
    if(profil){
        Info_profile->show(); // Показываем второе окно
        connect(this, &MainWindow::signal, Info_profile, &info_profile::slot);
        emit signal(ui->login->text());
        ui->login->clear(); // Очистка логина
        ui->pass->clear(); // Очистка пароля и ещё что то
        this->close(); // Закрываем основное окно
    }else{
        statusBar()->showMessage("Неверный логин или пароль", 3000);
        //statusBar()->showMessage("Неверный логин или пароль", 3000);
    }

}


void MainWindow::on_reg_clicked()
{
    Windows_reg->show();
    this->close();
}


void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about(this, "Ошибка", "Ошибка");
}
