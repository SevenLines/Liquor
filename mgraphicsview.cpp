#include "mgraphicsview.h"


QGraphicsScene MGraphicsView::getScene() const
{
    //return scene;
}

void MGraphicsView::setScene(const QGraphicsScene &value)
{
    //scene = value;
}


MGraphicsView::MGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
  
}

void MGraphicsView::setPixmap(const QPixmap &img)
{
        gPixmap = img;
        scene.addPixmap(gPixmap);
}


void MGraphicsView::mousePressEvent(QMouseEvent *)
{
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *)
{
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *)
{
}
