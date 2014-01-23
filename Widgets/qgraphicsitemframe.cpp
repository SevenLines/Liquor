#include "qgraphicsitemframe.h"

#include <QPainter>

QGraphicsItemFrame::QGraphicsItemFrame(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setPen(QPen(QBrush(Qt::white), 0, Qt::DashLine));
    setVisible(false);
}


void QGraphicsItemFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    
    painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    QGraphicsRectItem::paint(painter, option, widget);
    
    painter->restore();
}
