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
    // проверяет частицу на возможность добавления в гграфик
    bool checkParticle(Mick::KeyPoint &p);
    //void recountParticlesCount();
    
private slots:


public:
    MultiKeyPoints();
    
    bool isContains(KeyPoints *keyPoints);

    int scale();
    int countOfParticles();
    int count();

    /// Математическое ожидание
    /// power -- степень в которую возводится случайная велечина
    float expected(int power=1);
    /// дисперсия
    float dispersion();
    /// среднее квадратичное отклонение
    float deviation();
    
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
    
    void saveDumpToFile(QString filePath, bool fUseComma);
    void loadDumpFromFile(QString filePath);   
    
    /// удаляет набор из списка
    void removeSet(KeyPoints *keyPoints);
    /// удаляет набор из списка
    void removeSet(int i);  
    
    /// добавляет набор точек к объекту и становиться его родителем
    void addSet(KeyPoints *keyPoints);
    
    /// очитска списка наборов, force(true) - с удалением из памяти
    void clearSets();
    void clearGraph();
    
    void recalculateGraph();
    
signals:
    void graphChanged();
    void powerChanged(int newValue);
    void aboutToAddSet();
    void afterAddSet();
    void setRemoved(KeyPoints *keyPoints);
};

#endif // MULTIKEYPOINTS_H
