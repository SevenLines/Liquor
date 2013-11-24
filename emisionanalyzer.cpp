#include "emisionanalyzer.h"
#include <QDebug>

using namespace cv;
using namespace std;
using namespace boost::lambda;


Point getNeighboor(Point p, int num)
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
}

EmisionAnalyzer::EmisionAnalyzer()
{
}

void EmisionAnalyzer::setImage(Mat &image)
{
    if (image.type() != CV_8UC3) {
        qWarning() << "wrong format of input image, image is not set, should be CV_8UC3";
        return;
    }
    gImage = image;
    // using for storing specific info about each pixel
    gSymmetryInfo = Mat::zeros(gImage.rows, gImage.cols, CV_32SC3);
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

/**
 * @brief возвращает значение от 0 до 1000, соответствующей 
 * отношению горизонтальной семетрии в данной точке, в промилях
 *
 * @param pos
 * @return 
 */
float EmisionAnalyzer::getHorizontalSymmetry(cv::Point pos) 
{
    int lIndex = 0, rIndex = 0;
    
    Mat_<Vec3b> in = gImage;
    
    while( lIndex < gMaxRadius && in(pos.x-lIndex, pos.y)[0] == 0) {
        ++lIndex;
    }
    while( rIndex < gMaxRadius && in(pos.x+rIndex, pos.y)[0] == 0) {
        ++rIndex;
    }
    
    if (rIndex == 0 || lIndex == 0) 
        return 0;
    
    float val = (float)lIndex / rIndex;
    return val>1 ? 1.0f/val : val;
}

float EmisionAnalyzer::getVerticalSymmetry(cv::Point pos) 
{
    int lIndex = 0, rIndex = 0;
    
    Mat_<Vec3b> in = gImage;
    
    while( lIndex < gMaxRadius && in(pos.x, pos.y-lIndex)[0] == 0) {
        ++lIndex;
    }
    while( rIndex < gMaxRadius && in(pos.x, pos.y+rIndex)[0] == 0) {
        ++rIndex;
    }
    
    if (rIndex == 0 || lIndex == 0) 
        return 0;
    
    float val = (float)lIndex / rIndex;
    return val>1 ? 1.0f/val : val;
}

// return radius of the most available circle
int EmisionAnalyzer::isInsideCircle(Point pos)
{  
    Mat_<Vec3b> in = gImage;

    if ( in(pos)[0] != 0 ) return 0;
    
    int offset = 0;
    do {
        if ( in(pos.y + offset, pos.x)[0] != 0 ) break; // to right
        if ( in(pos.y - offset, pos.x)[0] != 0 ) break; // to left
        if ( in(pos.y,          pos.x - offset)[0] != 0 ) break; // to top
        if ( in(pos.y,          pos.x + offset)[0] != 0 ) break; // to bottom
        
        if ( in(pos.y + offset, pos.x - offset)[0] != 0 ) break;
        if ( in(pos.y - offset, pos.x - offset)[0] != 0 ) break; 
        if ( in(pos.y - offset, pos.x + offset)[0] != 0 ) break; 
        if ( in(pos.y + offset, pos.x + offset)[0] != 0 ) break; 
        
        offset ++;        
    }while(true);
    
    return offset;
}

int EmisionAnalyzer::checkForFunction(Point pos, int depth, bool (*predicate)(Point))
{
    return 0;
}

int EmisionAnalyzer::getMaxRadius(Point pos)
{
    int lIndex = 0, rIndex = 0;
    Mat_<Vec3b> in = gImage;
    
    while( lIndex < gMaxRadius && in(pos.x, pos.y-lIndex)[0] == 0) {
        if (in(pos.x, pos.y+rIndex)[0] != 0) {
            break;
        }
        ++lIndex;
        ++rIndex;
    } 
    return lIndex;
}

Point EmisionAnalyzer::findMaxPoint(Mat &in, Point p, int minValue)
{
    Mat_<Vec3b> inref = in;
    int newValue, curValue = inref(p)[0];
    
    inref(p)[1] |= EA_VISITED;
    
    if (inref(p)[0] == 0) {
        return Point(-1,-1);
    }
    
    Point outPos = p;
    Point pTemp;
    
    for (int i=1;i <= 8; ++i ) {
        // iterrate over all neighboors of point
        cv::Point neighP = getNeighboor(p, i);
        newValue = inref(neighP)[0];
        if (curValue < newValue && newValue >= minValue ) {
            if (! (inref(neighP)[1] & EA_VISITED) ) { // check s point visited
                
                //outPos = neighP; // find point with value greater then ours
                pTemp = findMaxPoint(in, neighP, minValue); 
                
                // check is new value is better then the last one
                if (pTemp.x != -1) {
                    if ( outPos.x == -1 || 
                         ( inref(outPos )[0] < inref(pTemp)[0]) ) {
                        outPos = pTemp; // then assign it to new resutl;
                    }
                }
            }
        }
    }
    
    return outPos;
}

bool EmisionAnalyzer::checkPointFlags(Mat &in, Point p, int flags)
{
    return in.at<Vec3b>(p.x, p.y)[1] & flags;
}

float EmisionAnalyzer::getSymmetryIndex(cv::Point pos)
{
    float hor = 0, ver = 0;
    int maxRadius = 0;
    if (gImage.at<Vec3b>(pos.x, pos.y)[0] == 0) { 
        hor = getHorizontalSymmetry(pos);
        ver = getVerticalSymmetry(pos);
        maxRadius = getMaxRadius(pos);
    }
    return ver*hor * 255;
}

cv::Point EmisionAnalyzer::getMaxRadius(Mat &in, 
                                        int minValue,
                                        cv::Point pos,
                                        int &curMaxValue,
                                        cv::Point &maxPosition)
{
    in.at<Vec3b>(pos.x, pos.y)[1] |= EA_VISITED;
    qDebug() << in.at<Vec3b>(pos.x, pos.y)[1];
    
    int newValue = getMaxRadius(pos);
    if (newValue > curMaxValue) {
       curMaxValue = newValue;
       maxPosition = pos;
    }
    
    for (int i=1; i<=8; i++) {
        cv:Point p = getNeighboor(pos, i);
        
        int value = in.at<Vec3b>(p.x, p.y)[0];
        int flag = in.at<Vec3b>(p.x, p.y)[1];
        
        if (value >= minValue && ! (flag & EA_VISITED) ) {
            getMaxRadius(in, minValue, p, curMaxValue, maxPosition);
        }
    }
    return maxPosition;
}

/**
 * @brief EmisionAnalyzer::findCircles
 * @param in, CV_8UC3 Mat image
 * @return image representaion of findin pixels
 */
cv::Mat EmisionAnalyzer::findCircles(cv::Mat in)
{
    int minValue;
    
    
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return cv::Mat();
    }    
        
    int iCols = gImage.cols, iRows = gImage.rows;
   

    Mat out = Mat::zeros(gImage.rows, gImage.cols, gImage.type());
    Mat_<Vec3b> outRef = out;
    
    
    QVector<PointValue> points;
    /*
     * 1. проходим по изображению пересчитываем пересчитываю
     * симетрические области 
     */
    for(int y = 0; y < iRows; ++y) {
        for (int x = 0; x < iCols; ++x ) {
            Point p = cv::Point(x, y);
            int value = isInsideCircle(p);
            if ( value ) {
                points.push_back(PointValue(p, value));
            }
            outRef(p)[0] = value;
        }
    }
    
    cv::GaussianBlur(out,out,cv::Size(5, 5), 3);
    
    /*
     * 2. Для каждой точки находим точку, 
     * с наибольшим значениме радиуса
     */
    PointValue p;
    /*foreach(p, points) {
        cv::Point p1 = findMaxPoint(out, p.point, 1);
        if (p1.x != -1 && p1 != p.point) {
          outRef(p1)[2] = 255;
        }
    }*/
    
    int countOfPointsWithGreaterValue;
    int depth = 5;
    foreach (p, points) {
        countOfPointsWithGreaterValue = 0;
        outRef(p.point)[1] |= EA_VISITED;
        
        for (int i=-depth; i<depth; ++i) {
            for (int j=-depth;j<depth; ++j) {
                if (i!=0 || j!=0) {
                    cv::Point nP(p.point.x+j, p.point.y+i);
                    if ( outRef(nP)[0] > outRef(p.point)[0] ) {
                        countOfPointsWithGreaterValue++;
                        goto stop;
                    }
                }
            }
        }
        stop: ;
        
        /*for (int i=0;i<=8;++i) {
            Point nP = getNeighboor(p.point, i);
            outRef(nP)[1] |= EA_VISITED;
        }
        
        for (int i=0;i<=8;++i) {
            Point nP = getNeighboor(p.point, i);
            outRef(nP)[1] |= EA_VISITED;
            
            if (outRef(nP)[0] > p.value) {
                countOfPointsWithGreaterValue++;
                continue;
            }
            for (int j=2;j<=8;j+=2) {
                Point nP2 = getNeighboor(nP, j);
                if ( !(outRef(nP2)[1] & EA_VISITED) ) {
                    if (nP2!=nP && outRef(nP2)[0] > outRef(nP)[0]) {
                        countOfPointsWithGreaterValue++;
                        continue;
                    }
                }
            }
        }
        
        for (int i=0;i<=8;++i) {
            Point nP = getNeighboor(p.point, i);
            outRef(nP)[1] = 0;
        }*/
        
        if (countOfPointsWithGreaterValue == 0) {
            outRef(p.point)[2] = 255;
        }
    }
    
    
    /* 
     * 2. проходим по изображению 
     * для каждой конкретной зоны находим точку
     * с наибольшим радиусом симметрии 
     * и с индексом симетрии
     * не меньше заданного значения.
     */
    /*int minSymmetryIndex = 120;
    int maxRadius;
    cv::Point maxPos;
    QVector<cv::Point> points;
    for(int i = 0; i < iRows; ++i) {
        for (int j = 0; j < iCols; ++j ) {
            cv::Point p = cv::Point(j,i);
            
            if (outRef(j,i)[0] > minSymmetryIndex) {
                if (!checkPointFlags(out, p, EA_VISITED)) {
                    maxRadius = 0;
                    maxPos = cv::Point(-1,-1);
                    
                    this->getMaxRadius(out, minSymmetryIndex, 
                                       p, maxRadius, maxPos);
                    points.push_back(maxPos);
                }
            }
        }
    }*/
    
    /*out = cv::Scalar(0);
    cv::Point p;
    foreach(p, points) {
        outRef(p.x, p.y)[1] = 255;
    }
    */
    return out;
    //out.convertTo(out, CV_8UC3);
}

bool EmisionAnalyzer::isCircle(cv::Point position, int radius)
{
}   
