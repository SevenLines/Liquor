#include "cellimagedelegate.h"

#include <QPainter>
#include <QDebug>
#include <QHeaderView>
#include <QLabel>
#include "imageprocesspreviewer.h"

CellImageDelegate::CellImageDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


void CellImageDelegate::paint(QPainter *painter, 
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    if (!index.data().canConvert<ProcessInfo>()) 
        return;
    
    ProcessInfo p = qvariant_cast<ProcessInfo>(index.data());
    QRect rect = option.rect;
    painter->drawImage(rect, p.image);
    
    QString text;
    auto i = p.params.constBegin();
    for(;i != p.params.constEnd();++i) {
        text += QString(tr("%1 = %2\n")).arg(i.key()).arg(i.value());
    }
    QFont font = painter->font();
    font.setBold(true);
    font.setStyleStrategy(QFont::ForceOutline);
    
    painter->setFont(font);
    painter->setPen(Qt::white);
    QRect textRect = rect;
    textRect.moveTop(rect.top() + 2);
    painter->drawText(textRect, Qt::AlignHCenter, text);
    
    
    if (option.state & QStyle::State_Selected) {
        rect.setSize(QSize(rect.width() - 1, rect.height() - 1));
        rect.moveTopLeft(QPoint(1,1));
        painter->setPen(QPen(option.palette.highlight(),4, Qt::DashLine));   
        painter->drawRect(option.rect);
    }
}

QSize CellImageDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    if (!index.data().canConvert<ProcessInfo>()) 
        return QStyledItemDelegate::sizeHint(option, index);
    
    ProcessInfo p = qvariant_cast<ProcessInfo>(index.data());
    return QSize(p.image.width(), p.image.height());
//    return QSize(300, 300);
}
