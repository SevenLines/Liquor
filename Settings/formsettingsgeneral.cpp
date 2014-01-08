#include "formsettingsgeneral.h"
#include "ui_formsettingsgeneral.h"

FormSettingsGeneral::FormSettingsGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSettingsGeneral)
{
    ui->setupUi(this);
}

FormSettingsGeneral::~FormSettingsGeneral()
{
    delete ui;
}
