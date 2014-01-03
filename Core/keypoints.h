#ifndef KEYPOINTS_H
#define KEYPOINTS_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QPainter>


struct MKeyPoint 
{
private:
    float mProportion;
public:
    
    bool fIgnore; // set to true if u want to ignore this particle
    
    
    QPointF pos; // position of particle on pixmap
    QString description; // description of particle
    int value; // real value of particle
    
    MKeyPoint();
    
    /// set proportion in percents
    void setProportion(int percents);
    int proportion();
    
    /// returns overcalculated value, with proportion
    float calcValue();
    
    /**
     * @brief draw particles on canvas
     */
    void draw(QPainter *p, float prop = 1);
};

class KeyPoints : public QObject
{
    Q_OBJECT
private:
    QVector<MKeyPoint> mPoints; 
    float mProportion;
    
public:
    explicit KeyPoints(QObject *parent = 0);
    
    int proportion();
    
    int count();
    
    MKeyPoint &operator[](int index);
    
signals:
    void proportionChange(int newValue);
    void cleared();
    
public slots:
    void setProportion(int value);
    void addKey(MKeyPoint &key);
    void draw(QPainter *p);
    void dumpToFile(QString filePath);
    void clear();
};

#endif // KEYPOINTS_H
