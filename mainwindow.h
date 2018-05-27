#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QSqlTableModel>
#include "databasesupport.h"
#include "statusindicatordelegate.h"
#include "findsermon.h"
#include "sermonsortfilterproxymodel.h"

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
    SermonSortFilterProxyModel *sortFilterSermonModel;
    FindSermon *findwin;
};

#endif // MAINWINDOW_H
