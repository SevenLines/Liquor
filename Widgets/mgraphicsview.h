#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "lightcorrector.h"

class MGraphicsView : public QGraphicsView
{
    Q_OBJECT
    
protected:
    QGraphicsScene *gScene;
    
    QPixmap bgPixmap;
    QBrush backgroundBrush2;
    QGraphicsRectItem *selectionFrame;

    QGraphicsPixmapItem *pixmapItem;
    QGraphicsPixmapItem *backgroundImageItem;
    LightCorrector *lightCorrector;
    
    
    bool fFitToScreen;
    bool fMovedAfterPress;
    bool fCursorAboveItems;
    QPointF pressPointScene;
    QPoint lastPoint;
    
public:
    explicit MGraphicsView(QWidget *parent = 0);
    ~MGraphicsView();
    void setPixmap(QPixmap img);
    QPointF centerOfScene();
    QPixmap pixmap();
    
    QGraphicsScene *scene() const;

    bool isLightCorrecterUnderMouse(QPoint p);

    /// возвращает объект под курсором, 
    /// кроме тех которые не отслеживают события мыши
    QGraphicsItem *itemAtPos(QPoint p);
    
signals:
    
public slots:
    void fitToScreen();
    void toggleLightCorrector(bool fShow);
    
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *);
    void dragMoveEvent(QDragMoveEvent *e);
    
    
    // QGraphicsView interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // MGRAPHICSVIEW_H
