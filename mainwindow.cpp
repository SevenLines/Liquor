#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QSettings>
#include "imageprocessing.h"
#include "histogram1d.h"
#include "emisionanalyzer.h"

using namespace cv;

#define WND_HISTOGRAMM "Histogramm"

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
    lastShowedImage = temp;    
    
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
    ImageProcessing::threshold(temp, temp, ui->sldThreshold->value());
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
    Histogram1D h;
    Mat imageROI;

    namedWindow(WND_HISTOGRAMM);
    QRect lblImageFrame = ui->lblImage->frame();
    if (!lblImageFrame.isEmpty()) {
        cv::Rect rect(lblImageFrame.x(), lblImageFrame.y(),
                  lblImageFrame.width(), lblImageFrame.height());
        imageROI = mImage(rect);
    } else {
        imageROI = mImage;
    }
    
    Mat histogram = h.getHistogramImage(imageROI);
    imshow(WND_HISTOGRAMM, histogram);
}


void MainWindow::on_actionZoom_Out_triggered()
{
    QSize size = ui->lblImage->size();
    size.setWidth(size.width() * 1.5);
    size.setHeight(size.height() * 1.5);
    ui->lblImage->resize(size);   
}

void MainWindow::on_actionZoom_In_triggered()
{
    QSize size = ui->lblImage->size();
    size.setWidth(size.width() / 1.5);
    size.setHeight(size.height() / 1.5);
    ui->lblImage->resize(size);
}

void MainWindow::on_pushButton_7_clicked()
{
    Mat temp = mImage.clone();
    ImageProcessing::erode(temp, temp, ui->sldErode->value());
    showImage(temp);
}

void MainWindow::on_pushButton_8_clicked()
{
    Mat temp = mImage.clone();
    ImageProcessing::dilate(temp, temp, ui->sldDilate->value());
    showImage(temp);
}

void MainWindow::on_sldErode_valueChanged(int value)
{
    ui->pushButton_7->click();
}

void MainWindow::on_sldDilate_valueChanged(int value)
{
    ui->pushButton_8->click();
}

void MainWindow::on_actionExit_triggered()
{
    close();   
}

void MainWindow::on_sldDilate_actionTriggered(int action)
{
    
}

void MainWindow::on_sldThreshold_valueChanged(int value)
{
    ui->pushButton_4->click();
}

void MainWindow::on_btnSaveImageStack_clicked()
{
    mImage = lastShowedImage;
}

void MainWindow::on_btnFindCircles_clicked()
{
    Mat temp =  mImage.clone();
    /*vector<Mat> hsv; // for each channel
    cvtColor(temp,temp,CV_RGB2HSV); */
    ImageProcessing::dilate(temp, temp, 10);
    ImageProcessing::erode(temp, temp, 10);
    ImageProcessing::threshold(temp, temp, 80);
    ImageProcessing::erode(temp, temp, 20);
    ImageProcessing::dilate(temp, temp, 10);
    std::cout << "done with erode dilate processing" << std::endl;
    //cv::split(temp, hsv);
    
    EmisionAnalyzer ea;
    Mat out;
    ea.findCircles(temp, out);
    showImage(out);
}

void MainWindow::on_btnClearBG_clicked()
{
    // get selected frame;
    Mat imageROI;
    QRect lblImageFrame = ui->lblImage->frame();
    if (!lblImageFrame.isEmpty()) {
        cv::Rect rect(lblImageFrame.x(), lblImageFrame.y(),
                  lblImageFrame.width(), lblImageFrame.height());
        imageROI = mImage(rect);
    } else {
        return; // exit if nothing selected;
    }
    
    
    
}
