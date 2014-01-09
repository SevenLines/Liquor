#ifndef MMDIAREA_H
#define MMDIAREA_H

#include <QMdiArea>
#include "mgraphicsviewea.h"

class MMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit MMdiArea(QWidget *parent = 0);
    
    
signals:
    
public slots:
    MGraphicsViewEA *addGraphicsViewEA(QPixmap pixmap);
    MGraphicsViewEA *currentGraphicsView();
    
};

#endif // MMDIAREA_H
