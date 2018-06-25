#ifndef PUBLISHSERMON_H
#define PUBLISHSERMON_H

#include <QDialog>
#include <QSettings>
#include <QSqlTableModel>

namespace Ui {
class PublishSermon;
}

class PublishSermon : public QDialog
{
    Q_OBJECT

public:
    explicit PublishSermon(QSettings *settings, QSqlTableModel *mainWinTableModel, QPersistentModelIndex *index = new QPersistentModelIndex(), QWidget *parent = 0);
    ~PublishSermon();

private:
    Ui::PublishSermon *ui;
};

#endif // PUBLISHSERMON_H
