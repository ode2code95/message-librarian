#include "editsermon.h"
#include "ui_editsermon.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>

EditSermon::EditSermon(QSettings *settings, QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::EditSermon), gsettings(settings)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    sermonTableModel = new QSqlTableModel(this, QSqlDatabase::database());
    sermonTableModel->setTable("sermon");
    sermonTableModel->setSort(Sermon_Date, Qt::AscendingOrder);
    sermonTableModel->select();

    sermonDataMapper = new QDataWidgetMapper(this);
    sermonDataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    sermonDataMapper->setModel(sermonTableModel);
    sermonDataMapper->addMapping(ui->title_lineEdit, Sermon_Title);
    sermonDataMapper->addMapping(ui->speaker_lineEdit, Sermon_Speaker);
    sermonDataMapper->addMapping(ui->location_lineEdit, Sermon_Location);
    sermonDataMapper->addMapping(ui->dateEdit, Sermon_Date);
    sermonDataMapper->addMapping(ui->description_lineEdit, Sermon_Description);

    if (id != -1) {
        for (int row = 0; row < sermonTableModel->rowCount(); ++row) {
            QSqlRecord record = sermonTableModel->record(row);
            if (record.value(Sermon_ID).toInt() == id) {
                sermonDataMapper->setCurrentIndex(row);
                break;
            }
            else {
                sermonDataMapper->toFirst();
            }
        }
    } else {
        //Insert new row at the end of the table.
        int row = sermonTableModel->rowCount() - 1;
        sermonTableModel->insertRow(row);
        sermonDataMapper->setCurrentIndex(row);
    }

    connect(ui->pB_First, SIGNAL(clicked()), this, SLOT(toFirstSermon()));
    connect(ui->pB_Previous, SIGNAL(clicked()), this, SLOT(toPreviousSermon()));
    connect(ui->pB_Next, SIGNAL(clicked()), this, SLOT(toNextSermon()));
    connect(ui->pB_Last, SIGNAL(clicked()), this, SLOT(toLastSermon()));
}

EditSermon::~EditSermon()
{
    gsettings = NULL;
    delete sermonTableModel;
    delete sermonDataMapper;
    delete ui;
}

void EditSermon::toFirstSermon()
{
    sermonDataMapper->toFirst();
    UpdateRecordIndexLabel();
}

void EditSermon::toPreviousSermon()
{
    sermonDataMapper->toPrevious();
    UpdateRecordIndexLabel();
}

void EditSermon::toNextSermon()
{
    sermonDataMapper->toNext();
    UpdateRecordIndexLabel();
}

void EditSermon::toLastSermon()
{
    sermonDataMapper->toLast();
    UpdateRecordIndexLabel();
}

void EditSermon::on_pB_Browse_clicked()
{
    QString defaultOpenFrom = gsettings->value("paths/importFrom", "D:/").toString();
    importFileNames = QFileDialog::getOpenFileNames(this,
          tr("Please select a sermon recording . . ."), defaultOpenFrom, tr("Audio Files (*.wav *.mp3)"));
    if (importFileNames.isEmpty()) {
        return;
    }
    QString fileName = "";
    QString fileNamesList = "";
    foreach (fileName, importFileNames) {
        QString newlineChar = "";
        if (importFileNames.count() > 1) {
            newlineChar = "\n";
        }
        QStringList splitPath = fileName.split("/");
        QString nameOnly = splitPath.at(splitPath.count() - 1);
        fileNamesList += nameOnly + newlineChar;
    }
    ui->importAudioFiles_readOnlyEdit->setText(fileNamesList);
}


void EditSermon::on_pB_Close_clicked()
{
    if (importFileNames.isEmpty() ||
            ui->title_lineEdit->text() == "" ||
            ui->speaker_lineEdit->text() == "" ||
            ui->location_lineEdit->text() == "" ||
            ui->description_lineEdit->text() == "") {
        QMessageBox::critical(this, "Invalid Entry!", "One or more of the fields is <b><i>empty!</i></b> Please check your entries and try again!");
        return;
    }
    QString destDir = gsettings->value("paths/databaseLocation", "C:/Audio Sermon Database").toString();
    QString UUID = QUuid::createUuid().toString();
    QDir objDestDir(destDir);
    objDestDir.mkdir(UUID);
    QString fileName;
    foreach (fileName, importFileNames) {
        QStringList splitPath = fileName.split("/");
        QString nameOnly = splitPath.at(splitPath.count() - 1);
        QFile::copy(fileName, destDir + "/" + UUID + "/" + nameOnly);
    }

    /*Database recording of metadata fields will go here
    . . .
    Also remember to record UUID!
    . . .
    */
    QDialog::close();
}

void EditSermon::on_pB_Add_clicked()
{
    int row = sermonDataMapper->currentIndex();
    bool result = sermonDataMapper->submit();
    if (!result) {
        QMessageBox::about(this, "Result printout", "Submit error!\nError Text: " + sermonTableModel->lastError().text());
    } else {
        QMessageBox::about(this, "Result printout", "No error found.");
    }
    sermonTableModel->insertRow(row);
    sermonDataMapper->setCurrentIndex(row);

    ui->title_lineEdit->clear();
    ui->speaker_lineEdit->clear();
    ui->location_lineEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->description_lineEdit->clear();
    ui->title_lineEdit->setFocus();
}

void EditSermon::on_pB_Delete_clicked()
{
    int row = sermonDataMapper->currentIndex();
    sermonTableModel->removeRow(row);
    sermonDataMapper->submit();
    sermonDataMapper->setCurrentIndex(qMin(row, sermonTableModel->rowCount() - 1));
}

void EditSermon::UpdateRecordIndexLabel()
{
    ui->recordIndex_lbl->setText(QString("Record %1 of %2").arg(sermonDataMapper->currentIndex() + 1).arg(sermonTableModel->rowCount()));
}
