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

    //计算直方图
    cv::Mat histo=h.getHistogram (image);

    //循环遍历每个箱子
    int sum=0;
    for(int i=0;i<256;i++)
    {
        cout<<"Value-"<<i<<" = "<<histo.at<float>(i)<<endl;
        sum+=i;
    }
    cout<<"Total Pixels Count="<<sum<<endl;

    cv::imshow ("image",image);
    cv::imshow ("Histogram",h.getHistogramImage (image,3));

    cv::Mat thresholded;
    cv::threshold (image,thresholded,120,255,cv::THRESH_TRUNC);
    cv::imshow ("Threshold",thresholded);




    cv::waitKey (0);
    return 0;
}




