#include "histogram1d.h"

Histogram1D::Histogram1D()
{
    //准备一维直方图的默认参数
    histSize[0]=256;    //256个箱子
    hranges[0]=0.0;     //从0开始（包含）
    hranges[1]=256.0;   //到256(不包含)
    ranges[0]=hranges;
    channels[0]=0;      //先关注通道0
}

/**
 * @brief Histogram1D::getHistogram,获取一维直方图
 * @param image
 * @return
 */
cv::Mat Histogram1D::getHistogram(const cv::Mat &image)
{
    cv::Mat hist;
    cv::calcHist (&image,1,         //仅为一幅图像的直方图
                  channels,         //使用通道
                  cv::Mat(),        //不使用掩码
                  hist,             //作为结果的直方图
                  1,                //只是一维的直方图
                  histSize,         //箱子的数量
                  ranges            //像素的范围
     );

    return hist;
}

/**
 * @brief Histogram1D::getHistogramImage
 * @param image
 * @param zoom
 * @return
 */
cv::Mat Histogram1D::getHistogramImage(const cv::Mat &image, int zoom)
{
    //先计算直方图
    cv::Mat hist=getHistogram (image);

    //创建图像
    return getImageOfHistogram (hist,zoom);
}

cv::Mat Histogram1D::getImageOfHistogram(const cv::Mat &hist, int zoom)
{
    //取证箱子的最大值和最小值
    double maxVal=0;
    double minVal=0;

    cv::minMaxLoc (hist,&minVal,&maxVal,0,0);

    //取得直方图的大小
    int histSize=hist.rows;

    //由于显示直方图的方形图像
    cv::Mat histImg(histSize*zoom,histSize*zoom,CV_8U,cv::Scalar(255));

    //设置最高点为90%（即图像的高度）的箱子数量
    int hpt=static_cast<int>(0.9*histSize);

    //为每个箱子画垂直线
    for(int h=0;h<histSize;h++)
    {
        float binVal=hist.at<float>(h);
        if(binVal>0)
        {
            int intensity=static_cast<int>(binVal*hpt/maxVal);
            cv::line (histImg,cv::Point(h*zoom,histSize*zoom),
                              cv::Point(h*zoom,(histSize-intensity)*zoom),
                              cv::Scalar(0),zoom);
        }
    }

    return histImg;
}

/**
 * @brief Histogram1D::applyLookUp
 * @param image
 * @param lookup
 * @return
 */
cv::Mat Histogram1D::applyLookUp(const cv::Mat& image, const cv::Mat& lookup)
{
    //输出图像
    cv::Mat result;

    //应用查找表
    cv::LUT (image,lookup,result);

    return result;
}


