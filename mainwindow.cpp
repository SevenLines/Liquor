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
using namespace OpenCVUtils;

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
    
    // fill channels combobox
    ui->cmbChannels->addItem("");
    ui->cmbChannels->addItem(tr("Red"));
    ui->cmbChannels->addItem(tr("Green"));
    ui->cmbChannels->addItem(tr("Blue"));
    ui->cmbChannels->addItem(tr("Hue"));
    ui->cmbChannels->addItem(tr("Saturation"));
    ui->cmbChannels->addItem(tr("Value"));
      
    // particle prop slider init 
    ui->sldKeyProp->setMax(200);
    ui->sldKeyProp->setMin(1);
    ui->sldKeyProp->setValue(100);
    connect(ui->sldKeyProp, SIGNAL(valueChanged(int)),
            &keyPoints, SLOT(setProportion(int)));
    connect(&keyPoints, SIGNAL(proportionChange(int)),
            ui->graphicsView, SLOT(update()));
    
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
    QString fileName 
            = QFileDialog::getOpenFileName(this,
                                     tr("Open File"),
                                     lastImagePath,
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

void MainWindow::loadImage(QString path, bool setActive)
{
    if(path.isNull() || path.isEmpty())
        return;
    
    QImage temp;
    temp.load(path);
    
    lastImagePath = path;
   
    if (setActive) {
        mImage = QPixmap::fromImage(temp.copy());
    }
    
    showImage(temp.copy());
    ui->graphicsView->fitToScreen();
    
    pushCurrentImage(QFileInfo(path).baseName());
}

void MainWindow::analyze()
{
    log(tr("looking for particles"));
    
    Mat temp =  OpenCVUtils::FromQPixmap(currentImage());

    EmisionAnalyzer ea;
    
    ea.setMaxRadius(100);
    ea.setMinRadius(5);
    ea.setImage(temp);
    
    keyPoints.clear();
    ea.findCircles(keyPoints);
    stackIterate(tr("analyze"));
    
    QPixmap pixmap = ui->graphicsView->pixmap();
    QPainter p;
    p.begin(&pixmap);
        p.setPen(QPen(Qt::white,2));
        keyPoints.draw(&p);
        ui->graphicsView->setPixmap(pixmap);
    p.end();
    
    log(QString(tr("find %1 particle(s)")).arg(keyPoints.count()));
}

void MainWindow::pushCurrentImage(QString title, int index)
{
    imageStack.push(currentImage(), title, index); 
    log(tr("push current image to stack as '%1'").arg(title));
}

void MainWindow::setCurrentImage(QPixmap pixmap)
{
    mImage = pixmap;
    ui->graphicsView->setPixmap( pixmap);
    log(tr("change current image"));
}
                
void MainWindow::threshold(int value)
{
    log(tr("threshold: %1").arg(value));
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());    
    ImageProcessing::threshold(temp, temp, value);
    showImage(temp);               
}
                
void MainWindow::erode(int value)
{
    log(tr("erode: %1").arg(value));
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    ImageProcessing::erode(temp, temp, value);
    showImage(temp);      
}

void MainWindow::median(int value)
{
    log(tr("median filter: %1").arg(value));
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
    
    switch(channel) {
    case 0: log(tr("switch channel to hue"));break;
    case 1: log(tr("switch channel to staturation"));break;
    case 2: log(tr("switch channel to value"));break;   
    }    
}

void MainWindow::rgb(int channel)
{
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    
    Mat rgb[3];
    cv::split(temp, rgb); 
    
    cvtColor(rgb[2-channel], temp, CV_GRAY2BGR);
    
    showImage(temp);   
    
    switch(channel) {
    case 0: log(tr("switch channel to red"));break;
    case 1: log(tr("switch channel to green"));break;
    case 2: log(tr("switch channel to blue"));break;   
    }    
}

void MainWindow::normalize()
{
    log(tr("normalize"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    cv::normalize(temp, temp,0, 255, NORM_MINMAX, temp.type());
    showImage(temp);           
}

void MainWindow::invert()
{
    log(tr("invert"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    cv::bitwise_not(temp,temp);
    showImage(temp); 
}

void MainWindow::equalizehist()
{
    log(tr("equalize hist"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    Mat rgb[3];
    cv::split(temp, rgb);
    cv::equalizeHist(rgb[0],rgb[0]);
    cv::equalizeHist(rgb[1],rgb[1]);
    cv::equalizeHist(rgb[2],rgb[2]);
    cv::merge(rgb,3,temp);
    showImage(temp);       
}

void MainWindow::log(QString message)
{
    qDebug() << message;
    ui->statusBar->showMessage(message);
}

void MainWindow::stackIterate(QString title)
{
    setCurrentImage(ui->graphicsView->pixmap());
    int row = ui->lstImageStack->currentIndex().row();
    QString label;
    if (title.isNull()) {
        label  = "image";
        if (row == -1) {
            label += QString::number(ui->lstImageStack->model()->rowCount());
        } else {
            label += QString::number(row+1);
        }
    } else {
        label = title;
    }
    pushCurrentImage(label , row);  
}


void MainWindow::on_pushButton_5_clicked()
{
    showImage(mImage);
}

void MainWindow::loadIni()
{
    QSettings settings( "Liqour.cfg", QSettings::IniFormat);
    restoreGeometry(settings.value("Geometry", QByteArray()).toByteArray());
    loadImage(settings.value("LastImagePath", QString()).toString());
}

void MainWindow::saveIni()
{
    QSettings settings("Liqour.cfg", QSettings::IniFormat);
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("LastImagePath", lastImagePath);
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::exit(0);
}

void MainWindow::on_btnSaveImageStack_clicked()
{
    stackIterate();
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


void MainWindow::on_btnNormalize_clicked()
{
    normalize();
}

void MainWindow::on_btnInvert_clicked()
{
    invert();
}

void MainWindow::on_btnEqHist_clicked()
{
    equalizehist();
}

void MainWindow::on_cmbChannels_currentIndexChanged(const QString &value)
{
    if (value == "") showImage(currentImage());
    if (value == tr("Red")) rgb(0);
    if (value == tr("Green")) rgb(1);
    if (value == tr("Blue")) rgb(2);
    if (value == tr("Hue")) hsv(0);
    if (value == tr("Saturation")) hsv(1);
    if (value == tr("Value")) hsv(2);
}

void MainWindow::on_actionDump_keyPoints_triggered()
{
    QString fileName 
            = QFileDialog::getSaveFileName( 0,
                                            tr("Dump file the file"),
                                            QString(),
                                            tr("Texts (*.txt)"));
    if (fileName.isNull()) return;
    
    keyPoints.dumpToFile(fileName);
}

void MainWindow::on_btnGetFillAreas_clicked()
{
    EmisionAnalyzer emisionAnalyzer;
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    emisionAnalyzer.setImage(temp);
    
    QList<QList<cv::Point> > areas;
    
    emisionAnalyzer.findBlackAreas(areas);
    
    QList<cv::Point> area;
    int colorFlag =0;
    foreach(area, areas) {
        cv::Point p;
        foreach(p, area) {
            if (emisionAnalyzer.isOnEdge(p)) {
                temp.at<Vec3b>(p)[0] = 255;
            } else {
                temp.at<Vec3b>(p)[1] = colorFlag%2?196:64;
            }
        }
        colorFlag++;
    }
    
    setCurrentImage(OpenCVUtils::ToQPixmap(temp));
}
