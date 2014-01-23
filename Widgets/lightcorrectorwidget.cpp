#include "lightcorrectorwidget.h"
#include "ui_lightcorrectorwidget.h"

LightCorrectorWidget::LightCorrectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LightCorrectorWidget)
{
    ui->setupUi(this);
}

LightCorrectorWidget::~LightCorrectorWidget()
{
    delete ui;
}
