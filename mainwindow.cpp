#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QSettings>
#include <QPicture>
#include <QSplitter>
#include <QPropertyAnimation>
#include <QUrl>
#include <QThread>
#include <QDialog>
#include <qthreadex.h>
#include <QMessageBox>
#include <aboutdialog.h>
#include <QImageReader>
#include <QProcess>
#include <QDesktopServices>
#include <qmath.h>
#include <thresholdprocesspreviewer.h>

#include "particlesseeker.h"
#include "areasseeker.h"

#include "Utils/imageprocessing.h"
#include "Widgets/imageprocessingdialog.h"
#include "Core/erodeprocesspreviewer.h"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace OpenCVUtils;
using namespace Mick;

#define WND_HISTOGRAMM "Histogramm"

MainWindow::MainWindow(QString imagePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);   

    
    // устанавливаем указатель на главное окно
    applicationInfo.mainWindow = this;
    
    lastImageIndex = -1;

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
    ui->sldKeyProp->toggleButtonVisible(false);

    // tab widget
    // синхронизация интерфеса с текущем табом 
    connect(ui->tabDocuments, SIGNAL(invalidated()),
            SLOT(setCurrentStateAccordingActiveTab()));
    // контроль освобождения памяти наборов, если они уже не используются
    connect(ui->tabDocuments, SIGNAL(unsetKeyPoints(KeyPoints*)),
            SLOT(removeSet(KeyPoints*)));
    connect(ui->tabDocuments, SIGNAL(applyLightCorrectorForMe()),
            ui->lightCorrectorWidget, SIGNAL(apply()));
            
    
    
    ui->tabDocuments->addAction(ui->actionShow_particles);
    
    connect(ui->SequenceAnalyzeWdg, SIGNAL(keyPointsSetActivated(KeyPoints*)),
            ui->tabDocuments, SLOT(setKeyPoints(KeyPoints*)));
    connect(ui->SequenceAnalyzeWdg, SIGNAL(keyPointsSetRemoved(KeyPoints*)),
            SLOT(removeSet(KeyPoints*)));
    
    // actions
    connect(ui->actionShow_particles, SIGNAL(toggled(bool)),
            SLOT(toggleCurrentParticles(bool)));
    connect(ui->actionAdd_set_to_graph, SIGNAL(triggered()),
            SLOT(addKeyPointsToGraph()));
    
    
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
    connect(ui->SequenceAnalyzeWdg, SIGNAL(addKeyPointsToGraph()),
            SLOT(addKeyPointsToGraph()));
    connect(ui->SequenceAnalyzeWdg, SIGNAL(graphUpdated()),
            SLOT(updateAddSetButtonState()));
    
    // image processing buttons
    connect(ui->btnInvert, SIGNAL(clicked()),
            SLOT(invert()));
    connect(ui->btnEqHist, SIGNAL(clicked()),
            SLOT(equalizehist()));
    connect(ui->btnNormalize, SIGNAL(clicked()),
            SLOT(normalize()));
    
    // sldKeyProp;
    connect(ui->sldKeyProp, SIGNAL(valueChanged(int)),
            SLOT(setCurrentKeyPointsProportions(int)));
    
    connect(ui->actionClose_current_tab, SIGNAL(triggered()),
            ui->tabDocuments, SLOT(closeCurrentTab()));
    
    // LightCorrectorWidget
    connect(ui->lightCorrectorWidget, SIGNAL(modeChanged(QPainter::CompositionMode)),
            ui->tabDocuments, SLOT(setLightCorrectorMode(QPainter::CompositionMode)));
    connect(ui->lightCorrectorWidget, SIGNAL(apply()),
            ui->tabDocuments, SLOT(applyLightCorrector()));
    connect(ui->lightCorrectorWidget, SIGNAL(toggled(bool)),
            ui->tabDocuments, SLOT(toggleLightCorrector(bool)));
    connect(ui->actionLight_controller, SIGNAL(toggled(bool)),
            ui->lightCorrectorWidget, SLOT(setCorrectionEnabled(bool)));
    connect(ui->lightCorrectorWidget, SIGNAL(toggled(bool)),
            ui->actionLight_controller, SLOT(setChecked(bool)));
    connect(ui->lightCorrectorWidget, SIGNAL(apply()),
            SLOT(stackIterate()));
    connect(ui->lightCorrectorWidget, SIGNAL(intensityChanged(int)),
            ui->tabDocuments, SLOT(setLightCorrectorIntensity(int)));
    
    
    // load saved presets
    loadIni();
                
}



