#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;

/**
  * 鼠标点击的回调事件
  */
void onMouse(int event,int x,int y,int flat,void* Param)
{
    cv::Mat *im=reinterpret_cast<cv::Mat*>(Param);
    switch (event)
    {
        case cv::EVENT_LBUTTONDOWN://鼠标左键按下的事件
        //显示像素的值(x,y)
        std::cout<<"at ("<<x<<","<<y<<") value is:"<<static_cast<int>(im->at<uchar>(cv::Point(x,y)))<<std::endl;
        break;
    }
}

int main(int argc, char *argv[])
{
    Mat image;
    image=imread ("/Users/zhangxiaomin/Works/A02-PrivateThings/Learning/OpenCV/Learning-OpenCV-QT5.6/Images/111.jpeg",IMREAD_GRAYSCALE);
    if(image.empty ())
    {
        cerr<<"文件路径错误!";
        return -1;
    }

    //创建窗口
    namedWindow ("src",WINDOW_AUTOSIZE);
    //显示图像
    imshow ("src",image);

    //注册鼠标点击事件的回调函数
    cv::setMouseCallback ("src",onMouse,reinterpret_cast<void*>(&image));

    //对图像进行水平翻转
    Mat result;
    flip (image,result,1);

    //在翻转后的图像上绘图
    circle(result,//目标图像
           cv::Point(155,110),//中心点
           65,//半径
           0,//颜色（这里使用黑色）
           3);//厚度

    putText (result,//目标图像
             "Test Test",//字体内容
             cv::Point(40,200),//文本位置
             cv::FONT_HERSHEY_PLAIN,//文本类型
             2.0,//字体大小
             255,//字体颜色(这里使用白色)
             2);//字体厚度

    namedWindow ("result",WINDOW_AUTOSIZE);
    //显示图像
    imshow ("result",result);



    waitKey (0);
    destroyWindow ("src");
    return 0;



//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}


