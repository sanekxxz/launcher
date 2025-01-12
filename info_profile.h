#ifndef INFO_PROFILE_H
#define INFO_PROFILE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>
namespace Ui {
class info_profile;
}

class info_profile : public QMainWindow
{
    Q_OBJECT
signals:
    void firstWindow(); // Сигнал для первого окна на открытие
public:
    explicit info_profile(QWidget *parent = nullptr);
    ~info_profile();
    void last_date(QDate date);
public slots:
    void slot(QString profil_user_name);
private slots:
    bool saveImageToDatabase(const QString& imagePath);
    void loadImage();
    void on_Get_Image_clicked();
    void loadImageFromDatabase();
    bool info_admin();
    void on_admin_clicked();
    void on_pushButton_2_clicked();
    void admin_info_user(QString str);

private:
    Ui::info_profile *ui;
};

#endif // INFO_PROFILE_H
