#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "info_profile.h"
#include "windows_reg.h"
#include <QtWidgets>
#include <QDebug>

/* Defining */
#define ORGANIZATION_NAME "Monsters, Inc"
#define ORGANIZATION_DOMAIN "Monsters.com"
#define APPLICATION_NAME "Monsters"
#define SETTINGS_TRAY "settings/tray"
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
    void on_pushButton_2_clicked();
private:
    Ui::MainWindow *ui;
    info_profile *Info_profile;
    windows_reg *Windows_reg;
};
#endif // MAINWINDOW_H
