#ifndef STATUSINDICATORDELEGATE_H
#define STATUSINDICATORDELEGATE_H

#include <QtWidgets>
#include <QStyledItemDelegate>

class StatusIndicatorDelegate : public QStyledItemDelegate
{
    //Q_OBJECT

public:
    StatusIndicatorDelegate(QWidget *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                     const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // STATUSINDICATORDELEGATE_H
