#ifndef MULTIKEYPOINTS_H
#define MULTIKEYPOINTS_H

#include <QList>
#include <QObject>
#include "keypoints.h"

class MultiKeyPoints : public QObject
{
    Q_OBJECT
private:
    QList<KeyPoints*> keyPointsSets;
    
    QVector<double> mKeys;
    QVector<double> mValues;
    
    int mScale;
    int mCount;
    float mMin;
    float mMax;
    float mMinValue;
    float mMaxValue;
    
    /// формирует график распределения, 
    /// ключ (x) -- размер частиц, значение (y) -- доля от общего чила
    /// scale -- частей 
    void getPoints();
    void getMinMax(float &min, float &max);
    
public:
    MultiKeyPoints();
    /// добавляет набор точек к объекту и становиться его родителем
    void addSet(KeyPoints *keyPoints);
    
    /// очитска списка наборов, force(true) - с удалением из памяти
    void clearSets(bool force = false);
    void clearGraph();
    
    void setScale(int scale);
    int scale();
    int count();
    
    float minKey();
    float maxKey();
    float minValue();
    float maxValue();
    
    // возвращает график
    const QVector<double> &keys();
    const QVector<double> &values();
    
signals:
    void graphChanged();
};

#endif // MULTIKEYPOINTS_H
