#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<string>
#include<vector>
#include<QFile>
#include<random> //引入随机引擎
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


/**
 * @brief loadFromQrc:从QRC中加载图片资源
 * @param qrc:qrc资源的相对路径
 * @param flag:cv::imread的读入标记
 * @return:cv::Mat
 */
cv::Mat loadFromQrc(QString qrc,int flag=cv::IMREAD_COLOR)
{
    QFile file(qrc);
    cv::Mat ima;
    if(file.open (QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        ima = cv::imdecode(buf, flag);
    }
    return  ima;
}

/**
 * @brief colorReduce:使用迭代器扫描图像
 * @param image
 * @param div
 */
void colorReduce(cv::Mat image,int div=64)
{
    // div必须是2的幂
    int n=static_cast<int>(
                log(static_cast<double>(div))/log(2.0)+0.5);
    //用来截取像素值的掩码
    uchar mask=0xFF<<n; //如果div=16, mask=0xF0
    uchar div2=div>>1; //div2=div/2

    //迭代器
    cv::Mat_<cv::Vec3b>::iterator it=image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend=image.end<cv::Vec3b>();

    //扫面全部像素
    for(;it!=itend;++it)
    {
        (*it)[0] &=mask;
        (*it)[0] +=div2;
        (*it)[1] &=mask;
        (*it)[1] +=div2;
        (*it)[2] &=mask;
        (*it)[2] +=div2;
    }
}

/**
 * @brief sharpen:图像锐化处理
 * @param image:原始图像
 * @param result:锐化处理后的图像
 */
void sharpen(const cv::Mat &image, cv::Mat &result)
{
    //判断是否需要分配图像数据。如果需要，就分配
    result.create (image.size(),image.type ());

    //获取通道数
    int nchannels=image.channels ();

    //处理所有行，除了第一行和最后一行
    for(int j=1;j<image.rows-1;j++)
    {
        const uchar* previous = image.ptr<const uchar>(j-1); //上一行
        const uchar* current = image.ptr<const uchar> (j);  //当前行
        const uchar* next = image.ptr<const uchar> (j+1);   //下一行

        uchar* output = result.ptr<uchar> (j);

        for(int i=nchannels;i<(image.cols-1)*nchannels;i++)
        {
            //应用锐化算子
            *output++=cv::saturate_cast<uchar>(
                        5*current[i]-current[i-nchannels]-
                    current[i+nchannels]-previous[i]-next[i]);

//            std::cout<<*output<<std::endl;
        }
    }

    //把未处理的像素设为0
    result.row (0).setTo (cv::Scalar(0));
    result.row (result.rows-1).setTo (cv::Scalar(0));
    result.col (0).setTo (cv::Scalar(0));
    result.col (result.cols-1).setTo (cv::Scalar(0));
}

void shapped2D(const cv::Mat &image, cv::Mat &result)
{
    //构造内核
    cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));

    //对内核进行赋值
    kernel.at<float>(1,1)=5.0;
    kernel.at<float>(0,1)=-1.0;
    kernel.at<float>(1,0)=-1.0;
    kernel.at<float>(1,2)=-1.0;
    kernel.at<float>(2,1)=-1.0;

    //对图像进行滤波
    cv::filter2D (image,result,image.depth (),kernel);
}

int main(int argc, char *argv[])
{

    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

//    cv::Mat imageColorReduce=image.clone ();
//    const int64 start=cv::getTickCount ();

//    colorReduce (imageColorReduce);
//    double duration=(cv::getTickCount ()-start)/cv::getTickFrequency ();
//    std::cout<<"duration:"<<duration<<std::endl;
//    cv::imshow ("Images",image);
//    cv::imshow ("ColorReduce",imageColorReduce);

    cv::Mat Result;
    cv::Mat filterResult;

    const int64 start_1=cv::getTickCount ();
    sharpen (image,Result);
    double duration_1=(cv::getTickCount ()-start_1)/cv::getTickFrequency ();


    const int64 start_2=cv::getTickCount ();
    shapped2D (image,filterResult);
    double duration_2=(cv::getTickCount ()-start_2)/cv::getTickFrequency ();

    cv::imshow ("Image",image);
    cv::imshow ("Shappen",Result);

    cv::imshow ("Filter2D",filterResult);

    std::cout<<"sharpped cost:"<<duration_1<<std::endl;

    std::cout<<"sharpped2D cost:"<<duration_2<<std::endl;

    cv::waitKey (0);

    return 0;

}




