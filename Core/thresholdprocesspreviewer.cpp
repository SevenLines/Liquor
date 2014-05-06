#include "thresholdprocesspreviewer.h"
#include "imageprocessing.h"
#include "opencvutils.h"

const QString ThresholdProcessPreviewer::PARAM_BORDER = "threshold";

ThresholdProcessPreviewer::ThresholdProcessPreviewer(Mat image, QObject *parent) :
    ImageProcessPreviewer(image, parent)
{
    regenImages();
}


QString ThresholdProcessPreviewer::title()
{
    return tr("Threshold. Select the most convinient variant");
}

void ThresholdProcessPreviewer::regenImages()
{
    float kW = (float)_size.width() / baseImage.cols;
    float kH = (float)_size.height() / baseImage.rows;
    float k = qMin(kW, kH);
    
    if (k <= 0)
        return;
    
    Mat baseScaledImage;// baseImage.rows*k, baseImage.cols*k, baseImage.type());
    
    if (k!=1) {
        resize(baseImage, baseScaledImage, Size(), k, k);
    } else {
        baseScaledImage = baseImage;
    }
    
    int c = 10;
    for (int i=40;i<250;i+=c) {
        Mat img;
        ImageProcessing::threshold(baseScaledImage, img, i);
        ProcessInfo info;
        info.image = OpenCVUtils::ToQImage(img);
        info.params[ThresholdProcessPreviewer::PARAM_BORDER] = i;
        images.append(info);
    }
}
