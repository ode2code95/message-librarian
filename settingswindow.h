#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    explicit SettingsWindow(QSettings *settings, QWidget *parent = 0);
    ~SettingsWindow();

private slots:
    void on_databaseLocation_lineEdit_textChanged(const QString &newPath);

    void on_browseLocation_pushButton_clicked();

    void on_browseImportAudioFrom_pushButton_clicked();

    void on_importAudioFrom_lineEdit_textChanged(const QString &newPath);

private:
    Ui::SettingsWindow *ui;
    QSettings *gsettings;
};

#endif // SETTINGSWINDOW_H
