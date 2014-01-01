#ifndef HORIZONTALSLIDEREX_H
#define HORIZONTALSLIDEREX_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

namespace Ui {
class HorizontalSliderEx;
}

class HorizontalSliderEx : public QWidget
{
    Q_OBJECT
    
public:
    explicit HorizontalSliderEx(QWidget *parent = 0);
    ~HorizontalSliderEx();
    
    int value();
    int max();
    int min();
    QString title();
    
    QSlider *slider();
    QSpinBox *spinner();
    QLabel *label();
    
private:
    Ui::HorizontalSliderEx *ui;

public slots:
    void setValue(int value);
    void setMax(int value);
    void setMin(int value); 
    void setTitle(QString);
signals:  
    void valueChanged(int newValue);
    void minimumChanged(int newValue);
    void maximumChanged(int newValue);

};

#endif // HORIZONTALSLIDEREX_H
