#
2.5 扫描图像并访问相邻像素
[示例代码](https://github.com/ShawnZhang31/Learning-OpenCV-QT5.6/tree/high-effi-scan)。
在图像处理中经常用这样的处理函数，它在计算每个像素的数值时，需要使用周边像素的值。如果相邻像素在上一行或下一行，就需要同时扫描图像的多行。

## 2.5.1 准备工作
为了便于说明问题，我们将使用一个锐化图像的的处理函数。它基于拉普拉斯算子（将在[第六章](../chapter06.md)讨论）。在图像处理领域有一个众所周知的结论：如果从图像中减去拉普拉斯算子部门，图像的边缘就会放大，因而图像会变得更加尖锐。
可以用以下方法计算锐化的数值:
```
sharpened_pixel=5*current-left-right-up-down
```
这里的left是与当前像素相邻的左侧像素，up是上一行的相邻像素，以此类推。
## 2.5.2 如何实现
这里不能使用就地处理，用户必须提供一个输出图像。图像扫描中使用了三个指针，一个表示当前行，一个表示上面一行，一个表示下面一行。另外，因为在计算每一个像素时都需要访问他的相邻的像素，所以有些像素的值是无法计算的，比如第一行，最后一行，最后一列。这个循环可以这样写:
```
/**
* @brief sharpen:图像锐化处理
* @param image:原始图像
* @param result:锐化处理后的图像
*/
void sharpen(const cv::Mat &image, cv::Mat &result)
{
//判断是否需要分配图像数据。如果需要，就分配
result.create (image.size(),image.type ());

//获取通道数
int nchannels=image.channels ();

//处理所有行，除了第一行和最后一行
for(int j=1;j<image.rows-1;j++)
{
const uchar* previous = image.ptr<const uchar>(j-1); //上一行
const uchar* current = image.ptr<const uchar> (j); //当前行
const uchar* next = image.ptr<const uchar> (j+1); //下一行

uchar* output = result.ptr<uchar> (j);

for(int i=nchannels;i<(image.cols-1)*nchannels;i++)
{
//应用锐化算子
*output++=cv::saturate_cast<uchar>(
5*current[i]-current[i-nchannels]-
current[i+nchannels]-previous[i]-next[i]);

// std::cout<<*output<<std::endl;
}
}

//把未处理的像素设为0
result.row (0).setTo (cv::Scalar(0));
result.row (result.rows-1).setTo (cv::Scalar(0));
result.col (0).setTo (cv::Scalar(0));
result.col (result.cols-1).setTo (cv::Scalar(0));
}
```
## 2.5.3 实现原理
若要访问上一行和下一行的相邻像素，只需定义额外的指针，并与当前行的指针一起递增，然后就可以在扫描循环内访问上下的指针了。
在计算输出像素的值时，我们调用了cv::saturate_cast模板函数，并传入运算结果。这是因为计算像素的数学表达式的结果经常超出允许的范围（即小于0或大于255）。使用这个函数可以把结果调整到8位无符号数的范围，具体作为是把小于0的数值调整为0，大于255的数值调整为255———这就是cv::saturate_cast<uchar>函数的作用。刺猬，如果输入参数是浮点数，就会得到最接近的整数。可以在调用这个函数时显式地指定其他数据参数，以确保结果在该数据类型定义的范围内。
由于边框上的像素没有完整的相邻像素，因此不能用前面的方法计算，需要另行处理。这里简单地把他们设置为0。有时也可以对这些像素做特殊计算，但在大多数情况下，花时间处理这些极少数像素是没有意义的。在本利中，我们用两个特殊的方法把边框的像素设置为了0，他们是row和col。这两个方法返回一个特殊的cv::Mat实例，其中包含一个单行ROI，具体范围取决于参数。这里没有进行复制，因为只要这个一维矩阵的元素被修改，原始图像也会被修改。我们用setTo方法来实现这个功能，此方法将对矩阵中的所有元素赋值:
```
result.row(0).setTo(cv::Scalar(0));
```
## 2.5.4 扩展阅读
在对像素领域进行计算时，通常用一个核心矩阵表示。这个核心矩阵展现了如何将于计算相关的像素组合起来，才能得到预期结果。针对本节使用的锐化滤波器，核心矩阵可以是这样养的：
![核心矩阵](/assets/hexin.png)
除非另有说明，当前像素用核心矩阵中心单元格表示。核心矩阵中的每个单元格表示相关像素的乘法系数，像素应用核心矩阵得到的结果，集这些乘积累加。核心矩阵的大小就是邻域的大小（这里是3x3）。在图像上应用核心矩阵不只是为了描述方便，他也是信号处理中卷积概念的基础。核心矩阵定义了一个用于图像的滤波器。
鉴于滤波是图像处理中的常见操作,OpenCV专门为此定义了一个函数，即cv::filter2D。要使用这个函数，只需要定义一个内核(已矩阵的形式)，顶用函数并传入图像和内核，即可返回滤波后的图像。因此，使用这个函数重新定义锐化函数非常容易:
```
/**
* @brief shapped2D:使用filter2D进行滤波
* @param image:输入图像
* @param result:滤波后的图像
*/
void shapped2D(const cv::Mat &image, cv::Mat &result)
{
//构造内核
cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));

//对内核进行赋值
kernel.at<float>(1,1)=5.0;
kernel.at<float>(0,1)=-1.0;
kernel.at<float>(1,0)=-1.0;
kernel.at<float>(1,2)=-1.0;
kernel.at<float>(2,1)=-1.0;

//对图像进行滤波
cv::filter2D (image,result,image.depth (),kernel);
}
```
**注意，使用大内核的filter2D函数是特别有利的的，因为这时它使用了更高效的算法**




