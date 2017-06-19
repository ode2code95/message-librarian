#include "newentry.h"
#include "ui_newentry.h"
#include <QFileDialog>
#include <QDate>
#include <Qmessagebox>
#include <QUuid>

NewEntry::NewEntry(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntry), gsettings(settings)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

NewEntry::~NewEntry()
{
    gsettings = NULL;
    delete ui;
}

void NewEntry::on_cancelButton_clicked()
{
    QDialog::close();
}

void NewEntry::on_pushButton_clicked()
{
    QString defaultOpenFrom = gsettings->value("paths/importFrom", "D:/").toString();
    importFileNames = QFileDialog::getOpenFileNames(this,
          tr("Please select a sermon recording . . ."), defaultOpenFrom, tr("Audio Files (*.wav *.mp3)"));
    if (importFileNames.isEmpty()) {
        return;
    }
    QString fileName = "";
    QString fileNamesList = "";
    foreach (fileName, importFileNames) {
        QString newlineChar = "";
        if (importFileNames.count() > 1) {
            newlineChar = "\n";
        }
        QStringList splitPath = fileName.split("/");
        QString nameOnly = splitPath.at(splitPath.count() - 1);
        fileNamesList += nameOnly + newlineChar;
    }
    ui->importAudioFiles_readOnlyEdit->setText(fileNamesList);
}

void NewEntry::on_saveButton_clicked()
{
    if (importFileNames.isEmpty() ||
            ui->title_lineEdit->text() == "" ||
            ui->speaker_lineEdit->text() == "" ||
            ui->location_lineEdit->text() == "" ||
            ui->description_lineEdit->text() == "") {
        QMessageBox::critical(this, "Invalid Entry!", "One or more of the fields is <b><i>empty!</i></b> Please check your entries and try again!");
        return;
    }
    QString destDir = gsettings->value("paths/databaseLocation", "C:/Audio Sermon Database").toString();
    QString UUID = QUuid::createUuid().toString();
    QDir objDestDir(destDir);
    objDestDir.mkdir(UUID);
    QString fileName;
    foreach (fileName, importFileNames) {
        QStringList splitPath = fileName.split("/");
        QString nameOnly = splitPath.at(splitPath.count() - 1);
        QFile::copy(fileName, destDir + "/" + UUID + "/" + nameOnly);
    }
    /*Database recording of metadata fields will go here
    . . .
    Also remember to record UUID!
    . . .
    */
}
