#ifndef EDITSERMON_H
#define EDITSERMON_H

#include <QDialog>

namespace Ui {
class EditSermon;
}

class EditSermon : public QDialog
{
    Q_OBJECT

public:
    explicit EditSermon(QWidget *parent = 0);
    ~EditSermon();

private:
    Ui::EditSermon *ui;
};

#endif // EDITSERMON_H
