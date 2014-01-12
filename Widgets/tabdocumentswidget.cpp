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
    viewTab->setPixmap(pixmap);
    // фиксируем текущее изображение
    viewTab->fixCurrentImage();
    
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
        return viewTab->currentFixedImage();
    return QPixmap();
}

QPixmap TabDocumentsWidget::fixCurrentImage()
{
    MGraphicsViewEATab *viewTab = __currentGraphicsView();
    if (viewTab)
        viewTab->fixCurrentImage();
}

void TabDocumentsWidget::closeTab(int index)
{
    removeTab(index);
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

void TabDocumentsWidget::MGraphicsViewEATab::fixCurrentImage()
{
    fixedImage = pixmap();
}

QPixmap TabDocumentsWidget::MGraphicsViewEATab::currentFixedImage()
{
    return fixedImage;
}
