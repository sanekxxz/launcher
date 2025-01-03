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
public slots:
    void slot(QString name);
private:
    Ui::info_profile *ui;
};

#endif // INFO_PROFILE_H
