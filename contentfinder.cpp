#include "contentfinder.h"



void ContentFinder::setThreshold(float value)
{
    threshold = value;
}

void ContentFinder::setHistogram(const cv::Mat &value)
{
    histogram = value;

    cv::normalize(histogram,histogram,1.0);
}

ContentFinder::ContentFinder()
{
    threshold=0.1f;
    ranges[0]=hranges;
    ranges[1]=hranges;
    ranges[2]=hranges;
}

float ContentFinder::getThreshold() const
{
    return threshold;
}

/**
 * @brief ContentFinder::find
 * @param image
 * @return
 */
cv::Mat ContentFinder::find (const cv::Mat& image)
{
    hranges[0]=0.0;
    hranges[1]=256.0;
    channels[0]=0;
    channels[1]=1;
    channels[2]=2;

    return find(image,hranges[0],hranges[1],channels);
}

/**
 * @brief ContentFinder::find
 * @param image
 * @param minValue
 * @param maxValue
 * @param channels
 * @return
 */
cv::Mat ContentFinder::find(const cv::Mat& image, float minValue, float maxValue, int *channels)
{
    cv::Mat result;
    hranges[0]=minValue;
    hranges[1]=maxValue;

    //直方图的维度数与通道列表一致
    for(int i=0;i<histogram.dims;i++)
    {
        this->channels[i]=channels[i];
    }

    cv::calcBackProject (&image, 1,
                         channels,
                         histogram,
                         result,
                         ranges,
                         255.0);

    //对方向投影结果做阈值化，得到二值图像
    if(threshold>0.0)
    {
        cv::threshold (result,result,255.0*threshold,255.0,cv::THRESH_BINARY);
    }

    return result;
}
