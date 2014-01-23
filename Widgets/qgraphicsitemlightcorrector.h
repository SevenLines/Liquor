#ifndef QGRAPHICSITEMLIGHTCORRECTOR_H
#define QGRAPHICSITEMLIGHTCORRECTOR_H

#include <QGraphicsEllipseItem>

#include "qgraphicsitemholder.h"
#include "lightcorrector.h"

class QGraphicsItemLightCorrector : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
private:
    LightCorrector *mLightCorrector;
    
    QGraphicsEllipseItem *circleArea;
    QGraphicsItemHolder *outerHolder;
    
    QBrush rectBrush;
    
    QPen penBorder;
    QPen hoverBorder;
    QPen lastBorder;
    
    QPointF pressPosition;
    QPointF pressObjectPosition;
    QPointF lastPos;
    
public:
    explicit QGraphicsItemLightCorrector(QGraphicsItem *parent = 0);
    
    float getRadius();
    LightCorrector *lightCorrector();
    
signals:
    /// вызывется когда пользователь щелкает на этот объект
    /// удерживая CTRL
    void applyMe();
    
public slots:
    void recalculate();
    void recalculateRadius();
    
    void hideBorder();
    void showBorder();
    void toggleBorder(bool fShow);
    void setLightCorrector(LightCorrector *value);
    void apply(QImage &img);
    void setRadius(float value);
    void setCompositionMode(QPainter::CompositionMode value);

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    // QGraphicsItem interface
    
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // QGRAPHICSITEMLIGHTCORRECTOR_H
