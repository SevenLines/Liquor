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
};

#endif // IMAGEPROCESSING_H
