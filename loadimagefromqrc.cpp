#include "loadimagefromqrc.h"

LoadImageFromQrc::LoadImageFromQrc()
{

}

/**
 * @brief LoadImageFromQrc::operator (),重载操作符实现从QRC加载图片
 * @param qrc
 * @param flag
 * @return
 */
cv::Mat LoadImageFromQrc::operator()(QString qrc,int flag)
{
    QFile file(qrc);
    cv::Mat ima;
    if(file.open (QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        ima = cv::imdecode(buf, flag);
    }
    return  ima;
}



