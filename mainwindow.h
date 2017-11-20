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

public:
    Mat testImage;
    Mat destImage;

    //私有方法
private:
    //使用openCV的图像来设置lable，缩放
    void setLablePixmapWithMat(QLabel *lable,Mat image);
    //使用openCV的图像来设置lable，不缩放
    void setLablePixmapWithMatNoScale(QLabel *lable,Mat image);
    //显示错误提示框
    void showWarningMessageBox(QString title,QString text);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
