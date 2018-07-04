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

/**
 * @brief colorReduce:使用迭代器扫描图像
 * @param image
 * @param div
 */
void colorReduce(cv::Mat image,int div=64)
{
    // div必须是2的幂
    int n=static_cast<int>(
                log(static_cast<double>(div))/log(2.0)+0.5);
    //用来截取像素值的掩码
    uchar mask=0xFF<<n; //如果div=16, mask=0xF0
    uchar div2=div>>1; //div2=div/2

    //迭代器
    cv::Mat_<cv::Vec3b>::iterator it=image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend=image.end<cv::Vec3b>();

    //扫面全部像素
    for(;it!=itend;++it)
    {
        (*it)[0] &=mask;
        (*it)[0] +=div2;
        (*it)[1] &=mask;
        (*it)[1] +=div2;
        (*it)[2] &=mask;
        (*it)[2] +=div2;
    }
}

int main(int argc, char *argv[])
{

    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

    cv::Mat imageColorReduce=image.clone ();
    colorReduce (imageColorReduce);
    cv::imshow ("Images",image);
    cv::imshow ("ColorReduce",imageColorReduce);
    cv::waitKey (0);

    return 0;

}




