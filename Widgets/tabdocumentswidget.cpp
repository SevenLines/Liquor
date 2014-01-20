#include <QMdiSubWindow>
#include <QDebug>
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
    
    // еняем политику изменения размеров
    QSizePolicy sizePolicy = viewTab->sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Preferred);
    viewTab->setSizePolicy(sizePolicy);
    
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

void TabDocumentsWidget::setKeyPoints(Mick::KeyPoints *keyPoints, bool takeParentship)
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab) {
        keyPoints->setTitle(tabText(currentIndex()));
        viewTab->setKeyPoints(keyPoints, takeParentship);
    }
}

void TabDocumentsWidget::closeTab(int index)
{
    QWidget *wgt = widget(index);
    if (wgt) {
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
    if (takeParentship) {
        keyPoints->setParent(this);
    }
}

Mick::KeyPoints *TabDocumentsWidget::MGraphicsViewEATab::getKeyPoints()
{
    return keyPoints;
}
