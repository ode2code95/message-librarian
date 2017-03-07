#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QFileDialog>

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

void SettingsWindow::on_browseLocation_pushButton_clicked()
{
    ui->databaseLocation_lineEdit->setText(QFileDialog::getExistingDirectory(this, "Choose a folder . . ."));
}
