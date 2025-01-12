#include "info_profile.h"
#include "ui_info_profile.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QPixmap>
#include <QByteArray>
#include <QDateTimeEdit>
#include <QSqlError>
#include <QTimer>
#include "main.h"
info_profile::info_profile(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::info_profile)
{
    ui->setupUi(this);
    if(info_admin()){
        ui->admin->show();
        ui->line_profil->show();
        info_profile::setMinimumSize(705,570);
        info_profile::setMaximumSize(705,570);
    }else{
        ui->admin->hide();
        ui->line_profil->hide();
        info_profile::setMinimumSize(705,525);
        info_profile::setMaximumSize(705,525);
    }
    last_date(QDate::currentDate());
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);
}

info_profile::~info_profile()
{
    delete ui;
}
bool info_profile::info_admin(){
    QSqlQueryModel model;
    model.setQuery("SELECT hwidId,user_group FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString hwid = model.record(i).value("hwidId").toString();
        int group = model.record(i).value("user_group").toInt();
        if(hwid == generateHWID() && group == 1){
            return true;
        }
    }
    return false;
}
void info_profile::slot(QString profil_user_name)
{
    ui->profil_login->setText(profil_user_name);
    QString slot_name = profil_user_name;
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
        QDate last_date_entry = model.record(i).value("last_date_entry").toDate();
        QDate data = model.record(i).value("created_account_date").toDate();
        if(slot_name == name){
            ui->profil_id->setText(id);
            ui->profil_login->setText(name);
            ui->profil_email->setText(email);
            ui->profil_hwid->setText(hwid);
            ui->profil_uuid->setText(uuid);
            ui->last_datetime->setDate(last_date_entry);
            ui->created_account_date->setDate(data);
        }
    }
    loadImageFromDatabase();
}
void info_profile::last_date(QDate date){
    date = QDate::currentDate();
    QSqlQuery query;
    QSqlQueryModel model;
    model.setQuery("SELECT hwidId,last_date_entry,id FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString hwid = model.record(i).value("hwidId").toString();
        QDate last = model.record(i).value("last_date_entry").toDate();
        int id = model.record(i).value("id").toInt();
        if(last.isNull() && hwid == generateHWID()){
            if(!(last == date)){
                query.prepare("UPDATE launcher SET last_date_entry = :last_date WHERE id = :id");
                query.bindValue(":id", id);
                query.bindValue(":last_date", date);
                if (query.exec()) {
                    break;
                }
            }
        }
        if(last != date && hwid == generateHWID()){
            query.prepare("UPDATE launcher SET last_date_entry = :last_date WHERE id = :id");
            query.bindValue(":id", id);
            query.bindValue(":last_date", date);
            if (query.exec()) {
                break;
            }
        }
    }
}
bool info_profile::saveImageToDatabase(const QString& imagePath){
    QSqlDatabase db;
    QFile file(imagePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка при открытии файла изображения.";
        db.close();
        return false;
    }
    QByteArray imageData = file.readAll();
    file.close();
    if (imageData.isEmpty()) {
        qDebug() << "Данные изображения пусты";
        db.close();
        return false;
    }

    QSqlQuery query(db);
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        int id = model.record(i).value("id").toInt();
        QString name = model.record(i).value("name").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        if(hwid == generateHWID() && ui->profil_login->text() == name){
            qDebug() << id;
            query.prepare("UPDATE launcher SET image_data = :image_data WHERE id = :id");
            query.bindValue(":id", id);
            query.bindValue(":image_data", imageData);
            break;
        }
    }


    if (!query.exec()) {
        qDebug() << "Ошибка при выполнении запроса:" << query.lastError().text();
        db.close();
        return false;
    }
    qDebug() << "Изображение успешно сохранено.";
    loadImageFromDatabase();
    db.close();
    return true;
}
void info_profile::loadImage() {
    QString imagePath = QFileDialog::getOpenFileName(nullptr, "Select Image File", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!imagePath.isEmpty()) {
        if(saveImageToDatabase(imagePath)){
            QMessageBox::information(nullptr, "Success", "Image saved to database.");
        } else {
            QMessageBox::warning(nullptr, "Error", "Could not save image to database.");
        }
    } else {
        qDebug() << "User canceled the selection";
        QMessageBox::warning(nullptr, "Warning", "User canceled the selection");
    }
}
void info_profile::loadImageFromDatabase()
{
    uint16_t currentImage = 1;
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString id = model.record(i).value("id").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        QString name = model.record(i).value("name").toString();
        if(hwid == generateHWID() && ui->profil_id->text() == id){
            currentImage = id.toInt();
            break;
        }
    }
    QSqlDatabase db;
    QSqlQuery query(db);
    query.prepare("SELECT image_data FROM launcher LIMIT 1 OFFSET :offset"); // Выбираем одно изображение с индексом offset
    query.bindValue(":offset", currentImage - 1);
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        db.close();
        return;
    }
    if (!query.next()) {
        qDebug() << "No image found";
        ui->foto->setText("No image found.");
        db.close();
        return;
    }
    QByteArray imageData = query.value(0).toByteArray();
    db.close();
    // 2. Преобразование QByteArray в QPixmap
    QPixmap pixmap;
    if (pixmap.loadFromData(imageData)) {
        // 3. Отображение изображения (в QLabel)
        ui->foto->setPixmap(pixmap.scaled(ui->foto->size(), Qt::KeepAspectRatio)); //  масштабируем
        //ui->label->clear();
        // или (в QGraphicsView)
    } else {
        //qDebug() << "Error loading image from data";
        ui->foto->setText("Error loading image.");
    }
    db.close();
}
void info_profile::on_Get_Image_clicked()
{
    loadImage();
}


