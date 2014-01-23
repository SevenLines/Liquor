#include "qgraphicsitemholder.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "MainInclude.h"
#include <QBrush>

QGraphicsItemHolder::QGraphicsItemHolder(QGraphicsItem *parent)
    :QGraphicsEllipseItem(parent)
{
    setWidth(10);  
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIgnoresTransformations);

    brushHover.setStyle(Qt::SolidPattern);
    brushDefault.setStyle(Qt::SolidPattern);
    brushHover.setColor(QColor(255,0,0,128));
    brushDefault.setColor(QColor(255,255,255,128));
    
    setBrush(brushDefault);
    
}

void QGraphicsItemHolder::setWidth(int value)
{
    mWidth = value;
    recalculate();
}

void QGraphicsItemHolder::recalculate()
{
    setRect(pos().x() - mWidth/2,
            pos().y() - mWidth / 2,
            mWidth,
            mWidth);  
    update();
}


void QGraphicsItemHolder::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(brushHover);
    emit enter();
}

void QGraphicsItemHolder::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(brushDefault);
    emit leave();
}


void QGraphicsItemHolder::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressPosition = event->scenePos();
    pressObjectPosition = pos();
}

void QGraphicsItemHolder::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF newPos = pressObjectPosition + event->scenePos() - pressPosition;
    if (applicationInfo.isMoveObjectButtons(event->buttons())) {
        if (lastPos != newPos) {
            setPos(newPos);
            emit hasMoved();
        }
    }
    lastPos = newPos;
}

void QGraphicsItemHolder::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}
