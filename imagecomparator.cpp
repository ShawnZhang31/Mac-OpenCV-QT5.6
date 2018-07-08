#include "imagecomparator.h"

ImageComparator::ImageComparator():nBins(8)
{

}

/**
 * @brief ImageComparator::setReferenceImage,设置并计算基准图像的直方图
 * @param image
 */
void ImageComparator::setReferenceImage(const cv::Mat& image)
{
    hist.setSize (nBins);
    refH=hist.getHistogram (image);
}

/**
 * @brief ImageComparator::compare,使用交叉法比较BGR图像的相似度
 * @param image
 * @return
 */
double ImageComparator::compare(const cv::Mat& image)
{
    inputH=hist.getHistogram (image);

    //用交叉法比较直方图
    return cv::compareHist (refH,inputH,cv::HISTCMP_INTERSECT);
}

