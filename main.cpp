#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
#include<QFile>
#include<random> //引入随机引擎
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "colordetector.h"

/**
 * @brief loadFromQrc:从QRC中加载图片资源
 * @param qrc:qrc资源的相对路径
 * @param flag:cv::imread的读入标记
 * @return:cv::Mat
 */
cv::Mat loadFromQrc(QString qrc,int flag=cv::IMREAD_COLOR)
{
    QFile file(qrc);
    cv::Mat ima;
    if(file.open (QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        ima = cv::imdecode(buf, flag);
    }
    return  ima;
}


int main(int argc, char *argv[])
{

    //读取输入的图像
    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

    cv::Mat hsv;
    cv::cvtColor (image,hsv,CV_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split (hsv,channels);

    cv::imshow ("channel-0",channels[0]);
    cv::imshow ("channel-1",channels[1]);
    cv::imshow ("channel-2",channels[2]);

    cv::imshow ("hsv",hsv);

    cv::waitKey (0);
    return 0;
}




