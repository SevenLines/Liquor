#ifndef POINTVALUE_H
#define POINTVALUE_H

#include <opencv/cv.hpp>

struct PointValue
{
    cv::Point point;
    int value;
    
public :
    
    PointValue() {
        PointValue(cv::Point(), 0);
    }
    
    PointValue(cv::Point point, int value) 
    {
        this->point = point;
        this->value = value;
    }
    
};
#endif // POINTVALUE_H
