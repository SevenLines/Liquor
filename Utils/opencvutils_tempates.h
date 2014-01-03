#ifndef OPENCVUTILS_TEMPATES_H
#define OPENCVUTILS_TEMPATES_H

#include <opencv/cv.h>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <boost/tuple/tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <functional>

using namespace boost::tuples;
using namespace boost::lambda;
using namespace std;


namespace OpenCVUtils {

    
    template<class MatXX, class InfoType, class TypeCompare> 
    void __bypassArea( Point firstPoint,
                     MatXX image,
                     TypeCompare &compareObject,
                     bool (*comparer)(Point const &, MatXX &, TypeCompare&),
                     InfoType &object,
                     void (*func)(Point const &, MatXX &, InfoType &)){
        int i;
        
        QList<Point> points;
        QList<int> lastNeighs;
        
        Mat1b flagImage = Mat::zeros(image.rows, image.cols, CV_8U);
    
        points.reserve(flagImage.cols*flagImage.rows);
        lastNeighs.reserve(flagImage.cols*flagImage.rows);
        
        points.push_back(firstPoint);
        lastNeighs.push_back(1);
        // пока есть точки
        while(points.size() > 0) {
            Point &curPoint = points.last();
            int &lastNeigh = lastNeighs.last();
    
            // помечаю
            flagImage(curPoint) |= 1;
            
            // обхожу всех соседей текущей точки
            for (i=lastNeigh; i<=8; ++i) {
                Point neigh = OpenCVUtils::getNeighboor(curPoint, i);
                if (isInside(flagImage, neigh)) {
                    if ( !(flagImage(neigh)&1) ) {
                        if ( comparer(neigh, image, compareObject) ) { //image(neigh) == keyValue 
                            
                            // вызываем привязанную функцию
                            if (func)
                                func(neigh, image, object);
                            
                            // если точка черного цвета и не пройдена
                            // повторяем процедуру для данной точки         
                            points.append(neigh);
                            lastNeighs.append(1);
                            
                            lastNeigh = i;
                            break;
                        }
                    }
                }
            }
            // если не было найденно непройденных соседей, 
            // точку можно выкинуть
            if (i>8) {
                points.pop_back();
                lastNeighs.pop_back();
            }
        }  
    }

    template<class MatXX>
    bool __comparer_eq(cv::Point const &point, MatXX &image, int &value) {
        return image(point) == value;
    }
    
    /**
     * @brief bypassArea обходит зону пикселей у которых значение
     * равно keyValue в канале channel
     */
    template<class MatXX, class InfoType> 
    void bypassArea( Point firstPoint,
                     MatXX image,
                     int keyValue,
                     InfoType &object,
                     void (*func)(Point const &, MatXX &, InfoType &))
    {    
        
        __bypassArea(firstPoint, 
                   image, 
                   keyValue, 
                   __comparer_eq,
                   object, 
                   func);
    }
   
    template<class MatXX>
    bool __comparer_neq(cv::Point const &point, MatXX &image, int &value) {
        return image(point) != value;
    }
    
    template<class MatXX>
    void __getMinMax(cv::Point const &point, MatXX &image, tuple<int, int> &min_max){
        if ( image(point) < get<0>(min_max))
            get<0>(min_max) = image(point);
        if ( image(point) > get<1>(min_max))
            get<1>(min_max) = image(point);
    }
    
    template<class MatXX>
    void getMinMax(cv::Point const &point, MatXX &Image, int emptyValue ) 
    {
        tuple<int, int> min_max;
        min_max = make_tuple(Image(point),Image(point));
        
        __bypassArea(point,
                     Image,
                     emptyValue,
                     __comparer_neq,
                     min_max,
                     __getMinMax
                     );
    }
    
    
    template<class MatXX>
    void __getKeyAreaPoints(cv::Point const &point, MatXX  &,
                            tuple<QList<Point> *, cv::Mat1b *> &info) {
        (*get<0>(info)).append(point);
        (*get<1>(info))(point) |= 1;
    }    
    
    /**
     * Возвращает список замкнутых областей со значением пикселей равных keyValue
     */
    template<class MatXX>
    void getKeyAreas(QList<QList<cv::Point> > &areas, int keyValue, MatXX &Image)
    {
    
        areas.clear();
        
        int iCols = Image.cols, iRows = Image.rows;
        int k,l;
        
        // создаю временное изображение для запоминания пройденных пикселей
        cv::Mat1b flagImage = Mat::zeros(Image.rows, Image.cols, CV_8U);
        
        // ищу заполненные области
        for ( k=0; k<iRows; ++k ) {
            for ( l=0; l<iCols; ++l ) {
                Point point = cv::Point(l,k);
                // проверяю каждый пиксель
                if ( Image(point) == keyValue // если черный
                     && !(flagImage(point)&1) ) { // и не посещенный
                    
                    QList<Point> areaPoints;
                    tuple<QList<Point> *, cv::Mat1b *> info;
                    info = make_tuple(&areaPoints, &flagImage);
                    
                    bypassArea(point, 
                              Image, 
                              keyValue, 
                              info, 
                             __getKeyAreaPoints);
                    
                    areas.append(areaPoints);
                }
            }
        }
    }
}

#endif // OPENCVUTILS_TEMPATES_H
