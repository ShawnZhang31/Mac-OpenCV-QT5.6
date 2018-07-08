#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H

#include<shawnopencv.h>

class ColorHistogram
{
private:
    int histSize[3];        //每个维度的大小
    float hranges[2];       //像素的取值范围
    const float* ranges[3]; // 每个像素点范围
    int channels[3];        //需要处理的通道
public:
    ColorHistogram();
    cv::Mat getHistogram(const cv::Mat &image);
};

#endif // COLORHISTOGRAM_H
