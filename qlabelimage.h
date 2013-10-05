#ifndef QLABELIMAGE_H
#define QLABELIMAGE_H

#include <QLabel>


class QLabelImage : public QLabel
{
    Q_OBJECT
public:
    enum Edge{
              egTopLeft = 1, 
              egTopRight = 2, 
              egBottomRight = 4, 
              egBottomLeft = 8,
              egInside = 16,
              egOutside = 0
             };
    
    explicit QLabelImage(QWidget *parent = 0);
    // возвращает рамку
    QRect frame();
    

private:
    // рамка
    QRect gFrame;
    
    // показывать рамку
    bool gShowFrame;
    // если двигаем рамку
    bool gMoveFrame;
    
    // положения мышь
    QPoint pPressed, pCurrent, pLast;
    int gSelectedHolder, gLastSelectedHolder, gCurrentSelectedHolder;
    
    // минимальный размер зоны для выделения
    int minWidthHeight;
    int holderRadius;

signals:

public slots:
    // QWidget interface
protected:
   
    // устанавливает новые размер рамки, если rectangle больше размера 
    // изображения, то она обрезается
    void setFrame(QRect rectangle);
    // переместить рамку в соответствии с данным сдвигом
    void moveFrame(QPoint offset);
    void moveHolder(QPoint offset);
    void moveLeftTop(QPoint offset);
    void moveRightTop(QPoint offset);
    void moveLeftBottom(QPoint offset);
    void moveRightBottom(QPoint offset);

    // возврщает 0 если точка ни над одним из углов
    // и enum Edge если над углом
    int isUnderHolder(QPoint pos);
    // сбросить размеры рамки
    void resetFrame();

    void paintEvent(QPaintEvent *);
    void drawFrame2(QPainter &p);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // QLABELIMAGE_H
