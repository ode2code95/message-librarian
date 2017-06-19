#ifndef NEWENTRY_H
#define NEWENTRY_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class NewEntry;
}

class NewEntry : public QDialog
{
    Q_OBJECT

public:
    explicit NewEntry(QSettings *settings, QWidget *parent = 0);
    ~NewEntry();

private slots:
    void on_cancelButton_clicked();

    void on_pushButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::NewEntry *ui;
    QSettings *gsettings;
    QStringList importFileNames;
};

#endif // NEWENTRY_H
