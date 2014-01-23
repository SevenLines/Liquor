#ifndef LIGHTCORRECTORWIDGET_H
#define LIGHTCORRECTORWIDGET_H

#include <QWidget>

namespace Ui {
class LightCorrectorWidget;
}

class LightCorrectorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LightCorrectorWidget(QWidget *parent = 0);
    ~LightCorrectorWidget();
    
private:
    Ui::LightCorrectorWidget *ui;
};

#endif // LIGHTCORRECTORWIDGET_H
