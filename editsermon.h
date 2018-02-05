#ifndef EDITSERMON_H
#define EDITSERMON_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QDate>
#include <Qmessagebox>
#include <QUuid>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QCloseEvent>

#include "mainwindow.h" //Imports sermon table enum

namespace Ui {
class EditSermon;
}

class EditSermon : public QDialog
{
    Q_OBJECT

public:
    explicit EditSermon(QSettings *settings, QSqlTableModel *mainWinTableModel, MainWindow *parent = 0, QString id = "", QPersistentModelIndex *index = new QPersistentModelIndex());
    ~EditSermon();

private slots:
    void toFirstSermon();

    void toPreviousSermon();

    void toNextSermon();

    void toLastSermon();

    void on_pB_Close_clicked();

    void on_pB_Browse_clicked();

    void on_pB_Add_clicked();

    void on_pB_Delete_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::EditSermon *ui;
    QSettings *gsettings;
    QStringList audioFileNames;
    QSqlTableModel *sermonTableModel;
    QDataWidgetMapper *sermonDataMapper;
    MainWindow *parentWindow;

    void UpdateRecordIndexLabel();
    bool ValidateEntry();
    void GenerateNewEntry();
    void RemoveSermon(bool permanentlyDeleteFiles);
    void RemoveEntry();
    void UpdateAudioFileListing();
};

#endif // EDITSERMON_H
