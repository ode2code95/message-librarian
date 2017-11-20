#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QFileDialog>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::SettingsWindow(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow), gsettings(settings)
{
    ui->setupUi(this);
    ui->databaseLocation_lineEdit->setText(gsettings->value("paths/databaseLocation", "C:/Audio Sermon Database").toString());
    ui->importAudioFrom_lineEdit->setText(gsettings->value("paths/importFrom", "D:/").toString());
    ui->unpairedStorage_lineEdit->setText(gsettings->value("paths/unpairedStorage", gsettings->value("paths/databaseLocation", "C:/Audio Sermon Database").toString() + "/Unpaired Audio File Storage").toString());
}

SettingsWindow::~SettingsWindow()
{
    gsettings = NULL;
    delete ui;
}

void SettingsWindow::on_databaseLocation_lineEdit_textChanged(const QString &newPath)
{
    gsettings->setValue("paths/databaseLocation", newPath);
}

void SettingsWindow::on_importAudioFrom_lineEdit_textChanged(const QString &newPath)
{
    gsettings->setValue("paths/importFrom", newPath);
}

void SettingsWindow::on_unpairedStorage_lineEdit_textChanged(const QString &newPath)
{
    gsettings->setValue("paths/unpairedStorage", newPath);
}

void SettingsWindow::on_browseLocation_pushButton_clicked()
{
    QString newpath = QFileDialog::getExistingDirectory(this, "Choose a drive or folder . . .", ui->databaseLocation_lineEdit->text());
    if (newpath != "") {   //don't set empty path if user cancelled dialog.
        ui->databaseLocation_lineEdit->setText(newpath);
    }
}

void SettingsWindow::on_browseImportAudioFrom_pushButton_clicked()
{
    QString newpath = QFileDialog::getExistingDirectory(this, "Choose a drive or folder . . .", ui->importAudioFrom_lineEdit->text());
    if (newpath != "") {   //don't set empty path if user cancelled dialog.
        ui->importAudioFrom_lineEdit->setText(newpath);
    }
}

void SettingsWindow::on_browseUnpairedStorage_pushButton_clicked()
{
    QString newpath = QFileDialog::getExistingDirectory(this, "Choose a drive or folder . . .", ui->unpairedStorage_lineEdit->text());
    if (newpath != "") {   //don't set empty path if user cancelled dialog.
        ui->unpairedStorage_lineEdit->setText(newpath);
    }
}
