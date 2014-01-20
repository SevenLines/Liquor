#ifndef PARTICLESSEEKER_H
#define PARTICLESSEEKER_H

#include "qthreadex.h"
#include "emisionanalyzerthread.h"

class ParticlesSeeker : public EmisionAnalyzerThread
{
    Q_OBJECT
public:
    ParticlesSeeker(QObject *parent=0);

protected:
    void runAnalyze();
};

#endif // PARTICLESSEEKER_H
