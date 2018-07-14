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
#include<watershedsegmenter.h>

using namespace std;

#define IMAGE_DIR "/Users/zhangxiaomin/Works/A02-PrivateThings/Learning/OpenCV/images/"


int main(int argc, char *argv[])
{
    QString fileNameOri="group.jpg";
    QString filePathOri=IMAGE_DIR+fileNameOri;

    cv::Mat origin=cv::imread (filePathOri.toStdString ());

    QString fileName="binary.bmp";
    QString filePath=IMAGE_DIR+fileName;

    cv::Mat image=cv::imread (filePath.toStdString (),0);

    //消除噪声和细小物体
    cv::Mat fb;
    cv::erode (image,fb,cv::Mat(),cv::Point(-1,-1),4);

    //标记不含物体的像素
    cv::Mat bg;
    cv::dilate (image,bg,cv::Mat(),cv::Point(-1,-1),4);
    cv::threshold (bg,bg,1,128,cv::THRESH_BINARY_INV);

    //创建标记图像
    cv::Mat markers(image.size(),CV_8U,cv::Scalar(0));
    markers=fb+bg;

    WatershedSegmenter segmenter;

    //设置标记图像，然后执行分割过程
    segmenter.setMarkers (markers);
    segmenter.process (origin);

    //创建分水岭分割类的对象

    cv::imshow ("Segmentation",segmenter.getSegmentation ());

    cv::imshow ("Watersheds",segmenter.getWatersheds ());



    QString towerFileName="tower.jpg";
    cv::Mat tower=cv::imread (((QString)(IMAGE_DIR+towerFileName)).toStdString ());

    //标识背景元素
    cv::Mat imageMask(tower.size(),CV_8U,cv::Scalar(0));
    cv::rectangle (imageMask,cv::Point(5,5),cv::Point(tower.cols-5,tower.rows-5),cv::Scalar(255),3);

    //标识前景元素
    cv::rectangle (imageMask,cv::Point(tower.cols/2-10,tower.rows/2-10),
                   cv::Point(tower.cols/2+10,tower.rows/2+10),
                   cv::Scalar(1),10);

    segmenter.setMarkers (imageMask);
    segmenter.process (tower);



    cv::imshow ("tower",segmenter.getWatersheds ());





    cv::waitKey (0);
    return 0;
}




