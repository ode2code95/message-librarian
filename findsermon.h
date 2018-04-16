#ifndef FINDSERMON_H
#define FINDSERMON_H

#include <QDialog>
#include <QSqlTableModel>
#include <QCloseEvent>

namespace Ui {
class FindSermon;
}

class FindSermon : public QDialog
{
    Q_OBJECT

public:
    explicit FindSermon(QSqlTableModel *model, QWidget *parent = 0);
    ~FindSermon();

private slots:
    void beginSearch();

    void on_clearSearch_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::FindSermon *ui;
    QSqlTableModel *mainTableModel;
};

#endif // FINDSERMON_H
