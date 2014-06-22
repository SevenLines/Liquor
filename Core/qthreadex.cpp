#include "qthreadex.h"

QThreadEx::QThreadEx(QObject *parent) :
    QThread(parent)
{
    fProgressEnabled = false;
    fCancelEnabled = false;
    fCancel = false;
}

bool QThreadEx::progressEnabled()
{
    return fProgressEnabled;
}

bool QThreadEx::cancelEnabled()
{
    return fCancelEnabled;
}

void QThreadEx::setCancelEnabled(bool value)
{
    fCancelEnabled = value;
}

void QThreadEx::setProgressEnabled(bool value)
{
    fProgressEnabled = value;
}

void QThreadEx::cancel()
{
    fCancel = true;
    emit canceled();
}
