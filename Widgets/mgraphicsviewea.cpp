#include "mgraphicsviewea.h"
#include "qgraphicsparticleitem.h"
#include <QGraphicsScene>


MGraphicsViewEA::MGraphicsViewEA(QWidget *parent)
    : MGraphicsView(parent)
{
    keyPointsRoot = 0;
    keyPointsRoot = gScene->createItemGroup(QList<QGraphicsItem*>());
    keyPointsRoot->setHandlesChildEvents(false);
}

void MGraphicsViewEA::setKeyPoints(KeyPoints *keyPoints)
{
    // соединяем представление с данными
    if (this->keyPoints != keyPoints ) {
        this->keyPoints = keyPoints;
        connect(keyPoints, SIGNAL(proportionChange(int)),
                SLOT(setProportion(int)));
        connect(keyPoints, SIGNAL(cleared()), 
                SLOT(clearParticleItems()));
    }
  
    // убираем существующие частицы
    clearParticleItems();
    
    // добавляем новы частицы
    for(int i=0;i<keyPoints->count();i++) {
        MKeyPoint &k = (*keyPoints)[i];
        QGraphicsParticleItem *circle = new QGraphicsParticleItem();
        
        gScene->addItem(circle);
        circle->setKeyPoint(&k); 
        keyPointsRoot->addToGroup(circle);
    }
    setProportion(this->keyPoints->proportion());
}

void MGraphicsViewEA::toogleKeyPoints(bool fShow)
{
    keyPointsRoot->setVisible(fShow);
}

void MGraphicsViewEA::setProportion(int value)
{
    foreach(QGraphicsItem *item, keyPointsRoot->childItems()) {
        QGraphicsParticleItem *circle = dynamic_cast<QGraphicsParticleItem*>(item);
        circle->setProportion(value);
    }
}

void MGraphicsViewEA::clearParticleItems()
{
    qDeleteAll(keyPointsRoot->childItems());
}
