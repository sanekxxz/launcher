#include "lostpassword.h"
#include "ui_lostpassword.h"
#include <QProgressBar>
#include "main.h"
lostpassword::lostpassword(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lostpassword)
{
    ui->setupUi(this);
    setWindowTitle("Востановление пароля");
    connect(ui->lostpass_exit, &QPushButton::clicked, this, &lostpassword::handleButtonClick);
    ui->lostpass_exit->setCheckable(true);
    ui->lostpass_exit->setChecked(true);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
}

lostpassword::~lostpassword()
{
    delete ui;
}
bool lostpassword::check_info_user(const QString &name_str,const QString &email_str){
    bool check_info = false;
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        if(name_str == name && email_str == email){
            check_info = true;
        }

    }
    return check_info;
}
bool lostpassword::check_info_user(const QString &name_str,const QString &email_str, const QString &hwid_str){
    bool check_info = false;
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        if(name_str == name && email_str == email && hwid_str == hwid){
            qDebug() << hwid_str;
            check_info = true;
        }

    }
    return check_info;
}
bool lostpassword::check_info_user(const QString &name_str,const QString &email_str, const QString &hwid_str, const QString &password_str){
    bool check_info = false;
    QSqlQuery query;
    QString salt = QtBCrypt::generateSalt();
    QString hashedPassword = QtBCrypt::hashPassword(password_str, salt);
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        int id = model.record(i).value("id").toInt();
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        if(name_str == name && email_str == email && hwid_str == hwid){
            query.prepare("UPDATE launcher SET password = :hashedPassword WHERE id = :id");
            query.bindValue(":id", id);
            query.bindValue(":hashedPassword", hashedPassword);
            if (query.exec()) {
                check_info = true;
                break;
            }
        }
    }
    return check_info;
}
void lostpassword::on_Start_Button_clicked()
{
    QSqlQuery query;
    bool state_exet = ui->lostpass_exit->isChecked();
    QObject::connect(this, &lostpassword::buttonClickedWithState, [&](bool state){
        state_exet = state;
    });
    //QString const &name_str = ui->login_user->text();
    //QString const &email_str = ui->email_user->text();
    QString new_pass;
    for(int value = 0; value <= ui->progressBar->maximum(); value++){
        if(state_exet){
            QThread::msleep(100);
            ui->progressBar->setValue(value);
            ui->labe_prog_value->setText(QString::number(value));
            qApp->processEvents(QEventLoop::AllEvents);
            if(value == 25){
                QThread::msleep(500);
                if(ui->login_user->text().size()== 0 || ui->email_user->text().size() == 0){
                    //statusBar()->showMessage("Поля не заполнены", 3000);
                    ui->info_pass->setText("Поля не заполнены");
                    for(int i = value; i != 0; i--){
                        QThread::msleep(100);
                        ui->progressBar->setValue(i);
                        ui->labe_prog_value->setText(QString::number(i));
                        qApp->processEvents(QEventLoop::AllEvents);
                    }
                    break;
                }
            }
            if(value == 50){
                QThread::msleep(500);
                if(!(check_info_user(ui->login_user->text(), ui->email_user->text()))){
                    //statusBar()->showMessage("Логин или E-mail не верны", 5000);
                    ui->info_pass->setText("Логин или E-mail не верны");
                    QThread::msleep(500);
                    for(int i = value; i != 0; i--){
                        QThread::msleep(100);
                        ui->progressBar->setValue(i);
                        ui->labe_prog_value->setText(QString::number(i));
                        qApp->processEvents(QEventLoop::AllEvents);
                    }
                    break;
                }
            }
            if(value == 75){
                if(check_info_user(ui->login_user->text(), ui->email_user->text(), generateHWID())){
                    new_pass = pass_rand();
                }else{
                    //statusBar()->showMessage("Всё круто", 5000);
                    QThread::msleep(500);
                    for(int i = value; i != 0; i--){
                        QThread::msleep(100);
                        ui->progressBar->setValue(i);
                        ui->labe_prog_value->setText(QString::number(i));
                        qApp->processEvents(QEventLoop::AllEvents);
                    }
                    break;
                }
            }
            if(value == 99 && check_info_user(ui->login_user->text(), ui->email_user->text(), generateHWID(), new_pass)){
                qDebug() << new_pass;
                ui->info_pass->setText("Новый пароль: " + new_pass);
                statusBar()->showMessage(new_pass, 5000);
            }
    }
    }
    ui->progressBar->setValue(0);
    ui->labe_prog_value->setText(QString::number(0));
}
QString lostpassword::pass_rand(){
    QRandomGenerator *generator = QRandomGenerator::global();
    QString randomString;
    for(int i = 0; i < 10; i++){
        QList<QString> myStrings = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","!","#","$","%","&","(",")","*","+",",","-",".","/","|","\\","'",":",";","<",
            "=",">","?","@","^","_","`","{","}","~",};
        randomString+= myStrings.at(generator->bounded(myStrings.size()));
    }
    return randomString;
}
void lostpassword::on_lostpass_exit_clicked()
{
    this->close();
    emit firstWindow();
    ui->progressBar->setValue(0);
    ui->lostpass_exit->setChecked(true);
}


