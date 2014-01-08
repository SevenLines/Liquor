#include "formsettings.h"
#include "ui_formsettings.h"

FormSettings::FormSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);
}

FormSettings::~FormSettings()
{
    delete ui;
}
