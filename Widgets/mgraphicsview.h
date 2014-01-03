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
    QGraphicsPixmapItem *pixmapItem;
    bool fFitToScreen;
    QPoint pressPointScene, lastPoint;
    
public:
    explicit MGraphicsView(QWidget *parent = 0);
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
};

#endif // MGRAPHICSVIEW_H
