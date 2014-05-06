#include "imageprocessingdialog.h"
#include "ui_imageprocessingdialog.h"
#include "Delegates/cellimagedelegate.h"
#include <QDebug>


ImageProcessingDialog::ImageProcessingDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ImageProcessingDialog)
{
    ui->setupUi(this);
    ui->lstVariants->setItemDelegate(new CellImageDelegate(this));
    ui->lstVariants->setModel(&model);
    ui->lstVariants->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->lstVariants->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    connect(ui->lstVariants, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(accept()));
}

void ImageProcessingDialog::setPreviewer(ImageProcessPreviewer *previewer)
{
    model.setPreviewer(previewer);
    this->setWindowTitle(previewer->title());
}

ImageProcessingDialog::~ImageProcessingDialog()
{
    delete ui;
}

ProcessInfo ImageProcessingDialog::selectedImage()
{
    if (!ui->lstVariants->currentIndex().data().canConvert<ProcessInfo>()) 
        return ProcessInfo();
    
    return qvariant_cast<ProcessInfo>(ui->lstVariants->currentIndex().data());
}


void ImageProcessingDialog::showEvent(QShowEvent *)
{
    QRect rect = ui->lstVariants->geometry();
    rect.setHeight(400);
    qDebug() << "show";
}
