#include "multikeypoints.h"
#include <QDebug>
#include <QFile>

using namespace Mick;

MultiKeyPoints::MultiKeyPoints()
{
    setScale(10);
    mMin = mMax = mMaxValue = mMinValue = 0;
    mCount = 0;
}

bool MultiKeyPoints::isContains(KeyPoints *keyPoints)
{
    if (!keyPoints) return false;
    return keyPointsSets.contains(keyPoints);
}

void MultiKeyPoints::removeSet(KeyPoints *keyPoints, bool force)
{
    if (keyPointsSets.contains(keyPoints)) {
        keyPoints->disconnect(this);
        keyPointsSets.removeOne(keyPoints);
        if (force) {
            delete keyPoints;
        }
    }
    
    getPoints();
}

void MultiKeyPoints::addSet(KeyPoints *keyPoints)
{
    emit aboutToAddSet();
    if (keyPoints == 0)
        return;
    // проверяем на наличие набора
    if (!keyPointsSets.contains(keyPoints)) {
        
        // добавлем в список
        keyPointsSets.append(keyPoints);
        // меняем родителя на текущий объект
        keyPoints->setParent(this);
        
        connect(keyPoints, SIGNAL(enabledChange(bool)),
                SLOT(getPoints()));
    }        
    getPoints();
    emit afterAddSet();
}

void MultiKeyPoints::clearSets(bool force)
{
    if (force)
        qDeleteAll(keyPointsSets);
    
    foreach(KeyPoints *set, keyPointsSets) {
        set->disconnect(this);
    }

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
        if (!set->isEnabled()) 
            continue;
        for(int i=0;i<set->count();++i) {
            if (!(*set)[i].isIgnore()) {
                ++countOfPoints;
                float value = (*set)[i].calcValue();
                QPointF &p = graph[(int)(value / step)];
                p.setY(p.y() + 1);
            }
        }
    }

    
    //bool ffirst = true;
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
        if (!set->isEnabled()) 
            continue;
        
        for(int i=0;i<set->count();++i) {
            if (!(*set)[i].isIgnore()) {
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
}

KeyPoints *MultiKeyPoints::operator[](int index)
{
    return at(index);
}

KeyPoints *MultiKeyPoints::at(int index)
{
    return keyPointsSets[index];
}

void MultiKeyPoints::saveDumpToFile(QString filePath)
{
    QFile data(filePath);
    if (data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        QTextStream out(&data);
        
        int counter = 1;
        
        foreach(KeyPoints *set, keyPointsSets) {
            if (!set->isEnabled()) 
                continue;
            
            for(int i=0;i<set->count();++i) {
                Mick::KeyPoint &p = (*set)[i];
                if (!p.isIgnore()) {
                    out << (counter++) 
                           << '\t' << p.calcValue()
                              << '\t' << p.pos().x()
                                 << '\t' << p.pos().y()
                                    << endl;
                }
            }
        }
    }
}

void MultiKeyPoints::loadDumpFromFile(QString filePath)
{
    KeyPoints *newSet = new KeyPoints();
    
    QFile data(filePath);
    if (data.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream out(&data);
        
        while(!out.atEnd()) {
            QString line = out.readLine();
            QStringList values = line.split("\t",  QString::SkipEmptyParts);
            if (values.count() == 4) {
                Mick::KeyPoint k;
                k.setValue(values[1].toFloat());
                k.setPos(QPointF(values[2].toFloat(), values[3].toFloat()));
                newSet->addKey(k);
            }
        }
        data.close();
    }
    
    addSet(newSet);
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

int MultiKeyPoints::countOfParticles()
{
    return mCount;
}

int MultiKeyPoints::count()
{
    return keyPointsSets.count();
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
