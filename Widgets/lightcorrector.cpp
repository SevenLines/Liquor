#include "lightcorrector.h"
#include "opencv/cv.h"
#include "opencvutils.h"

#include <QPainter>
#include <QDebug>

using namespace cv;

LightCorrector::LightCorrector(QGraphicsItem *parent) :
    	QGraphicsItemGroup(parent)
{
    circleArea = new QGraphicsEllipseItem(this);
    outerHolder = new QGraphicsRectItem(this);
    
    outerHolder->setRect(0,0,10,10);
    
    correctorGradient.setColorAt(0, QColor(0,0,255,255));
    correctorGradient.setColorAt(1, QColor(0,0,255,0));

    
    setRadius(100);
    
    setAcceptHoverEvents(true);
    setHandlesChildEvents(true);
}

void LightCorrector::setRadius(float value)
{
    mRadius = value;
    recalculate();
}

void LightCorrector::apply(QPixmap img)
{
    // строим Mat объект
    Mat mat = OpenCVUtils::FromQPixmap(img);
    // меняем цветовую модель
    cvtColor(mat, mat, CV_BGR2HSV);
    // создаем временное изображение, в новой цветовой модели
    QPixmap temp = OpenCVUtils::ToQPixmap(mat);
    
    // рисуем кооректирующий градиент
    QPainter p;
    if (p.begin(&temp)) {
        p.setBrush(QBrush(correctorGradient));
        p.drawEllipse(pos(), mRadius, mRadius);
        p.end();
    }
    
    // возвращаемся к предыдущей цветовой модели
    mat = OpenCVUtils::FromQPixmap(temp);
    cvtColor(mat, mat, CV_HSV2BGR);
    img = OpenCVUtils::ToQPixmap(mat);
}

void LightCorrector::recalculate()
{
    QPointF center = pos();
    
    outerHolder->setPos(center.x()+mRadius, 0);
    
    circleArea->setRect(center.x() - mRadius, 
                        center.y() - mRadius,
                        2*mRadius,
                        2*mRadius);
    
    qDebug() << "recalculate";
    
    correctorGradient.setRadius(mRadius);
    
    circleArea->setBrush(QBrush(correctorGradient));
}


void LightCorrector::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "move";
}
