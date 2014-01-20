#ifndef QTHREADEX_H
#define QTHREADEX_H

#include <QThread>

class QThreadEx : public QThread
{
    Q_OBJECT
protected:
    bool fProgressEnabled;
    bool fCancelEnabled;
    bool fCancel;
    
public:
    explicit QThreadEx(QObject *parent = 0);
    
    /// используется progressDialog'ом для включения/отключения
    /// соответствующих элементов диалога
    bool progressEnabled();
    bool cancelEnabled();
    
    
    void setCancelEnabled(bool value);
    void setProgressEnabled(bool value);
    
signals:
    void progressChanged(int value, int max, QString label = QString());
    void canceled();
public slots:
    void cancel();
};

#endif // QTHREADEX_H
