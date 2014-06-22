#include "opencvutils.h"

using namespace cv;


QImage OpenCVUtils::ToQImage(cv::Mat const &in)
{
    Mat image;// = OpenCVUtils::ToRGB(in);    
    
    cvtColor(in, image,CV_BGR2RGB);
    
    QImage dest((uchar*) image.data,
                image.cols,
                image.rows,
                image.step,
                QImage::Format_RGB888);
    
    QImage dest2(dest);
    dest2.detach(); // enforce deep copy
    return dest2; 
}

QPixmap OpenCVUtils::ToQPixmap(cv::Mat const &in)
{
    QPixmap img = QPixmap::fromImage(OpenCVUtils::ToQImage(in));
    return img;   
}

cv::Mat OpenCVUtils::ToRGB(cv::Mat const &in)
{
    Mat temp = in.clone();
    
    switch(temp.type()) {
    case CV_8UC3:
        cvtColor(temp,temp, CV_BGR2RGB);
        break;
    case CV_8U:
        cvtColor(temp,temp, CV_GRAY2RGB);
        break;
    }
    
    return temp;
}

cv::Mat OpenCVUtils::FromQImage(const QImage &src)
{
    // just to be sure that the image in the right format
    QImage tmpImage = src.convertToFormat(QImage::Format_RGB888);
    
    // create Mat image from QImage
    cv::Mat tmpMat(tmpImage.height(),
                tmpImage.width(),
                CV_8UC3,
                (uchar*)tmpImage.bits(),
                tmpImage.bytesPerLine());
    
    /** 
     * cause tmpMat bind to tmpImage data, 
     * which will be deleted 
     * at the end of this function
     */
    cv::Mat result;
    cvtColor(tmpMat, result,CV_BGR2RGB);
    return result;
}

Mat OpenCVUtils::FromQPixmap(const QPixmap &src)
{
    return OpenCVUtils::FromQImage(src.toImage());
}


Point OpenCVUtils::getNeighboor(Point p, int num)
{
    switch(num) {
        case 0: return p; break;
        case 1: return cv::Point(p.x-1, p.y-1); break;
        case 2: return cv::Point(p.x, p.y-1); break;
        case 3: return cv::Point(p.x+1, p.y-1); break;
        case 4: return cv::Point(p.x+1, p.y); break;
        case 5: return cv::Point(p.x+1, p.y+1); break;
        case 6: return cv::Point(p.x, p.y+1); break;
        case 7: return cv::Point(p.x-1, p.y+1); break;
        case 8: return cv::Point(p.x-1, p.y); break;
    }
    return p;
}

bool OpenCVUtils::isInside(cv::Mat &image, cv::Point &p)
{
    return (p.x >= 0 && 
            p.y >=0 && 
            p.x < image.cols && 
            p.y < image.rows);
}







