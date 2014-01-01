#ifndef QGRAPHICSPARTICLEITEM_H
#define QGRAPHICSPARTICLEITEM_H

#include <QGraphicsEllipseItem>
#include "keypoints.h"

class QGraphicsParticleItem : public QGraphicsEllipseItem
{
private:
    //float radius;
    int percentsProp;
    //QPoint pos;
    MKeyPoint *keyPoint; // сылка на оригинальный объект
    
    void recalculate();
    
public:
    explicit QGraphicsParticleItem(QGraphicsItem *parent = 0);
    
    void setKeyPoint(MKeyPoint *keyPoint);
    void setProportion(int percents);
    
signals:
    
public slots:
    
    
    // QGraphicsItem interface
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // QGRAPHICSPARTICLEITEM_H
