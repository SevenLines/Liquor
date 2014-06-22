#ifndef ERODEPROCESSPREVIEWER_H
#define ERODEPROCESSPREVIEWER_H

#include "imageprocesspreviewer.h"

using namespace cv;

class ErodeProcessPreviewer : public ImageProcessPreviewer
{
    Q_OBJECT
    
    
public:
    explicit ErodeProcessPreviewer(Mat image, QObject *parent = 0);
    
    static const QString PARAM_RADIUS;

signals:
    
public slots:
    
    // ImageProcessPreviewer interface
public:
    virtual QString title();
    virtual void regenImages();
};



#endif // ERODEPROCESSPREVIEWER_H
