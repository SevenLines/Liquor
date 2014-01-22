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

QGraphicsItem *MGraphicsView::itemAtPos(QPoint p)
{
   QGraphicsItem *item = itemAt(p);
   if (item) {
       return (item->acceptedMouseButtons() == 0)?0:item;
   }
   return 0;
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
    pixmapItem->setAcceptedMouseButtons(0);
    
    // set background brush
    setBackgroundBrush(QBrush(Qt::gray));
    
    // включаем отслеживание движения мыши
    setMouseTracking(true);
    
    // отключаем скроллбары чтобы избежать проблем с изменением размеров
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 
    
    // рамка для выделения
    selectionFrame = gScene->addRect(0,0,0,0);
    selectionFrame->setPen(QPen(QBrush(Qt::blue), 0, Qt::DashLine));
    selectionFrame->setVisible(false);
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
    
    // управление выбором частиц
    if (e->buttons().testFlag(Qt::LeftButton)) {
        QGraphicsItem *item = itemAtPos(e->pos());
        
        if (item) {
            fCursorAboveItems  = true;
        } else {
            // тут рамку рисуем
            fCursorAboveItems = false;
            selectionFrame->setVisible(true);
            selectionFrame->setRect(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        0,0);
        }
    }
    
    // для правильной обработки вызова контекстного меню
    if ( e->buttons().testFlag(Qt::RightButton)) {
        fCursorAboveItems = true;
    }
    
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *)
{
    fMovedAfterPress = false;


    selectionFrame->setVisible(false);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *e)
{ 
    QPoint curPoint = e->pos();   
    QPoint offset = curPoint - lastPoint;   
    QPointF cPointSceneF = mapToScene(e->pos());
    
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

    if (e->buttons().testFlag(Qt::LeftButton)) {   
        // если курсор не над объектами рисуем рамку выделения
        if (!fCursorAboveItems) {
            selectionFrame->setRect(QRectF(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        cPointSceneF.x() - pressPointScene.x(),
                        cPointSceneF.y() - pressPointScene.y()).normalized());
        }
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
