#include "multikeypoints.h"
#include <QDebug>

MultiKeyPoints::MultiKeyPoints()
{
    setScale(10);
    mMin = mMax = mMaxValue = mMinValue = 0;
    mCount = 0;
}

void MultiKeyPoints::addSet(KeyPoints *keyPoints)
{
    // проверяем на наличие набора
    if (keyPointsSets.contains(keyPoints)) 
        return;
    
    // добавлем в список
    keyPointsSets.append(keyPoints);
    // меняем родителя на текущий объект
    keyPoints->setParent(this);
    
    getPoints();
}

void MultiKeyPoints::clearSets(bool force)
{
    if (force)
        qDeleteAll(keyPointsSets);
    keyPointsSets.clear();
    
    getPoints();
}

void MultiKeyPoints::getPoints()
{
    clearGraph();
    
    if (mScale < 1 )
        mScale = 1;
    
    //  находим границы
    getMinMax(mMin, mMax);

    float step = (float)( mMax - mMin ) / mScale;
    
    // временный массив под график
    QPointF * graph = new QPointF[mScale];
    
    // заполняем массив начальными значениями
    for (int i=0;i<mScale;++i) {
        graph[i].setX(step * i);
        graph[i].setY(0);
    }
    
    // расчитываем график
    int countOfPoints = 0;
    foreach(KeyPoints *set, keyPointsSets) {
        for(int i=0;i<set->count();++i) {
            ++countOfPoints;
            float value = (*set)[i].calcValue();
            QPointF &p = graph[(int)(value / step)];
            p.setY(p.y() + 1);
        }
    }
    
    bool ffirst = true;
    mMinValue = mMaxValue = -1;
    if (countOfPoints!=0) {   
        // перерассчитываем долю
        /*for (int i=0;i<mScale;++i) {
            float value = (float) graph[i].y() / countOfPoints;
            // пересчитывем минимальное и максимальное значение
            if (ffirst) {
                mMinValue = value;
                mMaxValue = value;
                ffirst = false;
            } else {
                if (value < mMinValue) mMinValue = value;
                if (value > mMaxValue) mMaxValue = value;
            }
            graph[i].setX(step * (i+1));
            graph[i].setY( value );
        }*/
        
        // передаем данные в список под график
        for (int i=0;i<mScale;++i) {
            mKeys.append(graph[i].x());
            mValues.append(graph[i].y());
        }
        
    }
    mCount = countOfPoints;
    
    // удаляем временный массив
    delete[] graph;
    
    emit graphChanged();
}

void MultiKeyPoints::getMinMax(float &min, float &max)
{
    min = max = -1; // чтобы было понятно что ничего не нашли
    bool ffirst = true;
    foreach(KeyPoints *set, keyPointsSets) {
        for(int i=0;i<set->count();++i) {
            float value = (*set).keyValue(i);
            if (ffirst) {
                ffirst = false;
                min = max = value;
            } else {
                if (value > max) {
                    max = value;
                }
                if (value < min) {
                    min = value;
                }
            }
        }
    }
}

void MultiKeyPoints::clearGraph()
{
    mKeys.clear();
    mValues.clear();
}

void MultiKeyPoints::setScale(int scale)
{
    if (scale < 1) {
        scale = 1;
    }
    mScale = scale;
    getPoints();
}

int MultiKeyPoints::scale()
{
    return mScale;
}

int MultiKeyPoints::count()
{
    return mCount;
}

float MultiKeyPoints::minKey()
{
    return mMin;
}

float MultiKeyPoints::maxKey()
{
    return mMax;
}

float MultiKeyPoints::minValue()
{
    return mMinValue;
}

float MultiKeyPoints::maxValue()
{
    return mMaxValue;
}

const QVector<double> &MultiKeyPoints::keys()
{
    return mKeys;
}

const QVector<double> &MultiKeyPoints::values()
{
    return mValues;
}
