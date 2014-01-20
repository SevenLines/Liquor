#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

class MGraphicsView : public QGraphicsView
{
    Q_OBJECT
    
protected:
    QGraphicsScene *gScene;
    
    QPixmap bgPixmap;
    QBrush backgroundBrush2;

    QGraphicsPixmapItem *pixmapItem;
    QGraphicsPixmapItem *backgroundImageItem;
    bool fFitToScreen;
    bool fMovedAfterPress;
    QPointF pressPointScene;
    QPoint lastPoint;
    
public:
    explicit MGraphicsView(QWidget *parent = 0);
    ~MGraphicsView();
    void setPixmap(QPixmap img);
    QPointF centerOfScene();
    QPixmap pixmap();
    
    QGraphicsScene *scene() const;
    
signals:
    
public slots:
    void fitToScreen();
    
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
