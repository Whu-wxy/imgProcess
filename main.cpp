#include "imgprocess.h"
#include <QApplication>
#include <QDebug>

#include "imgutils.h"

#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include "opencv2/highgui/highgui.hpp"

#include "iostream"

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    ImgProcess w;
//    w.show();

    QList<QPolygon> polys;
    processGeoJson("F:\\imgs\\scan\\7.json", polys);
    qDebug()<<polys.count();

    return a.exec();
}
