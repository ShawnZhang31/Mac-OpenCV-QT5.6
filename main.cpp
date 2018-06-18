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
    Mat image=imread ("/Users/zhangxiaomin/Desktop/111.jpeg");
    if(image.empty ())
    {
        cerr<<"文件路径错误!";
        return -1;
    }

    namedWindow ("src",WINDOW_AUTOSIZE);
    imshow ("src",image);
    waitKey (0);
    destroyWindow ("src");
    return 0;



//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
