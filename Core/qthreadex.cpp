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

void QThreadEx::cancel()
{
    fCancel = true;
}
