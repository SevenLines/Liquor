#include "particlesseeker.h"
#include <QDebug>

ParticlesSeeker::ParticlesSeeker(QObject *parent)
    : EmisionAnalyzerThread(parent)
{

}

void ParticlesSeeker::runAnalyze()
{
    ea.find();
}
