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
    
    void setProgress(int value);
    void setMax(int value);
    void setMin(int value);
    
    int min();
    int max();
    int progress();
    
    
private:
    QMovie *movieIcon;
    QMovie *movieIconSmall;
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
