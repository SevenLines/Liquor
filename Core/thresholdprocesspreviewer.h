#ifndef THRESHOLDPROCESSPREVIEWER_H
#define THRESHOLDPROCESSPREVIEWER_H

#include "imageprocesspreviewer.h"

class ThresholdProcessPreviewer : public ImageProcessPreviewer
{
    Q_OBJECT
public:
    explicit ThresholdProcessPreviewer(Mat image,QObject *parent = 0);
    
    static const QString PARAM_BORDER;
    
signals:
    
public slots:
    
    
    // ImageProcessPreviewer interface
public:
    virtual QString title();
    
public:
    virtual void regenImages();
};


#endif // THRESHOLDPROCESSPREVIEWER_H
