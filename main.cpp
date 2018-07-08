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

    //均衡之前的直方图
    cv::Mat beHist=h.getHistogramImage (image,2);

    //对图像执行直方图均衡化处理
    cv::Mat result;
    cv::equalizeHist (image,result);

    //均衡化之后的直方图
    cv::Mat afHist=h.getHistogramImage (result,2);

    //显示结果
    cv::imshow ("image",image);
    cv::imshow ("beHist",beHist);
    cv::imshow ("result",result);
    cv::imshow ("afHist",afHist);






    cv::waitKey (0);
    return 0;
}




