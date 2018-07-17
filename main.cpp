#include "mainwindow.h"
#include "databasesupport.h"
#include <QApplication>

#define CMDLN_SWITCH_SETUP \
    "-init-setup"\

QCoreApplication* createApplication(int &argc, char *argv[]);
int LoadNormal(QApplication *a);
int InitSetup();
void InvokeUpdater();

int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    int errorCode = 0;

    if (qobject_cast<QApplication *>(app.data())) {
        // proceed with normal loading
        errorCode = LoadNormal(static_cast <QApplication *> (app.data()));
    } else {
        // setup switch detected
        errorCode = InitSetup();
    }

    return errorCode;
}

QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
        if (!qstrcmp(argv[i], CMDLN_SWITCH_SETUP))
            return new QCoreApplication(argc, argv);
    return new QApplication(argc, argv);
}

int LoadNormal(QApplication *a)
{
    //Create connection to database, abort on error

    if (!DatabaseSupport::InitDatabase())
        return 1;

    if (!DatabaseSupport::GetSkipVersionCheck()) {
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
    }

    if (!DatabaseSupport::LoadDatabase())
        return 5;

    MainWindow w;
    w.showMaximized();

    return a->exec();
}

int InitSetup()
{
    if (!DatabaseSupport::InitDatabase(true))
        return 1;

    if (!DatabaseSupport::LoadDatabase(true))
        return 2;

    DatabaseSupport::CloseConnectionIfOpen();

    return 0;
}

void InvokeUpdater() //Get file location of running EXE in order to find updater binary.
{
    QMessageBox::warning(0, "Software Updater", "No online updates are available. Please run an update from a thumb drive or CD.");
}
