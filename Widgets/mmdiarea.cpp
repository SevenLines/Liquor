#include <QMdiSubWindow>
#include "mmdiarea.h"

MMdiArea::MMdiArea(QWidget *parent) :
    QMdiArea(parent)
{
}

MGraphicsViewEA *MMdiArea::addGraphicsViewEA(QPixmap pixmap)
{
    MGraphicsViewEA *newGraphicsView = new MGraphicsViewEA();
    newGraphicsView->setPixmap(pixmap);
    this->addSubWindow(newGraphicsView);
    return newGraphicsView;
}

MGraphicsViewEA *MMdiArea::currentGraphicsView()
{
    MGraphicsViewEA *view = 
            dynamic_cast<MGraphicsViewEA*>(this->activeSubWindow()->widget());
    return view;
}
