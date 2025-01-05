#ifndef MAIN_H
#define MAIN_H
#include "QSqlQuery"
#include <QCoreApplication>
#include <QSysInfo>
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <QSettings>
#include <tchar.h>
#include <Windows.h>
QString generateHWID();
bool createConnection();
#endif // MAIN_H
