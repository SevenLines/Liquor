#include <QDebug>
#include "particlesseeker.h"
#include "Utils/opencvutils.h"
#include "Utils/opencvutils_tempates.h"
#include <boost/tuple/tuple.hpp>

using namespace cv;
using namespace std;
using namespace boost::tuples;
using namespace boost;


ParticlesSeeker::ParticlesSeeker()
{
    keyPoints = 0;
    
    fCancelEnabled = true;
    fProgressEnabled = true;
}

void ParticlesSeeker::setImage(Mat &image)
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

}

Mat &ParticlesSeeker::image()
{
    return gImage;
}


void ParticlesSeeker::setMinRadius(int value)
{
    gMinRadius = value;
}

int ParticlesSeeker::minRadius()
{
    return gMinRadius;
}

void ParticlesSeeker::setMaxRadius(int value)
{
    gMaxRadius = value;
}

int ParticlesSeeker::maxRadius()
{
    return gMaxRadius;
}

void ParticlesSeeker::setKeyPoints(KeyPoints *keyPoints)
{
    this->keyPoints = keyPoints;
}

KeyPoints *ParticlesSeeker::getKeyPoints()
{
    return this->keyPoints;
}

void ParticlesSeeker::getMinMax(QList<Point> area,
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

bool ParticlesSeeker::isOnEdge(Point point)
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
int ParticlesSeeker::getSymmetryValue(Point pos, int i)
{  
    if ( gImageRef(pos) != EA_BLACK ) return 0;
    
    int offset = 1;
    int maxOffset = -1;
    int newX, newY;
    
    // определяем максимально допустимое значение, на основе ранее расчитанных пикселей
    // больше которого искать смысла нет
    
    // кол-во направлений по которым перестала расширяться область
    i = qMin(i, 8);
    int stopItems = 0;
    do {
        // прохожу по сем восьми направлениям
        for (int i=0;i<8;++i) {
            switch(i) {
            case 0: // bottom
                newY = pos.y + offset;
                newX = pos.x;
                break;
            case 1: // right
                newY = pos.y;
                newX = pos.x + offset;
                break;
            case 2: // top
                newY = pos.y - offset;
                newX = pos.x;
                break;
            case 3: // left
                newY = pos.y;
                newX = pos.x - offset;
                break;
            case 4: 
                newY = pos.y + offset;
                newX = pos.x + offset;
                break;
            case 5:
                newY = pos.y - offset;
                newX = pos.x + offset;
                break;
            case 6:
                newY = pos.y + offset;
                newX = pos.x - offset;
                break;
            case 7:
                newY = pos.y - offset;
                newX = pos.x - offset;
                break;
            }
            
            if ( newY >= gImageRef.rows || newY < 0 || newX <0 || newX >= gImageRef.cols ) {
                // проверка на остановку
                if ( gImageRef(newY, newX) != EA_BLACK ) {
                    stopItems++;
                } /*else { // расчет максимально допустимого отклонения на основе предыдущих рассчетов
                    if ( maxOffset == -1 ) {
                        if ( int value = getInfoValue(newY, newX) != 0 ) {
                            maxOffset = value;
                        }
                    }
                }*/
            }
        }
        
        if (stopItems >= i) {
            break;
        }
        offset ++;  
        
        if (maxOffset!=-1 && offset==maxOffset) {
            break;
        }
    }while(true);
    
    //setInfoValue(offset, pos);
    
    return offset;
}

int ParticlesSeeker::getInfoValue(Point pos, int channel)
{
    return gSymmetryInfo.at<Vec3i>(pos)[channel];
}

int ParticlesSeeker::getInfoValue(int y, int x, int channel)
{
    return getInfoValue(cv::Point(x,y), channel);
}

void ParticlesSeeker::setInfoValue(int value, Point pos, int channel)
{
    gSymmetryInfo.at<Vec3i>(pos)[channel] = value ;
}


Point ParticlesSeeker::findMaxPoint(Mat &in, Point p, int minValue)
{
    Mat_<Vec3b> inref = in;
    // using for storing specific info about each pixel
    //gSymmetryInfo = Mat::zeros(gImage.rows, gImage.cols, CV_32SC3);
    
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
    (*flagImage)(point) |= ParticlesSeeker::EA_VISITED;
}

/**
 * @brief ParticlesSeeker::findCircles
 * @param in, CV_8UC3 Mat image
 * @return image representaion of findin pixels
 */
void ParticlesSeeker::findCircles()
{
    fCancel = false;
    
    int progressMax = 5;
    int progress = 0;
    
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return;
    }    
    if (keyPoints == 0) {
        qWarning() << "keypoints is not set";
        return;
    }
    
    
    // изображение для хранения значений пикселей
    Mat valueImageOrigin = Mat::zeros(gImage.rows, gImage.cols, CV_16SC1);
    Mat1i valueImage = valueImageOrigin;
    
    QVector<Point> extremPoints; // локальные экстремумы

    // заполненые области    
    QList< QList<cv::Point> > blackAreas;
    // нахожу заполненые области
    
    emit progressChanged(progress++,progressMax,
                         tr("Looking for black areas"));
    
    OpenCVUtils::getKeyAreas<cv::Mat1b>(blackAreas, EA_BLACK, gImageRef);
    
    emit progressChanged(progress++,progressMax,
                         tr("Caclulate symmetry indicies"));
    
    int symmValue = 0;
    foreach(QList<cv::Point> area, blackAreas) {
        foreach(cv::Point p, area) {
            if (fCancel) return;
            symmValue = getSymmetryValue(p, 1);
            valueImage(p) = symmValue;
        }
    }
    
    emit progressChanged(progress++,progressMax,
                         tr("Bluring values"));
    
    // размываю индексы симметрии
    cv::GaussianBlur(valueImageOrigin, valueImageOrigin,cv::Size(5, 5), 3);

    emit progressChanged(progress++,progressMax,
                         tr("Looking for symmetry consistent areas"));
    
    int countOfPointsWithGreaterValue;
    // Ищем точки, в радиусе depth от которых 
    // нету точек с большим значением
    int depth = 5;
    foreach (QList<Point> area, blackAreas) {
        foreach(Point p, area) {
            countOfPointsWithGreaterValue = 0;
            for (int i=-depth; i<depth; ++i) {
                for (int j=-depth;j<depth; ++j) {
                    if (fCancel) return;
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
    
    emit progressChanged(progress++,progressMax,
                         tr("Building key points set"));
    
    // флаги
    Mat1b flagImage = Mat::zeros(gImage.rows, gImage.cols, CV_8U);
    
    // Для каждой области с экстремум выбираем среднюю точку
    foreach (Point p, extremPoints) {
        if (fCancel) return;
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
            for(int i=0;i<keyPoints->count();++i) {
                Mick::KeyPoint &k = (*keyPoints)[i];
                int diff = (k.pos() - key.pos()).manhattanLength();
                if ( diff - k.value() - key.value() < 0 ) {
                    fWas = true;
                    break;
                }
            }
            // если наложений не было добавляем точку
            if (!fWas)
                keyPoints->addKey(key);
        }
    }
    
    emit progressChanged(progressMax,progressMax);
}

void ParticlesSeeker::run()
{
    findCircles();
    emit finished(this);
}

void ParticlesSeeker::findBlackAreas(QList<QList<Point> > &areas)
{
    if ( gImage.empty() ) {
        qWarning() << "image is not set!";
        return;
    }    
    
    OpenCVUtils::getKeyAreas<Mat1b>(areas, EA_BLACK, gImageRef);
}
 
