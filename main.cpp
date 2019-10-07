#include "imgprocess.h"
#include <QApplication>

#include "imgutils.h"

#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include "opencv2/highgui/highgui.hpp"

#include "iostream"

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgProcess w;
    w.show();



    QString srcPath = "F:\\Lena256.bmp";
    Mat src = imread(srcPath.toStdString());

    src = screen0(src);

    namedWindow("res", cv::WINDOW_NORMAL);
    imshow("res", src);


    waitKey(0);

    return a.exec();
}
