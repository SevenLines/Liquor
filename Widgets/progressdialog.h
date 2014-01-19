#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    
    void setLabel(QString label);
    QString label();
    
    void showCancelButton(bool fShow = true);
    void showProgressBar(bool fShow = true);
    

    
    int min();
    int max();
    int progress();
    
public slots:
    void setProgress(int value);
    void setProgress(int value, int max, QString label);
    void setMax(int value);
    void setMin(int value);
    
signals:
    void cancel();
    
private:
    QMovie *movieIcon;
    QMovie *movieIconSmall;
    Ui::ProgressDialog *ui;
    
    // QWidget interface
protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
};

#endif // PROGRESSDIALOG_H
