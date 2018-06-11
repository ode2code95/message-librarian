#include "sermonsortfilterproxymodel.h"

SermonSortFilterProxyModel::SermonSortFilterProxyModel()
{
}

//Might also need to reimplement lessThan virtual method to make sorting of filter results possible.

bool SermonSortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
  bool flag = true;
  QHash<int, QRegExp>::const_iterator i;

  for (i = multiFilter.constBegin(); i != multiFilter.constEnd(); ++i) {
      //skip the loop body this time because it already passes but continue validation if further search criteria exists.
      if (i.value().isEmpty())
          continue;
      QModelIndex mIndex = sourceModel()->index(sourceRow, i.key(), sourceParent);
      flag = flag && (sourceModel()->data(mIndex).toString().contains(QRegExp(i.value())));
      if (!flag) //don't waste iterations if we already know that this row doesn't qualify.
          return false;
  }

  QModelIndex dateModelIndex = sourceModel()->index(sourceRow, Sermon_Date, sourceParent);
  return flag && dateInRange(sourceModel()->data(dateModelIndex).toDate());
}

bool SermonSortFilterProxyModel::dateInRange(const QDate &date) const
{
  return (!minDate.isValid() || date >= minDate)
          && (!maxDate.isValid() || date <= maxDate);
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
    multiFilter.clear();
    setFilterMinimumDate(QDate::currentDate().addYears(-5), false);
    setFilterMaximumDate(QDate::currentDate(), false);
    invalidateFilter();
    sort(-1);
}
