#include <QDebug>
#include "emisionanalyzer.h"
#include "Utils/opencvutils.h"
#include "Utils/opencvutils_tempates.h"
#include <boost/tuple/tuple.hpp>

using namespace cv;
using namespace std;
using namespace boost::lambda;
using namespace boost::tuples;
using namespace boost;


EmisionAnalyzer::EmisionAnalyzer()
{
    progressCallback = 0;
}

void EmisionAnalyzer::setImage(Mat &image)
{
    // чтобы не было ошибок преобразования 
    if (image.type() != CV_8UC3) {
        qWarning() << "wrong format of input image, image is not set, should be CV_8UC3";
        return;
    }
    
    // создаем пустое изображение
    gImage = Mat::zeros(gImage.rows, gImage.cols, CV_8U);
    cvtColor(image, gImage, CV_RGB2GRAY);
    // 
    gImageRef = gImage;

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

void EmisionAnalyzer::getMinMax(QList<Point> area,
                                Mat1i image,
                                int &min, int &max)
{
    min = image(area.at(0));
    max = image(area.at(0));
    foreach (Point p, area) {
        if (image(p) < min) min = image(p);
        if (image(p) > max) max = image(p);
    }
}

bool EmisionAnalyzer::isOnEdge(Point point)
{
    for(int i=1;i<=8;++i) {
        Point neigh = OpenCVUtils::getNeighboor(point, i);
        if (gImageRef(neigh) == EA_EMPTY) {
            return true;
        }
    }
    return false;
}

// return radius of the most available circle
int EmisionAnalyzer::getSymmetryValue(Point pos, int i)
{  
    if ( gImageRef(pos) != EA_BLACK ) return 0;
    
    int offset = 0;
    // кол-во направлений по которым перестала расширяться область
    i = qMin(i, 8);
    int stopItems = 0;
    do {
        if ( pos.y + offset >= gImageRef.rows
             || gImageRef(pos.y + offset, pos.x) != EA_BLACK ) stopItems++; // to right
        
        if ( pos.y - offset < 0 
              ||gImageRef(pos.y - offset, pos.x) != EA_BLACK ) stopItems++; // to left
        
        if ( pos.x - offset < 0 
             || gImageRef(pos.y, pos.x - offset) != EA_BLACK ) stopItems++; // to top
        
        if ( pos.y + offset >= gImageRef.rows
             || gImageRef(pos.y, pos.x + offset) != EA_BLACK ) stopItems++; // to bottom
        
        if ( pos.y + offset >= gImageRef.rows || pos.x - offset < 0
             || gImageRef(pos.y + offset, pos.x - offset) != EA_BLACK ) stopItems++;
        
        if ( pos.y - offset < 0 || pos.x - offset < 0 
             || gImageRef(pos.y - offset, pos.x - offset) != EA_BLACK ) stopItems++;
        
        if ( pos.y - offset < 0 || pos.x + offset >= gImageRef.cols
             || gImageRef(pos.y - offset, pos.x + offset) != EA_BLACK ) stopItems++; 
        
        if ( pos.y + offset > gImageRef.rows || pos.x + offset >= gImageRef.cols
             || gImageRef(pos.y + offset, pos.x + offset) != EA_BLACK ) stopItems++; 
        
        if (stopItems >= i) {
            break;
        }
        
        offset ++;        
    }while(true);
    
    return offset;
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
        cv::Point neighP = OpenCVUtils::getNeighboor(p, i);
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

template<class MatXX>
void __getAreaPoints(cv::Point const &point, MatXX &,
                     tuple< int *,cv::Point *, cv::Mat1b*> &info)
{
    int *count = get<0>(info);
    (*count)++;
    
    cv::Point *p = get<1>(info);
    p->x += point.x;
    p->y += point.y;
    
    cv::Mat1b* flagImage = get<2>(info);
    (*flagImage)(point) |= EmisionAnalyzer::EA_VISITED;
}

/**
 * @brief EmisionAnalyzer::findCircles
 * @param in, CV_8UC3 Mat image
 * @return image representaion of findin pixels
 */
void EmisionAnalyzer::findCircles(KeyPoints &keyPoints)
{
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return;
    }    
    
    // изображение для хранения хначений пикселей
    Mat valueImageOrigin = Mat::zeros(gImage.rows, gImage.cols, CV_16SC1);
    Mat1i valueImage = valueImageOrigin;
    
    QVector<Point> extremPoints; // локальные экстремумы

    // заполненые области    
    QList< QList<cv::Point> > blackAreas;
    // нахожу заполненые области
    OpenCVUtils::getKeyAreas<cv::Mat1b>(blackAreas, EA_BLACK, gImageRef);
    
    int symmValue;
    foreach(QList<cv::Point> area, blackAreas) {
        foreach(cv::Point p, area) {
            symmValue = getSymmetryValue(p, 1);
            valueImage(p) = symmValue;
        }
    }
    
    // размываю индексы симметрии
    cv::GaussianBlur(valueImageOrigin, valueImageOrigin,cv::Size(5, 5), 3);

    
    int countOfPointsWithGreaterValue;
    // Ищем точки, в радиусе depth от которых 
    // нету точек с большим значением
    int depth = 5;
    foreach (QList<Point> area, blackAreas) {
        foreach(Point p, area) {
            countOfPointsWithGreaterValue = 0;
            for (int i=-depth; i<depth; ++i) {
                for (int j=-depth;j<depth; ++j) {
                    cv::Point neigh(p.x+j, p.y+i);
                    if(OpenCVUtils::isInside(valueImage, neigh)) {
                        if (valueImage(p) < valueImage(neigh)) {
                            countOfPointsWithGreaterValue++;
                            goto stop;
                        }
                    }
                }
            }
            stop:
            if (countOfPointsWithGreaterValue == 0) {
                extremPoints.push_back(p);
            }
        }
    }
    
    // флаги
    Mat1b flagImage = Mat::zeros(gImage.rows, gImage.cols, CV_8U);
    
    // Для каждой области с экстремум выбираем среднюю точку
    foreach (Point p, extremPoints) {
        // если пиксел не посещенный
        if ( !(flagImage(p) & EA_VISITED) ) {
            // помечаем как посещенный
            flagImage(p) |= EA_VISITED;
            
            // кол-во точек в области
            int pointsCount = 1;
            cv::Point avgPoint = p; 
            
            // передаем информацию для функции обхода области
            // ко-во точек, сумма точек, флаги
            tuple< int *,cv::Point *, cv::Mat1b*>  info;
            info = make_tuple(&pointsCount, &avgPoint, &flagImage);
            OpenCVUtils::bypassArea(p, 
                                    valueImage, 
                                    valueImage(p),
                                    info,
                                    __getAreaPoints);
            
            // пересчитываем центр масс
            avgPoint.x /= pointsCount;
            avgPoint.y /= pointsCount;
            
            // создаем новую ключевую точку 
            Mick::KeyPoint key;
            key.setPos(QPointF(avgPoint.x, avgPoint.y));
            key.setValue(valueImage(p));
            // проверяем не находится ли центр масс области 
            // в зоне действия какой-либо другой точки
            bool fWas = false;
            for(int i=0;i<keyPoints.count();++i) {
                Mick::KeyPoint &k = keyPoints[i];
                int diff = (k.pos() - key.pos()).manhattanLength();
                if ( diff - k.value() - key.value() < 0 ) {
                    fWas = true;
                    break;
                }
            }
            // если наложений не было добавляем точку
            if (!fWas)
                keyPoints.addKey(key);
        }
    }
}

void EmisionAnalyzer::findBlackAreas(QList<QList<Point> > &areas)
{
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return;
    }    
    
    OpenCVUtils::getKeyAreas<Mat1b>(areas, EA_BLACK, gImageRef);
}
 
