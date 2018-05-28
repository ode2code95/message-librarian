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

    connect(ui->title_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->speaker_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->location_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->from_dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(beginSearch()));
    connect(ui->to_dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(beginSearch()));
    connect(ui->description_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(beginSearch()));
    connect(ui->caseSensitive_checkBox, SIGNAL(toggled(bool)), this, SLOT(beginSearch()));
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

    ui->clearSearch_pushButton->setEnabled(true);
    Qt::CaseSensitivity caseSense = ui->caseSensitive_checkBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    QHash<int, QRegExp> searchHash;
    searchHash.insert(Sermon_Title, QRegExp(ui->title_lineEdit->text(), caseSense));
    searchHash.insert(Sermon_Speaker, QRegExp(ui->speaker_lineEdit->text(), caseSense));
    searchHash.insert(Sermon_Location, QRegExp(ui->location_lineEdit->text(), caseSense));
    searchHash.insert(Sermon_Description, QRegExp(ui->description_lineEdit->text(), caseSense));
    mainSortFilterModel->setMultiFilterRegExp(searchHash);

    mainSortFilterModel->setFilterMinimumDate(ui->from_dateEdit->date(), false);
    mainSortFilterModel->setFilterMaximumDate(ui->to_dateEdit->date()); //Only need to invalidate filter once; save some processing
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
