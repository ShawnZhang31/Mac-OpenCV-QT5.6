#2.2 用指针扫描图像
在处理非常庞大数量的像素的时候我们可以使用指针或者迭代器来进行高效处理。本节主要说明如何使用指针进行处理。
## 2.2.1 准备工作
彩色图像由三通道像素组成，每个通道表示蓝绿红三原色中一种颜色的亮度值，每个数值都是8位无符号字符类型，因此颜色总数为256\*256\*256，即超过1600万中颜色。因此为了减低分析的复杂性，有时需要减少图像中颜色的数量。一种实现方法是把RGB空间细分到大小相等的方块中。例如，如果把每个颜色数量减少到1/8，那么颜色总数就变成32\*32\*32。将旧图像中的每个颜色值划分到一个方块，该方块的中间值就是新的颜色值；新图像使用新的颜色值，颜色数就减少。
因此，基本的减色算法很简单。假设N是减色因子，将图像中每个像素的值除以N（这里假定使用整数除法，不保留余数）。然后将结果乘以N，得到N的倍数，并且刚好不超过原始像素值。加上N/2，就等到相邻N倍数之间的中间值。
## 2.2.2 如何实现
```
/**
* @brief reduceColor:降低颜色空间
* @param image:需要降低的图片
* @param div:降低的倍数
*/
void reduceColor(cv::Mat image,int div=64)
{
int nl=image.rows;//行数
int nc=image.cols*image.channels ();//每行元素的数量
for(int i=0;i<nl;i++)
{
//取得i行的地址
uchar* data=image.ptr (i);
for(int j=0;j<nc;j++)
{
data[j]=data[j]/div*div+div/2;//每个元素
}//一行元素处理完成
}
}
```
## 2.2.3 实现原理
在彩色图像中，图像数据缓冲区的前3字节表示左上角像素的三个通道的值，接下来的3字节表示第一行的第二个像素，以此类推（**注意OpenCV默认的通道次序为BGR**）。一个宽W高H的图像所需的内存块大小为WxHx3 uchars。不过出于性能上的考虑，我们会用几个额外的像素来填补行的长度。这是因为，如果函数是某个数字（例如8）的整数倍，图像处理的性能可能会提高，因此最好根据内存配置情况将数据对齐。当然，这些额外的像素既不会显示也不会被保存，它们的额外数据会被忽略。OpenCV把经过填充的行的长度指定为有效宽度。如果图像没有用额外的像素填充，那么有效宽度就等于实际的图像宽度。用cols和rows属性可得到图像的宽度和高度。与之类似，用step数据属性可得到单位是字节的有效宽度。即使图像的类型不是uchar，step仍然能提供行的字节数。我们而已通过elemSize方法（**例如一个三通道短整型的矩阵CV_16SC3,elemSize会返回6**）获得像素的大小，通过nchannels方法（**灰度图像为1，彩色图像为3**）获得图像中通道的数量，最后用total方法返回矩阵中的像素（即矩阵的条目）总数。
## 2.2.4 扩展阅读
1. 对连续的图像的高效扫描
为了提高性能，可以在图像的每行末尾用额外的像素进行填充。有趣的是，在去掉填充后，图像仍可被看坐一个包含WxH像素的长一维数组。用cv::Mat的isContinuous方法可以轻松判断图像由没有被填充。如果图像没有被填充像素，他就返回true。我们还能这样测试矩阵的连续性。

```
//检查行的长度（字节数）与“列的个数*单个像素”的字节数是否相等
image.step==image.cols*image.elemSize();
```

为了确保完整性，测试时好需要检查矩阵是否只有一行；如果是，这个矩阵就是连续的。但是无论不管哪种情况，都可以用isContinuous方法检查矩阵的连续性。在一些特殊的处理算法中，你可以充分利用图像的连续性，在单个（更长）循环中处理图像。那么处理图像的函数就可以改为：

```
void colorReduce(cv::Mat image,int div=64)
{
int nl=image.rows;//行数

//每行元素的总数
int nc=image.cols*image.channels ();

if(image.isContinuous ())
{
//图像是连续的
nc=nc*nl;
nl=1;//装换程一个一维数组
}

int n=static_cast<int>(log(static_cast<double>(div))/log(2.0)+0.5);

//用来截取像素的掩码
uchar mask=0xFF<<n;//如果div=16，那么mask=0xF0
uchar div2=div>>1;//div2=div/2

//对于连续图像，这个循环只执行一次
for(int j=0;j<nl;j++)
{
uchar* data=image.ptr<uchar>(j);
for(int i=0;i<nc;i++)
{
*data &=mask;
*data++ +=div2;
}//一行结束
}
}
```

如果连续测试结果表明图像中没有填充像素，我们就把宽度设为1，高度设为WxH，从而去除外层的循环。注意，这里还需要用reshape方法。上面的函数可以重写为：
```
/**
* @brief colorReduce:使用连续图像的高效扫描法处理
* @param image
* @param div
*/
void colorReduce(cv::Mat image,int div=64)
{
// int nl=image.rows;//行数

// //每行元素的总数
// int nc=image.cols*image.channels ();

if(image.isContinuous ())
{
//图像是连续的
// nc=nc*nl;
// nl=1;//装换程一个一维数组
image.reshape (1,//新的通道数
1);//新的函数
}

int nl=image.rows;//行数

//每行元素的总数
int nc=image.cols*image.channels ();

int n=static_cast<int>(log(static_cast<double>(div))/log(2.0)+0.5);

//用来截取像素的掩码
uchar mask=0xFF<<n;//如果div=16，那么mask=0xF0
uchar div2=div>>1;//div2=div/2

//对于连续图像，这个循环只执行一次
for(int j=0;j<nl;j++)
{
uchar* data=image.ptr<uchar>(j);
for(int i=0;i<nc;i++)
{
*data &=mask;
*data++ +=div2;
}//一行结束
}
}
```
如果是用reshape方法修改矩阵的维数，就不需要复制内存或者重新分配内存了。
2. 低层次指针算法
在cv::Mat类中，图像数据时存放在无符号字符型的内存块中的。其中data属性表示内存块第一个元素的地址，它会返回一个无符号字符型的指针。如果要从图像的起点开始循环，你可以使用如下代码：
```
uchar* data=image.data;
```
利用有效宽度移动指针，可以从一行移动到下一行，代码如下所示:
```
data +=image.step;//下一行
```
用step属性可得到一行的总字节数(包括填充像素)。通常可以用下面的方法得到第j行、第i列的像素的地址：
```
//(j,i)像素的地址，即&image.at(j,i)
data=image.data+j*image.step+i*image.elemSize();
```
然而，这种处理方法并不推荐使用

