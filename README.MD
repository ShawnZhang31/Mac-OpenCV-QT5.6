#
2.1 访问像素值
若要访问矩阵中的每个独立元素，只需要指定它的行号和列号即可。返回的对应元素可以是单个数值，也可以是多通道图像的数值向量。为了说明如何直接访问像素值，我们将创建一个简单的函数，用它在图像中加入**椒盐噪声（salt-and-pepper noise）**。
## 2.1.1 实现方式
在实现此椒盐噪声的方法中需要使用随机函数。关于随机函数我个人不推荐使用Windows默认的Random算法，这其实是一种伪随机数。个人更加推崇Linux的随机算法，这是一种真随机数。关于真伪随机数已经超出了本节的讨论内容，大家可以自行Google。
```
#include<random> //引入随机引擎

/**
* @brief salt:椒盐噪声产生方法
* @param image:用于生产椒盐的图像
* @param n:椒盐迭代器的迭代次数
*/
void salt(cv::Mat image, int n)
{
std::default_random_engine generator;//随机算法
std::uniform_int_distribution<int> randomRow(0,image.rows-1);//行分布
std::uniform_int_distribution<int> randomCol(0,image.cols-1);//列分布

int i,j;//用于存储每个随机值
for(int k=0;k<n;k++)
{
//随机图形的位置
i=randomRow(generator);
j=randomCol(generator);

if(image.type ()==CV_8UC1)//灰度图像
{
image.at<uchar>(i,j)=255;
}
else if(image.type ()==CV_8UC3)//彩色图像
{
image.at<cv::Vec3b>(i,j)[0]=255;
image.at<cv::Vec3b>(i,j)[1]=255;
image.at<cv::Vec3b>(i,j)[2]=255;
}
}
}
```
## 2.1.2 实现原理
利用cv::Mat的at(int x,int y)方法尅访问元素，其中x是行号，y是列号。在编译的时候必须明确返回值的类型，因为cv::Mat可以接受**任何类型的元素**，所以我们在开发的时候需要指定返回值的预期类型。正因为如此，at方法被实现成一个模板方法，在调用at方法时，你必须制定元素的类型，例如:
```
image.at<uchar>(i,j)=255;
...
image.at<cv::Vec3b>(i,j)[0]=255;
image.at<cv::Vec3b>(i,j)[1]=255;
image.at<cv::Vec3b>(i,j)[2]=255;
```
**程序员必须保证指定的类型与矩阵内的类型是一致的。at方法不会进行任何类型转换，而且OpenCV存储通道数据的次序是蓝色、绿色和红色。因此蓝色是通道0**
## 2.1.3 cv::Mat_模板类
cv::Mat类的定义采用了C++模板，因此它的通用性很强。如果每次调用都必须在模板参数中指明返回类型，cv::Mat类的at方法有时会显得冗长。如果已经知道矩阵的类型，就可以用cv::Mat_类（**cv::Mat类的模板子类**）。cv::Mat_类定义了一些新的方法，但没有定义新的数据属性，因此这两个类的指针或引用可以直接相互转换。新方法中有一个operator()，可以用来直接访问矩阵的元素。因此可以这样写代码。
```
//用Mat模板操作图像
cv::Mat_<uchar> img(image);
img(50,100)=0;//访问第50行，第100列处那个值
```
在创建cv::Mat_变量时，我们就定义了它的元素类型，因此在编译时就已经知道了operator()的返回类型。使用操作符operator()和使用at方法产生的结果是完全相同的。






