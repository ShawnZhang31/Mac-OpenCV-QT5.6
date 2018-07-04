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
 * @brief salt:椒盐噪声产生方法
 * @param image:用于生产椒盐的图像
 * @param n:椒盐迭代器的迭代次数
 */
void salt(cv::Mat image, int n)
{
    std::default_random_engine generator;//随机算法
    std::uniform_int_distribution<int> randomRow(0,image.rows-1);//行分布
    std::uniform_int_distribution<int> randomCol(0,image.cols-1);//列分布

    int i,j;//用于存储每个随机值
    for(int k=0;k<n;k++)
    {
        //随机图形的位置
        i=randomRow(generator);
        j=randomCol(generator);

        if(image.type ()==CV_8UC1)//灰度图像
        {
            image.at<uchar>(i,j)=255;
        }
        else if(image.type ()==CV_8UC3)//彩色图像
        {
            image.at<cv::Vec3b>(i,j)[0]=255;
            image.at<cv::Vec3b>(i,j)[1]=255;
            image.at<cv::Vec3b>(i,j)[2]=255;
        }
    }
}

/**
 * @brief reduceColor:降低颜色空间
 * @param image:需要降低的图片
 * @param div:降低的倍数
 */
void reduceColor(cv::Mat image,int div=64)
{
    int nl=image.rows;//行数
    int nc=image.cols*image.channels ();//每行元素的数量
    for(int i=0;i<nl;i++)
    {
        //取得i行的地址
        uchar* data=image.ptr (i);
        for(int j=0;j<nc;j++)
        {
          data[j]=data[j]/div*div+div/2;//每个元素
        }//一行元素处理完成
    }
}


/**
 * @brief colorReduce:使用连续图像的高效扫描法处理
 * @param image
 * @param div
 */
void colorReduce(cv::Mat image,int div=64)
{
//    int nl=image.rows;//行数

//    //每行元素的总数
//    int nc=image.cols*image.channels ();

    if(image.isContinuous ())
    {
        //图像是连续的
//        nc=nc*nl;
//        nl=1;//装换程一个一维数组
        image.reshape (1,//新的通道数
                       1);//新的函数
    }

        int nl=image.rows;//行数

        //每行元素的总数
        int nc=image.cols*image.channels ();

    int n=static_cast<int>(log(static_cast<double>(div))/log(2.0)+0.5);

    //用来截取像素的掩码
    uchar mask=0xFF<<n;//如果div=16，那么mask=0xF0
    uchar div2=div>>1;//div2=div/2

    //对于连续图像，这个循环只执行一次
    for(int j=0;j<nl;j++)
    {
        uchar* data=image.ptr<uchar>(j);
        for(int i=0;i<nc;i++)
        {
            *data &=mask;
            *data++ +=div2;
        }//一行结束
    }
}

int main(int argc, char *argv[])
{

    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");
    std::cout<<"image step="<<image.step<<std::endl;
    std::cout<<"image elemSize="<<image.elemSize ()<<std::endl;
    std::cout<<"image nChannels="<<image.channels ()<<std::endl;
    if(image.isContinuous ())
    {
        std::cout<<"image is continuous"<<std::endl;
    }
    else
    {
        std::cout<<"image is not continuous"<<std::endl;
    }
//    salt(image,3000);

    cv::Mat imageReduceColor=image.clone ();
    cv::Mat imageColorReduce=image.clone ();
    reduceColor (imageReduceColor);
    colorReduce (imageColorReduce);
    cv::imshow ("Images",image);
    cv::imshow ("reduceColor",imageReduceColor);
    cv::imshow ("ColorReduce",imageColorReduce);
    cv::waitKey (0);

    return 0;

}




