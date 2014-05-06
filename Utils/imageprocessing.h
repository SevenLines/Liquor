#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv/cv.h>

using namespace cv;

class ImageProcessing
{
public:
    static void salt(Mat &image, int n);
    static void colorReduce(Mat &image, int div = 64);
    static void showHIstogramm(Mat &image);
    static void threshold(Mat &image, Mat &result, int value);
    static void adaptiveThreshold(Mat &image, Mat &result, 
                                  int maxValue, int oddBlockSize, int method=ADAPTIVE_THRESH_GAUSSIAN_C);
    static void dilate(Mat &image, Mat &result, int dilationSize);
    static void erode(Mat &image, Mat &result, int erosionSize);
};

#endif // IMAGEPROCESSING_H
