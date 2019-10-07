#ifndef IMGUTILS_H
#define IMGUTILS_H

#include <QImage>

#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include "opencv2/highgui/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace cv;

Mat convolution(Mat src, Mat kernel, bool bPadding = false,
                bool bToGray = false, bool useCVConv = false, bool compare2CVConv=true);

cv::Mat QImageToMat(QImage image);

QImage MatToQImage(cv::Mat mtx);

Mat screen0(Mat src);



#endif // IMGUTILS_H
