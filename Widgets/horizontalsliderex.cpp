#include "horizontalsliderex.h"
#include "ui_horizontalsliderex.h"

#include <QDebug>
#include <QMouseEvent>

HorizontalSliderEx::HorizontalSliderEx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HorizontalSliderEx)
{
    ui->setupUi(this);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
    //connect(ui->btnApply, SIGNAL(clicked()), SIGNAL(apply()));
}

HorizontalSliderEx::~HorizontalSliderEx()
{
    delete ui;
}

int HorizontalSliderEx::value()
{
    return ui->spinBox->value();
}

int HorizontalSliderEx::max()
{
    return ui->spinBox->maximum();
}

int HorizontalSliderEx::min()
{
    return ui->spinBox->minimum();
}

QString HorizontalSliderEx::title()
{
    return label()->text();
}

QSlider *HorizontalSliderEx::slider()
{
    return ui->horizontalSlider;
}

QSpinBox *HorizontalSliderEx::spinner()
{
    return ui->spinBox;
}

QLabel *HorizontalSliderEx::label()
{
    return ui->label;
}

void HorizontalSliderEx::setValue(int value)
{
    ui->spinBox->setValue(value);
    emit this->valueChanged(value);
}

void HorizontalSliderEx::setMax(int value)
{
    ui->spinBox->setMaximum(value);
    ui->horizontalSlider->setMaximum(value);
    emit this->maximumChanged(value);
}

void HorizontalSliderEx::setMin(int value)
{
    ui->spinBox->setMinimum(value);
    ui->horizontalSlider->setMinimum(value);
    emit this->minimumChanged(value);
}

void HorizontalSliderEx::setTitle(QString value)
{
    ui->label->setText(value);
}

void HorizontalSliderEx::toggleButtonVisible(bool fShow)
{
    ui->btnApply->setVisible(fShow);
}


void HorizontalSliderEx::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e);
    if (e->key() == Qt::Key_Return) {
        emit apply();
    }
}

void HorizontalSliderEx::on_btnApply_clicked()
{
    emit valueChanged(this->value());
    emit apply();
}
