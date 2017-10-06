#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QPixmap>
#include<QMessageBox>
#include<QTextCodec>
#include<iostream>
#include<string>
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
 * @brief matConverToQImage,将OpenCV图像装换为QImage图像
 * @param image：OpenCV格式的图像
 * @return QImage:QImage格式的图像
 */
QImage MainWindow::matConverToQImage(Mat image)
{
    return QImage((const unsigned char*)(image.data),
                  image.cols,
                  image.rows,
                  QImage::Format_RGB888);
}

/**
 * @brief MainWindow::on_actionOpenTestImage_triggered
 * 打开测试文件
 */
void MainWindow::on_actionOpenTestImage_triggered()
{
    QString fileName=QFileDialog::getOpenFileName (this,tr("请选择测试用文件"),".",tr("Image Files(*.bmp *.jpg *.jpeg *.png)"));
    if(!fileName.isEmpty ())//读到文件并且不为空
    {
        QTextCodec *code=QTextCodec::codecForName ("gb18030");
        std::string name=code->fromUnicode (fileName).data ();
        this->testImage=imread (name);//将QString转换为String
        if(this->testImage.data)//读取文件成功
        {
            //将BRG转化为RGB方便操作
            cvtColor (this->testImage,this->testImage,CV_BGR2RGB);

            QImage img=QImage((const unsigned char*)(this->testImage.data),
                              this->testImage.cols,
                              this->testImage.rows,
                              this->testImage.cols*this->testImage.channels (),
                              QImage::Format_RGB888);
            ui->OriginImage->clear ();
            img=img.scaled (ui->OriginImage->width (),ui->OriginImage->height ());
            ui->OriginImage->setPixmap (QPixmap::fromImage (img));
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle (tr("未找到数据"));
            msgBox.setText (tr("未找到数据!"));
            msgBox.exec ();
        }


    }
    else
    {
       QMessageBox msgBox;
       msgBox.setWindowTitle (tr("测试文件不存在"));
       msgBox.setText (tr("选择的测试文件不存在，或者文件已损坏无法读取!"));
       msgBox.exec ();
    }
}

/**
 * @brief MainWindow::on_actionExit_triggered
 * 退出程序
 */
void MainWindow::on_actionExit_triggered()
{
    qApp->exit ();
}

/**
 * @brief MainWindow::on_actionRestore_triggered:复原文件
 *
 */
void MainWindow::on_actionRestore_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        this->testImage.copyTo (this->destImage);
        QImage img=QImage((const unsigned char*)(this->destImage.data),
                          this->destImage.cols,
                          this->destImage.rows,
                          this->destImage.cols*this->destImage.channels (),
                          QImage::Format_RGB888
                          );
        img=img.scaled (ui->DestImage->width (),ui->DestImage->height ());
        ui->DestImage->clear ();
        ui->DestImage->setPixmap (QPixmap::fromImage (img));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle (tr("错误!"));
        msgBox.setText (tr("当前尚未选择测试文件！"));
        msgBox.exec ();
    }
}
