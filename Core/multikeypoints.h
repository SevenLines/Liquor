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
    int mPower;
    float mMin;
    float mMax;
    float mMinValue;
    float mMaxValue;
    
    void getMinMax(float &min, float &max);
    //void recountParticlesCount();
    
private slots:


public:
    MultiKeyPoints();
    
    bool isContains(KeyPoints *keyPoints);

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

    int power();
    
    KeyPoints *operator[](int index);
    KeyPoints *at(int index);
    
public slots:
    
    // степень значения используемого при расчете графика
    void setPower(int power);
    void setScale(int scale);
    
    void saveDumpToFile(QString filePath);
    void loadDumpFromFile(QString filePath);   
    
    /// удаляет набор из списка
    void removeSet(KeyPoints *keyPoints, bool force = false);
    /// удаляет набор из списка
    void removeSet(int i, bool force = false);  
    
    /// добавляет набор точек к объекту и становиться его родителем
    void addSet(KeyPoints *keyPoints);
    
    /// очитска списка наборов, force(true) - с удалением из памяти
    void clearSets(bool force = false);
    void clearGraph();
    
    void recalculateGraph();
    
signals:
    void graphChanged();
    void powerChanged(int newValue);
    void aboutToAddSet();
    void afterAddSet();
};

#endif // MULTIKEYPOINTS_H
