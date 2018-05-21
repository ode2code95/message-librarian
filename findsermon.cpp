#include "findsermon.h"
#include "ui_findsermon.h"

FindSermon::FindSermon(SermonSortFilterProxyModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindSermon), mainSortFilterModel(model)
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
    mainSortFilterModel = NULL;
}

void FindSermon::closeEvent(QCloseEvent *event)
{
    mainSortFilterModel->resetFilters();
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

        mainSortFilterModel->resetFilters();
        ui->clearSearch_pushButton->setEnabled(false);
        return;
    }

    // Use QSortFilterProxyModel Class here instead! Excellent idea! <- IN PROGRESS . . .
    // Also sort by header click too! <- DONE!

    ui->clearSearch_pushButton->setEnabled(true);

    QHash<int, QRegExp> searchHash;
    searchHash.insert(Sermon_Title, QRegExp(ui->title_lineEdit->text()));
    searchHash.insert(Sermon_Speaker, QRegExp(ui->speaker_lineEdit->text()));
    searchHash.insert(Sermon_Location, QRegExp(ui->location_lineEdit->text()));
    searchHash.insert(Sermon_Description, QRegExp(ui->description_lineEdit->text()));
    mainSortFilterModel->setMultiFilterRegExp(searchHash);

    mainSortFilterModel->setFilterMinimumDate(ui->from_dateEdit->date());
    mainSortFilterModel->setFilterMaximumDate(ui->to_dateEdit->date());
}

void FindSermon::on_clearSearch_pushButton_clicked()
{
    ui->title_lineEdit->clear();
    ui->speaker_lineEdit->clear();
    ui->location_lineEdit->clear();
    ui->from_dateEdit->setDate(QDate::currentDate().addYears(-5));
    ui->to_dateEdit->setDate(QDate::currentDate());
    ui->description_lineEdit->clear();

    mainSortFilterModel->resetFilters();
    ui->clearSearch_pushButton->setEnabled(false);
}
