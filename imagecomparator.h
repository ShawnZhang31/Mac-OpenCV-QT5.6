#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H
#include<shawnopencv.h>
#include<colorhistogram.h>

class ImageComparator
{
private:
    cv::Mat refH;   // 基准直方图
    cv::Mat inputH; // 输入图像的直方图
    ColorHistogram hist;    //生成直方图
    int nBins;              // 每个颜色通道使用的箱子数量
public:
    ImageComparator();
    void setReferenceImage(const cv::Mat &image);
    double compare(const cv::Mat &image);
};

#endif // IMAGECOMPARATOR_H
