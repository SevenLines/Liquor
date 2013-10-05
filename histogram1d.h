#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include <opencv/cv.h>

using namespace cv;

class Histogram1D
{
private:
    int histSize[1];
    float hRanges[2];
    const float* ranges[1];
    int channels[1];
public:
    Histogram1D() {
        histSize[0] = 256;
        hRanges[0] = 0.0;
        hRanges[1] = 255.0;
        ranges[0] = hRanges;
        channels[0] = 0;
    }
    
    MatND getHistogram(Mat &image) {
        MatND hist;
        calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
        return hist;
    }
    
    Mat getHistogramImage(Mat &image) {
        MatND hist  = getHistogram(image);
        double maxVal = 0;
        double minVal = 0;
        
        minMaxLoc(hist, &minVal, &maxVal, 0, 0);
        
        Mat histImage(histSize[0], histSize[0], CV_8U, Scalar(255));
        
        int hpt = static_cast<int>(0.9 * histSize[0] );
        for (int h = 0; h < histSize[0]; ++h ) {
            float binVal = hist.at<float>(h);
            int intencity = static_cast<int>(binVal * hpt / maxVal);
            line(histImage, 
                 Point(h, histSize[0]), 
                 Point(h,  histSize[0] - intencity),
                 Scalar::all(0));
        }
        return histImage;
    }
};

#endif // HISTOGRAM1D_H
