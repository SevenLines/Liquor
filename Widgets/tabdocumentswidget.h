#ifndef MMDIAREA_H
#define MMDIAREA_H

#include <QTabWidget>
#include "mgraphicsviewea.h"
#include "imagestack.h"
#include "keypoints.h"

using namespace Mick;

class TabDocumentsWidget : public QTabWidget
{
    Q_OBJECT
   
public:
    explicit TabDocumentsWidget(QWidget *parent = 0);
    
private:
    
    
    
    /// специальное расширение класс под табы
    class MGraphicsViewEATab : public MGraphicsViewEA 
    {
    friend class TabDocumentsWidget;
    private:
        QPixmap fixedImage;
        ImageStack imageStack;
    public:
        explicit MGraphicsViewEATab(QWidget *parent = 0);
        
        /// фиксирует текущее изображение
        void fixCurrentImage(QString title, bool asKey = false, int pos = -1);
        void setKeyPoints(Mick::KeyPoints *keyPoints, bool takeParentship);
        Mick::KeyPoints *getKeyPoints();
    };
    
    MGraphicsViewEATab *__currentGraphicsView();
    
signals:
    
public:
    
    MGraphicsViewEA *addGraphicsViewEA(QPixmap pixmap, QString title);
    MGraphicsViewEA *currentGraphicsView();
    
    /// изображение текущем табе
    QPixmap currentImage();
    /// зафиксированное изображение текущего таба
    QPixmap currentFixedImage();    
    ImageStack *currentImageStack();
    Mick::KeyPoints *currentKeyPoints();
    QString currentTabName();
    
public slots:
    void setCurrentImage(QPixmap pixmap);
    /// фиксирует текущее изображение, чтобы изображение не добавлялось
    /// в стэк изображений, укажите в качестве названия QString()
    void fixCurrentImage(QString title = QString(), bool asKey = false, int pos = -1);
    
    // set wiget keyPoints and take parenting under it
    void setKeyPoints(KeyPoints *keyPoints, bool takeParentship = false);
    
    /// закруть вкладку
    void closeTab(int index);
    void closeCurrentTab();
    /// растянуть изображение по виджету
    void fitToTab();
    
};

#endif // MMDIAREA_H

