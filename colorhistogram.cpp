#include "colorhistogram.h"

ColorHistogram::ColorHistogram()
{
    histSize[0]=histSize[1]=histSize[2]=256;
    hranges[0]=0.0;
    hranges[1]=256.0;
    ranges[0]=hranges;
    ranges[1]=hranges;
    ranges[2]=hranges;
    channels[0]=0;
    channels[1]=1;
    channels[2]=2;
}

/**
 * @brief ColorHistogram::getHistogram
 * @param image
 * @return
 */
cv::Mat ColorHistogram::getHistogram(const cv::Mat &image)
{
    cv::Mat hist;

    hranges[0]=0.0;
    hranges[1]=256.0;
    channels[0]=0;
    channels[1]=1;
    channels[2]=2;

    cv::calcHist (&image,1,
                  channels,
                  cv::Mat(),
                  hist,
                  3,
                  histSize,
                  ranges);

    return  hist;
}


cv::Mat ColorHistogram::getHueHistogram(const cv::Mat &image, int minSaturation)
{
    cv::Mat hist;

    //装换成HSV色彩空间
    cv::Mat hsv;
    cv::cvtColor (image, hsv, CV_BGR2HSV);
//    cv::imshow ("hsv1",hsv);

    //掩码
    cv::Mat mask;
    //根据需要创建掩码
    if(minSaturation>0)
    {
        //将3个通道分割进3副图像
        std::vector<cv::Mat> v;
        cv::split (hsv,v);

        //屏蔽低饱和度的像素
        cv::threshold (v[1],mask,minSaturation,255,cv::THRESH_BINARY);
    }

//    cv::imshow ("hsv2",hsv);

    //准备一维色调直方图的参数
    hranges[0]=0.0;
    hranges[1]=180.0;
    channels[0]=0;

    //计算直方图
    cv::calcHist (&hsv,1,
                  channels,
                  mask,
                  hist,
                  1,
                  histSize,
                  ranges);

    return hist;
}

/**
 * @brief ColorHistogram::setSize
 * @param size
 */
void ColorHistogram::setSize(int size)
{
    histSize[0]=histSize[1]=histSize[2]=size;
}
