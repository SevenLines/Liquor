#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QSettings>
#include <QPicture>
#include <QSplitter>
#include <QPropertyAnimation>


#include "Utils/imageprocessing.h"
#include "Core/emisionanalyzer.h"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace OpenCVUtils;
using namespace Mick;

#define WND_HISTOGRAMM "Histogramm"

MainWindow::MainWindow(QString imagePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    ui->graphicsView->addAction(ui->actionSave_Image); 
    lastImageIndex = -1;
    keyPoints = 0;
    
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

    
    // particle show checkable
    connect(ui->actionShow_particles, SIGNAL(toggled(bool)), 
            ui->graphicsView, SLOT(toogleKeyPoints(bool)));
    
    // add showparticles menu item to graphicsview   context menu 
    ui->graphicsView->addContextMenuAction(ui->actionShow_particles);
    
    // load image is any passed as parameter to cmd
    loadImage(imagePath);
      
    // load saved presets
    loadIni();
    
    // threshold control init
    ui->sldThreshold->setMax(255);
    connect(ui->sldThreshold, SIGNAL(apply()),
            SLOT(stackIterate()));
    connect(ui->sldThreshold, SIGNAL(valueChanged(int)),
            this, SLOT(threshold(int)));
    
    // erode control init
    ui->sldErode->setMax(20);
    connect(ui->sldErode, SIGNAL(apply()),
            SLOT(stackIterate()));
    connect(ui->sldErode, SIGNAL(valueChanged(int)),
            this, SLOT(erode(int)));  
    
    // median control init
    ui->sldMedian->setMax(50);
    connect(ui->sldMedian, SIGNAL(apply()),
            SLOT(stackIterate()));
    connect(ui->sldMedian, SIGNAL(valueChanged(int)),
            this, SLOT(median(int)));
    
    // keyPoints graph buttons
    connect(ui->btnAddParticleSet, SIGNAL(clicked()),
            SLOT(addKeyPointsToGraph()));
    connect(ui->SequenceAnalyzeWdg, SIGNAL(graphUpdated()),
            SLOT(updateAddSetButtonState()));
    
    connect(ui->SequenceAnalyzeWdg, SIGNAL(keyPointsSetActivated(KeyPoints*)),
            SLOT(setCurrentKeyPoints(KeyPoints*)));
    
    setCurrentKeyPoints(0);

}


QPixmap MainWindow::currentImage()
{
    return mImage;
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->actionShow_particles->setChecked(false);
    
    pushCurrentImage(QFileInfo(path).baseName());
}

void MainWindow::FindParticles()
{
    log(tr("looking for particles"));
    
    Mat temp =  OpenCVUtils::FromQPixmap(currentImage());

    EmisionAnalyzer ea;
    
    ea.setMaxRadius(100);
    ea.setMinRadius(5);
    ea.setImage(temp);
    
    // создаем новый набор под ключевые точки
    ui->graphicsView->setKeyPoints(0);
    KeyPoints *points = createNewKeyPoints();
    // ищем частицы
    ea.findCircles(*points);
    setCurrentKeyPoints(points);
    // сохраняем обработанное изображение
    // stackIterate(tr("analyze"));
    
    
    ui->actionShow_particles->setChecked(true);
    
    log(QString(tr("find %1 particle(s)")).arg(keyPoints->count()));
}


void MainWindow::FindParticleAreas()
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

void MainWindow::updateAddSetButtonState()
{
    if (!ui->SequenceAnalyzeWdg->isContains(keyPoints)) {
        ui->btnAddParticleSet->setText(tr("Add particle set"));
    } else {
        ui->btnAddParticleSet->setText(tr("Update particle set"));
    }
}

void MainWindow::addKeyPointsToGraph()
{
    ui->LeftTabWidget->setCurrentIndex(1);
    ui->SequenceAnalyzeWdg->addKeyPoints(keyPoints);
}

void MainWindow::clearKeyPoints()
{
    ui->SequenceAnalyzeWdg->clearKeyPoints();
}

KeyPoints *MainWindow::createNewKeyPoints()
{
    KeyPoints *keyPoints = new KeyPoints(this);
    return keyPoints;
}

void MainWindow::setCurrentKeyPoints(KeyPoints *keyPoints)
{
    if (this->keyPoints) {   
        removeCurrentKeyPoints();
    }
    // подключаем сигналы
    if (keyPoints) {
        keyPoints->setProportion(ui->sldKeyProp->value());
        connect(ui->sldKeyProp, SIGNAL(valueChanged(int)),
                keyPoints, SLOT(setProportion(int)));
        connect(keyPoints, SIGNAL(proportionChange(int)),
                ui->graphicsView, SLOT(update()));
    }
    this->keyPoints = keyPoints;
    ui->graphicsView->setKeyPoints(keyPoints);
    
    emit currentKeyPointsChanged(this->keyPoints);
    
    ui->SequenceAnalyzeWdg->setActive(keyPoints);
    
    updateAddSetButtonState();
}

