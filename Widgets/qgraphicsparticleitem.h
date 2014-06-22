#ifndef QGRAPHICSPARTICLEITEM_H
#define QGRAPHICSPARTICLEITEM_H

#include <QGraphicsEllipseItem>
#include "keypoints.h"

class QGraphicsParticleItem : public QGraphicsEllipseItem
{
private:
    QBrush brushDefault;
    QBrush brushIgnore;
    QBrush brushSelected;
    QBrush brushPrevious;
    
    QPen penDefault;
    QPen penSelected;
    QPen previousPen;
    
    Mick::KeyPoints::Type mType;
    
    bool fSelected;
    
    //float radius;
    int percentsProp;
    //QPoint pos;
    Mick::KeyPoint *keyPoint; // сылка на оригинальный объект
    
    void recalculate();
    
public:
    explicit QGraphicsParticleItem(QGraphicsItem *parent = 0);
    
    void setKeyPoint(Mick::KeyPoint *keyPoint);
    void setProportion(int percents);
    void setParticleProportion(int percents);
    void setPos(QPointF pos);
    void setType(Mick::KeyPoints::Type type);
    void move(QPointF offset);
    
    void toggleSelect(bool fSelected);
    
    QPointF pos();
    Mick::KeyPoints::Type type();
    bool isIgnore();
    int particleProportion();
    
signals:
    
public slots:
    void toggleIgnore(bool fIgnore);
    
    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // QGRAPHICSPARTICLEITEM_H
