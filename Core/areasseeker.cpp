#include "areasseeker.h"

AreasSeeker::AreasSeeker(QObject *parent) :
    EmisionAnalyzerThread(parent)
{
    setCancelEnabled(false);
    setProgressEnabled(false);
}


void AreasSeeker::runAnalyze()
{
//    ea.findBlackAreas();
}
