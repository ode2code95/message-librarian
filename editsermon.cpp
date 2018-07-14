#include "editsermon.h"
#include "ui_editsermon.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>

#include <QDebug> //may remove when debugging is finished.

//
EditSermon::EditSermon(QSettings *settings, QSqlTableModel *mainWinTableModel, MainWindow *parent, QString id, QPersistentModelIndex *index) :
    QDialog(parent),
    ui(new Ui::EditSermon), gsettings(settings), sermonTableModel(mainWinTableModel), parentWindow(parent)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());

    sermonDataMapper = new QDataWidgetMapper(this);
    sermonDataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    sermonDataMapper->setModel(sermonTableModel);
    sermonDataMapper->addMapping(ui->title_lineEdit, Sermon_Title);
    sermonDataMapper->addMapping(ui->speaker_lineEdit, Sermon_Speaker);
    sermonDataMapper->addMapping(ui->location_lineEdit, Sermon_Location);
    sermonDataMapper->addMapping(ui->dateEdit, Sermon_Date);
    sermonDataMapper->addMapping(ui->description_lineEdit, Sermon_Description);

    if (id == "$#_Create_New" || sermonTableModel->rowCount() == 0) {
        //User called for new record, or else we need to create one because the table is empty.
        //Insert new row at the end of the table.
        qDebug("New record requested. Initialize table with new row.");
        int row = qMax(0, sermonTableModel->rowCount());    //added 'qMax' statement to protect against a negative row reference.
        sermonTableModel->insertRow(row);
        sermonDataMapper->setCurrentIndex(row);
    } else if (id == "" && index->isValid()) {
        sermonDataMapper->setCurrentIndex(index->row());
        qDebug(QString("Clicked column = %1").arg(index->column()).toLocal8Bit());
        switch (index->column())
        {
        case Sermon_ID:
            if (index->data().isNull()) {
                show(); //show the Edit dialog first, then click the Browse button.
                ui->pB_Browse->click();
            }
            break;
        case Sermon_Title:
            ui->title_lineEdit->setFocus();
            break;
        case Sermon_Speaker:
            ui->speaker_lineEdit->setFocus();
            break;
        case Sermon_Location:
            ui->location_lineEdit->setFocus();
            break;
        case Sermon_Date:
            ui->dateEdit->setFocus();
            break;
        case Sermon_Description:
            ui->description_lineEdit->setFocus();
            break;
        case Sermon_Transcription:
            //No field for this in Edit. Do nothing
            break;
        default:
            ui->title_lineEdit->setFocus();
            break;
        }
    } else {
        bool foundMatch = false;
        for (int row = 0; row < sermonTableModel->rowCount(); ++row) {
            QSqlRecord record = sermonTableModel->record(row);
            if (record.value(Sermon_ID).toString() != "" && record.value(Sermon_ID).toString() == id) {
                foundMatch = true;
                sermonDataMapper->setCurrentIndex(row);
                break;
            } else if ((row == sermonTableModel->rowCount() - 1) && !foundMatch) {
                qDebug("No match found! Initializing table with first entry.");
                sermonDataMapper->toFirst();
            }
        }
    }
    UpdateRecordIndexLabel();

    connect(ui->pB_First, SIGNAL(clicked()), this, SLOT(toFirstSermon()));
    connect(ui->pB_Previous, SIGNAL(clicked()), this, SLOT(toPreviousSermon()));
    connect(ui->pB_Next, SIGNAL(clicked()), this, SLOT(toNextSermon()));
    connect(ui->pB_Last, SIGNAL(clicked()), this, SLOT(toLastSermon()));
}

EditSermon::~EditSermon()
{
    gsettings = NULL;
    sermonTableModel = NULL;
    delete sermonDataMapper;
    delete ui;
}

void EditSermon::toFirstSermon()
{
    if (!ValidateEntry())
        return; //Dialog missing some data and user opted to continue editing.

    sermonDataMapper->submit();
    sermonDataMapper->toFirst();
    UpdateRecordIndexLabel();
}

void EditSermon::toPreviousSermon()
{
    if (!ValidateEntry())
        return; //Dialog missing some data and user opted to continue editing.

    sermonDataMapper->submit();
    sermonDataMapper->toPrevious();
    UpdateRecordIndexLabel();
}

void EditSermon::toNextSermon()
{
    if (!ValidateEntry())
        return; //Dialog missing some data and user opted to continue editing.

    sermonDataMapper->submit();
    sermonDataMapper->toNext();
    UpdateRecordIndexLabel();
}

void EditSermon::toLastSermon()
{
    if (!ValidateEntry())
        return; //Dialog missing some data and user opted to continue editing.

    sermonDataMapper->submit();
    sermonDataMapper->toLast();
    UpdateRecordIndexLabel();
}

