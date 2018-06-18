#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


int main(int argc, char *argv[])
{
    //载入图片
    cv::Mat image=cv::imread ("/Users/zhangxiaomin/Desktop/111.jpeg");
    //载入logo
    cv::Mat logo=cv::imread ("/Users/zhangxiaomin/Desktop/logo.png");

    cv::imshow ("imageROI",image);
    cv::waitKey (0);

    //定义ROI
    cv::Mat imageROI(image,cv::Rect(image.cols-logo.cols,
                                    image.rows-logo.rows,
                                    logo.cols,logo.rows));

    logo.copyTo (imageROI);

    cv::imshow ("imageROI",image);
    cv::waitKey (0);

    return 0;

}


