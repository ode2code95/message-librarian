#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include "editsermon.h"

#define ABOUTTEXT \
    "<i><b>Audio Sermon Organizer</b> © 2017 by Stanley B. Gehman.</i><p>"\
    "This software is intended to assist the organizational efforts of those "\
    "responsible to maintain audio sermon libraries. It is free of charge; however, "\
    "if you wish to contribute to the project either through code or financial "\
    "assistance, we do welcome your input. Due to the nature of free software, "\
    "we do not provide any warranty, expressed or implied.<p>Tech support may be "\
    "somewhat limited, but we encourage you to report bugs and request new "\
    "features. We look forward to hearing from you.<p>Contact: <b>Stanley Gehman</b> "\
    "Email: <b><u>sg.tla@emypeople.net</u></b> Phone: <b>(217) 254 - 4403</b>"\

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    globalSettings = new QSettings("Anabaptist Codeblocks Foundation", "Audio Sermon Organizer", this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Audio Sermon Organizer", ABOUTTEXT);
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionEdit_triggered()
{
    EditSermon newwin(globalSettings, this, 0); //Store id in settings and restore here.
    newwin.exec();
}

void MainWindow::on_actionPreferences_triggered()
{
    SettingsWindow swin(globalSettings, this);
    swin.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}
