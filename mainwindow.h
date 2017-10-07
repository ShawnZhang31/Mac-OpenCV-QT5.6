#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QString>
#include<QImage>
#include<QLabel>

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
    void on_actionOpenTestImage_triggered();

    void on_actionExit_triggered();

    //成员变脸
    void on_actionRestore_triggered();

    void on_actionHorizenFilp_triggered();

    void on_actionVerticalFlip_triggered();

    void on_actionHorizalAndVerticalFlip_triggered();

    void on_actionErode_triggered();

    void on_actionDialation_triggered();

    void on_actionOpenning_triggered();

    void on_actionClosing_triggered();

    void on_actionMorphologicalGradient_triggered();

    void on_actionTopHat_triggered();

    void on_actionBlackHat_triggered();

    void on_actionNoNormalize_triggered();

    void on_actionNormalizeFilter_triggered();

    void on_actionNormalize_triggered();

    void on_actionGaussFilter_triggered();

    void on_actionMediaFilter_triggered();

    void on_actionBilateralFilter_triggered();

    void on_action_PyrUp_triggered();

    void on_action_PyrDown_triggered();

    void on_action_Resize_triggered();

    void on_action_ResizeS_triggered();

    void on_actioncNresize_triggered();

    void on_actioncResize_triggered();

    void on_actionantiClockwise_triggered();

    void on_actionNFlipResize_triggered();

public:
    Mat testImage;
    Mat destImage;

public:
    QImage matConverToQImage(Mat image);

private:
    void setLableWithcvImage(QLabel* lable,Mat image);//使用MAT来指定lable
    void setLableWithcvImageWithoutScale(QLabel* lable,Mat image);//使用MAT来指定lable Without Scale
    void showWarningMessageBox(QString title,QString text);//警示窗口


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
