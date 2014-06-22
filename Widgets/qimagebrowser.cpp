#include "qimagebrowser.h"
#include "ui_qimagebrowser.h"

QImageBrowser::QImageBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QImageBrowser)
{
    ui->setupUi(this);
}

QImageBrowser::~QImageBrowser()
{
    delete ui;
}
