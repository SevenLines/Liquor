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
    void toggleButtonVisible(bool fShow);
    
    
signals:  
    void valueChanged(int newValue);
    void minimumChanged(int newValue);
    void maximumChanged(int newValue);
    void keyPressed(QKeyEvent *);
    /**
     * @brief called when user press RETURN key
     */
    void apply();

    
    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void on_btnApply_clicked();
};

#endif // HORIZONTALSLIDEREX_H
