#ifndef QGRAPHICSITEMFRAME_H
#define QGRAPHICSITEMFRAME_H

#include <QGraphicsItem>

class QGraphicsItemFrame : public QGraphicsRectItem
{
public:
    QGraphicsItemFrame(QGraphicsItem *parent=0);
    
    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // QGRAPHICSITEMFRAME_H
