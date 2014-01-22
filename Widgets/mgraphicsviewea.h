#ifndef MGRAPHICSVIEWEA_H
#define MGRAPHICSVIEWEA_H

#include "mgraphicsview.h"
#include "keypoints.h"
#include "qgraphicsparticleitem.h"
#include <QList>

using namespace Mick;

class MGraphicsViewEA : public MGraphicsView
{
    Q_OBJECT
    
protected:
    Mick::KeyPoints *keyPoints;
    
    QGraphicsItemGroup *keyPointsRoot;

    
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
    void setKeyPoints(Mick::KeyPoints *keyPoints);
    Mick::KeyPoints *getKeyPoints();
    
    ~MGraphicsViewEA();
public slots:
    void toogleKeyPoints(bool fShow);
    void setProportion(int value);
    void clearParticleItems();
    
    void clearKeyPoints();
    // очищает список выбранных объектов
    void clearSelected();
    // добавляет объект к списку выделенных объектов
    void selectItem(QGraphicsParticleItem *particle, bool deselectSelected = false);
    // изменяет пропорции выделенных объектов
    void incrementSelectItemsProportion(int increment);
    // вкл / выкл выделенный объекты
    void toggleIgnoreSeletItems(bool fIgnore);
    
    bool selectedItemsIgnoreState();
    
    // выделеяет все частицы внутри рамки
    void selectParticlesInsideFrame();
    
    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    
signals:
    /// вызывается когда устанавливается новый набор точек
    /// для виджета, а старый набор lastKeyPoints убирается
    void unsetKeyPoints(KeyPoints *lastKeyPoints);
};

#endif // MGRAPHICSVIEWEA_H
