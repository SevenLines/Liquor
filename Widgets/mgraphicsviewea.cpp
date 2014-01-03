#include "mgraphicsviewea.h"

#include <QGraphicsScene>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>

QGraphicsParticleItem *MGraphicsViewEA::getParticleAtPos(QPoint pos)
{
    QGraphicsItem *item = this->itemAt(pos);
    if (item) 
        return dynamic_cast<QGraphicsParticleItem*>(item);
    else 
        return 0;
}

MGraphicsViewEA::MGraphicsViewEA(QWidget *parent)
    : MGraphicsView(parent)
{
    // setup keyPointsRoot object
    keyPointsRoot = 0;
    keyPointsRoot = gScene->createItemGroup(QList<QGraphicsItem*>());
    keyPointsRoot->setHandlesChildEvents(false);
    
    iParticleIncrement = 3;
    
    // init actions for context menu
    toggleParticleAction = new QAction(tr("Ignore"), this);
    toggleParticleAction->setCheckable(true);
    connect(toggleParticleAction, SIGNAL(triggered(bool)),
            SLOT(toggleIgnoreSeletItems(bool)));  
    
    // init context menu
    contextMenu = new QMenu(this);   
    contextMenu->addAction(toggleParticleAction);
    
    
    // рамка для выделения
    selectionFrame = gScene->addRect(0,0,0,0);
    selectionFrame->setPen(QPen(QBrush(Qt::blue), 0, Qt::DashLine));
    selectionFrame->setVisible(false);

}

MGraphicsViewEA::~MGraphicsViewEA() 
{
}

void MGraphicsViewEA::addContextMenuAction(QAction *action)
{
    contextMenu->addAction(action);
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

void MGraphicsViewEA::clearSelected()
{
    foreach(QGraphicsParticleItem *p, selectedParticles) {
        p->toggleSelect(false);
    }
    selectedParticles.clear();
}

void MGraphicsViewEA::selectItem(QGraphicsParticleItem *particle, bool deselectSelected)
{
    if (!selectedParticles.contains(particle)) {
        selectedParticles.append(particle);
        particle->toggleSelect(true);
    } else if (deselectSelected) {
        selectedParticles.removeOne(particle);
    }
}

void MGraphicsViewEA::incrementSelectItemsProportion(int increment)
{
    foreach(QGraphicsParticleItem *p, selectedParticles) {
        int percents = p->particleProportion();
        p->setParticleProportion(percents + increment);
    }
}

void MGraphicsViewEA::toggleIgnoreSeletItems(bool fIgnore)
{
    foreach(QGraphicsParticleItem *p, selectedParticles) {
        p->toggleIgnore(fIgnore);
    } 
}

int MGraphicsViewEA::selectedItemsIgnoreState()
{
    int i=0;
    foreach(QGraphicsParticleItem *p, selectedParticles) {
        i += p->isIgnore();
    }  
    return i==0?0:i==selectedParticles.count()?1:2;
}

int MGraphicsViewEA::selectInsideFrame()
{
    foreach( QGraphicsItem *item , keyPointsRoot->childItems()) {
        QGraphicsParticleItem *p 
                = dynamic_cast<QGraphicsParticleItem*>(item);
        if (p) {
            if (selectionFrame->rect().contains(p->pos())) {
                selectItem(p);
            }
        }
    } 
}

void MGraphicsViewEA::contextMenuEvent(QContextMenuEvent *e)
{
    if ( e->reason() == QContextMenuEvent::Mouse) {
        // контекстное меню частиц
        toggleParticleAction->setVisible(selectedParticles.count());  
        toggleParticleAction->setChecked(selectedItemsIgnoreState());
        
        // показывае меню
        contextMenu->exec(e->globalPos());
    }
}


void MGraphicsViewEA::wheelEvent(QWheelEvent *e)
{
    MGraphicsView::wheelEvent(e);
    
    if (e->modifiers().testFlag(Qt::NoModifier)) {
        QGraphicsParticleItem *particle = getParticleAtPos(e->pos());
        if (particle) {
            // изменение пропорции мышью
            if (e->delta() > 0)
                incrementSelectItemsProportion(iParticleIncrement);
            else 
                incrementSelectItemsProportion(-iParticleIncrement);
        }
    }
}


void MGraphicsViewEA::mouseMoveEvent(QMouseEvent *e)
{
    QPointF lPointSceneF = mapToScene(lastPoint);
    QPointF cPointSceneF = mapToScene(e->pos());
    QPointF offset = cPointSceneF - lPointSceneF;
    
    MGraphicsView::mouseMoveEvent(e); 
    
    if (e->buttons().testFlag(Qt::LeftButton)) {   
        if (fParticleSelected == false) {
            clearSelected();
            selectionFrame->setRect(QRectF(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        cPointSceneF.x() - pressPointScene.x(),
                        cPointSceneF.y() - pressPointScene.y()).normalized());
        } else {
            foreach(QGraphicsParticleItem *p, selectedParticles) {
                p->move(offset);
            }
        }
    } else {
    }
}


void MGraphicsViewEA::mousePressEvent(QMouseEvent *e)
{
    MGraphicsView::mousePressEvent(e);
    if (e->buttons().testFlag(Qt::LeftButton)) {
        QGraphicsParticleItem *particle = getParticleAtPos(e->pos());
        if (particle) {
            fParticleSelected  = true;
            if (!selectedParticles.contains(particle)) {
                if (e->modifiers().testFlag(Qt::ShiftModifier)) {
                    selectItem(particle);
                } else {
                    clearSelected();
                    selectItem(particle);
                }
            }
        } else {
            fParticleSelected = false;
            selectionFrame->setVisible(true);
            selectionFrame->setRect(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        0,0);
        }
    }
    if ( e->buttons().testFlag(Qt::RightButton)) {
        fParticleSelected = true;
    }
}


void MGraphicsViewEA::mouseReleaseEvent(QMouseEvent *e)
{
    if (!fMovedAfterPress) {  
        if (!fParticleSelected) {
            clearSelected();
        }
    } 
    MGraphicsView::mouseReleaseEvent(e);
    if (selectionFrame->isVisible()) {
        selectInsideFrame();
    }
    selectionFrame->setVisible(false);
}