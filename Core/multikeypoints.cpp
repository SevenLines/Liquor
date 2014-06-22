#include "multikeypoints.h"
#include <QDebug>
#include <QFile>
#include <qmath.h>
#include <limits>

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

void MultiKeyPoints::removeSet(KeyPoints *keyPoints)
{
    if (keyPointsSets.contains(keyPoints)) {
        keyPoints->disconnect(this);
        keyPointsSets.removeOne(keyPoints);
    }
    
    recalculateGraph();
    emit setRemoved(keyPoints);
}

void MultiKeyPoints::removeSet(int i)
{
    removeSet(keyPointsSets.at(i));
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
        /*// меняем родителя на текущий объект
        keyPoints->setParent(this);*/
        
        connect(keyPoints, SIGNAL(enabledChange(bool)),
                SLOT(recalculateGraph()));
    }        
    recalculateGraph();
    emit afterAddSet();
}

void MultiKeyPoints::clearSets()
{  
    
    while(keyPointsSets.count()) {
        removeSet(0);
    }
    
    recalculateGraph();
}

void MultiKeyPoints::recalculateGraph()
{
    clearGraph();
    
    if (mScale < 1 )
        mScale = 1;
    
    //  находим границы
    getMinMax(mMin, mMax);

    float step = (float)( mMax ) / (mScale - 1);
    
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
    foreach(KeyPoints *set, keyPointsSets) {
        if (!set->isEnabled()) 
            continue;
        for(int i=0;i<set->count();++i) {
            if (!(*set)[i].isIgnore()) {
                ++countOfPoints;
                
                float value = (set->keyValue(i));
                index = (int)(value / step);
                graph[index].setY(graph[index].y()+pow(value, mPower));
            }
        }
    }

    
    //bool ffirst = true;
    mMinValue = mMaxValue = -1;
    if (countOfPoints!=0) {           
        // передаем данные в список под график
        for (int i=0;i<mScale;++i) {
            // чтобы не добавлять в график пустые
//            if (graph[i].y() == 0) continue;
            mKeys.append(graph[i].x());
            mValues.append(graph[i].y());
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

void MultiKeyPoints::saveDumpToFile(QString filePath, bool fUseComma)
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
                    QString strValue = QString::number(set->keyValue(i));
                    QString strPosX = QString::number(p.pos().x());
                    QString strPosY = QString::number(p.pos().y());
                    
                    // replace dot char with comma
                    if (fUseComma) {
                        strValue = strValue.replace('.',',');
                        strPosX = strPosX.replace('.',',');
                        strPosY = strPosY.replace('.',',');
                    } else {
                        strValue = strValue.replace(',','.');
                        strPosX = strPosX.replace(',','.');
                        strPosY = strPosY.replace(',','.'); 
                    }
                    out << (counter++) 
                           << '\t' << strValue
                              << '\t' << strPosX
                                 << '\t' << strPosY
                                    << endl;
                }
            }
        }
    }
}

void MultiKeyPoints::loadDumpFromFile(QString filePath)
{
    KeyPoints *newSet = new KeyPoints();
    newSet->setTitle(QFile(filePath).fileName());
    // TODO check for type
    newSet->setType(KeyPoints::Particles);
    
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
                k.setValue(values[1].replace(',','.').toFloat());
                k.setPos(QPointF(values[2].replace(',','.').toFloat(), values[3].replace(',','.').toFloat()));
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

float MultiKeyPoints::expected(int power)
{
    float out = 0;
    int count = 0;
    float value;
    foreach(KeyPoints *set, keyPointsSets) {
        if (!set->isEnabled())
            continue;

        for(int i=0;i<set->count();++i) {
            Mick::KeyPoint &p = (*set)[i];
            if (p.isIgnore())
                continue;

            count++;
            value = set->keyValue(i);
            out += power!=1?pow(value, power):value;
        }
    }

    if(count == 0) {
        return std::numeric_limits<float>::quiet_NaN();
    }
    out /= count;
    return out;
}

float MultiKeyPoints::dispersion()
{
    float expct = expected(); // мат. ожидание
    float expct_2 = expected(2); // мат. ожидание квадрата велечины
    return expct_2 - expct*expct;
}

float MultiKeyPoints::deviation()
{
    return sqrt(dispersion());
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
