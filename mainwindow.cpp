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


#define NOTIMPLEMENTEDTEXT \
    "We're sorry, this feature is not available in this release."\
    "Please stay tuned for further developements!"\

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    globalSettings = new QSettings("Anabaptist Codeblocks Foundation", "Audio Sermon Organizer", this);

    sermonTableModel = new QSqlTableModel(this, QSqlDatabase::database());
    sermonTableModel->setTable("sermon");
    sermonTableModel->setSort(Sermon_Date, Qt::AscendingOrder);
    sermonTableModel->select();

    sermonTableModel->setHeaderData(Sermon_ID, Qt::Horizontal, "Audio Binding");
    sermonTableModel->setHeaderData(Sermon_Title, Qt::Horizontal, "Title");
    sermonTableModel->setHeaderData(Sermon_Speaker, Qt::Horizontal, "Speaker");
    sermonTableModel->setHeaderData(Sermon_Location, Qt::Horizontal, "Location");
    sermonTableModel->setHeaderData(Sermon_Date, Qt::Horizontal, "Date");
    sermonTableModel->setHeaderData(Sermon_Description, Qt::Horizontal, "Description");
    sermonTableModel->setHeaderData(Sermon_Transcription, Qt::Horizontal, "Transcription");

    ui->mainSermonTableView->setModel(sermonTableModel);
    ui->mainSermonTableView->horizontalHeader()->moveSection(Sermon_ID, Sermon_Description);    //Awsome code!! moves columns around in the table view without changing the order in the sql table itself!
    ui->mainSermonTableView->horizontalHeader()->setSectionResizeMode(Sermon_Title, QHeaderView::Stretch);
    ui->mainSermonTableView->resizeColumnsToContents();
    ui->mainSermonTableView->setColumnWidth(Sermon_ID, 130);
    ui->mainSermonTableView->setColumnWidth(Sermon_Transcription, 130);
    ui->mainSermonTableView->setItemDelegateForColumn(Sermon_ID, new StatusIndicatorDelegate);  //Invokes our custom state indicator icons for certain data types in our table.
    ui->mainSermonTableView->setItemDelegateForColumn(Sermon_Transcription, new StatusIndicatorDelegate);   //Same here.

    ui->mainSermonTableView->selectRow(globalSettings->value("metadata/lastActiveSermon", "0").toInt());

    setCentralWidget(ui->mainSermonTableView);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete globalSettings;
    delete sermonTableModel;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //Save current sermon selection from main table.
    globalSettings->setValue("metadata/lastActiveSermon", ui->mainSermonTableView->currentIndex().row());
    event->accept();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Audio Sermon Organizer", ABOUTTEXT);
}

void MainWindow::on_actionExit_triggered()
{
    // This code runs AFTER the main window is destroyed. All saving of data must be done in the reimplemented 'closeEvent'!
    QCoreApplication::quit();
}

void MainWindow::on_actionEdit_triggered()
{
    EditSermon newwin(globalSettings, sermonTableModel, this, "", ui->mainSermonTableView->currentIndex()); //Here we will set the ID of the currently selected sermon in MainWindow
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

void MainWindow::on_actionNew_triggered()
{
    EditSermon newwin(globalSettings, sermonTableModel, this, "$#_Create_New"); //Invoke Edit Sermon ui with new blank sermon entry at the end.
    newwin.exec();
}

void MainWindow::on_actionOpen_triggered()
{
    QMessageBox::critical(this, "Not Implemented Yet", NOTIMPLEMENTEDTEXT);
    //ui->statusBar->set
}

void MainWindow::on_actionSearch_triggered()
{
    QMessageBox::critical(this, "Not Implemented Yet", NOTIMPLEMENTEDTEXT);
}

void MainWindow::on_actionPublish_triggered()
{
    QMessageBox::critical(this, "Not Implemented Yet", NOTIMPLEMENTEDTEXT);
}

void MainWindow::on_mainSermonTableView_doubleClicked(const QModelIndex &index)
{
    //This will eventually OPEN the sermon, not EDIT it.
    EditSermon newwin(globalSettings, sermonTableModel, this, "", index); //Invoke Edit Sermon ui with current selected row from MainWindow as the current Sermon.
    newwin.exec();
}
