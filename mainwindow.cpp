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
 * @brief MainWindow::selectMatchTemplate:选择一个模板
 * @return
 */
Mat MainWindow::selectMatchTemplate ()
{
     Mat tempImage;
    QString fileName=QFileDialog::getOpenFileName (this,tr("选择测试文件"),".",tr("Image Files(*.bmp *.jpg *.jpeg *.png)"));
    if(!fileName.isEmpty ())//读到文件了
    {
        QTextCodec *codec=QTextCodec::codecForName ("gb18030");
        std::string name=codec->fromUnicode (fileName).data ();
        tempImage=imread (name);
        if(tempImage.data)
        {
            cvtColor (tempImage,tempImage,CV_BGR2RGB);
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
    return tempImage;
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
 * @brief MainWindow::on_actionHistEqualize_triggered:直方图均衡化
 */
void MainWindow::on_actionHistEqualize_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //定义一个数组，用于存储各通道的向量
        vector<Mat> splitBGR(this->testImage.channels ());
        //分割通道
        split (this->testImage,splitBGR);
        //对各个通道分别进行直方图均衡化
        for(int i=0;i<this->testImage.channels ();i++)
        {
            equalizeHist (splitBGR[i],splitBGR[i]);
        }

        //合并图像
        Mat mergeImg;
        merge(splitBGR,mergeImg);
        // 复制图像
        mergeImg.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_actionContrast_triggered:调节对比度
 */
void MainWindow::on_actionContrast_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        this->testImage.convertTo (this->destImage,-1,2);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_actionLight_triggered:亮度
 */
void MainWindow::on_actionLight_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        this->testImage.convertTo (this->destImage,-1,1,80);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_actionContrastAndLight_triggered:对比度和亮度
 */
void MainWindow::on_actionContrastAndLight_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        this->testImage.convertTo (this->destImage,-1,2,80);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D1_triggered：卷积，算子1
 */
void MainWindow::on_action_filter2D1_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子1
        //建立核
        Mat kernel=(Mat_<float>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D2_triggered:卷积，算子2
 */
void MainWindow::on_action_filter2D2_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子2
        //建立核
        Mat kernel=Mat(3,3,CV_32F,Scalar(-1));
        //分配像素值
        kernel.at<float>(1,1)=8;
        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D3_triggered:卷积，算子3
 */
void MainWindow::on_action_filter2D3_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子3
        //建立核
        Mat kernel=Mat(3,3,CV_32F,Scalar(-1));
        //分配像素值
        kernel.at<float>(1,1)=8.9f;
        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D4_triggered:卷积，算子4
 */
void MainWindow::on_action_filter2D4_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子4
        //建立核
        Mat kernel=Mat(3,3,CV_32F,Scalar(0));
        //分配像素值
        kernel.at<float>(1,1)=5.0;
        kernel.at<float>(0,1)=-1.0;
        kernel.at<float>(2,1)=-1.0;

        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D5_triggered:卷积，算子5
 */
void MainWindow::on_action_filter2D5_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子5
        //建立核
        Mat kernel=Mat(3,3,CV_32F,Scalar(0));
        //分配像素值
        kernel.at<float>(1,1)=5.0;
        kernel.at<float>(1,0)=-1.0;
        kernel.at<float>(1,2)=-1.0;

        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_action_filter2D6_triggered:卷积，算子6
 */
void MainWindow::on_action_filter2D6_triggered()
{
    if(this->testImage.data)//已经有数据了
    {
        //算子6
        //建立核
        Mat kernel=(Mat_<float>(3,3)<<0,-1,0,-1,6,-1,0,-1,0);

        filter2D (this->testImage,this->destImage,this->testImage.depth (),kernel);

        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误!"),tr("当前尚未选择测试文件！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_SQDIFF_triggered:模板匹配，TM_SQDIFF
 */
void MainWindow::on_actionTM_SQDIFF_triggered()
{
    //TM_SQDIFF模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_SQDIFF;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (0,255,255),2,8,0);
        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_SQDIFF_NORMED_triggered:TM_SQDIFF_NORMED
 */
void MainWindow::on_actionTM_SQDIFF_NORMED_triggered()
{
    //TM_SQDIFF_NORMED模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_SQDIFF_NORMED;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (0,255,255),2,8,0);
        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_CCORR_triggered:TM_CCORR
 */
void MainWindow::on_actionTM_CCORR_triggered()
{
    //TM_CCORR模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_CCORR;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
//        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (255,255,0),2,8,0);
//        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
//                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMatNoScale (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_CCORR_NORMED_triggered:TM_CCORR_NORMED
 */
void MainWindow::on_actionTM_CCORR_NORMED_triggered()
{
    //TM_CCORR_NORMED模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_CCORR_NORMED;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (0,255,255),2,8,0);
        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_CCOFF_triggered
 */
void MainWindow::on_actionTM_CCOFF_triggered()
{
    //TM_CCOEFF_NORMED模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_CCOEFF;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (0,255,255),2,8,0);
        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionTM_CCOEFF_NORMED_triggered
 */
void MainWindow::on_actionTM_CCOEFF_NORMED_triggered()
{
    //TM_CCOEFF_NORMED模板匹配效果
    Mat templ;
    Mat result;
    int match_method=TM_CCOEFF_NORMED;

    templ=this->selectMatchTemplate ();
    if(templ.data)
    {
        Mat img_display;
        this->testImage.copyTo (img_display);

        int result_cols=this->testImage.cols-templ.cols+1;
        int result_rows=this->testImage.rows-templ.rows+1;

        result.create (result_cols,result_rows,CV_32FC1);
        matchTemplate (this->testImage,templ,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        //使用minMaxLoc对匹配结果进行定位
        double minVal;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        Point matchLoc;
        minMaxLoc (result,&minVal,&maxVal,&minLoc,&maxLoc,Mat());
        if(match_method==TM_SQDIFF||match_method==TM_SQDIFF_NORMED)
            matchLoc=minLoc;
        else
            matchLoc=maxLoc;

        //处理结果
        rectangle (img_display,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                   Scalar (0,255,255),2,8,0);
        rectangle (result,matchLoc,Point(matchLoc.x+templ.cols,matchLoc.y+templ.rows),
                           Scalar (0,255,255),2,8,0);
        img_display.copyTo (this->destImage);
        this->setLablePixmapWithMat (ui->dstImage,this->destImage);
    }
    else
    {
        this->showWarningMessageBox (tr("错误"),tr("选择的模板文件无效！"));
    }
}

/**
 * @brief MainWindow::on_actionWrapAffine_triggered:单纯仿射
 */
void MainWindow::on_actionWrapAffine_triggered()
{
    //单纯仿射
    Point2f srcTri[3];
    Point2f dstTri[3];
    Mat rot_mat(2,3,CV_32FC1);
    Mat warp_mat(2,3,CV_32FC1);
    Mat src,warp_dst,warp_rotate_dst;
    //读取图像
    this->testImage.copyTo (src);
    warp_dst=Mat::zeros (src.rows,src.cols,src.type ());
    //用3个点确定A仿射变换
    srcTri[0]=Point2f(0,0);
    srcTri[1]=Point2f(src.cols-1,0);
    srcTri[2]=Point2f(0,src.rows-1);
    dstTri[0]=Point2f(src.cols*0.0,src.rows*0.33);
    dstTri[1]=Point2f(src.cols*0.85,src.rows*0.25);
    dstTri[2]=Point2f(src.cols*0.15,src.rows*0.7);
    warp_mat=getAffineTransform (srcTri,dstTri);
    warpAffine (src,warp_dst,warp_mat,warp_dst.size());

    warp_dst.copyTo (this->destImage);

    this->setLablePixmapWithMatNoScale (ui->dstImage,this->destImage);


}

/**
 * @brief MainWindow::on_actionRotateWrapAffine_triggered：旋转仿射
 */
void MainWindow::on_actionRotateWrapAffine_triggered()
{
    //旋转仿射
    Point2f srcTri[3];
    Point2f dstTri[3];
    Mat rot_mat(2,3,CV_32FC1);
    Mat warp_mat(2,3,CV_32FC1);
    Mat src,warp_dst,warp_rotate_dst;
    //读取图像
    this->testImage.copyTo (src);
    warp_dst=Mat::zeros (src.rows,src.cols,src.type ());
    //用3个点确定A仿射变换
    srcTri[0]=Point2f(0,0);
    srcTri[1]=Point2f(src.cols-1,0);
    srcTri[2]=Point2f(0,src.rows-1);
    dstTri[0]=Point2f(src.cols*0.0,src.rows*0.33);
    dstTri[1]=Point2f(src.cols*0.85,src.rows*0.25);
    dstTri[2]=Point2f(src.cols*0.15,src.rows*0.7);
    warp_mat=getAffineTransform (srcTri,dstTri);
    warpAffine (src,warp_dst,warp_mat,warp_dst.size());

    //旋转矩阵
    Point center=Point(warp_dst.cols/2,warp_dst.rows/2);
    double angle=-50.0f;
    double scale=0.6f;
    rot_mat=getRotationMatrix2D (center,angle,scale);
    warpAffine (warp_dst,warp_rotate_dst,rot_mat,warp_dst.size());


    warp_rotate_dst.copyTo (this->destImage);

    this->setLablePixmapWithMatNoScale (ui->dstImage,this->destImage);
}
