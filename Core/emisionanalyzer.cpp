#include <QDebug>
#include "emisionanalyzer.h"
#include <numeric>
#include "Utils/opencvutils.h"
#include "Utils/opencvutils_tempates.h"

using namespace cv;
using namespace std;

EmisionAnalyzer::EmisionAnalyzer(QObject *parent)
    : QObject(parent)
{
    mKeyPoints = 0;
    fCancel = 0;
    mFillColor = 0;
    mEmptyColor = 255;
    
    mMaxRadius = 500;
    mMinRadius = 1;
    mMinTupleRadius = 1;
}

bool checkSize(vector<Point> vec)
{
    return vec.size() < 5;
}

/**
 * @brief returns list of contours for image
 * @param borders will be filled with contours data
 */
void EmisionAnalyzer::findContours(vector<vector<Point> > &borders)
{
    // поиск контуров
    Mat canny_out;
    Canny(mImageRoi, canny_out, mFillColor, mEmptyColor);
    boundRoi = Rect();
    
    // ищем контуры
    cv::findContours(canny_out, borders, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    
    // если всего 1 контур, скорее всего это рмка вокруг изображения
    if (borders.size()==1) {
        // кропем по рмке
        boundRoi = boundingRect(borders[0]);
        canny_out = canny_out(boundRoi);
        borders.clear();
        // повторяем процедуру поиска частиц
        cv::findContours(canny_out, borders, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    }
    
    // удаляем объекты у которых количество точек меньше заданного значения = 5
    borders.erase(remove_if(borders.begin(), borders.end(), checkSize),
                  borders.end());
}

void EmisionAnalyzer::setImage(Mat &image)
{
    // чтобы не было ошибок преобразования 
    if (image.type() != CV_8UC3) {
        qWarning() << "wrong format of input image, image is not set, should be CV_8UC3";
        return;
    }
    
    // создаем пустое изображение
    mImage = Mat::zeros(mImage.rows, mImage.cols, CV_8U);
    cvtColor(image, mImage, CV_RGB2GRAY);
    cropImage();
    // 
//    mImageRoi = mImage;
}

Mat EmisionAnalyzer::image()
{
    return mImage;
}

void EmisionAnalyzer::setMinRadius(int value)
{
    mMinRadius = value;
    mMinTupleRadius = qMax(mMinTupleRadius, mMinRadius);
}

int EmisionAnalyzer::minRadius()
{
    return mMinRadius;
}

void EmisionAnalyzer::setMaxRadius(int value)
{
    mMaxRadius = value;
}

int EmisionAnalyzer::maxRadius()
{
    return mMaxRadius;
}

void EmisionAnalyzer::setMinTupleRadius(int value)
{
    mMinTupleRadius = value;
}

int EmisionAnalyzer::minTupleRadius()
{
    return mMinTupleRadius;
}

void EmisionAnalyzer::setKeyPoints(KeyPoints *keyPoints)
{
    this->mKeyPoints = keyPoints;
}

KeyPoints *EmisionAnalyzer::keyPoints()
{
    return this->mKeyPoints;
}

Mick::KeyPoint  EmisionAnalyzer::fromContour(vector<Point> &contour)
{
    Mick::KeyPoint key;
    
    if (contour.size() == 0)
        return key;
    
    Point ps;
    for (vector<Point>::iterator i=contour.begin();i!=contour.end();++i) {
//        key.border.push_back(*i);
        ps.x += i->x;
        ps.y += i->y;
    }
    ps.x /= contour.size();
    ps.y /= contour.size();
    Point v(ps.x - contour[0].x, ps.y - contour[0].y);
    float value = sqrt(v.x*v.x + v.y*v.y);
    
    key.setPos(QPointF(ps.x, ps.y));
    key.setValue(value);
    key.setProportion(100);

    return key;
}


int EmisionAnalyzer::getInfoValue(Point pos, int channel)
{
    return mSymmetryInfo.at<Vec3i>(pos)[channel];
}

int EmisionAnalyzer::getInfoValue(int y, int x, int channel)
{
    return getInfoValue(cv::Point(x,y), channel);
}

void EmisionAnalyzer::setInfoValue(int value, Point pos, int channel)
{
    mSymmetryInfo.at<Vec3i>(pos)[channel] = value ;
}

void EmisionAnalyzer::cropImage()
{
//    std::vector<cv::Point> points;
//    cv::Mat_<uchar>::iterator it = mImage.begin<uchar>();
//    cv::Mat_<uchar>::iterator end = mImage.end<uchar>();
//    for (; it != end; ++it)
//    {
//        if (*it) points.push_back(it.pos());
//    }
//    cv::RotatedRect box = minAreaRect(cv::Mat(points));
//    cv::Rect roi;
//    roi.x = box.center.x - (box.size.width / 2);
//    roi.y = box.center.y - (box.size.height / 2);
//    roi.width = box.size.width;
//    roi.height = box.size.height;
    mImageRoi = mImage;
}


/**
 * @brief EmisionAnalyzer::findCircles
 * @param in, CV_8UC3 Mat image
 * @return image representaion of findin pixels
 */
void EmisionAnalyzer::find()
{
    fCancel = false;
     
    int progressMax = 4;
    int progress = 0;
    int l = 8; // step
    int app = 2; // approximation value
    
    if ( mImage.empty() ) {
        qWarning() << "image is not set!";
        return;
    }    
    if (mKeyPoints == 0) {
        qWarning() << "keypoints is not set";
        return;
    }
    
    mKeyPoints->clear();
    
    emit progressChanged(0,progressMax, QString("looking for countours"));
    vector<vector<Point> > borders;
    vector<vector<Point> > single_objects, tuples, complex;
    findContours(borders);
    
    emit progressChanged(++progress,progressMax, QString("split objects by complexity"));
    // для каждого контура
    for(int i=0;i<borders.size();++i) {
        vector<Point> &brd = borders[i];
        
        // объект слишком маленький и его можно пропустить
        if (brd.size() < 3 * l) continue;
        
        // тупая апроксимация, рекомедуемое значение app = 2
        vector<Point> nbrd;
        for (int j=0;j<brd.size();j+=app) {
            nbrd.push_back(brd[j]);
        }
        brd = nbrd;
        borders[i] = brd;
        
        int count = brd.size();
        vector<Point> dangerBorder; // промежуток вогнутости
        int count_of_edges = 0; // кол-во промежутков вогнутости
        bool fLastWas = false; // флаг сигнализирующий о том проходим ли мы сейчас по промежутку вогнутости
        
        /// ищем промежутки вогнутости на границах
        for (int j=l;j<count + l;++j) {
            Point pl = brd[(j-l)%count];    /// [pl]-----[p]----[pr] // три точки образуют угол
            Point p = brd[j%count];         ///   |___l___|__l___| растояние между тремя точками
            Point pr = brd[(j+l)%count];
            
            /// берем среднюю точку и смотрим между краевыми pl и pr
            Point pmid = Point( 0.5*(pl.x + pr.x), 0.5*(pl.y + pr.y));
            
            /// и проверяем лежит ли она вне объекта
            if (mImageRoi(pmid) == mEmptyColor) {
                // если вне объекта значит начался промежуток вогнутости
                dangerBorder.push_back(p);
                fLastWas = true; // включаем флаг
            } else {
                /// если не лежит, завершаем промежуток вогнутости,
                /// если такое был, конечно
                if (fLastWas) {
                    // необходимо зафиксировать медианую точку промежутка вогнутости
                    // медианную потому что весь промежуток нас не интересует, вообще говоря
//                    Point avgPoint = dangerBorder[dangerBorder.size()/2];
//                    edges.push_back(avgPoint);
                    count_of_edges++;
                    
                    dangerBorder.clear();
                }
                // сбрасываем флаг
                fLastWas = false;
            }
        }
        
        // заполняем массив простых объектов
        if (count_of_edges <= 1) {
            single_objects.push_back(brd);
        } else if (count_of_edges >= 2 && count_of_edges <= 3) {
            tuples.push_back(brd);
        } else {
            complex.push_back(brd);
        }
    }
    
    emit progressChanged(++progress,progressMax, QString("spliting complex objects"));
    /// TODO spliting complex objects
    
    emit progressChanged(++progress,progressMax, QString("forming key points"));
    
    vector<vector<Point> >::iterator brd = single_objects.begin();
    for (;brd != single_objects.end(); ++brd) {
        Mick::KeyPoint key = fromContour(*brd);
        
        QPointF pos = key.pos();
        pos.setX(pos.x() + boundRoi.x); // учет сдвига если изображение было обрезано
        pos.setY(pos.y() + boundRoi.y); // учет сдвига если изображение было обрезано
        key.setPos(pos);
        key.setMarker(1);
        // проверяем размеры частицы на допустимость
        if (key.value() >= mMinRadius && key.value() <= mMaxRadius) {
            mKeyPoints->addKey(key);
        }
    }
    
    brd = tuples.begin();
    for (;brd!=tuples.end();++brd) {
        Mick::KeyPoint key = fromContour(*brd);

        QPointF pos = key.pos();
        pos.setX(pos.x() + boundRoi.x); // учет сдвига если изображение было обрезано
        pos.setY(pos.y() + boundRoi.y); // учет сдвига если изображение было обрезано
        key.setPos(pos);

        // проверяем размеры частицы на допустимость
        if (key.value() >= mMinRadius && key.value() <= mMaxRadius) {
            if (key.value() >= mMinTupleRadius)
                key.setMarker(2);
            else
                key.setMarker(1);
            mKeyPoints->addKey(key);
        }
    }

    brd = complex.begin();
    for (;brd!=complex.end();++brd) {
        Mick::KeyPoint key = fromContour(*brd);

        QPointF pos = key.pos();
        pos.setX(pos.x() + boundRoi.x); // учет сдвига если изображение было обрезано
        pos.setY(pos.y() + boundRoi.y); // учет сдвига если изображение было обрезано
        key.setPos(pos);
        // проверяем размеры частицы на допустимость
        if (key.value() >= mMinRadius && key.value() <= mMaxRadius) {
            if (key.value() >= mMinTupleRadius)
                key.setMarker(3);
            else
                key.setMarker(1);
            mKeyPoints->addKey(key);
        }
    }
    
    mKeyPoints->setType(KeyPoints::Particles);
    
    
    emit progressChanged(progressMax,progressMax);
}

void EmisionAnalyzer::cancel()
{
    fCancel = true;
}
