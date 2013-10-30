#include "emisionanalyzer.h"

using namespace cv;
using namespace std;



EmisionAnalyzer::EmisionAnalyzer()
{
}

void EmisionAnalyzer::setImage(Mat &image)
{
    gImage = image;
}

Mat &EmisionAnalyzer::image()
{
    return gImage;
}

void EmisionAnalyzer::setMinRadius(int value)
{
    gMinRadius = value;
}

int EmisionAnalyzer::minRadius()
{
    return gMinRadius;
}

void EmisionAnalyzer::setMaxRadius(int value)
{
    gMaxRadius = value;
}

int EmisionAnalyzer::maxRadius()
{
    return gMaxRadius;
}

int EmisionAnalyzer::getHorizontalSymmetry(cv::Mat &in, cv::Point pos) 
{
    //left index;
    int lIndex = 0, rIndex = 0;
    while( in.at<uchar>(pos.y, pos.x-lIndex) == 0) {
        ++lIndex;
    }
    while( in.at<uchar>(pos.y, pos.x+rIndex) == 0) {
        ++rIndex;
    }
    
    if (rIndex == 0 || lIndex == 0) 
        return 0;
    /*
    if (rIndex > lIndex) 
        return (float)lIndex/rIndex * 255;
    else 
        return (float)rIndex/lIndex * 255;*/
    in.at<int>(pos) = lIndex>rIndex ? rIndex : lIndex; 
}

int EmisionAnalyzer::getVerticalSymmetry(cv::Mat &in, cv::Point pos) 
{
    //left index;
    int lIndex = 0, rIndex = 0;
    while( in.at<uchar>(pos.y-lIndex, pos.x) == 0) {
        ++lIndex;
    }
    while( in.at<uchar>(pos.y+rIndex, pos.x) == 0) {
        ++rIndex;
    }
    
    in.at<int>(pos) = lIndex>rIndex ? rIndex : lIndex;   
}

int EmisionAnalyzer::getSymmetryIndex(cv::Mat &in, cv::Point pos)
{
    int hor = getHorizontalSymmetry(in, pos);
    int ver = getVerticalSymmetry(in, pos);
    
    if (ver == 0 || hor == 0) 
        return 0;
    
    if (hor > ver) 
        return (float)ver/hor * 255;
    else 
        return (float)hor/ver * 255;
}

void EmisionAnalyzer::findCircles(cv::Mat in, cv::Mat& out)
{
    int iRows, iCols;
    
    vector<Mat> rgb;
    cv::split(in, rgb);
    Mat red = rgb[0];
    
    iRows = red.rows;
    iCols = red.cols;

    out = Mat::zeros(in.rows, in.cols, CV_32SC3);
    
    for(int i = 0; i < iRows; ++i) {
        for (int j = 0; j < iCols; ++j ) {
            out.at<int>(i,j) = getSymmetryIndex(red, cv::Point(j,i) );
        }
    }
    
    out.convertTo(out, CV_8UC3);
}

bool EmisionAnalyzer::isCircle(cv::Point position, int radius)
{
}   
