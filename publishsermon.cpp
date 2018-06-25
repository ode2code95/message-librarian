#include "publishsermon.h"
#include "ui_publishsermon.h"

PublishSermon::PublishSermon(QSettings *settings, QSqlTableModel *mainWinTableModel, QPersistentModelIndex *index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PublishSermon)
{
    ui->setupUi(this);
}

PublishSermon::~PublishSermon()
{
    delete ui;
}
