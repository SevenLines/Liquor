#ifndef IMAGEPROCESSINGDIALOG_H
#define IMAGEPROCESSINGDIALOG_H

#include <QDialog>
#include "imageprocesspreviewer.h"
#include "Models/imageprocesspreviewermodel.h"

namespace Ui {
class ImageProcessingDialog;
}

class ImageProcessingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProcessingDialog(QWidget *parent = 0);
    ~ImageProcessingDialog();
    
    ProcessInfo selectedImage();
    
private:
    Ui::ImageProcessingDialog *ui;
    ImageProcessPreviewerModel model;
    
public slots:
    void setPreviewer(ImageProcessPreviewer *previewer);
    
    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *);
    

};

#endif // IMAGEPROCESSINGDIALOG_H
