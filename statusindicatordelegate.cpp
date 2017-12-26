#include "statusindicatordelegate.h"

StatusIndicatorDelegate::StatusIndicatorDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void StatusIndicatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->translate(option.rect.x(), option.rect.y());

    QPixmap stateIcon;
    if (index.data().isNull()) {
        stateIcon.load(":/images/fail.png");
    } else {
        stateIcon.load(":/images/ok.png");
    }

    painter->drawPixmap((option.rect.width() - stateIcon.width()) / 2, (option.rect.height() - stateIcon.height()) / 2, stateIcon.width(), stateIcon.height(), stateIcon);
    painter->restore();
}

QSize StatusIndicatorDelegate::sizeHint(const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    return QSize(option.rect.width(), option.rect.height());
}
