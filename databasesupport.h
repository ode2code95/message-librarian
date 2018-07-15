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
    static QString GetCompatibleDBTableName();
    static bool InitDatabase();
    static bool LoadDatabase();
    static bool CheckDatabaseVersion(QSqlDatabase curDB = QSqlDatabase::database());
    static bool UpdateDatabase();

    //only temporarily public for testing!
    static bool RenameSQLTable(QString oldName, QString newName);

    // You must call CheckVersionNumber before you can retreive the actual value;
    static int GetCompatibleVersion();
    static int GetDbVersion();
    static QString GetReleaseDescription();

private:
    //Made the constructor private because having an object of the database makes little sense,
    //considering that we are working with only one database connection at a time.
    DatabaseSupport();
    static bool CreateNewDatabase();
    static QString ExtractDatabaseVersion(QSqlDatabase db);
    //static bool RenameSQLTable(QString oldName, QString newName);
    static int compatibleVersion;
    static int dbVersion;
    static QString releaseDescription;
};

#endif // DATABASESUPPORT_H
