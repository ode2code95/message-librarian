#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>

bool InitDatabase();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Create connection to database, abort on error
    if (!InitDatabase())
        return 1;

    MainWindow w;
    w.show();
    
    return a.exec();
}

bool InitDatabase()
{
    QSettings settings("Anabaptist Codeblocks Foundation", "Audio Sermon Organizer");
    QString dbpath = settings.value("paths/databaseLocation", "C:/Audio Sermon Database").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbpath + "/Audio_Sermon_Database.db");
    bool ok = db.open();

    if (!ok) {
        QMessageBox::warning(0, "Error", "Cannot open database. Contact your support team for assistance.");
    }

    return ok;
}

