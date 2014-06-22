#include "thresholdprocesspreviewer.h"
#include "imageprocessing.h"
#include "opencvutils.h"
#include <QDebug>

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
    
    int c = 4;
    for (int i=20;i<200;i+=(2*c + 1)) {
        Mat img;
        if (i%2 != 1)
            i += 1;
        ImageProcessing::threshold(baseScaledImage, img, i);
//        ImageProcessing::adaptiveThreshold(baseScaledImage, img, 255, i, ADAPTIVE_THRESH_MEAN_C);
        ProcessInfo info;
        info.image = OpenCVUtils::ToQImage(img);
//        info.params[ThresholdProcessPreviewer::PARAM_BORDER] = (int)(i/k) % 2 == 0?i/k+1:i/k;
        info.params[ThresholdProcessPreviewer::PARAM_BORDER] = i;
        images.append(info);
    }
}
