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

    void on_actionHistEqualize_triggered();

    void on_actionContrast_triggered();

    void on_actionLight_triggered();

    void on_actionContrastAndLight_triggered();

    void on_action_filter2D1_triggered();

    void on_action_filter2D2_triggered();

    void on_action_filter2D3_triggered();

    void on_action_filter2D4_triggered();

    void on_action_filter2D5_triggered();

    void on_action_filter2D6_triggered();

    void on_actionTM_SQDIFF_triggered();

    void on_actionTM_SQDIFF_NORMED_triggered();

    void on_actionTM_CCORR_triggered();

    void on_actionTM_CCORR_NORMED_triggered();

    void on_actionTM_CCOFF_triggered();

    void on_actionTM_CCOEFF_NORMED_triggered();

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
    //选择模板
    Mat selectMatchTemplate();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
