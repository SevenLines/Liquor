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
    QMap<QString, int>::const_iterator i = p.params.constBegin();
    for(;i != p.params.constEnd();++i) {
        text += QString(tr("%1 = %2\n")).arg(i.key()).arg(i.value());
    }

    int lineHeight = 20;
    QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(lineHeight*0.8);
    painter->setFont(font);
    
    QRect textRect = rect;
    textRect.setBottom(textRect.top() + lineHeight * p.params.count());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255,255,255,220));
    painter->drawRect(textRect);
    
    painter->setPen(Qt::black);
    painter->drawText(textRect, Qt::AlignHCenter, text);
    
    if (option.state & QStyle::State_Selected) {
        int penWidth = 4;
        rect.setSize(QSize(rect.width() - penWidth/2, rect.height() - penWidth/2));
        rect.moveTopLeft(QPoint(penWidth/2,penWidth/2));
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(option.palette.highlight(), penWidth, Qt::DashLine));   
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
