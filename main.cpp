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

#define IMAGE_DIR "/Users/zhangxiaomin/Works/A02-PrivateThings/Learning/OpenCV/images"


int main(int argc, char *argv[])
{
/****************************************************************
//Chapter4-5扩展内容
    // 装载彩色图像
    ColorHistogram hc;


    //读取输入的图像,以黑白形式打开
    QString fileDir=IMAGE_DIR;
    QString fileName=fileDir+"/waves.jpg";

    cv::Mat image=cv::imread (fileName.toStdString ());

    //提取ROI
    cv::Mat imageROI=image(cv::Rect(0,0,100,45));

    //获取3D颜色直方图
    hc.setSize (8);
    cv::Mat shits=hc.getHistogram (imageROI);

    //创建搜索容器
    ContentFinder finder;
    //设置用来反向投影的直方图
    finder.setHistogram (shits);
    finder.setThreshold (0.05f);

    //取得颜色直方图的反射投影
    cv::Mat result=finder.find(image);

    cv::imshow ("imageROI",result);
*******************************************************************/

 /***************************Chapter4-6 用均值平移算法查找目标**************************************/
//    //装载参考图像
//    QString fileDir=IMAGE_DIR;
//    QString fileName=fileDir+"/baboon01.jpg";

//    cv::Mat image=cv::imread (fileName.toStdString ());

//    //获取狒狒两部的ROI
//    cv::Rect rect(110,45,35,45);
//    cv::Mat imageROI=image(rect);



//    //得到狒狒脸部的直方图
//    int minSat=65;
//    ColorHistogram hc;
//    cv::Mat colorHist=hc.getHueHistogram (imageROI,minSat);

//    ContentFinder finder;
//    finder.setHistogram (colorHist);

//    //打开第二张图
//    QString file2=fileDir+"/baboon02.jpg";
//    image=cv::imread (file2.toStdString ());

//    //装换程HSV色彩空间
//    cv::Mat hsv;
//    cv::cvtColor (image,hsv,CV_BGR2HSV);

//    //得到色调直方图的方向投影
//    int ch[1]={0};
//    finder.setThreshold (-1.0f);    //不做阈值化处理
//    cv::Mat result=finder.find (hsv,0.0f,180.0f,ch);

//    //窗口的初始位置

//    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
//                              10,
//                              1);

//    cv::meanShift(result,rect,criteria);//#include<opencv2/video/tracking.hpp>

//    cv::rectangle (image,rect,cv::Scalar(0,0,255));

//    cv::imshow ("image",image);



 /***************************Chapter4-6 用均值平移算法查找目标**************************************/

 /***************************Chapter4-8*****************************************/
//    //装载参考图像
//    QString fileDir=IMAGE_DIR;
//    QString fileName=fileDir+"/bike55.bmp";
//    cv::Mat image=cv::imread (fileName.toStdString (),0);

//    int xo=97,yo=112,width=25,height=30;
//    cv::Rect rectROI=cv::Rect(xo,yo,width,height);
//    cv::rectangle (image,rectROI,cv::Scalar(0,0,255));
//    cv::Mat imageROI=image(rectROI);

//    //计算累加值
//    //返回一个多通道图像下的Scalar数值
//    cv::Scalar sum=cv::sum (imageROI);
//    cout<<"sum="<<sum<<endl;

//    //计算积分图像
//    cv::Mat integralImage;
//    cv::integral (image,integralImage,CV_32S);

//    cv::imshow ("image",integralImage);

//    int sumInt = integralImage.at<int>(yo+height,xo+width)-
//                 integralImage.at<int>(yo+height,xo)-
//                 integralImage.at<int>(yo,xo+width)+
//                 integralImage.at<int>(yo,xo);

//    cout<<"sumInt="<<sumInt<<endl;
/***************************Chapter4-8*****************************************/

/***************************Chapter4-8：1.自适应的阈值化*****************************************/
    QString fileDir=IMAGE_DIR;
    QString fileName=fileDir+"/book.jpg";

    cv::Mat book = cv::imread (fileName.toStdString (),0);

    //使用固定阈值
    cv::Mat binnaryFixed;
    cv::threshold (book,binnaryFixed,70,255,cv::THRESH_BINARY);

    //计算积分图像
    cv::Mat ibook;
    cv::integral (book,ibook,CV_32S);

    cv::Mat adaptiveImg;
    cv::adaptiveThreshold (book,adaptiveImg,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,21,10);

    cv::imshow ("book",book);
    cv::imshow ("binaryFixed",binnaryFixed);
    cv::imshow ("adaptiveImg",adaptiveImg);


/***************************Chapter4-8：1.自适应的阈值化*****************************************/

    cv::waitKey (0);
    return 0;
}




