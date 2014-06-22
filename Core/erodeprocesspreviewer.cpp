#include "erodeprocesspreviewer.h"
#include "imageprocessing.h"
#include "opencvutils.h"
#include <QMap>

const QString ErodeProcessPreviewer::PARAM_RADIUS = "radius";

ErodeProcessPreviewer::ErodeProcessPreviewer(Mat image, QObject *parent) :
    ImageProcessPreviewer(image, parent)
{
    regenImages();
}


void ErodeProcessPreviewer::regenImages()
{    
    float kW = (float)_size.width() / baseImage.cols;
    float kH = (float)_size.height() / baseImage.rows;
    float k = qMin(kW, kH);
    float lastk = -1;
    float kfScaler = 1.5;
    
    if (k <= 0)
        return;
    
    Mat baseScaledImage;// baseImage.rows*k, baseImage.cols*k, baseImage.type());
    
    resize(baseImage, baseScaledImage, Size(), k, k, INTER_LANCZOS4);
    
    Mat img = baseScaledImage;
    ProcessInfo info;
    info.image = OpenCVUtils::ToQImage(img);
    info.params[ErodeProcessPreviewer::PARAM_RADIUS] = 0;
    images.append(info);
    
    for (int i=6;i<18;i+=1) {
        lastk = (k*kfScaler)*i;
        ImageProcessing::erode(baseScaledImage, img, lastk);
        info.image = OpenCVUtils::ToQImage(img);
        info.params[ErodeProcessPreviewer::PARAM_RADIUS] = i;
        images.append(info);
    }
}


QString ErodeProcessPreviewer::title()
{
    return tr("Erode. Select the most suitable variant. For the perfomance sake real result can be a bit differ from present one.");
}
