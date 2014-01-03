#include "qgraphicsparticleitem.h"
#include <QPen>
#include <QBrush>
#include <QString>
#include <QDebug>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

void QGraphicsParticleItem::recalculate()
{
    if (!keyPoint)
        return;
    
    float k = (float)percentsProp / 100;
    float r = keyPoint->calcValue() * k;
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
    fSelected = false;
    keyPoint = 0;
   
    brushDefault = QBrush(QColor::fromRgb(0,255,0,164));
    brushIgnore = QBrush(QColor::fromRgb(255,0,0,64));
    brushSelected = QBrush(QColor::fromRgb(0,0,255,164));
    
    penDefault.setColor(Qt::black);
    penSelected.setColor(Qt::blue);
    
    setBrush(brushDefault);
    setPen(penDefault);
    
    setAcceptHoverEvents(true);
}

void QGraphicsParticleItem::setKeyPoint(MKeyPoint *keyPoint)
{
    this->keyPoint = keyPoint;
    if (keyPoint) {
        toggleIgnore(keyPoint->fIgnore);
    }
}

void QGraphicsParticleItem::setProportion(int percents)
{
    this->percentsProp = percents;
    recalculate();
}

void QGraphicsParticleItem::setParticleProportion(int percents)
{
    if (keyPoint) {
        keyPoint->setProportion(percents);
        recalculate();
    }
}

void QGraphicsParticleItem::setPos(QPointF pos)
{
    if (keyPoint) {
        keyPoint->pos = pos;
        recalculate();
    }
}

void QGraphicsParticleItem::move(QPointF offset)
{
    if (keyPoint) {
        keyPoint->pos += offset;
        recalculate();
    }
}

void QGraphicsParticleItem::toggleSelect(bool fSelected)
{
    if (this->fSelected==fSelected) {
        return;
    }
    
    if (fSelected) {
        QGraphicsDropShadowEffect *eff = new QGraphicsDropShadowEffect();
        
        eff->setColor(Qt::white);
        eff->setOffset(0);
        if (keyPoint) {
            eff->setBlurRadius(keyPoint->calcValue());
        }   
        setGraphicsEffect(eff);
    } else {
        setGraphicsEffect(0);
    }  
    this->fSelected = fSelected;
}

bool QGraphicsParticleItem::isIgnore()
{
    return keyPoint?keyPoint->fIgnore:false;
}

QPointF QGraphicsParticleItem::pos()
{
    return keyPoint? keyPoint->pos : QPointF(-1.0f,-1.0f);
}

int QGraphicsParticleItem::particleProportion()
{
    return keyPoint? keyPoint->proportion() : 0;
}

void QGraphicsParticleItem::toggleIgnore(bool fIgnore)
{
    if (keyPoint) {
        keyPoint->fIgnore = fIgnore;
        setBrush(fIgnore ? brushIgnore : brushDefault);
    }
}


void QGraphicsParticleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //setBrush(QBrush(QColor::fromRgb(255,0,255,128)));;
}

void QGraphicsParticleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{ 
    //setBrush(QBrush(QColor::fromRgb(255,255,0,128)));
}
