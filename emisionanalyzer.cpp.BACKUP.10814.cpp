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

bool EmisionAnalyzer::isInside(Mat &in, Point &p)
{
    return (p.x >= 0 && p.y >=0 && p.x < in.cols && p.y < in.rows);
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
        if (   pos.y - offset < 0 
            || pos.x - offset < 0 
            || pos.x + offset >= in.cols 
            || pos.y + offset >= in.rows) {
            break;
        }
        
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
 * @brief Обходит всех соседей точки pos иеющие тоже значение
 * что и pos, и формирует средную точку
 * @param in
 * @param pos
 * @return 
 */
Point EmisionAnalyzer::getCenter(Mat &in, Point pos, Point center)
{
<<<<<<< HEAD
    Mat_<Vec3b> outRef = in;
    outRef(pos)[1] |= EA_VISITED; // mark as visited
    
    center += pos;
    for(int i=1;i<=8;++i) {
        Point neigh = getNeighboor(pos, i);
        if ( outRef(neigh)[0] == outRef(pos)[0] && !(outRef((neigh))[1]&EA_VISITED) ) {
            getCenter(in, neigh, center);
        }
    }
    return center;
=======
    in.at<Vec3b>(pos)[1] |= EA_VISITED; // mark as visited
    
    center.x+=1; // 
    
    if (center.x > 100) return Point(-1,-1); // wrong :D to deep
    
    for(int i=1;i<=8;++i) {
        Point neigh = getNeighboor(pos, i);
        if (isInside(in, neigh)) {
            if ( in.at<Vec3b>(neigh)[0] == in.at<Vec3b>(pos)[0] && !(in.at<Vec3b>((neigh))[1]&EA_VISITED) ) {
                if ( getCenter(in, neigh, center).x == -1) {
                    return Point(-1,-1);
                }
            }
        }
    }
    return pos;
>>>>>>> 0524073d6699d5deff12f2de859889bd6444baf5
}

/**
 * @brief EmisionAnalyzer::findCircles
 * @param in, CV_8UC3 Mat image
 * @return image representaion of findin pixels
 */
cv::Mat EmisionAnalyzer::findCircles(KeyPoints &keyPoints)
{
    int minValue;
    
    
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return cv::Mat();
    }    
        
    int iCols = gImage.cols, iRows = gImage.rows;
   

    Mat out = Mat::zeros(gImage.rows, gImage.cols, gImage.type());
    Mat_<Vec3b> outRef = out;
    
    
    PointValue p;
    QVector<PointValue> blackPoints; // черные точки
    QVector<PointValue> extrePoints; // локальные экстремумы
    
    /*
     * 1. проходим по изображению пересчитываю
     * симетрические области 
     */
    for(int y = 0; y < iRows; ++y) {
        for (int x = 0; x < iCols; ++x ) {
            Point p = cv::Point(x, y);
            int value = isInsideCircle(p);
            if ( value ) {
                blackPoints.push_back(PointValue(p, value));
            }
            outRef(p)[0] = value;
        }
    }
    
    // размываю точки
    cv::GaussianBlur(out,out,cv::Size(5, 5), 3);

    /**
     * Ищем точки, в радиусе depth от которых нету 
     * точек с большим значением
     */
    int countOfPointsWithGreaterValue;
    int depth = 5;
    foreach (p, blackPoints) {
        countOfPointsWithGreaterValue = 0;
        //outRef(p.point)[1] |= EA_VISITED;
        
        for (int i=-depth; i<depth; ++i) {
            for (int j=-depth;j<depth; ++j) {
                if (i!=0 || j!=0) {
                    cv::Point nP(p.point.x+j, p.point.y+i);
<<<<<<< HEAD
                    if ( outRef(nP)[0] > outRef(p.point)[0] ) {
                        countOfPointsWithGreaterValue++;
                        goto stop; // о----+
                    }                   // |
                }                       // |
            }                           // |
        }                               // |
        stop:        // <----- there ------+
        if (countOfPointsWithGreaterValue == 0) {
            extrePoints.push_back(p);
            
=======
                    if (isInside(out, nP)) {
                        if ( outRef(nP)[0] > outRef(p.point)[0] ) {
                            countOfPointsWithGreaterValue++;
                            goto stop; // о----+
                        }                   // |
                    }                       // |
                }                           // |
            }                               // |
        }                                   // |
        stop:        // <----- there ----------+
        if (countOfPointsWithGreaterValue == 0) {
            extrePoints.push_back(p);
>>>>>>> 0524073d6699d5deff12f2de859889bd6444baf5
        }
    }
    
    /**
     * Для каждой области с экстремуми выбираем среднюю точку
     */
    foreach (p, extrePoints) {
        if ( !(outRef(p.point)[1] & EA_VISITED) ) {
            MKeyPoint key;
            cv::Point pTemp = getCenter(out, p.point);
<<<<<<< HEAD
            key.pos = QPoint(pTemp.x, pTemp.y);
            key.value = p.value;
            keyPoints.addKey(key);
            outRef(pTemp)[2] = 255;
=======
            if (pTemp.x != -1) {
                key.pos = QPoint(pTemp.x, pTemp.y);
                key.value = p.value;
                keyPoints.addKey(key);
                outRef(pTemp)[2] = 255;
            }
            //outRef(p.point)[2] = 255;
>>>>>>> 0524073d6699d5deff12f2de859889bd6444baf5
        }
    }
    
    return out;
    //out.convertTo(out, CV_8UC3);
}

bool EmisionAnalyzer::isCircle(cv::Point position, int radius)
{
}   
