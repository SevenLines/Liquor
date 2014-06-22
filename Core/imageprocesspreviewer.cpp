#include "imageprocesspreviewer.h"

ImageProcessPreviewer::ImageProcessPreviewer(Mat &img, QObject *parent) :
    QObject(parent)
{
    baseImage = img;
    _size = QSize(300, 300);
    regenImages();
}

int ImageProcessPreviewer::count()
{
    return images.count();
}

ProcessInfo ImageProcessPreviewer::image(int i)
{
    if (i > images.count() || i < 0) {
        return ProcessInfo();
    }
    return images.at(i);
}

Mat ImageProcessPreviewer::image()
{
    return baseImage;
}

QString ImageProcessPreviewer::title()
{
    return tr("undefined");
}

QSize ImageProcessPreviewer::size()
{
    return _size;
}

void ImageProcessPreviewer::setImage(Mat &image)
{
    baseImage = image;
}

void ImageProcessPreviewer::setImageSize(QSize size, bool regen)
{
    this->_size = size;
    regenImages();
}

void ImageProcessPreviewer::regenImages()
{
    
}
