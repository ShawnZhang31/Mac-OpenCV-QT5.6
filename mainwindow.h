#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>

#include<iostream>
#include<string>
#include<vector>
using namespace std;

#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Open_TestImage_triggered();

    void on_action_Restore_triggered();

    void on_actionClear_triggered();

    void on_actionExit_triggered();


    void on_actionOpenCustomFile_triggered();

public:
    Mat testImage;//测试文件
    Mat destImage;//目标文件
    Mat customImage;//自定义文件

    float testFeature[25];//测试图像特征
    cv::Mat srcImage[10]; //用于对比的标准图像
    float srcFeature[10][25];   //标准图像的特征



    //私有方法
private:
    //使用openCV的图像来设置lable，缩放,仅仅适用于RGB图像
    void setLablePixmapWithMat(QLabel *lable,Mat image);
    //使用openCV的图像来设置lable，不缩放，仅仅适用于RGB图像
    void setLablePixmapWithMatNoScale(QLabel *lable,Mat image);

    //使用openCV的图像来设置lable，缩放,仅仅适用于GRAY图像
    void setLablePixmapWithGreyMat(QLabel *lable,Mat image);
    //使用openCV的图像来设置lable，不缩放，仅仅适用于GRAY图像
    void setLablePixmapWithGreyMatNoScale(QLabel *lable,Mat image);

    //提取图像特征
    void getFeature(cv::Mat m,float a[25]);
    //计算欧氏距离
    float ouDistance(float a[25],float b[25]);
    //数字识别
    int getResultNumber();


    //显示错误提示框
    void showWarningMessageBox(QString title,QString text);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
