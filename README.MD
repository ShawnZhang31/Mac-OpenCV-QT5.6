#2.6 实现简单的图像运算
[实例代码]()
图像就是普通的矩阵，可以进行加、减、乘、除运算，因此可以进行多种方式组合图形。OpenCV提供了很多图像运算符。
## 2.6.1 准备工作
使用OpenCV的运算符进行图片叠加.
## 2.6.2 如何实现
两幅图像迭代可以使用cv::add函数来完成，如果要控制迭代的权重，可以使用cv::addWeighted函数来完成:
```
cv::addWeighted(image1,0.7,image2,0.9,0.0,result);
```
## 2.6.3 实现原理
所有二进制运算函数的用法都一样：提供两个输入参数，指定一个输出参数。cv::add是典型的多种格式的函数:
```
// c[i]=a[i]+b[i];
cv::add(imageA,imageB,resultC);
// c[i]=a[i]+k；
cv::add(imageA,cv::Scalar(k),resultC);
// c[i]=k1*a[i]+k2*b[i]+k3
cv::addWeighted(imageA,k1,imageB,k2,k3,resultC);
// c[i]=k*a[i]+bi;
cv::scaleAdd(imageA,k,imageB,resultC);
```
有些函数还可以指定一个掩码:
```
// if(mask[i])
// c[i]=a[i]+b[i];
cv::add(imageA,imageB,resultC,mask);
```
使用掩码后，操作就只会在掩码值非空的像素上执行（掩码必须是单通道的）。
在所有的场合都要使用cv::saturate_cast函数，以确保结果在预定的像素值范围内。