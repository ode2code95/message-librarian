#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include <QFileDialog>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDate>

#include <QSqlQuery>

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
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    QString defaultOpenFrom = globalSettings->value("paths/importFrom", "D:/").toString();
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
          tr("Please select a sermon recording . . ."), defaultOpenFrom, tr("Audio Files (*.wav *.mp3)"));
    if (fileNames.count() == 0) {
        return;
    }

    /*QSqlQuery query("CREATE TABLE sermon ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "title VARCHAR(40) NOT NULL,"
                     "speaker VARCHAR(40) NOT NULL,"
                     "location VARCHAR(40) NOT NULL,"
                     "date VARCHAR(40) NOT NULL,"
                     "description VARCHAR(40) NOT NULL,"
                     "transcription VARCHAR(200) NOT NULL);", QSqlDatabase::database());

    if (!query.isActive()) {
        QMessageBox::warning(this, "Error", query.lastError().text());
    }*/

    QSqlTableModel model(this, QSqlDatabase::database());
    model.setTable("sermon");
    int row = 0;
    model.insertRows(row, 1);
    model.setData(model.index(row, 1), "The Fear of the Lord is the Beginning of Wisdom");
    model.setData(model.index(row, 2), "Lloyd Mast");
    model.setData(model.index(row, 3), "Mechanicsville Mennonite Church");
    model.setData(model.index(row, 4), QDate::currentDate());
    model.setData(model.index(row, 5), "4th Sunday Morning");
    model.setData(model.index(row, 6), "Greetings in Jesus' name this morning . . .");
    if (!model.submitAll()) {
        QMessageBox::warning(this, "Error", model.lastError().text());
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    SettingsWindow swin(globalSettings, this);
    swin.exec();
}
