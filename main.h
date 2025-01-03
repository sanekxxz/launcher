#ifndef MAIN_H
#define MAIN_H
#include "QSqlQuery"
#include <QCoreApplication>
#include <QSysInfo>
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <QSettings>
QString generateHWID();
bool createConnection();
#endif // MAIN_H
