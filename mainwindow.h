#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <qthreadex.h>
#include <particlesseeker.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Core/imagestack.h"
#include "Core/keypoints.h"
#include "Utils/opencvutils.h"
#include "mgraphicsviewea.h"
#include "MainInclude.h"
#include "progressdialog.h"

class QSettings;

using namespace cv;
using namespace Mick;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString imagePath, QWidget *parent = 0);
    Mat QPixmapToMat(QPixmap pixmap);
    
    QPixmap currentImage();


    ~MainWindow();
    
private slots:
   
    /// изменение изображение текущего таба
    void showImage(Mat image);
    void showImage(QPixmap pixmap);
    void showImage(QImage image);
    
    void loadImage(QString path, bool setActive = true);
    void pushCurrentImage(QString title, bool asKey=false, int index = -1);
    
    void FindParticles();
    void FindParticleAreas();
    
    void setCurrentImage(QPixmap pixmap);
    
    void threshold(int value);
    void erode(int value);
    void median(int value);
    void hsv(int channel);
    void rgb(int channel);
    void normalize();
    void invert();
    void equalizehist();
   
    void loadIni();
    void saveIni();
    
    void on_actionExit_triggered();
    
    void on_btnSaveImageStack_clicked();
    
    void on_actionSave_Image_triggered();
    
    void on_lstImageStack_clicked(const QModelIndex &index);

    void on_cmbChannels_currentIndexChanged(const QString &arg1);
    
    void on_actionDump_keyPoints_triggered();
    
    void on_btnHideLeftPanel_clicked();
    
    void on_btnHideBottomPanel_clicked();
    
    void closeEvent(QCloseEvent *);
    
    void on_actionOpen_Image_triggered();
    
    void on_actionFit_To_View_triggered();
    
    void on_actionFind_particles_triggered();
    
    void on_actionFind_Areas_triggered();
    
public slots:   
    void log(QString message, QtMsgType type = QtDebugMsg);    
    
    void setKeyPointsProportion(int value);
    void stackIterate(QString title = QString());
    
    void updateAddSetButtonState();
    
    // добавляет точки к графику
    void addKeyPointsToGraph();
    // очищает точки графику
    void clearKeyPoints();
    
    void toggleCurrentParticles(bool fShow);
    
private:
    Ui::MainWindow *ui;
    Mat lastShowedImage;
    QString lastImagePath;
    ProgressDialog progressDialog;
    
    QPixmap mImage;
    Mick::KeyPoints *keyPoints;
    
    QString currentKeyImageName;
    
    int lastImageIndex;

    /// создает новый набор точек
    KeyPoints *createNewKeyPoints();
    
private slots:

    /// устанавливает размер изображения по окну
    void fitToView();
    
    /// установить пропорции текущего набора ключевых точек 
    void setCurrentKeyPointsProportions(int value);
    
    /// устаналивает виджеты в соответствии с текущим выбраным изображением
    /// (история, слайдеры и т.д.)
    void setCurrentStateAccordingActiveTab();
    
    /// вызывается когда приложение окончило поиск частиц
    void finishLookingForKeyPoints(EmisionAnalyzer *sender);
    
    void prepareEmisionAnalyzerThread(EmisionAnalyzerThread *thread);
    
    /// запускает долгий процесс, открывает прогресс диалог
    void startLongProcess(QThreadEx *process, QString title="");

    void on_actionAbout_Qt_triggered();
    
    void on_actionAbout_triggered();
    
signals:
    
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
};

#endif // MAINWINDOW_H
