#ifndef WINDOWS_REG_H
#define WINDOWS_REG_H

#include <QMainWindow>
namespace Ui {
class windows_reg;
}

class windows_reg : public QMainWindow
{
    Q_OBJECT
signals:
    void firstWindow(); // Сигнал для первого окна на открытие
public:
    explicit windows_reg(QWidget *parent = nullptr);
    ~windows_reg();

private slots:
    void on_reg_clicked();

    void on_exit_reg_clicked();

private:
    Ui::windows_reg *ui;
};

#endif // WINDOWS_REG_H
