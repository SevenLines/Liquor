#include "qlabelimage.h"
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

QLabelImage::QLabelImage(QWidget *parent) :
    QLabel(parent)
{
    minWidthHeight = 3;
    holderRadius = 5;
}


QRect QLabelImage::frame()
{
    return gFrame;
}


void QLabelImage::setFrame(QRect rectangle)
{
    rectangle = rectangle.normalized();
    gFrame.setLeft(qMax(rectangle.left(),0));
    gFrame.setTop(qMax(rectangle.top(),0));
    gFrame.setRight(qMin(rectangle.right(),width()-1));
    gFrame.setBottom(qMin(rectangle.bottom(),height()-1));
}

void QLabelImage::moveFrame(QPoint offset)
{
    gFrame.moveTopLeft(gFrame.topLeft() + offset);
}

void QLabelImage::moveHolder(QPoint offset)
{
    if (gSelectedHolder & egBottomLeft) 
        moveLeftBottom(offset);
    if (gSelectedHolder & egBottomRight) 
        moveRightBottom(offset);
    if (gSelectedHolder & egTopLeft) 
        moveLeftTop(offset);
    if (gSelectedHolder & egTopRight) 
        moveRightTop(offset);
    
    if (gSelectedHolder == egInside) {
        moveFrame(offset);
    }
    
    setFrame(gFrame);
}

void QLabelImage::moveLeftTop(QPoint offset)
{
    gFrame.setTopLeft(gFrame.topLeft() + offset);
}

void QLabelImage::moveRightTop(QPoint offset)
{
    gFrame.setTopRight(gFrame.topRight() + offset);
}

void QLabelImage::moveLeftBottom(QPoint offset)
{
    gFrame.setBottomLeft(gFrame.bottomLeft() + offset);
}

void QLabelImage::moveRightBottom(QPoint offset)
{
    gFrame.setBottomRight(gFrame.bottomRight() + offset);
}

int QLabelImage::isUnderHolder(QPoint pos)
{
    if (gFrame.isEmpty())
        return 0;
    
    int result = 0;
    
    if (gFrame.contains(pos)) {
        result |= egInside;    
    }
    
    if( (pos - gFrame.topLeft()).manhattanLength() <= holderRadius ) {
        result |= egTopLeft;
    }
    
    if( (pos - gFrame.topRight()).manhattanLength() <= holderRadius ) {
        result |= egTopRight;
    }
    
    if( (pos - gFrame.bottomLeft()).manhattanLength() <= holderRadius ) {
        result |= egBottomLeft;
    }
    
    if( (pos - gFrame.bottomRight()).manhattanLength() <= holderRadius ) {
        result |= egBottomRight;
    }
    
    return result;
}

void QLabelImage::resetFrame()
{
    gFrame = QRect();
}


void QLabelImage::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter p(this);
    drawFrame2(p);
}

void QLabelImage::drawFrame2(QPainter &p)
{
    if(gShowFrame) {
        p.save();
        
        p.setCompositionMode(QPainter::CompositionMode_Difference);
        p.setPen(QPen(QBrush(Qt::white), 1, Qt::DashLine));
        p.drawRect(gFrame);
        
        p.setPen(QPen(QBrush(Qt::white), 1));
        
        QBrush selectedBrush(Qt::white);
        QBrush defaultBrush(Qt::NoBrush);
        
        
        p.setBrush(gCurrentSelectedHolder & egTopLeft ? selectedBrush : defaultBrush); 
        p.drawEllipse(gFrame.topLeft(), holderRadius,holderRadius);
        
        p.setBrush(gCurrentSelectedHolder & egTopRight ? selectedBrush : defaultBrush); 
        p.drawEllipse(gFrame.topRight(), holderRadius,holderRadius);
        
        p.setBrush(gCurrentSelectedHolder & egBottomLeft ? selectedBrush : defaultBrush); 
        p.drawEllipse(gFrame.bottomLeft(), holderRadius,holderRadius);

        p.setBrush(gCurrentSelectedHolder & egBottomRight ? selectedBrush : defaultBrush); 
        p.drawEllipse(gFrame.bottomRight(), holderRadius,holderRadius);
     
        p.restore();
    }   
}


void QLabelImage::mousePressEvent(QMouseEvent *e)
{
    pPressed = e->pos();
    
    if (e->button() == Qt::LeftButton ) {
        gSelectedHolder = isUnderHolder(pPressed);
        if (!gShowFrame && gSelectedHolder == egOutside ) {
            gShowFrame = true;
            setFrame(QRect(pPressed, pPressed));
            update();
        } else {
            gShowFrame = true;
        }
       
    }
    pLast = pPressed;
}

void QLabelImage::mouseReleaseEvent(QMouseEvent *e)
{
    if(gFrame.size().height() < minWidthHeight && gFrame.size().width() < minWidthHeight) {
        gShowFrame = false;
        resetFrame();
        update();
    }
}

void QLabelImage::mouseMoveEvent(QMouseEvent *e)
{
    pCurrent = e->pos();
    gCurrentSelectedHolder = isUnderHolder(pCurrent);
    if (e->buttons() & Qt::LeftButton ) {
        if (gSelectedHolder) {
            moveHolder(pCurrent - pLast);
        } else {
            setFrame(QRect(pPressed, pCurrent));
        }
        update();
    } 
    if (gCurrentSelectedHolder!=gLastSelectedHolder) {
        update();
    }
    gLastSelectedHolder = gCurrentSelectedHolder;
    pLast = pCurrent;
}

