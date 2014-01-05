#include "keypoints.h"
#include <QDebug>
#include <QFile>

// -=-=-=-=--=-=--=-=--=-=--=-=--=-=--=-=- //

MKeyPoint::MKeyPoint()
{
    mProportion = 1;
    fIgnore = false;
    value = 0;
}

void MKeyPoint::setProportion(int percents)
{
    mProportion = (float)percents / 100;
}

int MKeyPoint::proportion()
{
    return mProportion * 100;
}

float MKeyPoint::calcValue()
{
    return value * mProportion;
}

void MKeyPoint::draw(QPainter *p, float prop)
{
    int radius = this->value * prop;
    p->drawEllipse(this->pos, radius, radius);
}

// -=-=-=-=--=-=--=-=--=-=--=-=--=-=--=-=- //

KeyPoints::KeyPoints(QObject *parent) :
    QObject(parent)
{
        mProportion = 1;
}

void KeyPoints::setProportion(int value)
{
    mProportion = (float)value / 100;
    emit proportionChange(value);
}

int KeyPoints::proportion()
{
    return mProportion * 100;
}

int KeyPoints::count()
{
    return mPoints.count();         
}

MKeyPoint &KeyPoints::operator[](int index)
{
    return mPoints[index];
}

float KeyPoints::keyValue(int index)
{
    return mPoints[index].calcValue() * mProportion;
}

void KeyPoints::addKey(MKeyPoint &key)
{
    mPoints.append(key);
}

void KeyPoints::draw(QPainter *p)
{
    MKeyPoint point;
    foreach(point, mPoints) {
        point.draw(p, mProportion);
    }
}

void KeyPoints::dumpToFile(QString filePath)
{
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << tr("failed to open file");
        return;
    }
    
    MKeyPoint point;
    int i = 1;
    QTextStream stream(&file);
    foreach(point, mPoints) {
        if (!point.fIgnore) {
            stream << QString("%1\t%2\t(%3; %4)\n")
                    .arg(i)
                    .arg(point.calcValue() * mProportion)
                    .arg(point.pos.x())
                    .arg(point.pos.y());
            ++i;
        }
    }
    file.close();
}

void KeyPoints::clear()
{
    emit cleared();
    mPoints.clear();
}

// -=-=-=-=--=-=--=-=--=-=--=-=--=-=--=-=- //

