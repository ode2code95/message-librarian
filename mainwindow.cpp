#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
//#include "sermontree.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sermons = new SermonTree();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    QString defaultOpenFrom = "";//sermons.settings.importFrom;
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open Audio File"), defaultOpenFrom, tr("Audio Files (*.wav *.mp3)"));
}
