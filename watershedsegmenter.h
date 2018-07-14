#ifndef WATERSHEDSEGMENTER_H
#define WATERSHEDSEGMENTER_H

#include<shawnopencv.h>

class WatershedSegmenter
{
private:
    cv::Mat markers;

public:
    WatershedSegmenter();

    void setMarkers(const cv::Mat& markerImage)
    {
        //转换成整数型图像
        markerImage.convertTo(markers,CV_32S);
    }

    cv::Mat process(const cv::Mat &image)
    {
        //应用分水岭算法
        cv::watershed (image,markers);
        return  markers;
    }

    cv::Mat getSegmentation()
    {
        cv::Mat tmp;
        markers.convertTo (tmp,CV_8U);

        return tmp;
    }

    cv::Mat getWatersheds()
    {
        cv::Mat tmp;

        markers.convertTo (tmp,CV_8U,255,255);

        return tmp;
    }

};

#endif // WATERSHEDSEGMENTER_H
