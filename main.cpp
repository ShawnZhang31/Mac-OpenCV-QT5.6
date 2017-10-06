#include "mainwindow.h"
#include <QApplication>

#include<QString>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle (QObject::tr ("图像处理"));
    w.show();

    return a.exec();
}
