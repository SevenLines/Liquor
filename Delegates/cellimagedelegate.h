#ifndef CELLIMAGEDELEGATE_H
#define CELLIMAGEDELEGATE_H

#include <QStyledItemDelegate>

class CellImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CellImageDelegate(QObject *parent = 0);
    
signals:
    
public slots:
    
    
    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CELLIMAGEDELEGATE_H
