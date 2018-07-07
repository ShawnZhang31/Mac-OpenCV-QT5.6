#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class ColorDetector
{
private:
// 允许的最小差距
int maxDist;
// 目标颜色
cv::Vec3b target;
// 处理后的结果
cv::Mat result;

int getDistanceToTargetColor(const cv::Vec3b &color) const;
int getColorDistance(const cv::Vec3b &color1, const cv::Vec3b &color2) const;
public:
    ColorDetector();
    /**
     * @brief process:对图像执行颜色提取
     * @param image:要进行处理的图像
     */
    cv::Mat process(const cv::Mat &image);

    int getMaxDist() const;
    void setMaxDist(int value);
    cv::Vec3b getTarget() const;

    /**
     * @brief ColorDetector::setTarget,设置目标颜色
     * @param blue,B通道
     * @param green,G通道
     * @param red,R通道
     */
    void setTarget(uchar blue, uchar green, uchar red);

    ColorDetector(uchar blue, uchar green, uchar red, int distance);
    cv::Mat processWithFloodFill(const cv::Mat &image, cv::Point startPoint);
};

#endif // COLORDETECTOR_H
