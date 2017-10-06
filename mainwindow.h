#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QString>
#include<QImage>

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

public:
    Mat testImage;
    Mat destImage;

public:
    /**
     * @brief matConverToQImage,将OpenCV图像装换为QImage图像
     * @param image：OpenCV格式的图像
     * @return QImage:QImage格式的图像
     */
    QImage matConverToQImage(Mat image);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
