#ifndef KEYPOINTS_H
#define KEYPOINTS_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <vector>
namespace Mick {

    class KeyPoint {

        
    private:
        float mProportion;
        bool mIgnore; // set to true if u want to ignore this particle
        QPointF mPos; // position of particle on pixmap
        int mValue; // real value of particle
        int mMarker; // просто число которое особым образом зхарактеризует данную частицк
        
    public:
        // граница объекта
        std::vector<QPoint> border;
        
        KeyPoint();
        
        /// set proportion in percents
        void setProportion(int percents);
        int proportion();
        
        void setIgnore(bool fIgnore);
        bool isIgnore();
        
        void setPos(QPointF pos);
        QPointF const &pos();

        void setMarker(int marker);
        int marker();
        
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
        
    public:
        
        enum Type {
            Undefined = 0, // неопределенный тип
            Particles = 1, // частицы
            Area = 2 // области
        };
        
    private:
        QVector<KeyPoint> mPoints; 
        float mProportion;
        /// множитель используется дле перевода пикселей в микрометры
        float mMultiplier;
        bool mEnabled;
        QString mTitle;
        KeyPoints::Type mType;
        
    public:

        
        explicit KeyPoints(QObject *parent = 0);
        
        int proportion();
        
        int count();
        // возвращает количетво активных частиц в наборе
        int countActual();
        
        bool isEnabled();
        void setEnabled(bool value);
        
        KeyPoint &operator[](int index);
        float keyValue(int index);
        
        
        void setTitle(QString title);
        QString title();
        
        
        KeyPoints::Type type();
        void setType(KeyPoints::Type type);
        
    signals:
        void proportionChange(int newValue);
        void enabledChange(bool newValue);
        void cleared();
        
    public slots:
        void setProportion(int value);
        void addKey(KeyPoint key);
        void draw(QPainter *p);
        void dumpToFile(QString filePath);
        void clear();
    };

}

#endif // KEYPOINTS_H
