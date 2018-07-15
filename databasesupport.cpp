#include "databasesupport.h"
#include "editsermon.h"

#include <QDate>

#define COMPAT_DBTABLENAME \
    "Messages_Version_Alpha_1"\

int DatabaseSupport::compatibleVersion = -1;
int DatabaseSupport::dbVersion = -1;
QString DatabaseSupport::releaseDescription = "NULL";

DatabaseSupport::DatabaseSupport()
{
}

/* Returns the table  name  that this  version
 * of the software supports.  Other  versions,
 * if older, will need to be upgraded, or else
 * you will need to update your software!
 */
QString DatabaseSupport::GetCompatibleDBTableName()
{
    return COMPAT_DBTABLENAME;
}

bool DatabaseSupport::InitDatabase()
{
    QSettings settings("TrueLife Tracks", "Message Librarian");
    QString dbpath = settings.value("paths/databaseLocation", "C:/Audio Message Library").toString();
    if (!QDir(dbpath).exists()) {
        //The database path in settings does not exist. Display a message and ask the user what to do.
        int answer = QMessageBox::warning(0, "Error", "Your sermon library could not be found at the location specified in program settings. Do you want to initialze a new library at <i>" +
                                        dbpath + "</i>?<p>(If your sermon library is stored on an external disk or network resource, make sure that your computer can access it, and then try starting the program again.)",
                                        QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
        if (answer == QMessageBox::Yes) {
            qDebug("Creating default database directory . . .");
            if(!QDir(dbpath).mkpath(dbpath))
                qDebug("Error creating parent directory!");
        }
        else
            return false; //Database path did not exist, and user chose not to create it.
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbpath + "/Message_Library_Database.db");
    bool ok = db.open();

    if (!ok) {
        QMessageBox::warning(0, "Error", "Cannot open database. Error details: " + db.lastError().text() +
                             "\nPlease contact your support team for assistance.");
    }

    return ok;
}

bool DatabaseSupport::LoadDatabase()
{
    QSqlTableModel model(0, QSqlDatabase::database());
    model.setTable(GetCompatibleDBTableName());

    if (model.lastError().type() != QSqlError::NoError) {
        int result = QMessageBox::warning(0, "Error", "Cannot load database. Error details: " + model.lastError().text() +
                                          "\nDo you want to initialize a new one with default values?",
                                          QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
        if (result == QMessageBox::Yes) {
            if (CreateNewDatabase()) {
                QMessageBox::information(0, "Operation successful", "New database has been successfully created. Choose <b>Edit</b> from the <b>File</b> menu to add entries.");
                return true;
            }
            return false;
        } else if (result == QMessageBox::No) {
            return false;
        }
    }

    // No error occurred. Continue with program loading.
    return true;
}

/// If you send a db connection, that will be used instead of the default, as in preparing to merge libraries.
bool DatabaseSupport::CheckDatabaseVersion(QSqlDatabase curDB)
{
    // Get the database version that works with this release of the software.
    QString stringCompatibleVersion = QString(COMPAT_DBTABLENAME).remove("Messages_Version_");
    compatibleVersion = stringCompatibleVersion.right(2).remove("_").toInt();

    // Get the actual version number from the database.
    QString stringDbVersion = ExtractDatabaseVersion(curDB);

    int versionSeparatorCharPos = stringDbVersion.indexOf("_");
    if (stringDbVersion == "" || versionSeparatorCharPos == -1) {
        QMessageBox::critical(0, "Error", "Failed to load version number from database. Data structure does not match Message Librarian's signature."
                                         "\n Please contact your support team for assistance.");
        return false;
    }

    releaseDescription = stringDbVersion.left(versionSeparatorCharPos);
    dbVersion = stringDbVersion.right(2).remove("_").toInt(); //Allow UP TO 2 digits for version number

    // Compare them!
    if (dbVersion < compatibleVersion) {
        QMessageBox::warning(0, "Error", "Your message library needs to be updated to work with this version of the software.\nPlease press OK to begin this process.");
        return false;
    } else if (dbVersion > compatibleVersion) {
        dbVersion = -2; // return version number to -2 so that we know to attempt an update.
        QMessageBox::warning(0, "Error", "This message library is from a newer version of the software. You will need to update the program to continue.\nIf you need assistance, please contact your support team.");
        return false;
    }

    return  true; // Version matches. Proceed with program loading.
}

int DatabaseSupport::GetCompatibleVersion()
{
    return compatibleVersion;
}

int DatabaseSupport::GetDbVersion()
{
    return dbVersion;
}

QString DatabaseSupport::GetReleaseDescription()
{
    return releaseDescription;
}

bool DatabaseSupport::CreateNewDatabase()
{
    QSqlQuery query("CREATE TABLE " + QString(COMPAT_DBTABLENAME) + " ("
                     "id VARCHAR(40),"
                     "title CLOB NOT NULL,"
                     "speaker VARCHAR(40) NOT NULL,"
                     "location VARCHAR(40) NOT NULL,"
                     "date VARCHAR(40) NOT NULL,"
                     "description VARCHAR(40) NOT NULL,"
                     "transcription CLOB);", QSqlDatabase::database());

    if (!query.isActive()) {
        QMessageBox::warning(0, "Error", "Cannot create new database. Error details: " +
                             query.lastError().text() + "\n Please contact your support team for assistance.");
        return false;
    }

    query.finish();
    return true;
}

QString DatabaseSupport::ExtractDatabaseVersion(QSqlDatabase db)
{
    QStringList availableTables = db.tables();
    //Check for proper table count
    if (availableTables.size() < 1 || availableTables.size() > 1) {
            return "";
    }
    //Check to make sure it looks like our table
    for (int i = 0; i < availableTables.size(); i++) {
        if (!availableTables[i].contains("Messages_Version_"))
            return "";
    }

    return QString(availableTables[0]).remove("Messages_Version_"); //We assume that there is only one table in the database.
}

bool DatabaseSupport::UpdateDatabase()
{
    return true;
}

bool DatabaseSupport::RenameSQLTable(QString oldName, QString newName)
{
    QSqlQuery query("ALTER TABLE " + oldName + " RENAME TO " + newName + ";");

    if (!query.isActive()) {
        QMessageBox::warning(0, "Error", "Cannot rename table '" + oldName + "'. Error details: " +
                             query.lastError().text() + "\n Please contact your support team for assistance.");
        return false;
    }

    query.finish();
    return true;
}

