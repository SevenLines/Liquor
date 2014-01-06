#ifndef MULTIKEYPOINTS_H
#define MULTIKEYPOINTS_H

#include <QList>
#include <QObject>
#include "keypoints.h"

using namespace Mick;

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
    
    void getMinMax(float &min, float &max);
    
private slots:
    void getPoints();
    
    
public:
    MultiKeyPoints();
    
    bool isContains(KeyPoints *keyPoints);
    
    /// удаляет набор из списка
    void removeSet(KeyPoints *keyPoints, bool force = false);
    
    /// добавляет набор точек к объекту и становиться его родителем
    void addSet(KeyPoints *keyPoints);
    
    /// очитска списка наборов, force(true) - с удалением из памяти
    void clearSets(bool force = false);
    void clearGraph();
    
    void setScale(int scale);
    int scale();
    int countOfParticles();
    int count();
    
    
    float minKey();
    float maxKey();
    float minValue();
    float maxValue();
    
    // возвращает график
    const QVector<double> &keys();
    const QVector<double> &values();
    
    KeyPoints *operator[](int index);
    KeyPoints *at(int index);
    
    void saveDumpToFile(QString filePath);
    void loadDumpFromFile(QString filePath);
    
signals:
    void graphChanged();
    void aboutToAddSet();
    void afterAddSet();
};

#endif // MULTIKEYPOINTS_H
