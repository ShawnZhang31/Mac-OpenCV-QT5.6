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
    QString fileName="binary.bmp";
    QString filePath=IMAGE_DIR+fileName;

    cv::Mat image=cv::imread (filePath.toStdString (),0);

    cv::Mat kernel=cv::Mat();//核-只能使用单通道

    //腐蚀图像
    cv::Mat eroded;//目标图像
    cv::erode (image,eroded,kernel);

    cv::Mat fanImage=~image;

    //膨胀图像
    cv::Mat dilated;    //目标图像
    cv::dilate (image,dilated,kernel);

    cv::imshow ("Image",image);
    cv::imshow ("Eroded Image",eroded);

    cv::Mat fanDilated;
    cv::dilate (fanImage,fanDilated,cv::Mat());

    cv::imshow ("fan Image",~fanDilated);

    cv::imshow ("Dilated Image",dilated);


    cv::waitKey (0);
    return 0;
}




