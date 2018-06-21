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

int main(int argc, char *argv[])
{

    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");
    salt(image,3000);
    cv::imshow ("Images",image);
    cv::waitKey (0);

    return 0;

}