QPixmap MainWindow::currentImage()
{
    return ui->tabDocuments->currentFixedImage();
}

MainWindow::~MainWindow()
{
    // обнуляем указатель на главное окно
    applicationInfo.mainWindow = 0;
    // удаляем интерфейс
    delete ui;
}

void MainWindow::showImage(Mat image)
{
    mutex.lock();
    try {
        showImage(OpenCVUtils::ToQPixmap(image));
    } catch (...) {
        qDebug() << tr("error on MainWindow::showImage");
    }
    mutex.unlock();
}

void MainWindow::showImage(QPixmap pixmap)
{
    if (ui->tabDocuments->currentGraphicsView()) {
        ui->tabDocuments->currentGraphicsView()->setPixmap(pixmap);     
    }
}

void MainWindow::showImage(QImage image)
{
    showImage(QPixmap::fromImage(image));
}

void MainWindow::loadImage(QString path, bool setActive)
{
    if(path.isNull() || path.isEmpty())
        return;
    
    qDebug() << tr("Open: '%1'").arg(path);
    
    QImageReader imageReader(path);
    QImage temp = imageReader.read();
    if ( temp.isNull() ) {
        qWarning() << imageReader.errorString();
        if (imageReader.error() == QImageReader::UnsupportedFormatError) {
            qWarning() << imageReader.supportedImageFormats();
        }
        return;
    }

    lastImagePath = path;
    
    // добавляем
    ui->tabDocuments->addGraphicsViewEA(QPixmap::fromImage(temp),
                                        QFileInfo(path).fileName());
    ui->tabDocuments->setLightCorrector(
                ui->lightCorrectorWidget->addLightCorrector());
}

void MainWindow::FindParticles()
{
    QObject obj; // blank object for memory mangement of ImageProcessPreviewer
    
    if (!ImageProcessing(new ErodeProcessPreviewer(
                             OpenCVUtils::FromQPixmap(currentImage()),
                             &obj)))
        return;
    if (!ImageProcessing(new ThresholdProcessPreviewer(
                             OpenCVUtils::FromQPixmap(currentImage()),
                             &obj)))
        return;
    
    // fill holes
//    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
//    Mat temp = OpenCVUtils::FromQPixmap(currentImage());    
//    morphologyEx(temp, temp, MORPH_OPEN, kernel);
//    showImage(temp);
//    stackIterate();
    
    log(tr("looking for particles"));

    ParticlesSeeker *particlesSeeker = new ParticlesSeeker(0);
    particlesSeeker->setMaxRadius(100);
    particlesSeeker->setMinRadius(2);
    
    prepareEmisionAnalyzerThread(particlesSeeker);
    startLongProcess(particlesSeeker, tr("Looking for particles..."));
}

bool MainWindow::ImageProcessing(ImageProcessPreviewer *pr)
{
    ImageProcessingDialog dialog;
//    ImageProcessPreviewer *pr = new ErodeProcessPreviewer(
//                OpenCVUtils::FromQPixmap(currentImage()),
//                &dialog);
    dialog.setPreviewer(pr);
    dialog.setWindowState(Qt::WindowMaximized);
    
    // erode variants selection
    if (dialog.exec() != QDialog::Accepted)
        return false;
    
    if (typeid(*pr) == typeid(ErodeProcessPreviewer)) {
        ProcessInfo pErode = dialog.selectedImage();
        erode(pErode.params[ErodeProcessPreviewer::PARAM_RADIUS]);
        stackIterate();
        dilate(qMax((double)pErode.params[ErodeProcessPreviewer::PARAM_RADIUS],
               0.75*pErode.params[ErodeProcessPreviewer::PARAM_RADIUS]+1)
                );
    }
    if (typeid(*pr) == typeid(ThresholdProcessPreviewer)) {
        ProcessInfo pThreshold = dialog.selectedImage();
        threshold(pThreshold.params[ThresholdProcessPreviewer::PARAM_BORDER]);
    }
    stackIterate();
    return true;
}


