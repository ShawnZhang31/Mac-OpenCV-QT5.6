#ifndef LOADIMAGEFROMQRC_H
#define LOADIMAGEFROMQRC_H
#include<QString>
#include<shawnopencv.h>
#include<QFile>

class LoadImageFromQrc
{
public:
    LoadImageFromQrc();
    cv::Mat operator ()(QString qrc, int flag = cv::IMREAD_COLOR);
};

#endif // LOADIMAGEFROMQRC_H
