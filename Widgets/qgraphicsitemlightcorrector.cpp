#include <QGraphicsSceneMouseEvent>
#include <qmath.h>
#include <QDebug>
#include "MainInclude.h"
#include "qgraphicsitemlightcorrector.h"

QGraphicsItemLightCorrector::QGraphicsItemLightCorrector(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    //circleArea = new QGraphicsEllipseItem(this);
    mLightCorrector = 0;
    
    outerHolder = new QGraphicsItemHolder(this);
    
    rectBrush.setColor(Qt::red);    
    penBorder.setColor(Qt::red);

    toggleBorder(false);
    
    setAcceptHoverEvents(false);
    
    connect(outerHolder, SIGNAL(hasMoved()),
            SLOT(recalculateRadius()));
    connect(outerHolder, SIGNAL(enter()),
            SLOT(showBorder()));
    connect(outerHolder, SIGNAL(leave()),
            SLOT(hideBorder()));
}

float QGraphicsItemLightCorrector::getRadius()
{
    QPointF vec = outerHolder->pos();
    return sqrt(vec.x()*vec.x() + vec.y() * vec.y());
}

LightCorrector *QGraphicsItemLightCorrector::lightCorrector()
{
    return mLightCorrector;
}

void QGraphicsItemLightCorrector::setRadius(float value)
{
    if (!mLightCorrector) return;
    mLightCorrector->setRadius(value);
    recalculate();
}

void QGraphicsItemLightCorrector::apply(QImage &img)
{
    if (!mLightCorrector) return;
    mLightCorrector->apply(img);
}

void QGraphicsItemLightCorrector::setCompositionMode(QPainter::CompositionMode value)
{
    if (!mLightCorrector) return;
    mLightCorrector->setCompositionMode(value);
    recalculate();
}

void QGraphicsItemLightCorrector::recalculate()
{
    if (!mLightCorrector) {
        qWarning() << tr("light corrector is not set");
        return;
    }

    setPos(mLightCorrector->position());
    
    float radius = mLightCorrector->radius();
    setRect(-radius, 
            -radius,
            2*radius,
            2*radius);
    
    setBrush(QBrush(mLightCorrector->correctorGradient));
}

void QGraphicsItemLightCorrector::recalculateRadius()
{
    mLightCorrector->setRadius(getRadius());
    recalculate();
}

void QGraphicsItemLightCorrector::hideBorder()
{
    toggleBorder(false);
}

void QGraphicsItemLightCorrector::showBorder()
{
    toggleBorder(true);
}

void QGraphicsItemLightCorrector::toggleBorder(bool fShow)
{
    if (fShow) {
        setPen(penBorder);
    } else {
        setPen(Qt::NoPen);
    }
}

void QGraphicsItemLightCorrector::setLightCorrector(LightCorrector *value)
{
    mLightCorrector = value;
    if (mLightCorrector ) {
        qDebug() << mLightCorrector->radius();
        outerHolder->setPos(mLightCorrector->radius(),0);
    }
    recalculate();
}

void QGraphicsItemLightCorrector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!mLightCorrector) return;
    painter->save();
    painter->setCompositionMode(mLightCorrector->compositionMode());
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->restore();
}


void QGraphicsItemLightCorrector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressPosition = event->scenePos();
    pressObjectPosition = pos();
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        emit applyMe();
    }
    showBorder();
}

void QGraphicsItemLightCorrector::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!mLightCorrector) return;
    QPointF newPos = pressObjectPosition + event->scenePos() - pressPosition;
    if (applicationInfo.isMoveObjectButtons(event->buttons())) {
        if (lastPos != newPos) {
            mLightCorrector->setPosition(newPos);
            recalculate();
        }
    }
    lastPos = newPos;
}

void QGraphicsItemLightCorrector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    hideBorder();
}
