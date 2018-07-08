#ifndef CONTENTFINDER_H
#define CONTENTFINDER_H

#include<shawnopencv.h>

class ContentFinder
{
private:
    //直方图参数
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    float threshold;
    cv::Mat histogram;
public:
    /**
     * @brief ContentFinder
     */
    ContentFinder();
    float getThreshold() const;
    void setThreshold(float value);
    void setHistogram(const cv::Mat &value);
    cv::Mat find(const cv::Mat &image, float minValue, float maxValue, int *channels);
    cv::Mat find(const cv::Mat &image);
};

#endif // CONTENTFINDER_H
