#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QBrush>
#include <QRadialGradient>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
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
    setScene(gScene);
    
    // set pixmap
    //backgroundImageItem = gScene->addPixmap(QPixmap());
    pixmapItem = gScene->addPixmap(QPixmap());
    
    // set background brush
    setBackgroundBrush(QBrush(Qt::gray));
    
    // включаем отслеживание движения мыши
    setMouseTracking(true);
    
    // отключаем скроллбары чтобы избежать проблем с изменением размеров
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
/*
    bgPixmap = QPixmap(":/images/logo2_back.svg");
    backgroundImageItem->setPixmap(bgPixmap);
    backgroundImageItem->setFlags( QGraphicsItem::ItemIgnoresTransformations);*/

    fFitToScreen = false;
}

MGraphicsView::~MGraphicsView()
{
}

void MGraphicsView::setPixmap(QPixmap img)
{ 
    pixmapItem->setPixmap(img);
    gScene->setSceneRect(gScene->itemsBoundingRect());
}

QPointF MGraphicsView::centerOfScene()
{
    return this->viewport()->rect().center();
}

QPixmap MGraphicsView::pixmap()
{
    if (pixmapItem) {
        return pixmapItem->pixmap();
    } 
    return QPixmap();
}


void MGraphicsView::mousePressEvent(QMouseEvent *e)
{
    pressPointScene = mapToScene(e->pos());
    fMovedAfterPress = false;
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *)
{
    fMovedAfterPress = false;
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *e)
{ 
    QPoint curPoint = e->pos();
    
    QPoint offset = curPoint - lastPoint;   
    
    if (!e->buttons().testFlag(Qt::NoButton)) {
        fMovedAfterPress = true;
    }
    
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
    
    if (e->modifiers() & Qt::CTRL){
        if (e->delta() > 0) {
            scale(s, s);
        } else {
            scale(1.0/s, 1.0/s);
        }
    }
}


void MGraphicsView::dragMoveEvent(QDragMoveEvent *e)
{
    e->acceptProposedAction();
}


void MGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect); 
}
