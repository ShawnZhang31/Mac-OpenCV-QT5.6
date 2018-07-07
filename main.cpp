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

    //1. 创建图像处理器对象,并指定参数
    ColorDetector cdetect(50,224,254,200);

    //2. 读取输入的图像
    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

    //3. 处理并显示结果
    cv::namedWindow ("result");
//    cv::Mat result=cdetect.process (image);
    cv::Mat result=cdetect.processWithFloodFill (image,cv::Point(200,100));
    cv::imshow ("result",result);

    cv::waitKey (0);
    return 0;

}