void MainWindow::removeCurrentKeyPoints()
{
    this->keyPoints->disconnect(this);
    if (keyPoints && keyPoints->parent() == this) {
        delete keyPoints;
        keyPoints = 0;
    }
}

void MainWindow::loadIni()
{
    QSettings settings( "Liqour.cfg", QSettings::IniFormat);
    // restore main form state
    restoreGeometry(settings.value("Geometry", QByteArray()).toByteArray());
    loadImage(settings.value("LastImagePath", QString()).toString());
    
    // restore image processing presets
    // TODO
    settings.beginGroup("ImageProcessing");
    ui->sldErode->setValue(
                settings.value("Erode", 9).toInt());
    ui->sldThreshold->setValue(
                settings.value("Threshold", 85).toInt());
    settings.endGroup();
    
    // restore particles presets
    settings.beginGroup("Particles");
    setKeyPointsProportion(
                settings.value("Proportion", 100).toInt());
    settings.endGroup();
    
    // restore BottomWidget state
    settings.beginGroup("BottomTabWidget");
    ui->spliterBottom->restoreState(
                settings.value("spliterBottom", QByteArray()).toByteArray());
    ui->BottomTabWidget->setCurrentIndex(
                settings.value("CurrentIndex", true).toInt());
    ui->BottomTabWidget->setVisible(
                settings.value("Visible", true).toBool());
    settings.endGroup();
    
    // restore LeftPanel state
    settings.beginGroup("LeftPanel");
    ui->splitterLeft->restoreState(
                settings.value("splitterLeft", QByteArray()).toByteArray());
    ui->splitterLeftCenter->restoreState(
                settings.value("splitterLeftCenter", QByteArray()).toByteArray());
    ui->LeftPanel->setVisible(
                settings.value("Visible", true).toBool());
    ui->LeftTabWidget->setCurrentIndex(
                settings.value("CurrentIndex", 0).toInt());
    settings.endGroup();
    
    ui->SequenceAnalyzeWdg->saveIni(&settings);
}

void MainWindow::saveIni()
{
    QSettings settings("Liqour.cfg", QSettings::IniFormat);
    
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("LastImagePath", lastImagePath);
    
    // save image processing presets
    // TODO
    settings.beginGroup("ImageProcessing");
    settings.setValue("Erode", ui->sldErode->value());
    settings.setValue("Threshold", ui->sldThreshold->value());
    settings.endGroup();
    
    // save particles presets
    settings.beginGroup("Particles");
    settings.setValue("Proportion", ui->sldKeyProp->value());
    settings.endGroup();
    
    // save BottomWidget state
    settings.beginGroup("BottomTabWidget");
    settings.setValue("Visible", ui->BottomTabWidget->isVisible());
    settings.setValue("spliterBottom", ui->spliterBottom->saveState());
    settings.setValue("CurrentIndex", ui->BottomTabWidget->currentIndex());
    settings.endGroup();
    
    // save LeftPanel state
    settings.beginGroup("LeftPanel");
    settings.setValue("Visible", ui->LeftPanel->isVisible());
    settings.setValue("splitterLeft", ui->splitterLeft->saveState());
    settings.setValue("splitterLeftCenter", ui->splitterLeftCenter->saveState());
    settings.setValue("CurrentIndex", ui->LeftTabWidget->currentIndex());
    settings.endGroup();
    
    ui->SequenceAnalyzeWdg->loadIni(&settings);
}

void MainWindow::on_actionExit_triggered()
{
    close();
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
    
    if (keyPoints) 
        keyPoints->dumpToFile(fileName);
}

void MainWindow::on_btnHideLeftPanel_clicked()
{
    // чтобы прятать левую панельку
    bool fVisible = ui->LeftPanel->isVisible();
    ui->LeftPanel->setVisible(!fVisible);  
    ui->btnHideLeftPanel->setText(fVisible?">":"<");
}

void MainWindow::on_btnHideBottomPanel_clicked()
{
    bool fVisible = ui->BottomTabWidget->isVisible();
    ui->BottomTabWidget->setVisible(!fVisible);  
}


void MainWindow::closeEvent(QCloseEvent *)
{
    saveIni();
}

void MainWindow::on_actionOpen_Image_triggered()
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

void MainWindow::on_actionFit_To_View_triggered()
{
    ui->graphicsView->fitToScreen();
}

void MainWindow::setKeyPointsProportion(int value)
{
    //this->keyPoints.setProportion(value);
    ui->sldKeyProp->setValue(value);
}

void MainWindow::on_actionFind_particles_triggered()
{
    FindParticles();
}

void MainWindow::on_actionFind_Areas_triggered()
{
    FindParticleAreas();
}

