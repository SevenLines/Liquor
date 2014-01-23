#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "qgraphicsitemframe.h"
#include "lightcorrector.h"
#include "qgraphicsitemlightcorrector.h"

class MGraphicsView : public QGraphicsView
{
    Q_OBJECT
  /*  
private slots:
    /// сигнал костыль для виртуального метода applyLightCorrector
    void applyLightCorrectorSlot();*/
    
protected:
    QGraphicsScene *gScene;
    
    QPixmap bgPixmap;
    QBrush backgroundBrush2;
    QGraphicsItemFrame *selectionFrame;

    QGraphicsPixmapItem *pixmapItem;
    QGraphicsPixmapItem *backgroundImageItem;
    QGraphicsItemLightCorrector *lightCorrector;
    
    
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
    
    void setLightCorrector(LightCorrector *value);
    bool isLightCorrectorEnabled();
    
signals:
    
public slots:
    void fitToScreen();
    void toggleLightCorrector(bool fShow);
    void setLightCorrectorCompositionMode(QPainter::CompositionMode);
    
    virtual void applyLightCorrector();
    
    
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *);
    void dragMoveEvent(QDragMoveEvent *e);
    
    
    // QGraphicsView interface
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // MGRAPHICSVIEW_H
