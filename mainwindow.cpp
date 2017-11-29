#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QImage>
#include<QPixmap>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextCodec>
#include<QInputDialog>
#include<QLineEdit>

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
 * @brief MainWindow::on_actionBlack_White_Chao_triggered:黑白混沌图像
 */
void MainWindow::on_actionBlack_White_Chao_triggered()
{
    const int M=100;
    const int N=100;
    float chaotic[M*N];
    double t;
    bool isOK;
    QString text=QInputDialog::getText (NULL,tr("混沌初始值"),tr("请输入初始值"),QLineEdit::Normal,"0.98",&isOK);

    //混沌初始值
    if(isOK)
    {
        t=text.toDouble ();
    }
    else
        t=0.98;

    chaotic[0]=t;
     int i;

     //生成混沌序列
     for(i=1;i<M*N;i++)
     {
         chaotic[i]=1-2*chaotic[i-1]*chaotic[i-1];
     }

     //将混沌序列二值化，只有0和255两个数
     for(i=0;i<M*N;i++)
     {
         if(chaotic[i]>0)
             chaotic[i]=255;
         else
             chaotic[i]=0;
     }

     //建立一个混沌图像
     Mat chaoticImg(M,N,CV_8UC1);

     int flag=0;
     //用指针方式赋值
     for(int y=0;y<M;y++)
     {
         uchar* chaoticImgR=chaoticImg.ptr<uchar>(y);
         for(int x=0;x<N;x++)
         {
             chaoticImgR[x]=chaotic[flag];
             flag++;
         }
     }

     this->setLablePixmapWithGreyMat (ui->dstImage,chaoticImg);

}

/**
 * @brief MainWindow::on_actionGrey_Chaotic_triggered:灰度混沌图像
 */
void MainWindow::on_actionGrey_Chaotic_triggered()
{
    const int M=100;
    const int N=100;
    float chaotic[M*N];
    double t;
    bool isOK;
    QString text=QInputDialog::getText (NULL,tr("混沌初始值"),tr("请输入初始值"),QLineEdit::Normal,"0.98",&isOK);

    //混沌初始值
    if(isOK)
    {
        t=text.toDouble ();
    }
    else
        t=0.98;

    chaotic[0]=t;
     int i;

     //生成混沌序列
     for(i=1;i<M*N;i++)
     {
         chaotic[i]=1-2*chaotic[i-1]*chaotic[i-1];
     }

     //将混沌序列处理为256个灰度级别
     for(i=0;i<M*N;i++)
     {
         chaotic[i]=((int)(chaotic[i]*1000000))%256;
     }

     //建立一个混沌图像
     Mat chaoticImg(M,N,CV_8UC1);

     int flag=0;
     //用指针方式赋值
     for(int y=0;y<M;y++)
     {
         uchar* chaoticImgR=chaoticImg.ptr<uchar>(y);
         for(int x=0;x<N;x++)
         {
             chaoticImgR[x]=chaotic[flag];
             flag++;
         }
     }

     this->setLablePixmapWithGreyMat (ui->dstImage,chaoticImg);
}

/**
 * @brief MainWindow::on_actionColored_Chaotic_triggered:彩色混沌图像
 */
void MainWindow::on_actionColored_Chaotic_triggered()
{
    const int M=100;
    const int N=100;
    float chaotic[M*N*3];
    double t;
    bool isOK;
    QString text=QInputDialog::getText (NULL,tr("混沌初始值"),tr("请输入初始值"),QLineEdit::Normal,"0.98",&isOK);

    //混沌初始值
    if(isOK)
    {
        t=text.toDouble ();
    }
    else
        t=0.98;

    chaotic[0]=t;
     int i;

     //生成混沌序列
     for(i=1;i<M*N*3;i++)
     {
         chaotic[i]=1-2*chaotic[i-1]*chaotic[i-1];
     }

     //将混沌序列处理为256个灰度级别
     for(i=0;i<M*N;i++)
     {
         chaotic[i]=((int)(chaotic[i]*1000000))%256;
     }

     //建立一个混沌图像
     Mat chaoticImg(M,N,CV_8UC3);

     int flag=0;
     //用指针方式赋值
     for(int y=0;y<M;y++)
     {
         uchar* chaoticImgR=chaoticImg.ptr<uchar>(y);
         for(int x=0;x<N;x++)
         {
             chaoticImgR[x*3]=chaotic[flag];
             chaoticImgR[x*3+1]=chaotic[flag+M*N];
             chaoticImgR[x*3+2]=chaotic[flag+2*M*N];
             flag++;
         }
     }

     this->setLablePixmapWithMat (ui->dstImage,chaoticImg);
}
