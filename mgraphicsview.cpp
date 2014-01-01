#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QBrush>
#include <QRadialGradient>
#include <QScrollBar>
#include "mgraphicsview.h"

QGraphicsScene *MGraphicsView::scene() const
{
    return gScene;
}

void MGraphicsView::fitToScreen()
{
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
    
    // включаем отслеживание движения мыши
    setMouseTracking(true);
    // отключаем скроллбары чтобы избежать проблем с изменением размеров
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    setOptimizationFlag();
    
    //set quality
    //pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    
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
    QPoint curPoint = e->pos();
    
    QPoint offset = curPoint - lastPoint;   
    
    // двигаем вьюпорт сцены если нажата средняя кнопка
    if (e->buttons() & Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);
        QScrollBar *yScroll = verticalScrollBar();
        yScroll->setValue(yScroll->value() - offset.y());
        
        QScrollBar *xScroll = horizontalScrollBar();
        xScroll->setValue(xScroll->value() - offset.x());
    } else {
        setCursor(QCursor());
    }
    
    lastPoint = curPoint;
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
    
    //centerOn(pos);
}
