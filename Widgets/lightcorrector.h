#ifndef LIGHTCORRECTOR_H
#define LIGHTCORRECTOR_H

#include <QGraphicsEllipseItem>
#include <QPixmap>
#include <QRadialGradient>

class LightCorrector : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
private:
    int mRadius;
    
    QGraphicsEllipseItem *circleArea;
    QGraphicsRectItem *outerHolder;
    
    QRadialGradient correctorGradient;
    
public:
    explicit LightCorrector(QGraphicsItem *parent = 0);
    
    
signals:
    
public slots:
    void setRadius(float value);
    void apply(QPixmap img);
    void recalculate();
    
    
    // QGraphicsItem interface
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // LIGHTCORRECTOR_H
