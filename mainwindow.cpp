#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QImage>
#include<QPixmap>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::setLablePixmapWithMat:使用openCV的图像来设置lable，缩放
 * @param lable
 * @param image
 */
void MainWindow::setLablePixmapWithMat (QLabel *lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    img=img.scaled (lable->size ());
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

void MainWindow::setLablePixmapWithGreyMat (QLabel *lable,Mat image)
{
    cv::resize (image,image,Size(lable->width (),lable->height ()),0,0,3);
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.step,
                      QImage::Format_Indexed8);

    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

/**
 * @brief MainWindow::setLablePixmapWithMatNoScale:使用openCV的图像来设置lable，不缩放
 * @param lable
 * @param image
 */
void MainWindow::setLablePixmapWithMatNoScale (QLabel *lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

void MainWindow::setLablePixmapWithGreyMatNoScale (QLabel *label,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.step,
                      QImage::Format_Indexed8);
    label->clear ();
    label->setPixmap (QPixmap::fromImage (img));
}

/**
 * @brief MainWindow::showWarningMessageBox:显示提示框
 * @param title
 * @param text
 */
void MainWindow::showWarningMessageBox(QString title,QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle (title);
    msgBox.setText (text);
    msgBox.exec ();
}

/**
 * @brief MainWindow::on_action_Open_TestImage_triggered：打开测试文件
 */
void MainWindow::on_action_Open_TestImage_triggered()
{
    QString fileName=QFileDialog::getOpenFileName (this,tr("选择测试文件"),".",tr("Image Files(*.bmp *.jpg *.jpeg *.png)"));
    if(!fileName.isEmpty ())//读到文件了
    {
        QTextCodec *codec=QTextCodec::codecForName ("gb18030");
        std::string name=codec->fromUnicode (fileName).data ();
        this->testImage=imread (name);
        if(this->testImage.data)
        {
            cvtColor (this->testImage,this->testImage,CV_BGR2RGB);
            this->setLablePixmapWithMat (ui->testImage,this->testImage);
        }
        else
        {
            this->showWarningMessageBox (tr("错误"),tr("OpenCV读取文件不成功"));
        }
    }
    else //没有读到文件
    {
        this->showWarningMessageBox (tr("测试文件不存在"),tr("所选择的测试文件不存在，或者文件已损坏无法读取"));
    }
}

/**
 * @brief MainWindow::on_action_Restore_triggered:还原
 */
void MainWindow::on_action_Restore_triggered()
{
    this->testImage.copyTo (this->destImage);
    this->setLablePixmapWithMat (ui->dstImage,this->destImage);
}

/**
 * @brief MainWindow::on_actionClear_triggered:清除
 */
void MainWindow::on_actionClear_triggered()
{
    ui->testImage->clear ();
    ui->dstImage->clear ();
}

/**
 * @brief MainWindow::on_actionExit_triggered:退出程序
 */
void MainWindow::on_actionExit_triggered()
{
    qApp->exit (0);
}

/**
 * @brief MainWindow::getFeature:提取图像特征
 * @param m
 * @param a
 */
void MainWindow::getFeature (cv::Mat m,float a[25])
{
    int M,N;    //用来存储图像的m的宽高
    int i,j;
    M=m.cols;
    N=m.rows;

    for(i=0;i<25;i++)
    {
        a[i]=0;
    }

    //转换为灰度图像
    cv::cvtColor (m,m,COLOR_RGB2GRAY);
    //转化为二值图像
    cv::threshold (m,m,100,255,THRESH_BINARY);

    //将图像划分为5x5个子块，计算每块像素值的和
    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
        {
            if(m.at<uchar>(i,j)==255)
            {
                a[i/(M/5)*5+j/(N/5)]++;
            }
        }
    }

    for(i=0;i<25;i++)
    {
        a[i]=a[i]/((M/5)*(N/5));
    }
}

/**
 * @brief MainWindow::ouDistance:计算欧氏距离
 * @param a
 * @param b
 * @return
 */
float MainWindow::ouDistance (float a[25],float b[25])
{
    int i;
    float distance=0;

    for(i=0;i<25;i++)
        distance+=(a[i]-b[i])*(a[i]-b[i]);

    distance=sqrt(distance);
    return distance;
}

/**
 * @brief MainWindow::getResultNumber:数字识别时，先分别计算待检测的数字图像和用于特征识别的数字0~9的数字图像特征，然后分别计算待检测的数字图像的特征与用于数字图像0~9的特征之间的距离。
 * @return
 */
int MainWindow::getResultNumber ()
{
    int i;
    float min;  //用来粗出最小的欧氏距离;
    int mini;   //用来存储最小的欧氏距离的数字号
     getFeature (this->testImage,this->testFeature);


}

/**
 * @brief MainWindow::on_actionOpenCustomFile_triggered:打开自定义文件
 */
void MainWindow::on_actionOpenCustomFile_triggered()
{
    QString fileName=QFileDialog::getOpenFileName (this,tr("选择测试文件"),".",tr("Image Files(*.bmp *.jpg *.jpeg *.png)"));
    if(!fileName.isEmpty ())//读到文件了
    {
        QTextCodec *codec=QTextCodec::codecForName ("gb18030");
        std::string name=codec->fromUnicode (fileName).data ();
        this->customImage=imread (name);
        if(this->customImage.data)
        {
            cvtColor (this->customImage,this->customImage,CV_BGR2RGB);
            //this->setLablePixmapWithMat (ui->testImage,this->testImage);
        }
        else
        {
            this->showWarningMessageBox (tr("错误"),tr("OpenCV读取文件不成功"));
        }
    }
    else //没有读到文件
    {
        this->showWarningMessageBox (tr("测试文件不存在"),tr("所选择的测试文件不存在，或者文件已损坏无法读取"));
    }
}
