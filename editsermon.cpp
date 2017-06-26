#include "editsermon.h"
#include "ui_editsermon.h"

EditSermon::EditSermon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSermon)
{
    ui->setupUi(this);
}

EditSermon::~EditSermon()
{
    delete ui;
}
