#ifndef MGRAPHICSVIEWEA_H
#define MGRAPHICSVIEWEA_H

#include "mgraphicsview.h"
#include "keypoints.h"

class MGraphicsViewEA : public MGraphicsView
{
    Q_OBJECT
    
private:
    KeyPoints *keyPoints;
    QGraphicsItemGroup *keyPointsRoot;
    
public:
    
    static const int RADIUS = 0;
    static const int POS = 1;
    
    explicit MGraphicsViewEA(QWidget *parent = 0);
    
    void setKeyPoints(KeyPoints *keyPoints);
public slots:
    void toogleKeyPoints(bool fShow);
    void setProportion(int value);
    void clearParticleItems();
};

#endif // MGRAPHICSVIEWEA_H
