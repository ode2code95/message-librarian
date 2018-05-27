#ifndef DATABASESUPPORT_H
#define DATABASESUPPORT_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

enum {
    Sermon_ID = 0,
    Sermon_Title = 1,
    Sermon_Speaker = 2,
    Sermon_Location = 3,
    Sermon_Date = 4,
    Sermon_Description = 5,
    Sermon_Transcription = 6
};

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
