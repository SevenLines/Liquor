#ifndef LIGHTCORRECTORWIDGET_H
#define LIGHTCORRECTORWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QSettings>
#include "lightcorrector.h"

namespace Ui {
class LightCorrectorWidget;
}

class LightCorrectorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LightCorrectorWidget(QWidget *parent = 0);
    ~LightCorrectorWidget();
    
    bool isCorrectionEnabled();
    
private:
    LightCorrector *lastLightCorrector;
    LightCorrector firstLightCorrector;
    Ui::LightCorrectorWidget *ui;
    
public slots:
    void onItemChanged(int index);
    void setCompositionModeItem(QPainter::CompositionMode mode);
    void setCorrectionEnabled(bool fEnable);
    void setCorrectionEnabledF();
    
    void saveIni(QSettings *ini);
    void loadIni(QSettings *ini);
    
    LightCorrector *addLightCorrector();
    void setLightCorrector(LightCorrector *lightCorrector);
    
signals:
    void modeChanged(QPainter::CompositionMode);
    void apply();
    void toggled(bool fShow);
    void intensityChanged(int value);
};

#endif // LIGHTCORRECTORWIDGET_H
