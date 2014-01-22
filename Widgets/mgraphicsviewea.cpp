#include "mgraphicsviewea.h"

#include <QGraphicsScene>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include "MainInclude.h"

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
    // setup init values
    keyPoints = 0;
    
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
    emit unsetKeyPoints(keyPoints);
}

void MGraphicsViewEA::addContextMenuAction(QAction *action)
{
    contextMenu->addAction(action);
}

void MGraphicsViewEA::setKeyPoints(Mick::KeyPoints *keyPoints)
{    
    Mick::KeyPoints *lastKeyPoints = this->keyPoints;
    // соединяем представление с данными
    if (this->keyPoints != keyPoints ) {

        this->keyPoints = keyPoints;
        if (keyPoints) {
            connect(keyPoints, SIGNAL(proportionChange(int)),
                    SLOT(setProportion(int)));
            connect(keyPoints, SIGNAL(cleared()), 
                    SLOT(clearParticleItems()));
            /*connect(keyPoints, SIGNAL(destroyed()),
                    SLOT(clearKeyPoints()));*/
        }
    }
        
    // убираем существующие частицы
    clearParticleItems();
    
    if (keyPoints) {     
        // добавляем новы частицы
        for(int i=0;i<keyPoints->count();i++) {
            Mick::KeyPoint &k = (*keyPoints)[i];
            QGraphicsParticleItem *keyPoint = new QGraphicsParticleItem();
            
            gScene->addItem(keyPoint);
            
            keyPoint->setKeyPoint(&k); 
            keyPoint->setType(keyPoints->type());
            
            keyPointsRoot->addToGroup(keyPoint);
        }
        setProportion(this->keyPoints->proportion());
    }
    
    emit unsetKeyPoints(lastKeyPoints);
}

Mick::KeyPoints *MGraphicsViewEA::getKeyPoints()
{
    return keyPoints;
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
    selectedParticles.clear();
    qDeleteAll(keyPointsRoot->childItems());
}

void MGraphicsViewEA::clearKeyPoints()
{
    setKeyPoints(0);
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
        particle->toggleSelect(false);
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

bool MGraphicsViewEA::selectedItemsIgnoreState()
{
    int i=0;
    foreach(QGraphicsParticleItem *p, selectedParticles) {
        i += p->isIgnore();
    }  
    return i>selectedParticles.count()/2?true:false;
}

void MGraphicsViewEA::selectInsideFrame()
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
        // изменение пропорции мышью
        if (e->delta() > 0)
            incrementSelectItemsProportion(iParticleIncrement);
        else 
            incrementSelectItemsProportion(-iParticleIncrement);
    }
}


void MGraphicsViewEA::mouseMoveEvent(QMouseEvent *e)
{
    QPointF lPointSceneF = mapToScene(lastPoint);
    QPointF cPointSceneF = mapToScene(e->pos());
    QPointF offset = cPointSceneF - lPointSceneF;
    
    MGraphicsView::mouseMoveEvent(e); 
    if (applicationInfo.isMoveObjectButtons(e->buttons())) {   
        // если не было выбрано частиц рисуем рамку выделения
        if (fParticleSelected == false) {
            clearSelected();
            selectionFrame->setRect(QRectF(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        cPointSceneF.x() - pressPointScene.x(),
                        cPointSceneF.y() - pressPointScene.y()).normalized());
        } else {
            // иначе двигаем выбранные частицы
            
            foreach(QGraphicsParticleItem *p, selectedParticles) {
                p->move(offset);
            }
        }
    }
}


void MGraphicsViewEA::mousePressEvent(QMouseEvent *e)
{
    MGraphicsView::mousePressEvent(e);
    // управление выбором частиц
    if (applicationInfo.isMoveObjectButtons(e->buttons())) {
        QGraphicsParticleItem *particle = getParticleAtPos(e->pos());
        
        if (particle) {
            fParticleSelected  = true;
            if (!selectedParticles.contains(particle)) {
                if (e->modifiers().testFlag(Qt::ShiftModifier)) {
                    selectItem(particle, true);
                } else {
                    clearSelected();
                    selectItem(particle);
                }
            }
        } else {
            // тут рамку рисуем
            fParticleSelected = false;
            selectionFrame->setVisible(true);
            selectionFrame->setRect(
                        pressPointScene.x(),
                        pressPointScene.y(),
                        0,0);
        }
    }
    
    // для правильной обработки вызова контекстного меню
    if ( applicationInfo.isContextButtons(e->buttons()) ) {
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
    // отключаем рамку выбора
    if (selectionFrame->isVisible()) {
        selectInsideFrame();
    }
    selectionFrame->setVisible(false);
}
