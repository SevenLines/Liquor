#include "lightcorrector.h"

#include <QPainter>
#include <QDebug>


LightCorrector::LightCorrector(QObject *parent) :
    	QObject(parent)
{
    setIntensity(255);
    correctorGradient.setColorAt(1, QColor(255,255,255,0));

    mRadius = 0;
    mCompositionMode = QPainter::CompositionMode_SoftLight;
}

QPointF LightCorrector::position()
{
    return mPosition;
}

float LightCorrector::radius()
{
    return mRadius;
}

QPainter::CompositionMode LightCorrector::compositionMode()
{
    return mCompositionMode;
}

QRadialGradient LightCorrector::gradient()
{
    return correctorGradient;
}

int LightCorrector::intensity()
{
    // интенсивность это альфа канал первого стопа градиента
    return correctorGradient.stops().first().second.alpha();
}

void LightCorrector::setRadius(float value)
{
    mRadius = value;
    correctorGradient.setRadius(value);
}

void LightCorrector::setPosition(QPointF value)
{
    mPosition = value;
}

void LightCorrector::apply(QImage &img)
{
    qDebug() << tr("apply LightCorrector, radius:%1px, pos:(%2; %3), mode:%4")
                .arg(mRadius)
                .arg(mPosition.x())
                .arg(mPosition.y())
                .arg(mCompositionMode);
    QPainter p(&img);
    
    p.translate(mPosition);
    
    p.setCompositionMode(mCompositionMode);
    
    correctorGradient.setRadius(mRadius);
    p.setBrush(QBrush(correctorGradient));
    p.setPen(Qt::NoPen);
    
    p.drawEllipse(QPoint(0,0), mRadius, mRadius);
}

void LightCorrector::setCompositionMode(QPainter::CompositionMode value)
{
    mCompositionMode = value;
}

void LightCorrector::setFocalPoint(QPointF p)
{
    correctorGradient.setFocalPoint(p);
}

void LightCorrector::setIntensity(unsigned char value)
{
    correctorGradient.setColorAt(0, QColor(255,255,255,value));
}
