#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main.h"
#include "QMessageBox"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSettings>
#include <QDebug>
#include <QSqlError>
#include "qtbcrypt.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    //setWindowFlags(windowFlags() & ~(Qt::WindowMaximizeButtonHint));
    //ui->login->setStyleSheet("background-color: rgb(45, 45, 45);\nborder-style: solid;\nborder-width: 1px;\nborder-color: #ffffff;\nborder-radius: 5px;");
    Info_profile = new info_profile(); // Инициализируем второе окно
    connect(Info_profile, &info_profile::firstWindow, this, &MainWindow::show);    // подключаем к слоту запуска главного окна по кнопке во втором окне
    Windows_reg = new windows_reg(); // Инициализируем второе окнопоиск по базе данных sql qt
    connect(Windows_reg, &windows_reg::firstWindow, this, &MainWindow::show);

    Lost_Password = new lostpassword(); // Инициализируем второе окнопоиск по базе данных sql qt
    connect(Lost_Password, &lostpassword::firstWindow, this, &MainWindow::show);
    //Сохранение логина и проверка
    auto loadedData = loadLoginData();
    QString loaded_username = loadedData.first;
    int loaded_Checked = loadedData.second;
    ui->login->setText(loaded_username);
    if(ui->login->text().size() == 0 && loaded_Checked == Qt::Checked){
        loaded_Checked = 0;
    }else{
        ui->checkBox->setChecked(loaded_Checked);
    }
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
        QString hwid = model.record(i).value("hwidId").toString();
        if(ui->login->text() == name && QtBCrypt::hashPassword(ui->pass->text(),pass) == pass && hwid == generateHWID())
        {
            profil = true;
            break;
        }
    }
    if(profil){
        Info_profile->show(); // Показываем второе окно
        connect(this, &MainWindow::signal, Info_profile, &info_profile::slot);
        emit signal(ui->login->text());
        ui->pass->clear(); // Очистка пароля и ещё что то
        this->close(); // Закрываем основное окно
    }else{
        statusBar()->showMessage("Неверный логин или пароль", 3000);
    }
}
void saveLoginData(const QString& username, int &arg1) {
    QSettings settings("ADClain", "ADClain#1"); // Или используйте подходящий путь для пользователя
    settings.setValue("username", username); //Например, email или имя пользователя
    settings.setValue("Checked", arg1);
    settings.sync(); // Важно для немедленного сохранения
}
QPair <QString,int> loadLoginData(){
    QSettings settings("ADClain", "ADClain#1"); // Или используйте подходящий путь для пользователя
    QString username = settings.value("username", "").toString();
    int arg1 = settings.value("Checked", "").toInt();
    return {username, arg1};
}
void MainWindow::on_reg_clicked()
{
    Windows_reg->show();
    this->close();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->checkState() == Qt::Unchecked && ui->login->text().size() == 0){
    }else if(ui->login->text().size() == 0){
        statusBar()->showMessage("Поле Логин пусто", 3000);
        ui->checkBox->setCheckState(Qt::Unchecked);
    }
    if(ui->checkBox->checkState() == Qt::Checked){
        saveLoginData(ui->login->text(), arg1);
    }else{
        saveLoginData("", arg1);
    }
}
void MainWindow::on_lostpassword_clicked()
{
    Lost_Password->show(); // Показываем второе окно
    this->close(); // Закрываем основное окно
    //MessageBox::about(this, "Ошибка", "Ошибка");
}
