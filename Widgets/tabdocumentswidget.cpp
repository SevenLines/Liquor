#include <QMdiSubWindow>
#include <QDebug>
#include <QMessageBox>
#include "keypoints.h"
#include "tabdocumentswidget.h"

TabDocumentsWidget::TabDocumentsWidget(QWidget *parent) :
    QTabWidget(parent)
{
    connect(this, SIGNAL(tabCloseRequested(int)),
            SLOT(closeTab(int)));
}

TabDocumentsWidget::MGraphicsViewEATab *TabDocumentsWidget::__currentGraphicsView()
{
    if (this->currentWidget()) {
        return dynamic_cast<MGraphicsViewEATab*>(this->currentWidget());
    }
    return 0;
}

MGraphicsViewEA *TabDocumentsWidget::addGraphicsViewEA(QPixmap pixmap, QString title)
{
    MGraphicsViewEATab *viewTab = new MGraphicsViewEATab();
    
    foreach(QAction *action, actions()) {
        viewTab->addContextMenuAction(action);
    }
    
    // сигнал о том что у одной из вкладок изменили набор точек
    connect(viewTab, SIGNAL(unsetKeyPoints(KeyPoints*)),
            SIGNAL(unsetKeyPoints(KeyPoints*)));
    connect(viewTab, SIGNAL(applyLightCorrectorForMe()),
            SIGNAL(applyLightCorrectorForMe()));
    connect(this, SIGNAL(currentChanged(int)),
            SIGNAL(invalidated()));
    
    // еняем политику изменения размеров
    viewTab->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    
    viewTab->setPixmap(pixmap);
    // фиксируем текущее изображение
    viewTab->fixCurrentImage(title);
    
    // создаем новый таб
    int windowIndex = addTab(viewTab, QIcon(), title);
    setCurrentIndex(windowIndex);
    fitToTab();
    
    return viewTab;
}

MGraphicsViewEA *TabDocumentsWidget::currentGraphicsView()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        return dynamic_cast<MGraphicsViewEA*>(viewTab);
    }
    return 0;
}

QPixmap TabDocumentsWidget::currentImage()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        return viewTab->pixmap();
    }
    return QPixmap();
}

void TabDocumentsWidget::setCurrentImage(QPixmap pixmap)
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        viewTab->setPixmap(pixmap);
    }
}

QPixmap TabDocumentsWidget::currentFixedImage()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        return viewTab->fixedImage;
    return QPixmap();
}

void TabDocumentsWidget::fixCurrentImage(QString title, bool asKey, int pos)
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        viewTab->fixCurrentImage(title, asKey, pos);
}

ImageStack *TabDocumentsWidget::currentImageStack()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        return &viewTab->imageStack;
    return 0;
}

Mick::KeyPoints *TabDocumentsWidget::currentKeyPoints()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        return viewTab->keyPoints;
    return 0; 
}

QString TabDocumentsWidget::currentTabName()
{
    if (currentIndex()!=-1) {
        return tabText(currentIndex());
    }
    return QString();
}

LightCorrector *TabDocumentsWidget::currentLightCorrector()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        return viewTab->lightCorrector->lightCorrector();
    return 0; 
}

bool TabDocumentsWidget::isCorrectionEnabled()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        return viewTab->isLightCorrectorEnabled();
    return false; 
}

MGraphicsViewEA *TabDocumentsWidget::isContains(KeyPoints *keyPoints)
{
    for(int i=0;i<count();++i) {
        MGraphicsViewEA *v = tabWidget(i);
        if ( v && v->getKeyPoints() == keyPoints ) {
            return v;
        }
    }
    return 0;
}

MGraphicsViewEA *TabDocumentsWidget::tabWidget(int index)
{
    QWidget *wgt = widget(index);
    MGraphicsViewEA * view = 0;
    if (wgt && (view = dynamic_cast<MGraphicsViewEATab*>(wgt)) ) {
        return view;
    }
    return 0;
}

void TabDocumentsWidget::setKeyPoints(Mick::KeyPoints *keyPoints, bool takeParentship, bool setName)
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        if (setName) {
            keyPoints->setTitle(tabText(currentIndex()));
        }
        viewTab->setKeyPoints(keyPoints, takeParentship);
        emit invalidated();
    }
}

void TabDocumentsWidget::setKeyPointsWithName(KeyPoints *keyPoints)
{
    setKeyPoints(keyPoints, false, true);
}

void TabDocumentsWidget::closeTab(int index)
{
    QWidget *wgt = widget(index);
    if (wgt) {
        if (MGraphicsViewEA *view = dynamic_cast<MGraphicsViewEA*>(wgt)) {
            // если родиель совпадает с текущим виджетом значит
            // стоит спросить пользователя не нажал ли он случайно
            if (view->getKeyPoints() && view->getKeyPoints()->parent() == wgt) {
                if (QMessageBox::No == QMessageBox::question(0,tr("Confirmation"),
                                      tr("Are u sure u want to close this window"),
                                      QMessageBox::Yes | QMessageBox::No)) {
                    return;
                }
            }
        }
        wgt->close();
        delete wgt;
    }
}

void TabDocumentsWidget::closeCurrentTab()
{
    closeTab(currentIndex());
}

void TabDocumentsWidget::fitToTab()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        viewTab->fitToScreen();
    }
}

void TabDocumentsWidget::toggleLightCorrector(bool fShow)
{
    MGraphicsView *view = currentGraphicsView();
    if (view) {
        view->toggleLightCorrector(fShow);
    }   
}

void TabDocumentsWidget::setLightCorrectorMode(QPainter::CompositionMode value)
{
    MGraphicsView *view = currentGraphicsView();
    if (view) {
        view->setLightCorrectorCompositionMode(value);
    }
}

void TabDocumentsWidget::setLightCorrector(LightCorrector *lightCorrector)
{
    MGraphicsViewEATab *view = __currentGraphicsView();
    if (view) {
        view->setLightCorrector(lightCorrector);
    }
}

void TabDocumentsWidget::setLightCorrectorIntensity(int value)
{
    MGraphicsViewEATab *view = __currentGraphicsView();
    if (view) {
        view->setLightCorrectorIntensity(value);
    }
}

void TabDocumentsWidget::applyLightCorrector()
{
    MGraphicsViewEATab *view = __currentGraphicsView();
    if (view) {
        view->applyLightCorrector();
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

/// конструктор для класса расширения под таб
TabDocumentsWidget::MGraphicsViewEATab::MGraphicsViewEATab(QWidget *parent)
    : MGraphicsViewEA(parent)
{    
}

void TabDocumentsWidget::MGraphicsViewEATab::fixCurrentImage(QString title, bool asKey, int pos)
{
    fixedImage = pixmap();
    if (!title.isNull()) {
        imageStack.push(pixmap(), title, asKey, pos);
        qDebug() << tr("push current image to stack as '%1'").arg(title);
    }
}

void TabDocumentsWidget::MGraphicsViewEATab::setKeyPoints(Mick::KeyPoints *keyPoints, bool takeParentship)
{
    MGraphicsViewEA::setKeyPoints(keyPoints);
    /*if (takeParentship) {
        keyPoints->setParent(this);
    }*/
}

Mick::KeyPoints *TabDocumentsWidget::MGraphicsViewEATab::getKeyPoints()
{
    return keyPoints;
}
