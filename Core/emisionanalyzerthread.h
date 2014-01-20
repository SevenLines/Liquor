#ifndef EMSIONANALYZERTHREAD_H
#define EMSIONANALYZERTHREAD_H

#include "qthreadex.h"
#include "emisionanalyzer.h"

/**
 * @brief The EmisionAnalyzerThread class абстрактный класс 
 * для потоков реализующихобработку смесей
 */
class EmisionAnalyzerThread : public QThreadEx
{
    Q_OBJECT
public:
    EmisionAnalyzer ea;
    EmisionAnalyzerThread(QObject *parent=0);
    
    void setMaxRadius(int value);
    void setMinRadius(int value);
    void setImage(Mat &image);
    void setKeyPoints(KeyPoints *keyPoints);
    
    EmisionAnalyzer * const emisionAnalyzer();
    
    void run();
signals:
    void finished(EmisionAnalyzer *ea);
    
protected:
    virtual void runAnalyze()=0; // переопределить эту функцию в класе наследнике
    
};

#endif // EMSIONANALYZERTHREAD_H
