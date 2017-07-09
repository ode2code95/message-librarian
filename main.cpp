#include "mainwindow.h"
#include "databasesupport.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Create connection to database, abort on error
    DatabaseSupport dbHandler;

    if (!dbHandler.InitDatabase())
        return 1;

    if (!dbHandler.LoadDatabase())
        return 2;

    MainWindow w;
    w.show();
    
    return a.exec();
}
