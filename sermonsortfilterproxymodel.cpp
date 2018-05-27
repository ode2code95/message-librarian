#include "sermonsortfilterproxymodel.h"

SermonSortFilterProxyModel::SermonSortFilterProxyModel()
{
}

//Might also need to reimplement lessThan virtual method to make sorting of filter results possible.

bool SermonSortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
  /*bool flag = false;
  QHash<int, QRegExp>::const_iterator i = multiFilter.constBegin();
  while (i != multiFilter.constEnd()) {
      QModelIndex mIndex = sourceModel()->index(sourceRow, i.key(), sourceParent);
      flag = (sourceModel()->data(mIndex).toString().contains(QRegExp(i.value())));
  }

  QModelIndex dateModelIndex = sourceModel()->index(sourceRow, Sermon_Date, sourceParent);
  return flag && dateInRange(sourceModel()->data(dateModelIndex).toDate());
  */
  return true;
}

bool SermonSortFilterProxyModel::dateInRange(const QDate &date) const
{
  return (!minDate.isValid() || date > minDate)
          && (!maxDate.isValid() || date < maxDate);
}

void SermonSortFilterProxyModel::setFilterMinimumDate(const QDate &date, bool invalFltr)
{
  minDate = date;
  if (invalFltr) {
      invalidateFilter();
  }
}

void SermonSortFilterProxyModel::setFilterMaximumDate(const QDate &date, bool invalFltr)
{
  maxDate = date;
  if (invalFltr) {
      invalidateFilter();
  }
}

void SermonSortFilterProxyModel::setMultiFilterRegExp(const QHash<int, QRegExp> &filter)
{
   multiFilter = filter;
}

void SermonSortFilterProxyModel::resetFilters()
{
    for (int i = 0; i < multiFilter.size(); i++) {
        multiFilter[i] = QRegExp("");
    }
    setFilterMinimumDate(QDate::currentDate().addYears(-5));
    setFilterMaximumDate(QDate::currentDate());
    sort(-1);
}
