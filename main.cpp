#include "mainwindow.h"
#include "databasesupport.h"
#include <QApplication>

void InvokeUpdater();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Create connection to database, abort on error

    if (!DatabaseSupport::InitDatabase())
        return 1;

    if (!DatabaseSupport::CheckDatabaseVersion()) {
        if (DatabaseSupport::GetDbVersion() == -1) {
            return 2; //An error occurred and has already been reported to the user.
        }
        if (DatabaseSupport::GetDbVersion() == -2) {
            InvokeUpdater();
            return 3;
        }
        if (!DatabaseSupport::UpdateDatabase()) {
            return 4;
        }
    }

    if (!DatabaseSupport::LoadDatabase())
        return 5;

    MainWindow w;
    w.showMaximized();
    
    return a.exec();
}

void InvokeUpdater() //Get file location of running EXE in order to find updater binary.
{
    QMessageBox::warning(0, "Software Updater", "No online updates are available. Please run an update from a thumb drive or CD.");
}
