#ifndef DATABASESUPPORT_H
#define DATABASESUPPORT_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

class DatabaseSupport
{
public:
    DatabaseSupport();
    bool InitDatabase();
    bool LoadDatabase();

private:
    bool CreateNewDatabase();
};

#endif // DATABASESUPPORT_H
