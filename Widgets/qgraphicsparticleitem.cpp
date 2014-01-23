#include "qgraphicsparticleitem.h"
#include <QPen>
#include <QBrush>
#include <QString>
#include <QDebug>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

#include <math.h>

void QGraphicsParticleItem::recalculate()
{
    if (!keyPoint)
        return;
    float k, r;
    switch(mType) {
    case Mick::KeyPoints::Undefined:
        r = 25;
        setRect(keyPoint->pos().x() - r, keyPoint->pos().y() - r, r * 2, r *2);
        break;
    case Mick::KeyPoints::Particles:
        k = (float)percentsProp / 100;
        r = keyPoint->calcValue() * k;
        setRect(keyPoint->pos().x() - r, keyPoint->pos().y() - r, r * 2, r *2);
        setToolTip( QObject::tr("rad: %1 px\npos:(%2; %3)")
                    .arg(r)
                    .arg(keyPoint->pos().x())
                    .arg(keyPoint->pos().y()) );
        break;
    case Mick::KeyPoints::Area:
        r = sqrt(keyPoint->calcValue() * M_1_PI);
        setRect(keyPoint->pos().x() - r, keyPoint->pos().y() - r, r * 2, r *2);
        setToolTip( QObject::tr("Area: %1 px<sup>2</sup>")
                    .arg(keyPoint->value()) );
        break;
    }
}

QGraphicsParticleItem::QGraphicsParticleItem(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    percentsProp = 100;
    fSelected = false;
    keyPoint = 0;
    mType = Mick::KeyPoints::Undefined;
   
    brushDefault = QBrush(QColor::fromRgb(0,255,0,128));
    brushIgnore = QBrush(QColor::fromRgb(255,0,0,64));
    brushSelected = QBrush(QColor::fromRgb(255,0,255,164));
    
    penDefault.setColor(Qt::black);
    penSelected.setColor(Qt::blue);
    
    setBrush(brushDefault);
    setPen(penDefault);
    
    setAcceptHoverEvents(true);
}

void QGraphicsParticleItem::setKeyPoint(Mick::KeyPoint *keyPoint)
{
    this->keyPoint = keyPoint;
    if (keyPoint) {
        toggleIgnore(keyPoint->isIgnore());
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
        keyPoint->setPos(pos);
        recalculate();
    }
}

void QGraphicsParticleItem::setType(Mick::KeyPoints::Type type)
{
    mType = type;
}

void QGraphicsParticleItem::move(QPointF offset)
{
    if (keyPoint) {
        keyPoint->setPos(offset + keyPoint->pos());
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
        eff->setColor(Qt::yellow);
        eff->setOffset(0);
        if (keyPoint) {
            switch(mType) {
            case Mick::KeyPoints::Particles:
                eff->setBlurRadius(keyPoint->calcValue());
                break;
            case Mick::KeyPoints::Area:
                eff->setBlurRadius(25);
                break;
            default:
                eff->setBlurRadius(25);
            }
        }   
        setGraphicsEffect(eff);
        setPen(penSelected);
    } else {
        setGraphicsEffect(0);
        setPen(penDefault);
    }  
    this->fSelected = fSelected;
}

bool QGraphicsParticleItem::isIgnore()
{
    return keyPoint?keyPoint->isIgnore():false;
}

QPointF QGraphicsParticleItem::pos()
{
    return keyPoint? keyPoint->pos() : QPointF(-1.0f,-1.0f);
}

Mick::KeyPoints::Type QGraphicsParticleItem::type()
{
    return mType;
}

int QGraphicsParticleItem::particleProportion()
{
    return keyPoint? keyPoint->proportion() : 0;
}

void QGraphicsParticleItem::toggleIgnore(bool fIgnore)
{
    if (keyPoint) {
        keyPoint->setIgnore(fIgnore);
        setBrush(fIgnore ? brushIgnore : brushDefault);
    }
}

void QGraphicsParticleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //brushPrevious = brush();
    setBrush(brushSelected);;
}

void QGraphicsParticleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{ 
    if (keyPoint && keyPoint->isIgnore()) {
        setBrush(brushIgnore);
    } else {
        setBrush(brushDefault);
    }
}
