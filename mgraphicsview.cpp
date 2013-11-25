#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QBrush>
#include <QRadialGradient>
#include "mgraphicsview.h"

QGraphicsScene *MGraphicsView::scene() const
{
    return gScene;
}

void MGraphicsView::fitToScreen()
{
    qDebug() << viewport()->size();
    fitInView(pixmapItem, Qt::KeepAspectRatio);
    fFitToScreen = true;
}


MGraphicsView::MGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    // init scene
    gScene = new QGraphicsScene(this);
    pixmapItem = gScene->addPixmap(QPixmap());
    setScene(gScene);
    
    // set background brush
    setBackgroundBrush(QBrush(Qt::black));
    
    //set quality
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    
    fFitToScreen = false;
}

void MGraphicsView::setPixmap(const QPixmap &img)
{
    //gScene->setSceneRect(pixmapItem->boundingRect());
    pixmapItem->setPixmap(img);
    gScene->setSceneRect(QRectF());
}

QPointF MGraphicsView::centerOfScene()
{
    return this->viewport()->rect().center();
}

QPixmap MGraphicsView::pixmap()
{
    return pixmapItem->pixmap();
}


void MGraphicsView::mousePressEvent(QMouseEvent *e)
{
    pressPointScene = e->pos();
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *)
{
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QPointF curPoint = e->pos();
    
    QPointF center = centerOfScene();
    center = mapToScene(center.x(), center.y());
    
    QPointF newPoint = pressPointScene - curPoint;
    qDebug() << newPoint;
    centerOn(center + newPoint);
    pressPointScene = curPoint;
}


void MGraphicsView::resizeEvent(QResizeEvent *)
{   
    if (fFitToScreen) 
        fitInView(pixmapItem, Qt::KeepAspectRatio);
}


void MGraphicsView::wheelEvent(QWheelEvent *e)
{
    float s = 1.5f;
    fFitToScreen = false;
    
    QPointF pos = mapToScene(e->pos());
    
    if (e->modifiers() & Qt::CTRL){
        if (e->delta() > 0) {
            scale(s, s);
        } else {
            scale(1.0/s, 1.0/s);
        }
    }
    
    centerOn(pos);
}
