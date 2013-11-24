#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QSettings>
#include <QPicture>
#include "imageprocessing.h"
#include "histogram1d.h"
#include "emisionanalyzer.h"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;

#define WND_HISTOGRAMM "Histogramm"

MainWindow::MainWindow(QString imagePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->graphicsView->addAction(ui->actionSave_Image);
    
    lastImageIndex = -1;
    
    // threshold control init
   // ui->sldThreshold->setTitle(tr("threshold"));
    ui->sldThreshold->setMax(255);
    connect(ui->sldThreshold, SIGNAL(valueChanged(int)),
            this, SLOT(threshold(int)));
    
    // erode control init
    //ui->sldErode->setTitle(tr("erode"));
    ui->sldErode->setMax(20);
    connect(ui->sldErode, SIGNAL(valueChanged(int)),
            this, SLOT(erode(int)));  
    
    // median control init
    //ui->sldMedian->setTitle(tr("median"));
    ui->sldMedian->setMax(50);
    connect(ui->sldMedian, SIGNAL(valueChanged(int)),
            this, SLOT(median(int))); 
    
    // set Image stack
    ui->lstImageStack->setModel(&imageStack);
    
      
    
    // analyze button init
    connect(ui->btnAnalyze, SIGNAL(clicked()),
            this, SLOT(analyze()));
    
    loadImage(imagePath);
      
    loadIni();
}


QPixmap MainWindow::currentImage()
{
    return mImage;
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
    showImage(OpenCVUtils::ToQPixmap(image));
}

void MainWindow::showImage(QPixmap pixmap)
{
    ui->graphicsView->setPixmap(pixmap);     
}

void MainWindow::showImage(QImage image)
{
    showImage(QPixmap::fromImage(image));
}

QImage MainWindow::loadImage(QString path, bool setActive)
{
    if(path.isNull() || path.isEmpty())
        return QImage();
    
    QImage temp;
    temp.load(path);
    
    lastImagePath = path;
   
    if (setActive) {
        mImage = QPixmap::fromImage(temp.copy());
    }
    
    showImage(temp);
    ui->graphicsView->fitToScreen();
    
    pushCurrentImage(QFileInfo(path).baseName());
    
    return temp;
}

void MainWindow::analyze()
{
    Mat temp =  OpenCVUtils::FromQPixmap(currentImage());

    EmisionAnalyzer ea;
    
    ea.setMaxRadius(100);
    ea.setMinRadius(5);
    ea.setImage(temp);
    
    Mat out = ea.findCircles(temp);
    showImage(out);
}

void MainWindow::pushCurrentImage(QString title, int index)
{
    imageStack.push(currentImage(), title, index);   
}

void MainWindow::setCurrentImage(QPixmap pixmap)
{
    mImage = pixmap;
    ui->graphicsView->setPixmap( pixmap);
}
                
void MainWindow::threshold(int value)
{
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    
    ImageProcessing::threshold(temp, temp, value);
    
    showImage(temp);               
}
                
void MainWindow::erode(int value)
{
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    ImageProcessing::erode(temp, temp, value);
    showImage(temp);           
}

void MainWindow::median(int value)
{
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    
    int kSize = value;
    kSize = kSize + (kSize + 1) % 2;
    
    cv::medianBlur(temp, temp, kSize);
    showImage(temp);
}

void MainWindow::hsv(int channel)
{
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    
    cvtColor(temp, temp, CV_BGR2HSV);
    
    Mat hsv[3];
    cv::split(temp, hsv); 
    
    cvtColor(hsv[channel], temp, CV_GRAY2BGR);
    
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

void MainWindow::on_actionZoom_Out_triggered()
{
 
}

void MainWindow::on_actionZoom_In_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::exit(0);
}

void MainWindow::on_btnSaveImageStack_clicked()
{
    setCurrentImage(ui->graphicsView->pixmap());
    int row = ui->lstImageStack->currentIndex().row();
    QString label = "image";
    if (row == -1) {
        label += QString::number(ui->lstImageStack->model()->rowCount()+1);
    } else {
        label += QString::number(row+1);
    }
    pushCurrentImage(label , row);
}

void MainWindow::on_actionSave_Image_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(0,
                                                    tr("Select the file"),
                                                    QString(),
                                                    tr("Images (*.png)"));
    if (fileName.isNull()) return;
    ui->graphicsView->pixmap().save(fileName, "png");
}

void MainWindow::on_lstImageStack_clicked(const QModelIndex &index)
{
    setCurrentImage( this->imageStack.data(index).Pixmap );
}

void MainWindow::on_btnHue_clicked()
{
    hsv(0);
}

void MainWindow::on_btnSatturation_clicked()
{
    hsv(1);
}

void MainWindow::on_btnValue_clicked()
{
    hsv(2);
}
