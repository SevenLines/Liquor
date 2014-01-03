#ifndef MGRAPHICSVIEWEA_H
#define MGRAPHICSVIEWEA_H

#include "mgraphicsview.h"
#include "keypoints.h"
#include "qgraphicsparticleitem.h"
#include <QList>

class MGraphicsViewEA : public MGraphicsView
{
    Q_OBJECT
    
private:
    KeyPoints *keyPoints;
    
    QGraphicsItemGroup *keyPointsRoot;
    QGraphicsRectItem *selectionFrame;
    
    QAction *toggleParticleAction;
    QAction *toggleShowParticlesAction;
    QMenu *contextMenu;
    
    QList<QGraphicsParticleItem*> selectedParticles;
    
    int iParticleIncrement;
    bool fParticleSelected;
    
    QGraphicsParticleItem *getParticleAtPos(QPoint pos);
    
public:
    
    static const int RADIUS = 0;
    static const int POS = 1;
    
    explicit MGraphicsViewEA(QWidget *parent = 0);
    
    void addContextMenuAction(QAction *action);
    void setKeyPoints(KeyPoints *keyPoints);
    
    ~MGraphicsViewEA();
public slots:
    void toogleKeyPoints(bool fShow);
    void setProportion(int value);
    void clearParticleItems();
    
    // очищает список выбранных объектов
    void clearSelected();
    // добавляет объект к списку выделенных объектов
    void selectItem(QGraphicsParticleItem *particle, bool deselectSelected = false);
    // изменяет пропорции выделенных объектов
    void incrementSelectItemsProportion(int increment);
    // вкл / выкл выделенный объекты
    void toggleIgnoreSeletItems(bool fIgnore);
    
    int selectedItemsIgnoreState();
    // выделеяет все объекты внутри рамки
    int selectInsideFrame();
    
    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // MGRAPHICSVIEWEA_H
