#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "info_profile.h"
#include "windows_reg.h"
#include "lostpassword.h"
#include <QtWidgets>
#include <QDebug>
void saveLoginData(const QString& usernamem, int &arg1);
QPair <QString,int> loadLoginData();
void on_checkBox_stateChanged(int arg1);
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void signal(QString);
private slots:
    void on_auto_profil_clicked();
    void on_reg_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_lostpassword_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    info_profile *Info_profile;
    windows_reg *Windows_reg;
    lostpassword *Lost_Password;
};
#endif // MAINWINDOW_H
