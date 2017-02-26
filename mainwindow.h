#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "sermontree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    SermonTree *sermons;
};

#endif // MAINWINDOW_H
