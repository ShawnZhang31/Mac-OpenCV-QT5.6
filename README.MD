# 3.2 用GrabCut算法分割图像
物体通常都有自己特有的颜色，通过识别颜色接近的区域，通常可以提取这些颜色。OpenCV提供了一种常用的图像分割算法，即**GrabCut**算法。GrabCut 算法比较复杂，计算量也很大，但结果通常很精确。如果要 从静态图像中提取前景物体(例如从图像中剪切一个物体，并粘贴到另一幅图像)，最好采用 GrabCut 算法。
## 3.2.1 如何实现
```
int main(int argc, char *argv[])
{

//读取输入的图像
cv::Mat image=loadFromQrc (":/test/testImages/Chapter02/111.jpeg");

cv::Rect rectangle(300,300,400,400);
cv::Mat result; //分割结果
cv::Mat bgModel,fgModel; // 模型

//GrabCut分割算法
cv::grabCut (image, // 输入图像
result, //分割结果
rectangle, //包含前景的矩形
bgModel,fgModel, //模型
5, //迭代次数
cv::GC_INIT_WITH_RECT); //使用矩形

//取得标记为可能属于前景的像素
cv::compare (result,cv::GC_PR_FGD,result,cv::CMP_EQ);

//生成输出图像
cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
image.copyTo (foreground,result);

cv::imshow ("image",image);
cv::imshow ("result",result);

cv::waitKey (0);
return 0;

}
```
## 3.2.2 实现原理
在前面的例子中，只需要指定一个包含前景物体的矩形，GrabCut算法就能提取出它。此外，还可以把输入图像中的几个特定像素赋值为cv::GC_BGD和cv::GC_FGD，以掩码图像的形式提供这些值，作为 cv::grabCut 函数的第二个参数。同时要把输入模式标志指定为GC_INIT_WITH_MASK。获得这些输入标签的方法有很多种，例如可以提示用户在图像中交互式地标记一些元素。当然，将这两种输入模式结合使用也未尝不可。
利用输入信息，GrabCut 算法通过以下步骤进行背景/前景分割。首先，把所有未标记的像素 临时标为前景(cv::GC_PR_FGD)。基于当前的分类情况，算法把像素划分为多个颜色相似的组 (即 K 个背景组和 K 个前景组)。下一步是通过引入前景和背景像素之间的边缘，确定背景/前景 的分割，这将通过一个优化过程来实现。在此过程中，将试图连接具有相似标记的像素，并且避 免边缘出现在强度相对均匀的区域。使用 Graph Cuts 算法可以高效地解决这个优化问题，它寻找 最优解决方案的方法是:把问题表示成一幅连通的图形，然后在图形上进行切割，以形成最优的 形态。分割完成后，像素会有新的标记。然后重复这个分组过程，找到新的最优分割方案，如此 反复。因此，GrabCut 算法是一个逐步改进分割结果的迭代过程。根据场景的复杂程度，找到最
佳方案所需的迭代次数各不相同(如果情况简单，迭代一次就足够了)。












