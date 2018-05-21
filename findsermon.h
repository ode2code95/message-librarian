#ifndef FINDSERMON_H
#define FINDSERMON_H

#include <QDialog>
#include <QCloseEvent>

#include "sermonsortfilterproxymodel.h"
#include "mainwindow.h"

namespace Ui {
class FindSermon;
}

class FindSermon : public QDialog
{
    Q_OBJECT

public:
    explicit FindSermon(SermonSortFilterProxyModel *model, QWidget *parent = 0);
    ~FindSermon();

private slots:
    void beginSearch();

    void on_clearSearch_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::FindSermon *ui;
    SermonSortFilterProxyModel *mainSortFilterModel;
};

#endif // FINDSERMON_H
