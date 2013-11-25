#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "imagestack.h"
#include "opencvutils.h"
#include "keypoints.h"

class QSettings;

using namespace cv;

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
    void on_pushButton_clicked();
    
    void showImage(Mat image);
    void showImage(QPixmap pixmap);
    void showImage(QImage image);
    
    QImage loadImage(QString path, bool setActive = true);
    void analyze();
    void pushCurrentImage(QString title, int index = -1);
    
    void setCurrentImage(QPixmap pixmap);
    
    void threshold(int value);
    void erode(int value);
    void median(int value);
    void hsv(int channel);
    void rgb(int channel);
    void normalize();
    void invert();
    void equalizehist();
    void log(QString message);
    void stackIterate(QString title = QString());
  
    
    void on_pushButton_5_clicked();
    
    void loadIni();
    void saveIni();
    
    void on_actionExit_triggered();
    
    void on_btnSaveImageStack_clicked();
    
    void on_actionSave_Image_triggered();
    
    void on_lstImageStack_clicked(const QModelIndex &index);

    void on_btnNormalize_clicked();
    
    void on_btnInvert_clicked();
    
    void on_btnEqHist_clicked();

    void on_cmbChannels_currentIndexChanged(const QString &arg1);
    
    void on_btnAutoAnalzye_clicked();
    
    void on_actionDump_keyPoints_triggered();
    
private:
    Ui::MainWindow *ui;
    Mat lastShowedImage;
    QString lastImagePath;
    ImageStack imageStack;
    
    QPixmap mImage;
    KeyPoints keyPoints;
    
    int lastImageIndex;
};

#endif // MAINWINDOW_H
