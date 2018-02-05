#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QSqlTableModel>
#include "statusindicatordelegate.h"

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
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetCurrentModelIndex(QPersistentModelIndex *index);
    
private slots:
    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_actionEdit_triggered();

    void on_actionPreferences_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSearch_triggered();

    void on_actionPublish_triggered();

    void on_mainSermonTableView_doubleClicked(const QModelIndex &index);

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSettings *globalSettings;
    QSqlTableModel *sermonTableModel;
    QPersistentModelIndex *currentModelIndex;
};

#endif // MAINWINDOW_H