void info_profile::on_admin_clicked()
{
    QSqlQueryModel model;
    model.setQuery("SELECT id,name FROM launcher");
    bool user_list = true;
    for (int i = 0; i < model.rowCount(); ++i) {
        QString id = model.record(i).value("id").toString();
        QString name = model.record(i).value("name").toString();
        if(ui->line_profil->text() == name || ui->line_profil->text() == id){
            admin_info_user(ui->line_profil->text());
            ui->tabWidget->tabBar()->currentChanged(1);
            ui->line_profil->clear();
            user_list = false;
            break;
        }
    }
    if(user_list)
    ui->statusbar->showMessage("Пользователь не найден", 3000);
}

void info_profile::admin_info_user(QString str){
    int currentImage = 0;
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM launcher");
    for (int i = 0; i < model.rowCount(); ++i) {
        QString id = model.record(i).value("id").toString();
        QString name = model.record(i).value("name").toString();
        QString email = model.record(i).value("email").toString();
        QString pass = model.record(i).value("password").toString();
        QString uuid = model.record(i).value("uuid").toString();
        QString hwid = model.record(i).value("hwidId").toString();
        QDate last_date_entry = model.record(i).value("last_date_entry").toDate();
        QDate data = model.record(i).value("created_account_date").toDate();
        if(name == str || id == str){
            ui->admin_info_profil_id->setText(id);
            ui->admin_info_profil_login->setText(name);
            ui->admin_info_profil_email->setText(email);
            ui->admin_info_profil_hwid->setText(hwid);
            ui->admin_info_profil_uuid->setText(uuid);
            ui->admin_info_last_datetime->setDate(last_date_entry);
            ui->admin_info_created_account_date->setDate(data);
            currentImage = id.toInt();
        }
    }
    QSqlDatabase db;
    QSqlQuery query(db);
    query.prepare("SELECT image_data FROM launcher LIMIT 1 OFFSET :offset"); // Выбираем одно изображение с индексом offset
    query.bindValue(":offset", currentImage - 1);
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        db.close();
        return;
    }
    if (!query.next()) {
        qDebug() << "No image found";
        ui->admin_info_profil_foto->setText("No image found.");
        db.close();
        return;
    }
    QByteArray imageData = query.value(0).toByteArray();
    db.close();
    // 2. Преобразование QByteArray в QPixmap
    QPixmap pixmap;
    if (pixmap.loadFromData(imageData)) {
        // 3. Отображение изображения (в QLabel)
        ui->admin_info_profil_foto->setPixmap(pixmap.scaled(ui->admin_info_profil_foto->size(), Qt::KeepAspectRatio)); //  масштабируем
        //ui->label->clear();
        // или (в QGraphicsView)
    } else {
        //qDebug() << "Error loading image from data";
        ui->admin_info_profil_foto->setText("Error loading image.");
    }
    db.close();
}
void info_profile::on_pushButton_2_clicked()
{
    ui->tabWidget->tabBar()->currentChanged(0);
}

