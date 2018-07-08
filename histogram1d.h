#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include<shawnopencv.h>

class Histogram1D
{
private:
    int histSize[1];        //直方图中箱子的数量
    float hranges[2];       //值范围
    const float* ranges[1]; //值范围的指针
    int channels[1];        //要检查的通道数量
    static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom);
public:
    Histogram1D();
    cv::Mat getHistogram(const cv::Mat &image);
    cv::Mat getHistogramImage(const cv::Mat &image, int zoom = 1);
    cv::Mat applyLookUp(const cv::Mat &image, const cv::Mat &lookup);
};

#endif // HISTOGRAM1D_H
