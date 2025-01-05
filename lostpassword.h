#ifndef LOSTPASSWORD_H
#define LOSTPASSWORD_H

#include <QMainWindow>
#include <QTimer>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlResult>
#include <QSqlRecord>
#include <QThread>
#include <QRandomGenerator>
#include <QRandomGenerator64>
#include "qtbcrypt.h"
bool check_info_user(QString,QString);
QString pass_rand();
namespace Ui {
class lostpassword;
}

class lostpassword : public QMainWindow
{
    Q_OBJECT
signals:
    void firstWindow(); // Сигнал для первого окна на открытие
    void buttonClickedWithState(bool state);
public:
    explicit lostpassword(QWidget *parent = nullptr);
    ~lostpassword();
    QString pass_rand();
    bool check_info_user(const QString &name_str,const QString &email_str);
    bool check_info_user(const QString &name_str,const QString &email_str, const QString &hwid_str);
    bool check_info_user(const QString &name_str,const QString &email_str, const QString &hwid_str, const QString &password_str);
public slots:
    void handleButtonClick(bool checked) {
        //qDebug() << "Кнопка была нажата!"; // Ваш код для обработки нажатия кнопки
        emit buttonClickedWithState(checked);
    }
private slots:
    void on_Start_Button_clicked();
    void on_lostpass_exit_clicked();

private:
    Ui::lostpassword *ui;
};

#endif // LOSTPASSWORD_H
