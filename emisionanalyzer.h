#ifndef EMISIONANALYZER_H
#define EMISIONANALYZER_H

#include <opencv/cv.hpp>
#include <boost/lambda/lambda.hpp>
#include "keypoints.h"

using namespace cv;


/**
 * @brief возвращает соседа под номером num
 *
 * num:
 * +-----+
 * |1|2|3|
 * |8|x|4|
 * |7|6|5|
 * +-----+
 *
 * @param p точка x
 * @param num номер соседа точки
 * @return точка
 */
cv::Point getNeighboor(cv::Point p, int num);

struct PointValue
{
    cv::Point point;
    int value;
    
public :
    
    PointValue() : PointValue(cv::Point(), 0){}
    
    PointValue(cv::Point point, int value) 
    {
        this->point = point;
        this->value = value;
    }
    
};

class EmisionAnalyzer
{
    
private:
    
    enum flags{
        EA_VISITED = 1
    }; 
    
    
    int gMinRadius;
    int gMaxRadius;
    Mat gImage;
    
    Mat gSymmetryInfo;// using for storing specific info about each pixel
    
public:
    EmisionAnalyzer();
    
    
    // only binary images accepted
    void setImage(Mat &image);
    Mat &image();
    
    void setMinRadius(int value);
    int minRadius();
    
    void setMaxRadius(int value);
    int maxRadius();
    
    bool isInside(Mat &in, cv::Point &p);

    Mat findCircles(KeyPoints &keyPoints);
    
    bool isCircle(cv::Point position, int radius);
    float getSymmetryIndex(cv::Point pos);
    float getHorizontalSymmetry( cv::Point pos);
    float getVerticalSymmetry(cv::Point pos);
    
    int isInsideCircle(cv::Point pos);
    int checkForFunction(cv::Point pos, int depth,
                         bool (*predicate)(cv::Point pos) );
    
    int getMaxRadius(Point pos);
    
    cv::Point findMaxPoint(Mat &in, cv::Point p, int minValue);
    
    cv::Point getMaxRadius(Mat &in, int minValue, 
                            cv::Point pos,
                            int &curMaxValue,
                            cv::Point &maxPosition);
    
    cv::Point getCenter(Mat &in, cv::Point pos, Point center = Point());
    
    bool checkPointFlags(Mat &in, cv::Point p, int flags);
    
};

#endif // EMlISIONANALYZER_H
