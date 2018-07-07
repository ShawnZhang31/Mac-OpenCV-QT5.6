#include "colordetector.h"

/**
 * @brief ColorDetector::getMaxDist
 * @return
 */
int ColorDetector::getMaxDist() const
{
    return maxDist;
}

/**
 * @brief ColorDetector::setMaxDist
 * @param value
 */
void ColorDetector::setMaxDist(int value)
{
    if(value<=0)
        maxDist=0;
    maxDist = value;
}

/**
 * @brief ColorDetector::getTarget
 * @return
 */
cv::Vec3b ColorDetector::getTarget() const
{
    return target;
}

/**
 * @brief ColorDetector::setTarget,设置目标颜色
 * @param blue,B通道
 * @param green,G通道
 * @param red,R通道
 */
void ColorDetector::setTarget(uchar blue, uchar green, uchar red)
{
    target = cv::Vec3b(blue,green,red);
}

/**
 * @brief ColorDetector::process
 * @param image
 * @return
 */
cv::Mat ColorDetector::process (const cv::Mat &image)
{
    result.create (image.size(),CV_8U);

    cv::Mat_<cv::Vec3b>::const_iterator it=image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend=image.end<cv::Vec3b>();
    cv::Mat_<uchar>::iterator itout=result.begin<uchar>();

    for(;it!=itend;++it,++itout)
    {
        //比较与目标颜色的差距
        if(getDistanceToTargetColor (*it)<=maxDist)
        {
            *itout=255;
        }
        else
        {
            *itout=0;
        }
    }

    return result;
}

/**
 * @brief ColorDetector::processWithFloodFill
 * @param image
 * @param startPoint
 * @return
 */
cv::Mat ColorDetector::processWithFloodFill (const cv::Mat &image,cv::Point startPoint)
{
    result = image.clone ();
    cv::floodFill (result,
                   startPoint,
                   cv::Scalar(255,255,255),
                   (cv::Rect*)0,
                   cv::Scalar(35,35,35),
                   cv::Scalar(35,35,35),
                   cv::FLOODFILL_FIXED_RANGE);

    return result;
}


/**
 * @brief ColorDetector::getDistanceToTargetColor,获取与目标像素的差距
 * @param color
 * @return
 */
int ColorDetector::getDistanceToTargetColor(const cv::Vec3b& color) const
{
    return getColorDistance (color,target);
}


/**
 * @brief ColorDetector::getColorDistance
 * @param color1
 * @param color2
 * @return
 */
int ColorDetector::getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const
{
    return abs(color1[0]-color2[0])+
           abs(color1[1]-color2[1])+
           abs(color1[2]-color2[2]);
}

/**
 * @brief ColorDetector::ColorDetector
 */
ColorDetector::ColorDetector():maxDist(0),target(0,0,0)
{
    maxDist=0;
    target=cv::Vec3b(0,0,0);
}

/**
 * @brief ColorDetector::ColorDetector
 * @param blue
 * @param green
 * @param red
 * @param distance
 */
ColorDetector::ColorDetector(uchar blue, uchar green, uchar red, int distance)
{
    if(distance<=0)
        distance=0;
    maxDist=distance;

    target=cv::Vec3b(blue,green,red);

}


