 
# Mac+QT5+OpenCV

[toc]

## Mac上OpenCV的安装
1. 首先检查OpenCV是否安装成功
```
brew info opencv
```
2. 安装OpenCV必须的依赖工具Cmake
```
brew install cmake
```
3. 安装OpenCV
```
brew tap homebrew/science
brew install opencv
```
## OpenCV在QT中的配置
1. 查看OpenCV库文件的路径  
可以使用如下命令来查看安装的OpenCV在Mac中环境变量
```
pkg-config --cflags --libs opencv
```
使用如下命令，可以查看当前安装的所有库
```
pkg-config --list-all
```
如我的电脑查看到的OpenCV库文件的路径如下
```
-I/usr/local/Cellar/opencv/3.3.0_3/include/opencv -I/usr/local/Cellar/opencv/3.3.0_3/include -L/usr/local/Cellar/opencv/3.3.0_3/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_ml -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core
```
2. 配置在QT的工程设置中
```
#OpenCV库文件
INCLUDEPATH +=-I/usr/local/Cellar/opencv/3.3.0_3/include
INCLUDEPATH +=-I/usr/local/Cellar/opencv/3.3.0_3/include/opencv
INCLUDEPATH +=/usr/local/Cellar/opencv/3.3.0_3/include/opencv2

LIBS += -L/usr/local/Cellar/opencv/3.3.0_3/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_ml -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core

```
## 版本说明
1. ImageProcessing  
    图像处理的相关函数的综合练习