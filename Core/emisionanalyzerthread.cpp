#include "emisionanalyzerthread.h"

EmisionAnalyzerThread::EmisionAnalyzerThread(QObject *parent) :
    QThreadEx(parent)
{
    connect(&ea, SIGNAL(progressChanged(int,int,QString)),
            this, SIGNAL(progressChanged(int,int,QString)));
    connect(&ea, SIGNAL(finished(EmisionAnalyzer*)),
            this, SIGNAL(finished(EmisionAnalyzer*)));
    // остановка по требованию
    connect(this, SIGNAL(canceled()), &ea, SLOT(cancel()));
    
    setCancelEnabled(true);
    setProgressEnabled(true);
}

void EmisionAnalyzerThread::setMaxRadius(int value)
{
    ea.setMaxRadius(value);
}

void EmisionAnalyzerThread::setMinRadius(int value)
{
    ea.setMinRadius(value);
}

void EmisionAnalyzerThread::setMinTupleRadius(int value)
{
    ea.setMinTupleRadius(value);
}

void EmisionAnalyzerThread::setImage(Mat &image)
{
    ea.setImage(image);
}

void EmisionAnalyzerThread::setKeyPoints(KeyPoints *keyPoints)
{
    ea.setKeyPoints(keyPoints);
}

EmisionAnalyzer * const EmisionAnalyzerThread::emisionAnalyzer()
{
    return &ea;
}

void EmisionAnalyzerThread::run()
{
    runAnalyze();
    emit finished(&ea);
}
