#include "qgraphicsparticleitem.h"
#include <QPen>
#include <QBrush>
#include <QString>

void QGraphicsParticleItem::recalculate()
{
    if (!keyPoint)
        return;
    
    float k = (float)percentsProp / 100;
    float r = keyPoint->value * k;
    setRect(keyPoint->pos.x() - r, keyPoint->pos.y() - r, r * 2, r *2);
    setToolTip( QObject::tr("rad: %1\npos:(%2; %3)")
                .arg(r)
                .arg(keyPoint->pos.x())
                .arg(keyPoint->pos.y()) );
}

QGraphicsParticleItem::QGraphicsParticleItem(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    percentsProp = 100;
    
    keyPoint = 0;
   
    setPen(QPen(Qt::red));
    setBrush(QBrush(QColor::fromRgb(255,255,0,128)));
    
    setAcceptHoverEvents(true);
}

void QGraphicsParticleItem::setKeyPoint(MKeyPoint *keyPoint)
{
    this->keyPoint = keyPoint;
}

void QGraphicsParticleItem::setProportion(int percents)
{
    this->percentsProp = percents;
    recalculate();
}


void QGraphicsParticleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(QBrush(QColor::fromRgb(255,0,255,128)));;
}

void QGraphicsParticleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{ 
    setBrush(QBrush(QColor::fromRgb(255,255,0,128)));
}
