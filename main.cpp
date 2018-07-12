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
#include<colorhistogram.h>
#include<contentfinder.h>
#include<imagecomparator.h>

using namespace std;

#define IMAGE_DIR "/Users/zhangxiaomin/Works/A02-PrivateThings/Learning/OpenCV/images/"


int main(int argc, char *argv[])
{
    QString fileName="boldt.jpg";
    QString filePath=IMAGE_DIR+fileName;

    cv::Mat image=cv::imread (filePath.toStdString (),0);

    //梯度运算
    cv::Mat result;
    cv::morphologyEx (image, result, cv::MORPH_GRADIENT,cv::Mat());

    cv::imshow ("GRADIENT",~result);

    //顶帽运算
    cv::Mat topHat;
    cv::morphologyEx (image, topHat, cv::MORPH_BLACKHAT, cv::Mat());
    cv::imshow ("TOPHAT",topHat);


    cv::waitKey (0);
    return 0;
}




