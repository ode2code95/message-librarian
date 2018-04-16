#include "findsermon.h"
#include "ui_findsermon.h"

FindSermon::FindSermon(QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindSermon), mainTableModel(model)
{
    ui->setupUi(this);

    ui->from_dateEdit->setDate(QDate::currentDate().addYears(-5));
    ui->to_dateEdit->setDate(QDate::currentDate());

    /*
     * We have decided to make the search window top-level, so that it floats on top of the Main Window. <- TODO
     * Instead of a built-in search-results table, candidates will be SELECTED (with setFilter statement) in the main window <- TODO
     * until either the search window is closed or the cancel search button in the search window is clicked. <- DONE!
     * The cancel search button will return to default SELECT of main table, with all search fields cleared. <- DONE!
     */

    connect(ui->title_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->speaker_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->location_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->from_dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(beginSearch()));
    connect(ui->to_dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(beginSearch()));
    connect(ui->description_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
}

FindSermon::~FindSermon()
{
    delete ui;
    mainTableModel = NULL;
}

void FindSermon::closeEvent(QCloseEvent *event)
{
    mainTableModel->setFilter("");
    event->accept();
}

void FindSermon::beginSearch()
{
    //Here we will take the data from the search fields and compile an SQL querry for use below.
    mainTableModel->setFilter("title='XXXXX'"); //"takes a standard SQL WHERE clause without the WHERE"
}

void FindSermon::on_clearSearch_pushButton_clicked()
{
    ui->title_lineEdit->clear();
    ui->speaker_lineEdit->clear();
    ui->location_lineEdit->clear();
    ui->from_dateEdit->setDate(QDate::currentDate().addYears(-5));
    ui->to_dateEdit->setDate(QDate::currentDate());
    ui->description_lineEdit->clear();

    mainTableModel->setFilter("");
}
