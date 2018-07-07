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

#include "colordetector.h"

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


int main(int argc, char *argv[])
{

    //读取输入的图像
    cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

    cv::Rect rectangle(300,300,400,400);
    cv::Mat result; //分割结果
    cv::Mat bgModel,fgModel;    // 模型

    //GrabCut分割算法
    cv::grabCut (image,                     // 输入图像
                 result,                    //分割结果
                 rectangle,                 //包含前景的矩形
                 bgModel,fgModel,           //模型
                 5,                         //迭代次数
                 cv::GC_INIT_WITH_RECT);    //使用矩形

    //取得标记为可能属于前景的像素
    cv::compare (result,cv::GC_PR_FGD,result,cv::CMP_EQ);

    //生成输出图像
    cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
    image.copyTo (foreground,result);

    cv::imshow ("image",image);
    cv::imshow ("result",result);

    cv::waitKey (0);
    return 0;

}




