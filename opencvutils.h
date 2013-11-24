#ifndef OPENCVUTILS_H
#define OPENCVUTILS_H

#include <opencv/cv.h>
#include <QImage>
#include <QPixmap>

class OpenCVUtils
{
public:
    static QImage ToQImage(cv::Mat const &in);
    static QPixmap ToQPixmap(const cv::Mat &in);
    static cv::Mat ToRGB(cv::Mat const &in);
    static cv::Mat FromQImage(const QImage &src);
    static cv::Mat FromQPixmap(const QPixmap &src);
};

#endif // OPENCVUTILS_H
