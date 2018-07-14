#include "mainwindow.h"
#include "databasesupport.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Create connection to database, abort on error

    if (!DatabaseSupport::InitDatabase())
        return 1;

    if (!DatabaseSupport::LoadDatabase())
        return 2;

    MainWindow w;
    w.showMaximized();
    
    return a.exec();
}
