#include "databasesupport.h"
#include "editsermon.h"

#include <QDate>

DatabaseSupport::DatabaseSupport()
{

}

bool DatabaseSupport::InitDatabase()
{
    QSettings settings("Anabaptist Codeblocks Foundation", "Audio Sermon Organizer");
    QString dbpath = settings.value("paths/databaseLocation", "C:/Audio Sermon Database").toString();
    if (!QDir(dbpath).exists()) {
        qDebug("Creating default database directory . . .");
        if(!QDir(dbpath).mkpath(dbpath))
            qDebug("Error creating parent directory!");
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbpath + "/Audio_Sermon_Database.db");
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
    model.setTable("sermon");

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
    // No error occurred. Proceed with program loading.
    return true;
}

bool DatabaseSupport::CreateNewDatabase()
{
    QSqlQuery query("CREATE TABLE sermon ("
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

    return true;
}

