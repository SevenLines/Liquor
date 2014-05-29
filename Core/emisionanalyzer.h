#ifndef EMISIONANALYZER_H
#define EMISIONANALYZER_H

#include <opencv/cv.hpp>
#include "keypoints.h"
#include "pointvalue.h"
#include <QObject>

using namespace cv;
using namespace Mick;

class EmisionAnalyzer : public QObject
{
    Q_OBJECT
    
private:

    int mMinRadius;
    int mMaxRadius;
    
    Rect boundRoi;
    uchar mFillColor;
    uchar mEmptyColor; 
    
    bool fCancel;
    
    KeyPoints *mKeyPoints;
    
    Mat mImage; // копия изображения, тип CV_8U
    Mat1b mImageRoi; // для упрощения доступа к изображению
    
    Mat mSymmetryInfo;// using for storing specific info about each pixel
    
    inline int getInfoValue(cv::Point pos, int channel = 0);
    inline int getInfoValue(int y, int x, int channel = 0);
    inline void setInfoValue(int value, cv::Point pos, int channel = 0);
    void cropImage();

public:
    
    enum flags{
        EA_BLACK = 0,
        EA_VISITED = 1,
        EA_EMPTY = 255
    }; 
    
    EmisionAnalyzer(QObject *parent=0);
    
    // only binary images accepted
    void setImage(Mat &image);
    Mat image();
    
    void setMinRadius(int value);
    int minRadius();
    
    void setMaxRadius(int value);
    int maxRadius();
    
    void setKeyPoints(KeyPoints *mKeyPoints);
    KeyPoints *keyPoints();
    Mick::KeyPoint fromContour(vector<Point> &contour);

    /**
     * @brief поиск заполненых окружностей на изображении
     */
    void find();
    void findContours(vector<vector<Point> > &borders);
    
public slots:
    void cancel();
    
signals:
    void finished(EmisionAnalyzer *sender);
    void progressChanged(int value, int max, QString label = QString());
    
};

#endif // EMISIONANALYZER_H
