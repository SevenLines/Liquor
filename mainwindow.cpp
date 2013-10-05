#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QSettings>
#include "imageprocessing.h"
#include "histogram1d.h"

using namespace cv;

MainWindow::MainWindow(QString imagePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImage(imagePath);
      
    loadIni();
}

MainWindow::~MainWindow()
{
    saveIni();
    destroyAllWindows();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                 tr("Open File"),
                                 "/home/mick/Pictures",
                                 tr("Images (*.png *.jpeg *.jpg *.xpm)")
                                 );
    if (fileName.isNull())
        return;
    
    loadImage(fileName);
}

void MainWindow::showImage(Mat image)
{
    Mat temp = image.clone();
    switch(temp.type()) {
    case CV_8UC3:
        cvtColor(temp,temp, CV_BGR2RGB);
        break;
    case CV_8U:
        cvtColor(temp,temp, CV_GRAY2RGB);
        break;
    }
        
    
    QImage img = QImage((const unsigned char*)temp.data,
                        temp.cols,
                        temp.rows,
                        QImage::Format_RGB888);
    ui->lblImage->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::loadImage(QString path)
{
    if(path.isNull() || path.isEmpty())
        return;
    lastImagePath = path;
    mImage = imread(path.toLocal8Bit().data());
    showImage(mImage);    
}

void MainWindow::on_pushButton_2_clicked()
{
    Mat temp;
    flip(mImage, temp, 1);
    showImage(temp);
}


void MainWindow::on_pushButton_3_clicked()
{
    Mat temp = mImage.clone();
    ImageProcessing::salt(temp, 10000);
    showImage(temp);
}

void MainWindow::on_pushButton_4_clicked()
{
    Mat temp = mImage.clone();
    ImageProcessing::threshold(temp, temp, ui->edtColors->value());
    showImage(temp);
}

void MainWindow::on_pushButton_5_clicked()
{
    showImage(mImage);
}

void MainWindow::loadIni()
{
    QSettings settings;
    restoreGeometry(settings.value("Geometry", QByteArray()).toByteArray());
    loadImage(settings.value("LastImagePath", QString()).toString());
}

void MainWindow::saveIni()
{
    QSettings settings;
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("LastImagePath", lastImagePath);
}


void MainWindow::on_pushButton_6_clicked()
{
    Histogram1D hist;
    
    char *windowName = "Histogram";
    Mat histo = hist.getHistogramImage(mImage);
    namedWindow(windowName);
    imshow(windowName, histo);
}
