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

enum {
    Sermon_ID = 0,
    Sermon_Title = 1,
    Sermon_Speaker = 2,
    Sermon_Location = 3,
    Sermon_Date = 4,
    Sermon_Description = 5,
    Sermon_Transcription = 6
};

namespace Ui {
class EditSermon;
}

class EditSermon : public QDialog
{
    Q_OBJECT

public:
    explicit EditSermon(QSettings *settings, QWidget *parent = 0, QString id = "");
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

    void UpdateRecordIndexLabel();
    void GenerateNewEntry();
    void UpdateAudioFileListing();
};

#endif // EDITSERMON_H
