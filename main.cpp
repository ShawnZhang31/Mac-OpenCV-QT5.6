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

    //创建感兴趣区域
    cv::Mat imageROI;
    imageROI=image(cv::Rect(350,100,420,200));

    cv::Mat hist=h.getHistogram (imageROI);

    cv::normalize(hist,hist,1.0);

    cv::Mat result=h.applyBackProject (image,hist);


    cv::threshold (result,result,50,255,cv::THRESH_BINARY);

    cv::imshow ("result",result);

    cv::waitKey (0);
    return 0;
}




