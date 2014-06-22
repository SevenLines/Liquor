 #ifndef OPENCVUTILS_H
#define OPENCVUTILS_H

#include <opencv/cv.h>
#include <QImage>
#include <QPixmap>
#include <QList>


namespace OpenCVUtils {

    QImage ToQImage(cv::Mat const &in);
    QPixmap ToQPixmap(const cv::Mat &in);
    cv::Mat ToRGB(cv::Mat const &in);
    cv::Mat FromQImage(const QImage &src);
    cv::Mat FromQPixmap(const QPixmap &src);
    
    /**
     * @brief возвращает соседа под номером num
     *
     * num:
     * +-----+
     * |1|2|3|
     * |8|x|4|
     * |7|6|5|
     * +-----+
     *
     * @param p точка x
     * @param num номер соседа точки
     * @return точка
     */
    cv::Point getNeighboor(cv::Point p, int num);
    
    
    /**
     * @brief возвращает true если точка p попадает в границы изображения in
     */
    bool isInside(cv::Mat &image, cv::Point &p);



}

#endif // OPENCVUTILS_H
