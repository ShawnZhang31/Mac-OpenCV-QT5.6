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

int main(int argc, char *argv[])
{
    Mat hairImage=imread ("/Users/zhangxiaomin/Desktop/hairColor/face8_dilution.png");
    cv::cvtColor (hairImage,hairImage,CV_BGR2HLS);
    Mat colorImage=imread ("/Users/zhangxiaomin/Desktop/hairColor/front.jpg");
    cv::cvtColor (colorImage,colorImage,CV_BGR2HLS);

    vector<Mat> hairChannels;
    vector<Mat> colorChannels;

    cv::split (hairImage,hairChannels);
    cv::split (colorImage,colorChannels);

    Mat mergeImage;
    vector<Mat> mergeChannels;
    cv::split (hairImage,mergeChannels);
    //HLS
    mergeChannels.at (0)=colorChannels.at (0);
    mergeChannels.at (1)=hairChannels.at (1)+colorChannels.at (1)*0.3;
    mergeChannels.at(2)=colorChannels.at (2)*0.2;
//  cvAddWeighted (&colorChannels.at (2),0.2,&colorChannels.at (2),0.0,0.0,&mergeChannels.at(2));

/*测试代码
    //查看原数据是否有效
    namedWindow ("src",WINDOW_AUTOSIZE);
    imshow ("src",colorChannels.at (2));

    //数据转化及拷贝mat==》cvmat
    CvMat tmp1=colorChannels.at (2);
    CvMat *tmp2=cvCreateMat (tmp1.rows,tmp1.cols,tmp1.type);
    cvCopy(&tmp1,tmp2);

    namedWindow ("copy",WINDOW_AUTOSIZE);
    cvShowImage ("copy",tmp2);

    //矩阵乘以常数
    cvConvertScale(tmp2,tmp2,0.2,0.0);

    namedWindow ("convertscale",WINDOW_AUTOSIZE);
    cvShowImage ("convertscale",tmp2);

    //数据转换回mat类型
    mergeChannels.at(2)=cvarrToMat(tmp2);

    namedWindow ("result",WINDOW_AUTOSIZE);
    imshow ("result",mergeChannels.at(2));
 */
    //H通道变换
    mergeChannels.at (0)=colorChannels.at (0);

    //L通道变换
    CvMat tmp1=colorChannels.at (1);
    CvMat *tmp2=cvCreateMat (tmp1.rows,tmp1.cols,tmp1.type);
    cvCopy(&tmp1,tmp2);
    cvConvertScale(tmp2,tmp2,0.3,0.0);      //修改第3个参数
    mergeChannels.at(1)=hairChannels.at (1)+cvarrToMat(tmp2);

    //S通道变换
    tmp1=colorChannels.at (2);
    tmp2=cvCreateMat (tmp1.rows,tmp1.cols,tmp1.type);
    cvCopy(&tmp1,tmp2);
    cvConvertScale(tmp2,tmp2,1.0,0.0);      //修改第3个参数
    mergeChannels.at(2)=cvarrToMat(tmp2);

    //合并通道
    cv::merge (mergeChannels,mergeImage);
    cv::cvtColor (mergeImage,mergeImage,CV_HLS2BGR);

    //显示图像
    namedWindow ("src",WINDOW_AUTOSIZE);
    imshow ("src",mergeImage);
    waitKey (0);
    destroyWindow ("src");
    return 0;






    //HSV
//   mergeChannels.at (0)=colorChannels.at (0);
//   mergeChannels.at (1)=hairChannels.at (1);
//   mergeChannels.at(2)=colorChannels.at (2);


    //mergeImage.convertTo (mergeImage,mergeImage.type (),1,1.5);




//    namedWindow ("src",WINDOW_AUTOSIZE);
//    imshow ("src",mergeImage);
//    waitKey (0);
//    destroyWindow ("src");
//    return 0;



//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
