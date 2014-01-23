#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QBrush>
#include <QRadialGradient>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include "mgraphicsview.h"
#include "MainInclude.h"

QGraphicsScene *MGraphicsView::scene() const
{
    return gScene;
}


bool MGraphicsView::isLightCorrecterUnderMouse(QPoint p)
{
    QGraphicsItem *item = itemAtPos(p);
    if (item  ) {
        return dynamic_cast<LightCorrector*>(item)!=0;
    }
    return false;
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

void MGraphicsView::toggleLightCorrector(bool fShow)
{
    lightCorrector->setVisible(fShow);
}

void MGraphicsView::setLightCorrectorCompositionMode(QPainter::CompositionMode value)
{
    lightCorrector->setCompositionMode(value);
    lightCorrector->update();
}

void MGraphicsView::applyLightCorrector()
{
    QImage img = pixmap().toImage();//.convertToFormat(QImage::Format_ARGB32);
    lightCorrector->apply(img);
   
    pixmapItem->setPixmap(QPixmap::fromImage(img));
}

void MGraphicsView::setLightCorrector(LightCorrector *value)
{
    lightCorrector->setLightCorrector(value);
}

bool MGraphicsView::isLightCorrectorEnabled()
{
    return lightCorrector->isVisible();
}


//void MGraphicsView::applyLightCorrectorSlot()
//{
//    applyLightCorrector();
//}

MGraphicsView::MGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    // init scene
    gScene = new QGraphicsScene(this);
    setScene(gScene);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
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
    selectionFrame = new QGraphicsItemFrame(0);
    scene()->addItem(selectionFrame);

    fFitToScreen = false;
    
    // set light corrector;
    lightCorrector = new QGraphicsItemLightCorrector(0);
    gScene->addItem(lightCorrector);
    toggleLightCorrector(false);
    
    connect(lightCorrector, SIGNAL(applyMe()),
            SLOT(applyLightCorrector()));
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
    QGraphicsView::mousePressEvent(e);
    pressPointScene = mapToScene(e->pos());
    fMovedAfterPress = false;
    
    // управление выбором частиц
    if (applicationInfo.isMoveObjectButtons(e->buttons())) {
        
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
    if ( applicationInfo.isContextButtons(e->buttons())) {
        fCursorAboveItems = true;
    }
    
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    fMovedAfterPress = false;
    fCursorAboveItems = false;

    selectionFrame->setVisible(false);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *e)
{ 
    QGraphicsView::mouseMoveEvent(e);
    QPoint curPoint = e->pos();   
    QPoint offset = curPoint - lastPoint;   
    QPointF cPointSceneF = mapToScene(e->pos());
    
    if (!e->buttons().testFlag(Qt::NoButton)) {
        fMovedAfterPress = true;
    }
    
    // двигаем вьюпорт сцены если нажата средняя кнопка
    if (applicationInfo.isMoveCameraButtons(e->buttons())) {
        setCursor(Qt::ClosedHandCursor);
        QScrollBar *yScroll = verticalScrollBar();
        yScroll->setValue(yScroll->value() - offset.y());
        
        QScrollBar *xScroll = horizontalScrollBar();
        xScroll->setValue(xScroll->value() - offset.x());
    } else {
        setCursor(QCursor());
    }

    if (applicationInfo.isMoveObjectButtons(Qt::LeftButton)) {   
        // если курсор не над объектами рисуем рамку выделения
        if (!fCursorAboveItems) {
            selectionFrame->setRect(QRectF(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        cPointSceneF.x() - pressPointScene.x(),
                        cPointSceneF.y() - pressPointScene.y()).normalized());
        }/* else {
            if (isLightCorrecterUnderMouse(e->pos())) {
                lightCorrector->setPos(mapToScene(e->pos()));
            }
        }*/
    }
 
    lastPoint = curPoint;
}


void MGraphicsView::resizeEvent(QResizeEvent *)
{   
    /*if (fFitToScreen) 
        fitInView(pixmapItem, Qt::KeepAspectRatio);*/
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
