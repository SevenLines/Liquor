#ifndef LIGHTCORRECTOR_H
#define LIGHTCORRECTOR_H

#include <QImage>
#include <QRadialGradient>
#include <QPainter>

class QGraphicsItemLightCorrector;

class LightCorrector : public QObject
{
    Q_OBJECT
    friend class QGraphicsItemLightCorrector;
protected:
    
    int mRadius;
    QPointF mPosition;
    
    QRadialGradient correctorGradient;
    QPainter::CompositionMode mCompositionMode;

public:
    explicit LightCorrector(QObject *parent = 0);
    
    QPointF position();
    float radius();
    QPainter::CompositionMode compositionMode();
    QRadialGradient gradient();
    
public slots:
    void setRadius(float value);
    void setPosition(QPointF value);
    void apply(QImage &img);
    void setCompositionMode(QPainter::CompositionMode value);
};

#endif // LIGHTCORRECTOR_H