void MainWindow::FindParticleAreas()
{
    AreasSeeker *areasSeeker = new AreasSeeker(0);
    
    prepareEmisionAnalyzerThread(areasSeeker);
    
    startLongProcess(areasSeeker, tr("Looking for black areas"));
}

void MainWindow::pushCurrentImage(QString title, bool asKey, int index)
{
    if (asKey) {
        currentKeyImageName = title;
    }
    ui->tabDocuments->fixCurrentImage(title, asKey, index);
}

void MainWindow::setCurrentImage(QPixmap pixmap)
{
    if (ui->tabDocuments->currentGraphicsView()) {
        ui->tabDocuments->currentGraphicsView()->setPixmap( pixmap);
        ui->tabDocuments->fixCurrentImage();
    }
    qDebug() << tr("change current image");
}
                
void MainWindow::threshold(int value)
{
    qDebug() << tr("threshold: %1").arg(value);
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());    
    ImageProcessing::threshold(temp, temp, value);
    showImage(temp);               
}

                
void MainWindow::erode(int value)
{
    qDebug() << tr("erode: %1").arg(value);
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    ImageProcessing::erode(temp, temp, value);
    showImage(temp);      
}


void MainWindow::dilate(int value)
{
    qDebug() << tr("dilate: %1").arg(value);
    
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    ImageProcessing::dilate(temp, temp, value);
    showImage(temp);      
}

void MainWindow::median(int value)
{
    qDebug() << tr("median filter: %1").arg(value);
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
    case 0: qDebug() << tr("switch channel to hue") ;break;
    case 1: qDebug() << (tr("switch channel to staturation"));break;
    case 2: qDebug() << (tr("switch channel to value"));break;   
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
    case 0: qDebug() << (tr("switch channel to red"));break;
    case 1: qDebug() << (tr("switch channel to green"));break;
    case 2: qDebug() << (tr("switch channel to blue"));break;   
    }    
}

