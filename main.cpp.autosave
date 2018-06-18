#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

//测试函数，创建一幅图像
cv::Mat functionT()
{
    //创建图像
    cv::Mat ima(500,500,CV_8U,50);
    return  ima;
}

int main(int argc, char *argv[])
{
    //创建一个240*240的新图像
    cv::Mat image1(240,240,CV_8U,100);
    cv::imshow ("Image",image1);//显示图像
    cv::waitKey (0);//等待按键

    //重新分配一个图像
    image1.create (200,200,CV_8U);
    image1=200;

    cv::imshow ("Image",image1);//显示图像
    cv::waitKey (0);//等待

    //创建一个红色的图像，通道次序为BGR
    cv::Mat image2(240,320,CV_8SC3,cv::Scalar(0,0,255));
    cv::imshow ("Image",image2);//显示图像
    cv::waitKey (0);//等待

    //读入一幅图像
    cv::Mat image3=cv::imread ("/Users/zhangxiaomin/Desktop/111.jpeg");

    //所有这些图像都指向一个数据块
    cv::Mat image4(image3);
    image1=image3;

    //这些图像都是原图像的副本
    image3.copyTo (image2);
    cv::Mat image5=image3.clone ();

    //转换图像进行测试
    cv::flip (image3,image3,1);

    //检查哪些图像在处理过程中受到了影响
    cv::imshow ("image 3",image3);
    cv::imshow ("image 1",image1);
    cv::imshow ("image 2",image2);
    cv::imshow ("image 4",image4);
    cv::imshow ("image 5",image5);

    cv::waitKey (0);//等待按键;

    //从函数中回去一个灰度图像
    cv::Mat gray=functionT();

    cv::imshow ("Image",gray);
    cv::waitKey (0);//等待按键;

    //作为灰度图像读入
    image1=cv::imread ("/Users/zhangxiaomin/Desktop/111.jpeg",cv::IMREAD_GRAYSCALE);
    image1.convertTo (image2,CV_32F,1/255.0,0.0);

    cv::imshow ("Image",image2);
    cv::waitKey (0);//等待按键;

    return 0;

}


