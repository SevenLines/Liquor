#ifndef QGRAPHICSITEMHOLDER_H
#define QGRAPHICSITEMHOLDER_H

#include <QGraphicsRectItem>
#include <QBrush>

class QGraphicsItemHolder : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    
private:
    int mWidth;
public:
    QGraphicsItemHolder(QGraphicsItem *parent);
    
    QPointF pressPosition;
    QPointF pressObjectPosition;
    QPointF lastPos;
    
    QBrush brushDefault;
    QBrush brushHover;
    
public slots:
    void setWidth(int value);
    void recalculate();
signals:
    void hasMoved();
    void enter();
    void leave();
    
    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    
    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // QGRAPHICSITEMHOLDER_H
