#include "imageprocessing.h"
#include <qmath.h>
#include <QDebug>

#include <opencv/cv.h>

using namespace cv;

void ImageProcessing::salt(Mat &image, int n)
{
    for ( int k = 0; k < n; k++ ) {
        int i = qrand()%image.cols;
        int j = qrand()%image.rows;
        if (image.channels() == 1) {
            image.at<uchar>(j,i) = 255;
        } else if (image.channels() == 3 ){
            image.at<Vec3b>(j,i)[0] = 255;
            image.at<Vec3b>(j,i)[1] = 255;
            image.at<Vec3b>(j,i)[2] = 255;
        }
    }
}

void ImageProcessing::colorReduce(Mat &image, int div)
{
    int nl = image.rows;
    int nc = image.cols*image.channels();
    
    //длина одного цвет
    //double k = 256.0 / div;
    uchar converter[256];
    for (int i=0; i< 256; ++i) {
        converter[i] = 256-i;   
    }
    
    for(int j=0;j<nl;++j) {
        uchar *data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; ++i ) {
            data[i] = converter[data[i]];
        }
    }
}

void ImageProcessing::showHIstogramm(Mat &image)
{
    
}

void ImageProcessing::threshold(Mat &image, Mat &result, int value)
{
    cvtColor(image, result, CV_RGB2GRAY);   
    cv::threshold(result, result, value, 256, THRESH_BINARY );
    cvtColor(result, result, CV_GRAY2RGB);
}

void ImageProcessing::adaptiveThreshold(Mat &image, Mat &result, int maxValue, int oddBlockSize, int method)
{
    cvtColor(image, result, CV_RGB2GRAY);   
    cv::adaptiveThreshold(result, result, maxValue, method, THRESH_BINARY, oddBlockSize, 0);
    cvtColor(result, result, CV_GRAY2RGB);   
}

void ImageProcessing::dilate(Mat &image, Mat &result, int dilationSize)
{
    
    Mat element =  cv::getStructuringElement(
                MORPH_ELLIPSE,
                cv::Size(2*dilationSize + 1, 2*dilationSize + 1),
                cv::Point( dilationSize, dilationSize));
    
    cv::dilate(image, result, element);
}

void ImageProcessing::erode(Mat &image, Mat &result, int erosionSize)
{
    Mat element =  cv::getStructuringElement(
                MORPH_ELLIPSE,
                cv::Size(2*erosionSize + 1, 2*erosionSize + 1),
                cv::Point( erosionSize, erosionSize));
    cv::erode(image, result, element);
}