void EditSermon::on_pB_Browse_clicked()
{
    QString defaultOpenFrom = gsettings->value("paths/importFrom", "D:/").toString();
    audioFileNames = QFileDialog::getOpenFileNames(this,
          tr("Please select a sermon recording . . ."), defaultOpenFrom, tr("Audio Files (*.wav *.mp3 *.wma)"));
    if (audioFileNames.isEmpty()) {
        return;
    }
    QString fileNamesList = "";
    foreach (const QString &fileName, audioFileNames) {
        QString newlineChar = "";
        if (audioFileNames.count() > 1) {
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
    QDialog::close();
}

void EditSermon::on_pB_Add_clicked()
{
    if (!ValidateEntry())
        return; //Dialog missing some data and user opted to continue editing.

    int row = sermonDataMapper->currentIndex();
    sermonDataMapper->submit();
    row++;
    sermonTableModel->insertRow(row);
    sermonDataMapper->setCurrentIndex(row);
    UpdateRecordIndexLabel();

    audioFileNames.clear(); //Re-set internal list of audio file names for the current entry.
    ui->importAudioFiles_readOnlyEdit->clear();
    ui->title_lineEdit->clear();
    ui->speaker_lineEdit->clear();
    ui->location_lineEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->description_lineEdit->clear();
    ui->title_lineEdit->setFocus();
}

void EditSermon::on_pB_Delete_clicked()
{
    int wresult = QMessageBox::warning(this, "Delete Entry", "Are you sure you want to delete this sermon? You can choose to keep just the audio if you want to.", "Cancel", "Yes, delete everything!", "No, please keep the audio files!");
    switch (wresult){
    case 0: //Cancel
        //
        break;
    case 1: //Delete Everything
        RemoveSermon(true);
        break;
    case 2: //Save the audio, but remove the entry
        RemoveSermon(false);
        break;
    }
}

void EditSermon::UpdateRecordIndexLabel()
{
    ui->recordIndex_lbl->setText(QString("<html><head/><body><p align=center><span style= color:#ff0000;>Record %1 of %2</span></p></body></html>").arg(sermonDataMapper->currentIndex() + 1).arg(sermonTableModel->rowCount()));
    UpdateAudioFileListing();
}

void EditSermon::closeEvent(QCloseEvent *event)
{
    if (!ValidateEntry())
        event->ignore();
    else {
        //save the current sermonIndex so that we can re-select it in the main window
        //Eventually the second argument will specify which field in the Edit Sermon window was last active.
        parentWindow->SetCurrentModelIndex(new QPersistentModelIndex(sermonTableModel->index(sermonDataMapper->currentIndex(), 1)));
        event->accept();
    }
}

bool EditSermon::ValidateEntry() {
    //Check to see if any fields have been modified
    bool checkFurther = true;
    if (audioFileNames.isEmpty() &&
            ui->title_lineEdit->text() == "" &&
            ui->speaker_lineEdit->text() == "" &&
            ui->location_lineEdit->text() == "" &&
            ui->dateEdit->date() == QDate::currentDate() &&
            ui->description_lineEdit->text() == "") {
        int rvalue = QMessageBox::question(this, "Discard?", "This blank entry has not been modified. Do you want to discard it?", QMessageBox::Yes, QMessageBox::No);

        if (rvalue == QMessageBox::Yes) {
            RemoveEntry();  //Do this because a new blank record gets inserted into the table model whenever we call for a new entry.
            checkFurther = false;
        }
    }
    //If so, check to make sure all fields are filled
    if (checkFurther && (audioFileNames.isEmpty() ||
            ui->title_lineEdit->text() == "" ||
            ui->speaker_lineEdit->text() == "" ||
            ui->location_lineEdit->text() == "" ||
            ui->description_lineEdit->text() == "")) {
        int svalue = QMessageBox::critical(this, "Warning!", "One or more of the fields is <b><i>empty!</i></b> If you ignore this message, this entry will be incomplete!",
                                           QMessageBox::Cancel, QMessageBox::Ignore, QMessageBox::NoButton);
        if (svalue == QMessageBox::Cancel)
            return false;
        qDebug("User ignored warning. Continuing with partial entry.");
    }

    qDebug("Dialog closing or preparing for a different entry. Saving changes . . .");

    int currow = sermonDataMapper->currentIndex();
    sermonDataMapper->submit();
    sermonDataMapper->setCurrentIndex(currow);

    //Check to see if UUID has been generated already. If not, make a new one.
    //All read/write operations will reference the directory that matches the UUID.
    //Therefore we do not need to explicitly store file names in the database.
    if (sermonTableModel->record(sermonDataMapper->currentIndex()).field(Sermon_ID).isNull()) {
        qDebug("Null Sermon_ID detected! Checking to see if we have files to save . . .");
        if (!audioFileNames.isEmpty()) {
            //No UUID but there are files to save,
            //so create a new entry and copy in the files.
            GenerateNewEntry();
        }
    }
    return true;
}

void EditSermon::GenerateNewEntry() {
    qDebug("Creating entry and copying files . . .");
    QString destDir = gsettings->value("paths/databaseLocation", "C:/Audio Message Library").toString();
    QString UUID = QUuid::createUuid().toString();
    QDir objDestDir(destDir);
    objDestDir.mkdir(UUID);
    foreach (const QString &fileName, audioFileNames) {
        QStringList splitPath = fileName.split("/");
        QString nameOnly = splitPath.at(splitPath.count() - 1);
        QFile::copy(fileName, destDir + "/" + UUID + "/" + nameOnly);
    }

    //Write the new UUID back to the database.
    sermonTableModel->setData(sermonTableModel->index(sermonDataMapper->currentIndex(), Sermon_ID), UUID);
    sermonTableModel->submit();
}

void EditSermon::RemoveSermon(bool permanentlyDeleteFiles)
{
    QSqlRecord currentEntry = sermonTableModel->record(sermonDataMapper->currentIndex());

    //Delete files only if the entry is paired to some audio files
    if (!currentEntry.field(Sermon_ID).isNull()) {
        QString sourceDir = gsettings->value("paths/databaseLocation", "C:/Audio Message Library").toString();
        QString UUID = currentEntry.value(Sermon_ID).toString();
        QDir objSourceDir = (sourceDir + "/" + UUID);

        if (!permanentlyDeleteFiles) {
            //Back up the files to the default un-paired storage bin.
            QString storageBinDir = gsettings->value("paths/unpairedStorage", gsettings->value("paths/databaseLocation", "C:/Audio Message Library").toString() + "/Unpaired Audio File Storage").toString();
            QString thisBackupDirName = currentEntry.value(Sermon_Title).toString() + " - " +
                    currentEntry.value(Sermon_Speaker).toString() + " - " +
                    currentEntry.value(Sermon_Date).toString();
            QDir objStorageBinDir(storageBinDir);
            if (!objStorageBinDir.exists())
                objStorageBinDir.mkpath(storageBinDir);
            bool createDirSuccess = objStorageBinDir.mkdir(thisBackupDirName);
            QString thisStorageBinDir = storageBinDir + "/" + thisBackupDirName;

            bool copySuccess = true;
            foreach (const QString &fileName, audioFileNames) {
                QStringList splitPath = fileName.split("/");
                QString nameOnly = splitPath.at(splitPath.count() - 1);
                copySuccess = QFile::copy(fileName, thisStorageBinDir + "/" + nameOnly);
            }

            //Show messagebox indicating success or failure of backup procedure.
            if (createDirSuccess && copySuccess) {
                QMessageBox::information(this, "Backup Audio Succeeded",
                                         QString("%1 files have been successfully moved to %2")
                                         .arg(audioFileNames.count())
                                         .arg(thisStorageBinDir));
            } else {
                QMessageBox::warning(this, "Backup Audio Failed",
                    "An error occurred attempting to backup the audio files to the unpaired\nstorage bin. Since not all files were saved, the entry will not be deleted.\nPlease contact your support team for further assistance.");
                if (QDir(thisStorageBinDir).exists())
                    QDir(thisStorageBinDir).removeRecursively();
                return;
            }
        }
        //Delete the files and their UUID directory.
        if (objSourceDir.exists()) {
            if (objSourceDir.removeRecursively())
                QMessageBox::information(this, "Audio Files Removed",
                                         QString("%1 audio files and their internal parent folder have been\nsuccessfully removed from internal storage.").arg(audioFileNames.count()));
            else
                QMessageBox::warning(this, "Audio File Removal Failed",
                    "An error occurred attempting to remove the audio files from their internal storage location.\nPlease contact your support team for further assistance.");
        }
    }

    //Delete the sermon metadata from the database and refresh the table model.
    RemoveEntry();
}

void EditSermon::RemoveEntry()
{
    int row = sermonDataMapper->currentIndex();
    sermonTableModel->removeRow(row);
    sermonTableModel->select(); // Added this to pull database changes back into the table model. Previously deleted rows remained as empty entries until the next reload of the window.
    sermonDataMapper->submit();
    sermonDataMapper->setCurrentIndex(qMin(row, sermonTableModel->rowCount() - 1));
    UpdateRecordIndexLabel();
}

void EditSermon::UpdateAudioFileListing() {
    audioFileNames.clear(); //Re-set internal list of audio file names for the current entry.
    ui->importAudioFiles_readOnlyEdit->clear();
    if (!sermonTableModel->record(sermonDataMapper->currentIndex()).field(Sermon_ID).isNull()) {
        //The current database field does contain a valid UUID.
        QString sourceDir = gsettings->value("paths/databaseLocation", "C:/Audio Message Library").toString();
        QString UUID = sermonTableModel->record(sermonDataMapper->currentIndex()).value(Sermon_ID).toString();
        QDir objSourceDir = (sourceDir + "/" + UUID);
        QString fileName = "";
        QString fileNamesList = "";
        audioFileNames = objSourceDir.entryList(QDir::Files, QDir::Name);   //Currently contains only the names.
        QString newlineChar = "";
        foreach (const QString &fileName, audioFileNames) {
            fileNamesList += newlineChar + fileName;
            if (audioFileNames.count() > 1) {
                newlineChar = "\n";
            }
        }
        //Add the complete path to the filename for direct file referencing purposes.
        //Must use a separate for loop here because the foreach loop is read-only!
        for (int i = 0; i < audioFileNames.count(); i++) {
            audioFileNames[i].prepend(objSourceDir.path() + "/");
        }
        ui->importAudioFiles_readOnlyEdit->setText(fileNamesList);
    }
}
