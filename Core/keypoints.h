#ifndef KEYPOINTS_H
#define KEYPOINTS_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QPainter>

namespace Mick {

    class KeyPoint {
    private:
        float mProportion;
        bool mIgnore; // set to true if u want to ignore this particle
        QPointF mPos; // position of particle on pixmap
        int mValue; // real value of particle
        
    public:
        
        KeyPoint();
        
        /// set proportion in percents
        void setProportion(int percents);
        int proportion();
        
        void setIgnore(bool fIgnore);
        bool isIgnore();
        
        void setPos(QPointF pos);
        QPointF const &pos();
        
        /// value in pixels
        void setValue(int value);
        int value();
          
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
        QVector<KeyPoint> mPoints; 
        float mProportion;
        
    public:
        explicit KeyPoints(QObject *parent = 0);
        
        int proportion();
        
        int count();
        
        KeyPoint &operator[](int index);
        float keyValue(int index);
        
    signals:
        void proportionChange(int newValue);
        void cleared();
        
    public slots:
        void setProportion(int value);
        void addKey(KeyPoint &key);
        void draw(QPainter *p);
        void dumpToFile(QString filePath);
        void clear();
    };

}

#endif // KEYPOINTS_H