void MainWindow::normalize()
{
    qDebug() << (tr("normalize"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    cv::normalize(temp, temp,0, 255, NORM_MINMAX, temp.type());
    showImage(temp);           
}

void MainWindow::invert()
{
    qDebug() << (tr("invert"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    cv::bitwise_not(temp,temp);
    showImage(temp); 
}

void MainWindow::equalizehist()
{
    qDebug() << (tr("equalize hist"));
    Mat temp = OpenCVUtils::FromQPixmap(currentImage());
    Mat rgb[3];
    cv::split(temp, rgb);
    cv::equalizeHist(rgb[0],rgb[0]);
    cv::equalizeHist(rgb[1],rgb[1]);
    cv::equalizeHist(rgb[2],rgb[2]);
    cv::merge(rgb,3,temp);
    showImage(temp);       
}

void MainWindow::log(QString message, QtMsgType type)
{
    ui->statusBar->showMessage(message);
}

void MainWindow::stackIterate(QString title)
{
    if (!ui->tabDocuments->currentGraphicsView()) return;
    
    if (!ui->lstImageStack->model()) return;
    
    setCurrentImage(ui->tabDocuments->currentGraphicsView()->pixmap());
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
    pushCurrentImage(label , false, row);  
}

void MainWindow::updateAddSetButtonState()
{
    KeyPoints *keyPoints = ui->tabDocuments->currentKeyPoints(); 
    
    bool enabled =  !(keyPoints==0 || keyPoints->count()==0);
    ui->SequenceAnalyzeWdg->toggleAddParticleButtonEnbaled(enabled);
    ui->actionAdd_set_to_graph->setEnabled(enabled);
    
    QString buttonText;
    if (!ui->SequenceAnalyzeWdg->isContains(keyPoints)) {
        buttonText = tr("Add particle set");
    } else {
        buttonText = tr("Update particle set");
    }
    ui->SequenceAnalyzeWdg->setAddParticlesButtonText(buttonText);
    ui->actionAdd_set_to_graph->setText(buttonText);
}

void MainWindow::addKeyPointsToGraph()
{
    KeyPoints *keyPoints = ui->tabDocuments->currentKeyPoints(); 
    ui->LeftTabWidget->setCurrentIndex(1);
    ui->SequenceAnalyzeWdg->addKeyPoints(keyPoints);
}

void MainWindow::clearKeyPoints()
{
    ui->SequenceAnalyzeWdg->clearKeyPoints();
}

void MainWindow::toggleCurrentParticles(bool fShow)
{
    MGraphicsViewEA *view = ui->tabDocuments->currentGraphicsView();
    if (view) view->toogleKeyPoints(fShow);
}

KeyPoints *MainWindow::createNewKeyPoints()
{
    KeyPoints *keyPoints = new KeyPoints(this);
    keyPoints->setProportion(ui->sldKeyProp->value());
    return keyPoints;
}

void MainWindow::removeSet(KeyPoints *keyPoints)
{
    if (!ui->SequenceAnalyzeWdg->isContains(keyPoints)) {
        if (!ui->tabDocuments->isContains(keyPoints)) {
            if (keyPoints) {
                qDebug() << tr("set %1 is no longer used, and can be deleted")
                            .arg(keyPoints->title());
                keyPoints->deleteLater();
            }
        }
    }
}

void MainWindow::fitToView()
{
    ui->tabDocuments->fitToTab();
}

void MainWindow::setCurrentKeyPointsProportions(int value)
{
    KeyPoints *keyPoints = ui->tabDocuments->currentKeyPoints(); 
    if (keyPoints) {
        keyPoints->setProportion(value);
    }
}

void MainWindow::setCurrentStateAccordingActiveTab()
{
    // меняем модель
    ui->lstImageStack->setModel(ui->tabDocuments->currentImageStack());
    // сдвигаем слайдер для изменеия пропорций точек
    KeyPoints *keyPoints = ui->tabDocuments->currentKeyPoints();
    if (keyPoints) {
        ui->sldKeyProp->setValue(keyPoints->proportion());
    }
    // обновляем кнопку
    updateAddSetButtonState();
    
    // 
    ui->SequenceAnalyzeWdg->setActive(keyPoints);
    
    // включаем/выключаем итемы меню
    bool isAnyActiveTab = ui->tabDocuments->currentIndex()!=-1;
    ui->actionFind_Areas->setEnabled(isAnyActiveTab);
    ui->actionSave_Image->setEnabled(isAnyActiveTab);
    ui->actionFind_particles->setEnabled(isAnyActiveTab);
    ui->actionClose_current_tab->setEnabled(isAnyActiveTab);
    ui->actionFit_To_View->setEnabled(isAnyActiveTab);
    
    // устанавливаем LightCorrectorWidget
    if (ui->tabDocuments->currentLightCorrector()) {
        ui->lightCorrectorWidget->setLightCorrector(
                    ui->tabDocuments->currentLightCorrector());
    }
    ui->lightCorrectorWidget->setCorrectionEnabled(
                ui->tabDocuments->isCorrectionEnabled());
}

void MainWindow::finishLookingForKeyPoints(EmisionAnalyzer *sender)
{
    qDebug() << QString(tr("find %1 particle(s)"))
                .arg(sender->keyPoints()->count());
    
    ui->tabDocuments->setKeyPoints(sender->keyPoints(), true);
    ui->actionShow_particles->setChecked(true);
    
    setCurrentStateAccordingActiveTab();
}

void MainWindow::prepareEmisionAnalyzerThread(EmisionAnalyzerThread *thread)
{
    Mat temp =  OpenCVUtils::FromQPixmap(currentImage());
//    imwrite("preview.png", temp);

    // создаем новый набор под ключевые точки
    KeyPoints *points = createNewKeyPoints();
    points->setTitle(ui->tabDocuments->currentTabName());
    
    thread->setImage(temp);
    thread->setKeyPoints(points);

    connect(thread, SIGNAL(finished(EmisionAnalyzer*)),
            SLOT(finishLookingForKeyPoints(EmisionAnalyzer*)));
}

void MainWindow::startLongProcess(QThreadEx *process, QString title)
{
    progressDialog.showCancelButton(process->cancelEnabled());
    progressDialog.showProgressBar(process->progressEnabled());
    progressDialog.setLabel(title);
    
    connect(&progressDialog, SIGNAL(rejected()),
            process, SLOT(cancel()));
    
    connect(process, SIGNAL(started()),
            &progressDialog, SLOT(show()));
    connect(process, SIGNAL(finished()),
            &progressDialog, SLOT(hide()));
    
    connect(process, SIGNAL(progressChanged(int,int,QString)),
            &progressDialog, SLOT(setProgress(int,int,QString)));

    connect(process, SIGNAL(finished()), 
            process, SLOT(deleteLater()));
    
    process->start();
}

void MainWindow::startQuickProcess(QThreadEx *process, QString title)
{
//     TODO 
}


void MainWindow::loadIni()
{
    QSettings settings( "Liqour.cfg", QSettings::IniFormat);
    // restore main form state
    restoreGeometry(settings.value("Geometry", QByteArray()).toByteArray());

    
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
    
    ui->SequenceAnalyzeWdg->loadIni(&settings);
    ui->lightCorrectorWidget->loadIni(&settings);
    
    loadImage(settings.value("LastImagePath", QString()).toString());    
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
    
    ui->SequenceAnalyzeWdg->saveIni(&settings);
    ui->lightCorrectorWidget->saveIni(&settings);

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
    
    if (ui->tabDocuments->currentGraphicsView()) {
        ui->tabDocuments->currentGraphicsView()->pixmap().save(fileName, "png");
    }
}

void MainWindow::on_lstImageStack_clicked(const QModelIndex &index)
{
    ImageStack * imageStack = ui->tabDocuments->currentImageStack();
    if (imageStack) {
        PixmapInfo info = imageStack->data(index);
        if (info.keyImage) {
            currentKeyImageName = info.Title;
        }
        setCurrentImage( info.Pixmap );
    }
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
    /*QString fileName 
            = QFileDialog::getSaveFileName( 0,
                                            tr("Dump file the file"),
                                            QString(),
                                            tr("Texts (*.txt)"));
    if (fileName.isNull()) return;
    
    if (keyPoints) 
        keyPoints->dumpToFile(fileName);*/
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
    QStringList fileNames
            = QFileDialog::getOpenFileNames(this,
                                     tr("Open File"),
                                     lastImagePath,
                                     tr("Images (*.png *.jpeg *.jpg *.xpm)")
                                 );
    foreach(QString fileName, fileNames) {
        loadImage(fileName);
    }
}

void MainWindow::on_actionFit_To_View_triggered()
{
    if (ui->tabDocuments->currentGraphicsView()) {
        ui->tabDocuments->currentGraphicsView()->fitToScreen();
    }
}

void MainWindow::setKeyPointsProportion(int value)
{
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



void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    const QMimeData *mimeData = e->mimeData();
    if (mimeData->hasUrls()) {
        foreach(QUrl url, mimeData->urls()) {
            QString path = url.toLocalFile();
            loadImage(path);
        }
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
     QMessageBox::aboutQt(this);   
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog about(0);
    about.exec();
}

void MainWindow::on_actionOpen_Log_triggered()
{
    QDesktopServices::openUrl(QUrl(applicationInfo.logFilePath));
}
