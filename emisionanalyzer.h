#ifndef EMISIONANALYZER_H
#define EMISIONANALYZER_H

#include <opencv/cv.hpp>

using namespace cv;


class EmisionAnalyzer
{
private:
    int gMinRadius;
    int gMaxRadius;
    Mat gImage;
    
public:
    EmisionAnalyzer();
    
    
    // only binary images accepted
    void setImage(Mat &image);
    Mat &image();
    
    void setMinRadius(int value);
    int minRadius();
    
    void setMaxRadius(int value);
    int maxRadius();
    
    void findCircles(Mat in, Mat &out);
    bool isCircle(cv::Point position, int radius);
    int getSymmetryIndex(Mat &in, cv::Point pos);
    int getHorizontalSymmetry(cv::Mat &in, cv::Point pos);
    int getVerticalSymmetry(cv::Mat &in, cv::Point pos);
};

#endif // EMlISIONANALYZER_H
