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

    cv::Mat image1=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");
    cv::Mat image2=loadFromQrc (":/test/testImages/Chapter02/timg.jpg");

    cv::Mat result;
    cv::addWeighted (image1,0.7,image2,0.9,0.0,result);

    cv::imshow ("add",result);



    cv::waitKey (0);

    return 0;

}




