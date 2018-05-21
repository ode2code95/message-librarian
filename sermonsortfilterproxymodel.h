#ifndef SERMONSORTFILTERPROXYMODEL_H
#define SERMONSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>
#include <QHash>
#include <QRegExp>

#include "mainwindow.h"

class SermonSortFilterProxyModel : public QSortFilterProxyModel
{

public:
    SermonSortFilterProxyModel();

    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate &date, bool invalFltr = true);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date, bool invalFltr = true);

    QHash<int, QRegExp> multiFilterRegExp() const { return multiFilter; }
    void setMultiFilterRegExp(const QHash<int, QRegExp> &filter);
    void resetFilters();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    bool dateInRange(const QDate &date) const;

    QDate minDate;
    QDate maxDate;
    QHash<int, QRegExp> multiFilter;
};

#endif // SERMONSORTFILTERPROXYMODEL_H
