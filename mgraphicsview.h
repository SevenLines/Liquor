#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QGraphicsView>

class MGraphicsView : public QGraphicsView
{
    Q_OBJECT
    
private:
    QGraphicsScene scene;
    QPixmap gPixmap;
public:
    explicit MGraphicsView(QWidget *parent = 0);
    void setPixmap(const QPixmap &img);
    
    QGraphicsScene getScene() const;
    void setScene(const QGraphicsScene &value);
    
signals:
    
public slots:
    
    
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // MGRAPHICSVIEW_H
