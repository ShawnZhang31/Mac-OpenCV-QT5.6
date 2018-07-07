# 3.1 用策略设计模式比较颜色
[示例代码](https://github.com/ShawnZhang31/Learning-OpenCV-QT5.6/tree/strategy-design-pattern)
**策略设计模式**是一种面向对象的设计模式，用很巧妙的方法将算法封装。采用这种模式后，可以很轻松的替换掉算法，或者组合多个算法以实现更复杂的功能。而且这种模式能够尽可能地将算法的复杂性隐藏在一个直观的编程接口后面，更有利于算法的部署。
## 3.1.1 如何实现
一旦用策略设计模式把算法封装进类，就可以通过创建类的实例来部署算法，实例通常是在 程序初始化的时候创建的。在运行构造函数时，类的实例会用默认值初始化算法的各种参数，使 其立即进入可用状态。我们还可以用适当的方法来读写算法的参数值。在 GUI 应用程序中，可 以用多种部件(文本框、滑动条等)显示和修改参数，用户操作起来很容易。
**colordetect.h**
```
#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class ColorDetector
{
private:
// 允许的最小差距
int maxDist;
// 目标颜色
cv::Vec3b target;
// 处理后的结果
cv::Mat result;

int getDistanceToTargetColor(const cv::Vec3b &color) const;
int getColorDistance(const cv::Vec3b &color1, const cv::Vec3b &color2) const;
public:
ColorDetector();
/**
* @brief process:对图像执行颜色提取
* @param image:要进行处理的图像
*/
cv::Mat process(const cv::Mat &image);

int getMaxDist() const;
void setMaxDist(int value);
cv::Vec3b getTarget() const;

/**
* @brief ColorDetector::setTarget,设置目标颜色
* @param blue,B通道
* @param green,G通道
* @param red,R通道
*/
void setTarget(uchar blue, uchar green, uchar red);

ColorDetector(uchar blue, uchar green, uchar red, int distance);
};

#endif // COLORDETECTOR_H
```
**colordetect.cpp**
```
#include "colordetector.h"

/**
* @brief ColorDetector::getMaxDist
* @return
*/
int ColorDetector::getMaxDist() const
{
return maxDist;
}

/**
* @brief ColorDetector::setMaxDist
* @param value
*/
void ColorDetector::setMaxDist(int value)
{
if(value<=0)
maxDist=0;
maxDist = value;
}

/**
* @brief ColorDetector::getTarget
* @return
*/
cv::Vec3b ColorDetector::getTarget() const
{
return target;
}

/**
* @brief ColorDetector::setTarget,设置目标颜色
* @param blue,B通道
* @param green,G通道
* @param red,R通道
*/
void ColorDetector::setTarget(uchar blue, uchar green, uchar red)
{
target = cv::Vec3b(blue,green,red);
}

/**
* @brief ColorDetector::process
* @param image
* @return
*/
cv::Mat ColorDetector::process (const cv::Mat &image)
{
result.create (image.size(),CV_8U);

cv::Mat_<cv::Vec3b>::const_iterator it=image.begin<cv::Vec3b>();
cv::Mat_<cv::Vec3b>::const_iterator itend=image.end<cv::Vec3b>();
cv::Mat_<uchar>::iterator itout=result.begin<uchar>();

for(;it!=itend;++it,++itout)
{
//比较与目标颜色的差距
if(getDistanceToTargetColor (*it)<=maxDist)
{
*itout=255;
}
else
{
*itout=0;
}
}

return result;
}


/**
* @brief ColorDetector::getDistanceToTargetColor,获取与目标像素的差距
* @param color
* @return
*/
int ColorDetector::getDistanceToTargetColor(const cv::Vec3b& color) const
{
return getColorDistance (color,target);
}


/**
* @brief ColorDetector::getColorDistance
* @param color1
* @param color2
* @return
*/
int ColorDetector::getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const
{
return abs(color1[0]-color2[0])+
abs(color1[1]-color2[1])+
abs(color1[2]-color2[2]);
}

/**
* @brief ColorDetector::ColorDetector
*/
ColorDetector::ColorDetector():maxDist(0),target(0,0,0)
{
maxDist=0;
target=cv::Vec3b(0,0,0);
}

/**
* @brief ColorDetector::ColorDetector
* @param blue
* @param green
* @param red
* @param distance
*/
ColorDetector::ColorDetector(uchar blue, uchar green, uchar red, int distance)
{
if(distance<=0)
distance=0;
maxDist=distance;

target=cv::Vec3b(blue,green,red);

}



```
## 3.1.2 实现原理
```
result.create (image.size(),CV_8U);

cv::Mat_<cv::Vec3b>::const_iterator it=image.begin<cv::Vec3b>();
cv::Mat_<cv::Vec3b>::const_iterator itend=image.end<cv::Vec3b>();
cv::Mat_<uchar>::iterator itout=result.begin<uchar>();

for(;it!=itend;++it,++itout)
{
//比较与目标颜色的差距
if(getDistanceToTargetColor (*it)<=maxDist)
{
*itout=255;
}
else
{
*itout=0;
}
}

return result;
```
cv::Mat 类型的变量 image 表示输入图像，result 表示输出的二值图像。因此要先创建 迭代器，这样扫描循环就很容易实现了。注意，输入图像迭代器定义为常量，它们的元素无法修 改。在每个迭代步骤中计算当前像素的颜色与目标颜色的差距，检查它是否在公差(maxDist) 范围之内。如果是，就在输出图像中赋值 255(白色)，否则就赋值 0(黑色)。
也有其他可以计算这个差距的方法，例如计算包含 RGB 颜色值的三个向量之间的**欧几里得**距离。为了简化计算过程，我们把 RGB 值差距的绝对值(也称为**城区距离**)进行累加。注意，在现代体系结构中，浮点数的欧几里得距离的计算速度可能比简单的城区距离更快(还可以采用平方欧氏距离，以避免耗时的平方根运算)，在做设计时也要考虑到这点。    
一定要检查输出图像(包含二值映像)是否需要重新分配，以匹配输入 图像的尺寸。因此我们使用了 cv::Mat 的 create 方法。注意，只有在指定的尺寸或深度与当 前图像结构不匹配时，它才会进行重新分配。
## 3.1.3 扩展阅读
OpenCV 中定义了一个基类 cv::Algorithm，实现策略设计模式的概念。
1. 计算两个颜色向量间的距离
**城市距离**
```
return abs(color1[0]-color2[0])+
abs(color1[1]-color2[1])+
abs(color1[2]-color2[2]);

```
OpenCV中也有计算向量的**欧几里得范数**的函数，因此也可以这样计算距离:
```
return static_cast<int>(
cv::norm<int,3>(cv::Vec3i(color1[0]-color2[0],
color1[1]-color2[1],
color1[2]-color2[2])));
```
这里之所以使用 cv::Vec3i(三个向量的整型数组)，是因为减法运算得到的是整数值。
**还有一点非常有趣，回顾一下第2章的内容，我们会发现OpenCV中矩阵和向量等数据结构定义了基本的算术运算符。因此有人会想这样计算距离:**
```
return static_cast<int>( cv::norm<uchar,3>(color-target));// 错误!
```
**这种做法看上去好像是对的，但实际上是错误的，因为为了确保结果在输入数据类型的范围 之内(这里是 uchar)，这些运算符通常都调用了 saturate_cast(详情请参见 2.5节)。因此 在 target 的值比 color 大的时候，结果就会是 0 而不是负数。正确的做法应该是:**
```
cv::Vec3b dist;
cv::absdiff(color,target,dist);
return cv::sum(dist)[0];
```
**不过在计算三个数组间距离时调用这两个函数的效率并不高。**
2. 使用OpenCV函数
本节采用了在循环中使用迭代器的方法来进行计算。还有一种做法是调用 OpenCV 的系列函数，也能得到一样的结果。因此，检测颜色的方法还可以这样写:
```
cv::Mat ColorDetect::process(const cv::Mat &image)
{
cv::Mat output;
//计算与目标颜色的距离的绝对值
cv::absdiff(image,cv::Scalar(target),output);
//把通道分割进3副图像
std::vector<cv::Mat> images;
cv::split(output,images);
//3个通道相加(这里可能会出现饱和的情况)
output=images[0]+images[1]+images[2];
//用于阈值
cv::threshold(output,
output,
maxDist,
255,
cv::THRESH_BINARY_INV);
return output;
}
```
**一般来说，最好直接使用OpenCV函数。它可以快速建立复杂程序，减少潜在的错误，而且 程序的运行效率通常也比较高(得益于 OpenCV 项目参与者做的优化工作)。不过这样会执行很 多的中间步骤，消耗更多内存。**
3. floodFill函数
ColorDetector 类可以在一幅图像中找出与指定颜色接近的像素，它的判断方法是对像素 12 进行逐个检查。cv::floodFill 函数的做法与之类似，但有一个很大的区别，那就是它在判断一个像素时，还要检查附近像素的状态，这是为了识别某种颜色的相关区域。用户只需指定一个 起始位置和允许的误差，就可以找出颜色接近的连续区域。
首先根据亚像素确定搜寻的颜色，并检查它旁边的像素，判断它们是否为颜色接近的像素; 然后，继续检查它们旁边的像素，并持续操作。这样就可以从图像中提取出特定颜色的区域。
```
/**
* @brief ColorDetector::processWithFloodFill
* @param image
* @param startPoint
* @return
*/
cv::Mat ColorDetector::processWithFloodFill (const cv::Mat &image,cv::Point startPoint)
{
result = image.clone ();
cv::floodFill (result,
startPoint,
cv::Scalar(255,255,255),
(cv::Rect*)0,
cv::Scalar(35,35,35),
cv::Scalar(35,35,35),
cv::FLOODFILL_FIXED_RANGE);

return result;
}
```
4. 仿函数或函数对象
利用 C++的操作符重载功能，我们可以让类的实例表现得像函数。它的原理是重载 operator()方法，让调用类的处理方法就像调用纯粹的函数一样。这种类的实例被称为函数对 象或者**仿函数(functor)**。一个仿函数通常包含一个完整的构造函数，因此能够在创建后立即使 用。例如，可以在 ColorDetector 类中添加完整的构造函数:
重载实现方函数
```
cv::Mat operator()(const cv::Mat &image)
{
//这里放置颜色检验代码
}
```
若要用仿函数方法检测指定的颜色，只需要使用这样的代码片段:
```
cv::Mat result=cdetect(image); //调用方函数
```
5. OpenCV的算法基类
为实现计算机视觉的各项功能，OpenCV 提供了很多算法。为方便使用，大多数算法都被封 装成了通用基类**cv::Algorithm**的子类。这体现了策略设计模式的一些概念。首先，所有算法 都在专门的静态方法中动态地创建，以确保创建的算法总是有效的(即每个缺少的参数都有有效 的默认值)。




