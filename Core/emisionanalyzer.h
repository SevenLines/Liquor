#ifndef EMISIONANALYZER_H
#define EMISIONANALYZER_H

#include <opencv/cv.hpp>
#include <boost/tuple/tuple.hpp>
#include "keypoints.h"
#include "pointvalue.h"
#include <QObject>

using namespace boost::tuples;
using namespace cv;
using namespace Mick;


class EmisionAnalyzer  : public QObject
{
    Q_OBJECT
    
private:

    int gMinRadius;
    int gMaxRadius;
    
    KeyPoints *keyPoints;
    
    Mat gImage; // копия изображения, тип CV_8U
    Mat1b gImageRef; // для упрощения доступа к изображению
    
    Mat gSymmetryInfo;// using for storing specific info about each pixel
    
    /**
     * @brief Возвращает радиус максимальной окружности которую 
     * можно вписать в область с центром в текущей точке
     */
    int getSymmetryValue(cv::Point pos, int i=1);
    int checkForFunction(cv::Point pos, int depth,
                         bool (*predicate)(cv::Point pos) );
    
    
    cv::Point findMaxPoint(Mat &in, cv::Point p, int minValue);
    
    
    /**
     * @brief Находит все точки изображения
     * @param point -- точка с которой нчинается обход
     * @param areaPoints -- список под точки
     * @param flagImage -- изображения для фиксирования пройденных точек, CV_8U
     */
    void getBlackArea(Point point, QList<cv::Point> &areaPoints, Mat1b &flagImage);

public:
    
    /// функция вызывается при каждом изменении прогресса
    /// value, min, max, object
    void (*progressCallback)(int, int, int, void*);
    
    enum flags{
        EA_BLACK = 0,
        EA_VISITED = 1,
        EA_EMPTY = 255
    }; 
    
    EmisionAnalyzer();
    
    // only binary images accepted
    void setImage(Mat &image);
    Mat &image();
    
    void setMinRadius(int value);
    int minRadius();
    
    void setMaxRadius(int value);
    int maxRadius();
    
    void setKeyPoints(KeyPoints *keyPoints);
    KeyPoints *getKeyPoints();

    
    /**
     * @brief возвращает минимальное и максимальное значение для 
     * области пикселей заданной area на изображении image
     */
    void getMinMax(QList<cv::Point> area, Mat1i image, int &min, int &max);
    
    /**
     * @brief Возвращает true если пиксель угловой
     */
    bool isOnEdge(cv::Point point);

public slots:

    
    /**
     * @brief поиск заполненых окружностей на изображении
     */
    void findCircles();
    
    /**
     * @brief Заполняет массив areas списком областей у которых в 1-ом канале 
     * находится ноль, areas[0] -- список точки первой области и т.д.
     * @param areas -- список под точки
     */
    void findBlackAreas(QList<QList<cv::Point> > &areas);  
    
signals:
    void finishedLookingForCircles(EmisionAnalyzer *sender);
    
};

#endif // EMlISIONANALYZER_H
