#include "keypoints.h"
#include <QDebug>
#include <QFile>

using namespace Mick;

// -=-=-=-=--=-=--=-=--=-=--=-=--=-=--=-=- //

KeyPoint::KeyPoint()
{
    mProportion = 1;
    mIgnore = false;
    mValue = 0;
}

void KeyPoint::setProportion(int percents)
{
    mProportion = (float)percents / 100;
}

int KeyPoint::proportion()
{
    return mProportion * 100;
}

void KeyPoint::setIgnore(bool fIgnore)
{
    mIgnore = fIgnore;
}

bool KeyPoint::isIgnore()
{
    return mIgnore;
}

void KeyPoint::setPos(QPointF pos)
{
    mPos = pos;
}

const QPointF &KeyPoint::pos()
{
    return mPos;
}

void KeyPoint::setValue(int value)
{
    mValue = value;
}

int KeyPoint::value()
{
    return mValue;
}

void KeyPoints::setTitle(QString title)
{
    mTitle = title;
}

QString KeyPoints::title()
{
    return mTitle;
}

float KeyPoint::calcValue()
{
    return mValue * mProportion;
}

void KeyPoint::draw(QPainter *p, float prop)
{
    int radius = calcValue() * prop;
    p->drawEllipse(mPos, radius, radius);
}

// -=-=-=-=--=-=--=-=--=-=--=-=--=-=--=-=- //

KeyPoints::KeyPoints(QObject *parent) :
    QObject(parent)
{
        mProportion = 1;
        mEnabled = true;
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

int KeyPoints::countActual()
{
    int c = 0;
    for(int i=0;i<mPoints.count();++i) {
        if (!mPoints[i].isIgnore()) {
            c++;
        }
    }
    return c;
}

bool KeyPoints::isEnabled()
{
    return mEnabled;
}

void KeyPoints::setEnabled(bool value)
{
    if (mEnabled != value) {
        mEnabled = value;
        emit enabledChange(mEnabled);
    }
}

KeyPoint &KeyPoints::operator[](int index)
{
    return mPoints[index];
}

float KeyPoints::keyValue(int index)
{
    return mPoints[index].calcValue() * mProportion;
}

void KeyPoints::addKey(Mick::KeyPoint key)
{
    mPoints.append(key);
}

void KeyPoints::draw(QPainter *p)
{
    KeyPoint point;
    for(int i=0;i<mPoints.count();++i) {
        mPoints[i].draw(p, mProportion);
    }
}

void KeyPoints::dumpToFile(QString filePath)
{
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << tr("failed to open file");
        return;
    }
    
    KeyPoint point;
    int i = 1;
    QTextStream stream(&file);
    foreach(point, mPoints) {
        if (!point.isIgnore()) {
            stream << QString("%1\t%2\t(%3; %4)\n")
                    .arg(i)
                    .arg(point.calcValue() * mProportion)
                    .arg(point.pos().x())
                    .arg(point.pos().y());
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

