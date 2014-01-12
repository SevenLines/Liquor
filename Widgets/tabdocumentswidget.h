#ifndef MMDIAREA_H
#define MMDIAREA_H

#include <QTabWidget>
#include "mgraphicsviewea.h"

class TabDocumentsWidget : public QTabWidget
{
    Q_OBJECT
   
public:
    explicit TabDocumentsWidget(QWidget *parent = 0);
    
private:
    
    /// специальное расширение класс под табы
    class MGraphicsViewEATab : public MGraphicsViewEA 
    {
    private:
        QPixmap fixedImage;
    public:
        explicit MGraphicsViewEATab(QWidget *parent = 0);
        
        /// фиксирует текущее изображение
        void fixCurrentImage();
        QPixmap currentFixedImage();  
    };
    
    MGraphicsViewEATab *__currentGraphicsView();
    
signals:
    
public slots:
    MGraphicsViewEA *addGraphicsViewEA(QPixmap pixmap, QString title);
    MGraphicsViewEA *currentGraphicsView();
    
    /// изображение текущем табе
    QPixmap currentImage();
    void setCurrentImage(QPixmap pixmap);
    
    /// зафиксированное изображение текущего таба
    QPixmap currentFixedImage();
    /// фиксирует текущее изображение
    QPixmap fixCurrentImage();
    
    /// закруть текущую вкладку
    void closeTab(int index);
    /// растянуть изображение по виджету
    void fitToTab();
    
};

#endif // MMDIAREA_H

