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

    //闭合图像
    cv::Mat element5(5,5,CV_8U,cv::Scalar(1));
    cv::Mat closed;
    cv::morphologyEx (image,closed,cv::MORPH_CLOSE,element5);

    //开图像
    cv::Mat opened;
    cv::morphologyEx (image,opened,cv::MORPH_OPEN,element5);

    cv::imshow ("image",image);
    cv::imshow ("Closed",closed);

    cv::imshow ("Opened",opened);


    cv::waitKey (0);
    return 0;
}




