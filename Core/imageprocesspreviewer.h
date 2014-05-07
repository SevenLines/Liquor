#ifndef IMAGEPROCESSPREVIEWER_H
#define IMAGEPROCESSPREVIEWER_H

#include <QPixmap>
#include <QObject>
#include <opencv/cv.h>
#include <QMap>
#include <QMetaType>

using namespace cv;

struct ProcessInfo {
    QImage image;
    QMap<QString, int> params;
};

Q_DECLARE_METATYPE(ProcessInfo);

/**
/**  * @brief abstract class, at least one method (regenImages) should be overrided
/** */
class ImageProcessPreviewer : public QObject
{
    Q_OBJECT
protected:
    /// list of preprocessed small images 
    QList<ProcessInfo> images;
    Mat baseImage;
    QSize _size;
    
public:
    explicit ImageProcessPreviewer(Mat &img, QObject *parent = 0);
    
    
    /// returns count of preview images 
    virtual int count();
    
    /// return process ingo with index i
    virtual ProcessInfo image(int i);
    
    /// return base image
    virtual Mat image();
    
    virtual QString title();
    QSize size();
    
    /// build preview small preprocessed images from base
    /// should be overrided in offspring class
    virtual void regenImages();
    
signals:
    
public slots:
    virtual void setImage(Mat &image); 
    /**
     * @brief set base size for preview images
     * @param size size og image
     * @param regen set to true to immediatly regen all preivew image after setting size
     */
    virtual void setImageSize(QSize size, bool regen = false);
    
    
};

#endif // IMAGEPROCESSPREVIEWER_H
