#ifndef AREASSEEKER_H
#define AREASSEEKER_H

#include "emisionanalyzerthread.h"

class AreasSeeker : public EmisionAnalyzerThread
{
    Q_OBJECT
public:
    explicit AreasSeeker(QObject *parent = 0);
    
signals:
    
public slots:
    
    
    // EmisionAnalyzerThread interface
protected:
    void runAnalyze();
};

#endif // AREASSEEKER_H
