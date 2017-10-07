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
 * @brief setLableWithcvImage:使用OpenCV的图像来设置QT的Lable
 * @param lable
 * @param image
 */
void MainWindow::setLableWithcvImage (QLabel* lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    img=img.scaled (lable->width (),lable->height ());
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

/**
 * @brief MainWindow::setLableWithcvImageWithoutScale:使用OpenCV的图像来设置QT的Lable
 * @param lable
 * @param image
 */
void MainWindow::setLableWithcvImageWithoutScale (QLabel* lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
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
            this->setLableWithcvImage (ui->OriginImage,this->testImage);
        }
        else
        {
            this->showWarningMessageBox (tr("未找到数据"),tr("未找到数据"));
        }


    }
    else
    {
        this->showWarningMessageBox (tr("测试文件不存在"),tr("选择的测试文件不存在，或者文件已损坏无法读取"));
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
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_actionHorizenFilp_triggered:图像水平翻转
 */
void MainWindow::on_actionHorizenFilp_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        flip(this->testImage,this->destImage,1);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionVerticalFlip_triggered:图像垂直翻转
 */
void MainWindow::on_actionVerticalFlip_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        flip(this->testImage,this->destImage,0);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionHorizalAndVerticalFlip_triggered:图像水平垂直翻转
 */
void MainWindow::on_actionHorizalAndVerticalFlip_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        flip(this->testImage,this->destImage,-1);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionErode_triggered:腐蚀
 */
void MainWindow::on_actionErode_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //腐蚀操作
        //定义类型
        int erosion_type;
        erosion_type=MORPH_RECT;
        //定义大小
        int erosion_size=3;
        //创建腐蚀核
        Mat element=getStructuringElement (erosion_type,Size(2*erosion_size+1,2*erosion_size+1),Point(erosion_size,erosion_size));

        erode (this->testImage,this->destImage,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionDialation_triggered:膨胀
 */
void MainWindow::on_actionDialation_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //腐蚀操作
        //定义类型
        int dialation_type;
        dialation_type=MORPH_RECT;
        //定义大小
        int dialation_size=2;
        //创建腐蚀核
        Mat element=getStructuringElement (dialation_type,Size(2*dialation_size+1,2*dialation_size+1),Point(dialation_size,dialation_size));

        dilate (this->testImage,this->destImage,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionOpenning_triggered:开运算
 */
void MainWindow::on_actionOpenning_triggered()
{

    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //开运算
        //定义形状
        int morph_type=MORPH_RECT;
        int morph_size=3;
        Mat element=getStructuringElement (morph_type,Size(2*morph_size+1,2*morph_size+1),Point(morph_size,morph_size));

        morphologyEx (this->testImage,this->destImage,MORPH_OPEN,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionClosing_triggered:闭运算
 */
void MainWindow::on_actionClosing_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //闭运算
        //定义形状
        int morph_type=MORPH_RECT;
        int morph_size=3;
        Mat element=getStructuringElement (morph_type,Size(2*morph_size+1,2*morph_size+1),Point(morph_size,morph_size));

        morphologyEx (this->testImage,this->destImage,MORPH_CLOSE,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionMorphologicalGradient_triggered:形态学梯度
 */
void MainWindow::on_actionMorphologicalGradient_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //形态学梯度
        //定义形状
        int morph_type=MORPH_RECT;
        int morph_size=3;
        Mat element=getStructuringElement (morph_type,Size(2*morph_size+1,2*morph_size+1),Point(morph_size,morph_size));

        morphologyEx (this->testImage,this->destImage,MORPH_GRADIENT,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionTopHat_triggered:顶帽
 */
void MainWindow::on_actionTopHat_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //顶帽
        //定义形状
        int morph_type=MORPH_RECT;
        int morph_size=3;
        Mat element=getStructuringElement (morph_type,Size(2*morph_size+1,2*morph_size+1),Point(morph_size,morph_size));

        morphologyEx (this->testImage,this->destImage,MORPH_TOPHAT,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionBlackHat_triggered:黑帽
 */
void MainWindow::on_actionBlackHat_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //顶帽
        //定义形状
        int morph_type=MORPH_RECT;
        int morph_size=3;
        Mat element=getStructuringElement (morph_type,Size(2*morph_size+1,2*morph_size+1),Point(morph_size,morph_size));

        morphologyEx (this->testImage,this->destImage,MORPH_BLACKHAT,element);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionNormalizeFilter_triggered:均值滤波
 */
void MainWindow::on_actionNormalizeFilter_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //均值滤波
        blur(this->testImage,this->destImage,Size(7,7),Point(-1,-1));
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionNormalize_triggered:方框滤波，归一
 */
void MainWindow::on_actionNormalize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //方框滤波，归一化
        cv::boxFilter (this->testImage,this->destImage,-1,Size(5,5));
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionNoNormalize_triggered:方框滤波，非归一
 */
void MainWindow::on_actionNoNormalize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //方框滤波，非归一
        cv::boxFilter (this->testImage,this->destImage,-1,Size(3,3),Point(-1,-1),false);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionGaussFilter_triggered:高斯滤波
 */
void MainWindow::on_actionGaussFilter_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //高斯滤波
        cv::GaussianBlur (this->testImage,this->destImage,Size(7,7),0,0);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionMediaFilter_triggered:中值滤波
 */
void MainWindow::on_actionMediaFilter_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //中值滤波
        cv::medianBlur (this->testImage,this->destImage,7);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionBilateralFilter_triggered:双边滤波
 */
void MainWindow::on_actionBilateralFilter_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //双边滤波
        cv::bilateralFilter (this->testImage,this->destImage,31,31*2,31/2);
        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_action_PyrUp_triggered:使用金字塔方法放大
 */
void MainWindow::on_action_PyrUp_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //使用金字塔方法放大
        cv::pyrUp (this->testImage,this->destImage,Size(this->testImage.cols*2,this->testImage.rows*2));
        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_action_PyrDown_triggered:使用金字塔方法缩小
 */
void MainWindow::on_action_PyrDown_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //使用金字塔方法缩小
        cv::pyrDown (this->testImage,this->destImage,Size(this->testImage.cols/2,this->testImage.rows/2));
        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_action_Resize_triggered:放大
 */
void MainWindow::on_action_Resize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //使用金字塔方法缩小
        cv::resize (this->testImage,this->destImage,Size(this->testImage.cols*4,this->testImage.rows*4),0,0,3);
        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

void MainWindow::on_action_ResizeS_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //使用金字塔方法缩小
        cv::resize (this->testImage,this->destImage,Size(this->testImage.cols/4,this->testImage.rows/4),0,0,3);
        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actioncNresize_triggered:顺时针无缩放
 */
void MainWindow::on_actioncNresize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //顺时针无缩放效果
        //定义Point2f
        Point2f srcTri[3];
        Point2f dstTri[3];
        Mat rot_mat(2,3,CV_32FC1);
        Mat warp_mat(2,3,CV_32FC1);
        Mat src, warp_dst, warp_rotate_dst;

        //读入图像
        this->testImage.copyTo (src);
        warp_dst=Mat::zeros (src.rows,src.cols,src.type ());

        //用3个点确定A仿射变换
        Point center=Point(src.cols/2,src.rows/2);
        double angle=-50.0;
        double scale=0.3;
        rot_mat=getRotationMatrix2D (center,angle,scale);
        warpAffine (src,warp_rotate_dst,rot_mat,warp_dst.size());
        warp_rotate_dst.copyTo (this->destImage);

        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actioncResize_triggered:顺时针缩放效果
 */
void MainWindow::on_actioncResize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //顺时针缩放效果
        //定义Point2f
        Point2f srcTri[3];
        Point2f dstTri[3];
        Mat rot_mat(2,3,CV_32FC1);
        Mat warp_mat(2,3,CV_32FC1);
        Mat src, warp_dst, warp_rotate_dst;

        //读入图像
        this->testImage.copyTo (src);
        warp_dst=Mat::zeros (src.rows,src.cols,src.type ());

        //用3个点确定A仿射变换
        Point center=Point(src.cols/2,src.rows/2);
        double angle=-50.0;
        double scale=0.3;
        rot_mat=getRotationMatrix2D (center,angle,scale);
        warpAffine (src,warp_rotate_dst,rot_mat,warp_dst.size());
        warp_rotate_dst.copyTo (this->destImage);

        this->setLableWithcvImage (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionantiClockwise_triggered:逆时针效果
 */
void MainWindow::on_actionantiClockwise_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //逆时针效果
        //定义Point2f
        Point2f srcTri[3];
        Point2f dstTri[3];
        Mat rot_mat(2,3,CV_32FC1);
        Mat warp_mat(2,3,CV_32FC1);
        Mat src, warp_dst, warp_rotate_dst;

        //读入图像
        this->testImage.copyTo (src);
        warp_dst=Mat::zeros (src.rows,src.cols,src.type ());

        //用3个点确定A仿射变换
        Point center=Point(src.cols/2,src.rows/2);
        double angle=50.0;
        double scale=0.3;
        rot_mat=getRotationMatrix2D (center,angle,scale);
        warpAffine (src,warp_rotate_dst,rot_mat,warp_dst.size());
        warp_rotate_dst.copyTo (this->destImage);

        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}

/**
 * @brief MainWindow::on_actionNFlipResize_triggered:零旋转缩放效果
 */
void MainWindow::on_actionNFlipResize_triggered()
{
    //检查是否有数据可以操作
    if(this->testImage.data)//有数据
    {
        //零旋转缩放效果
        //定义Point2f
        Point2f srcTri[3];
        Point2f dstTri[3];
        Mat rot_mat(2,3,CV_32FC1);
        Mat warp_mat(2,3,CV_32FC1);
        Mat src, warp_dst, warp_rotate_dst;

        //读入图像
        this->testImage.copyTo (src);
        warp_dst=Mat::zeros (src.rows,src.cols,src.type ());

        //用3个点确定A仿射变换
        Point center=Point(src.cols/2,src.rows/2);
        double angle=0.0;
        double scale=0.3;
        rot_mat=getRotationMatrix2D (center,angle,scale);
        warpAffine (src,warp_rotate_dst,rot_mat,warp_dst.size());
        warp_rotate_dst.copyTo (this->destImage);

        this->setLableWithcvImageWithoutScale (ui->DestImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误！"),tr("请先选择一张测试图片！"));
    }
}
