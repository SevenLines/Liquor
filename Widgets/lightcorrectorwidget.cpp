#include "lightcorrectorwidget.h"
#include "ui_lightcorrectorwidget.h"

#include <QDebug>
#include <QPainter>
#include <QMetaEnum>

LightCorrectorWidget::LightCorrectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightCorrectorWidget)
{
    ui->setupUi(this);

    lastLightCorrector = 0;
    
#define ADDITEM(title) \
    ui->cmbModes->addItem(tr(#title), QPainter::CompositionMode_##title);
    
    ADDITEM(Plus);
    ADDITEM(Multiply);
    ADDITEM(Screen);
    ADDITEM(Overlay);
    ADDITEM(Darken);
    ADDITEM(Lighten);
    ADDITEM(ColorDodge);
    ADDITEM(ColorBurn);
    ADDITEM(HardLight);
    ADDITEM(SoftLight);
    ADDITEM(Difference);
    ADDITEM(Exclusion);

    connect(ui->cmbModes, SIGNAL(currentIndexChanged(int)),
            SLOT(onItemChanged(int)));
    connect(ui->btnApply, SIGNAL(clicked()),
            SIGNAL(apply()));
    connect(ui->groupBox, SIGNAL(toggled(bool)),
            SIGNAL(toggled(bool)));
    connect(this, SIGNAL(apply()),
            SLOT(setCorrectionEnabledF()));
            
#undef ADDITEM
}

LightCorrectorWidget::~LightCorrectorWidget()
{
    delete ui;
}

bool LightCorrectorWidget::isCorrectionEnabled()
{
    return ui->groupBox->isChecked();
}

void LightCorrectorWidget::onItemChanged(int index)
{
    emit modeChanged((QPainter::CompositionMode)
                     ui->cmbModes->itemData(index).toInt());
}

void LightCorrectorWidget::setCompositionModeItem(QPainter::CompositionMode mode)
{
    for(int i=0;i<ui->cmbModes->count();++i) {
        int m = ui->cmbModes->itemData(i).toInt();
        if (m == mode) {
            ui->cmbModes->setCurrentIndex(i);
            return;
        }
    }
}

void LightCorrectorWidget::setCorrectionEnabled(bool fEnable)
{
    ui->groupBox->setChecked(fEnable);
}

void LightCorrectorWidget::setCorrectionEnabledF()
{
    setCorrectionEnabled(false);
}

void LightCorrectorWidget::saveIni(QSettings *ini)
{
    ini->beginGroup("LightCorrectorWidget");
    if (lastLightCorrector) {
        ini->setValue("RadiusX", lastLightCorrector->radius());
        ini->setValue("Position", lastLightCorrector->position());
        ini->setValue("CompositionMode", 
                      lastLightCorrector->compositionMode());
    }
    ini->setValue("CorrectionEnabled", isCorrectionEnabled());
    ini->endGroup();
}

void LightCorrectorWidget::loadIni(QSettings *ini)
{
    ini->beginGroup("LightCorrectorWidget");
    firstLightCorrector.setRadius(ini->value("RadiusX", 0).toInt());
    firstLightCorrector.setPosition(
                ini->value("Position", QPoint()).toPointF());
    
    QPainter::CompositionMode mode = (QPainter::CompositionMode)ini->value("CompositionMode",
                          QPainter::CompositionMode_SoftLight).toInt();
    firstLightCorrector.setCompositionMode(mode);
    setCompositionModeItem(mode);
    setCorrectionEnabled(ini->value("CorrectionEnabled", false).toBool());
    ini->endGroup();
}

LightCorrector *LightCorrectorWidget::addLightCorrector()
{
    LightCorrector *corrector = new LightCorrector(this);
    if (lastLightCorrector) {
        corrector->setRadius(lastLightCorrector->radius());
        corrector->setPosition(lastLightCorrector->position());
        corrector->setCompositionMode(lastLightCorrector->compositionMode());;
    } else {
        corrector->setRadius(firstLightCorrector.radius());
        corrector->setPosition(firstLightCorrector.position());
    }
    corrector->setCompositionMode((QPainter::CompositionMode)
                              ui->cmbModes->itemData(
                                  ui->cmbModes->currentIndex()).toInt());
    lastLightCorrector = corrector;
    return corrector;
}

void LightCorrectorWidget::setLightCorrector(LightCorrector *lightCorrector)
{
    lastLightCorrector = lightCorrector;
    if (lastLightCorrector) {
        setCompositionModeItem(lastLightCorrector->compositionMode());
    }
}
