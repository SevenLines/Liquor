#include "progressdialog.h"
#include "ui_progressdialog.h"

#include <QMovie>
#include <QDebug>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    //ui->wdgAnimate->init(, QSize());
    
    movieIcon = new QMovie(":images/loading5.gif");
    movieIcon->setParent(this);
    movieIcon->start();
    
    movieIconSmall= new QMovie(":images/loading2.gif");
    movieIconSmall->setParent(this);
    movieIconSmall->start();
    
    ui->lblIcon2->setMovie(movieIconSmall);
    ui->lblIcon->setMovie(movieIcon);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setLabel(QString label)
{
    ui->lblLabel->setText(label);
}

QString ProgressDialog::label()
{
    return ui->lblLabel->text();
}

void ProgressDialog::showCancelButton(bool fShow)
{
    ui->btnCancel->setVisible(fShow);
}

void ProgressDialog::showProgressBar(bool fShow)
{
    ui->progressBar->setVisible(fShow);
}

void ProgressDialog::setProgress(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressDialog::setMax(int value)
{
    ui->progressBar->setMaximum(value);
}

void ProgressDialog::setMin(int value)
{
    ui->progressBar->setMinimum(value);
}

int ProgressDialog::min()
{
    return ui->progressBar->minimum();
}

int ProgressDialog::max()
{
    return ui->progressBar->maximum();
}

int ProgressDialog::progress()
{
    return ui->progressBar->value();
}
