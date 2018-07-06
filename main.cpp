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

void wave(const cv::Mat &image, cv::Mat &result)
{
    //映射参数
    cv::Mat srcX(image.rows,image.cols,CV_32F);
    cv::Mat srcY(image.rows,image.cols,CV_32F);

    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            //(i,j)像素的位置
            srcX.at<float>(i,j)=j;//保持在同一列
            srcY.at<float>(i,j)=i+5*sin(j/10.0);//原来在第i行修昂素，现在根据一个正玄曲线移动;

        }
    }

    cv::remap (image,result,srcX,srcY,cv::INTER_LINEAR);
}



int main(int argc, char *argv[])
{

    cv::Mat image1=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

    cv::Mat result;
    wave(image1,result);
    cv::imshow ("remap",result);




    cv::waitKey (0);

    return 0;

}




