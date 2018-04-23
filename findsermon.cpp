#include "findsermon.h"
#include "ui_findsermon.h"

FindSermon::FindSermon(QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindSermon), mainTableModel(model)
{
    ui->setupUi(this);

    ui->clearSearch_pushButton->setEnabled(false);
    ui->from_dateEdit->setDate(QDate::currentDate().addYears(-5));
    ui->to_dateEdit->setDate(QDate::currentDate());

    /*
     * We have decided to make the search window top-level, so that it floats on top of the Main Window. <- DONE!
     * Instead of a built-in search-results table, candidates will be SELECTED (with setFilter statement) in the main window <- TODO
     *  (//NEXT PROJECT: Incorporate partial matching. (I think this will require the SQL LIKE statement.))
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
    if (ui->title_lineEdit->text() == "" &&
            ui->speaker_lineEdit->text() == "" &&
            ui->location_lineEdit->text() == "" &&
            ui->from_dateEdit->date() == QDate::currentDate().addYears(-5) &&
            ui->to_dateEdit->date() == QDate::currentDate() &&
            ui->description_lineEdit->text() == "") { //this edit resulted in default values, so we can reset the search.

        mainTableModel->setFilter("");
        ui->clearSearch_pushButton->setEnabled(false);
        return;
    }

    ui->clearSearch_pushButton->setEnabled(true);
    //Here we will take the data from the search fields and compile an SQL querry for use below.
    QString titleSearchString = "title > 'the' AND "; //NEXT PROJECT: Incorporate partial matching. (I think this will require the SQL LIKE statement.)
    QString dateSearchString = "date >= '" + ui->from_dateEdit->date().toString("yyyy-MM-dd") +
            "' AND date <= '" + ui->to_dateEdit->date().toString("yyyy-MM-dd") + "'";
    mainTableModel->setFilter(titleSearchString + dateSearchString); //"takes a standard SQL WHERE clause without the WHERE"
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
    ui->clearSearch_pushButton->setEnabled(false);
}
