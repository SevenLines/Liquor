#include "multikeypoints.h"
#include <QDebug>
#include <QFile>
#include <qmath.h>

using namespace Mick;

MultiKeyPoints::MultiKeyPoints()
{
    setScale(10);
    mMin = mMax = mMaxValue = mMinValue = 0;
    mCount = 0;
    mPower = 1;
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
    
    recalculateGraph();
}

void MultiKeyPoints::removeSet(int i, bool force)
{
    removeSet(keyPointsSets.at(i), force);
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
                SLOT(recalculateGraph()));
    }        
    recalculateGraph();
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
    recalculateGraph();
}

void MultiKeyPoints::recalculateGraph()
{
    clearGraph();
    
    if (mScale < 1 )
        mScale = 1;
    
    //  находим границы
    getMinMax(mMin, mMax);

    float step = (float)( mMax - mMin ) / (mScale - 1);
    
    // временный массив под график
    QVector<QPointF> graph(mScale);// = new QPointF[mScale];
    
    // заполняем массив начальными значениями
    for (int i=0;i<mScale;++i) {
        graph[i].setX(step * i);
        graph[i].setY(0);
    }

    // расчитываем график
    int countOfPoints = 0;
    int index;
    float leftValue, rightValue, iValue;
    foreach(KeyPoints *set, keyPointsSets) {
        if (!set->isEnabled()) 
            continue;
        for(int i=0;i<set->count();++i) {
            if (!(*set)[i].isIgnore()) {
                ++countOfPoints;
                
                // значения ключа с учетом минимума, то есть от нуля и более
                float value = (set->keyValue(i) - mMin);//(*set)[i].calcValue();

                // индекс
                index = (int)(value / step);
                // точное положение по x
                iValue = (float)index * step;
                
                float multiplier = value * qPow(value, mPower);
                
                leftValue = (value - iValue) * multiplier;
                rightValue = (iValue + step - value) * multiplier;
                
                QPointF &pLeft = graph[index];
                pLeft.setY(pLeft.y() + leftValue);
                
                // множетель графика, учитывается степень значения ключа
                
                if (index < mScale - 1 ) {
                    QPointF &pRight = graph[index + 1];
                    pRight.setY(pRight.y() + rightValue);
                } else {
                    pLeft.setY(pLeft.y() + rightValue); 
                }
                
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
            // чтобы не добавлять в график пустые
            if (graph[i].y() != 0) {
                mKeys.append(graph[i].x());
                mValues.append(graph[i].y());
            }
        }
        
    }
    mCount = countOfPoints;
    
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
        
        out << tr("# number, radius (px), x position (px), y position (px)") 
            << endl;
        
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
            
            // check if comment
            if (line[0] == '#')
                continue;
            
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
    recalculateGraph();
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

void MultiKeyPoints::setPower(int power)
{
    mPower = power;
    emit powerChanged(power);
    recalculateGraph();
}

int MultiKeyPoints::power()
{
    return mPower;
}
