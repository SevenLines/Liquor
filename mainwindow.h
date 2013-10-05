#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>

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
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void showImage(Mat image);
    void loadImage(QString path);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    
    void on_pushButton_4_clicked();
    
    void on_pushButton_5_clicked();
    
    void on_pushButton_6_clicked();
    
    void loadIni();
    void saveIni();
    
    
private:
    Ui::MainWindow *ui;
    Mat mImage;
    QString lastImagePath;
};

#endif // MAINWINDOW_H
