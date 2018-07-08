#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
#include<QFile>
#include<random> //引入随机引擎

#include<shawnopencv.h>
#include<loadimagefromqrc.h>
#include<histogram1d.h>

using namespace std;



int main(int argc, char *argv[])
{

    //读取输入的图像,以黑白形式打开
    LoadImageFromQrc qrcImRead;
    cv::Mat image=qrcImRead(":/test/testImages/Chapter02/111.jpeg",cv::IMREAD_GRAYSCALE);

    //直方图对象
    Histogram1D h;

    //创建一个图像反转的查找表
    cv::Mat lut(1,256,CV_8U);

    for(int i=0;i<256;i++)
    {
        lut.at<uchar>(i)=255-i;
    }

    cv::imshow ("LUT",h.applyLookUp (image,lut));




    cv::waitKey (0);
    return 0;
}




