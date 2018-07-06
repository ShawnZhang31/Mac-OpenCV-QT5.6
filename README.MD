# 2.7 图像重映射
[实例代码](https://github.com/ShawnZhang31/Learning-OpenCV-QT5.6/tree/remap-image)
图像重映射不会修改图像的值，而是把每个图像的位置重新映射到新的位置。这可以用来创建图像特效，或者修正因镜片等原因造成的图像扭曲。
## 2.7.1 如何实现
使用OpenCV的remap函数。
```
void wave(const cv::Mat &image, cv::Mat &result)
{
//映射参数
cv::Mat srcX(image.rows,image.cols,CV_32F);
cv::Mat srcY(image.rows,image.cols,CV_32F);

for(int i=0;i<image.rows;i++)
{
for(int j=0;j<image.cols;j++)
{
//(i,j)像素的位置
srcX.at<float>(i,j)=j;//保持在同一列
srcY.at<float>(i,j)=i+5*sin(j/10.0);//原来在第i行修昂素，现在根据一个正玄曲线移动;

}
}

cv::remap (image,result,srcX,srcY,cv::INTER_LINEAR);
}
```