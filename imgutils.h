#ifndef IMGUTILS_H
#define IMGUTILS_H

#include <QImage>

#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include "opencv2/highgui/highgui.hpp"

#include "iostream"
#include <math.h>

using namespace std;
using namespace cv;

Mat convolution(Mat src, Mat kernel, bool bPadding = false,
                bool bToGray = false, bool useCVConv = false, bool compare2CVConv=true);

cv::Mat QImageToMat(QImage image);

QImage MatToQImage(cv::Mat mtx);

Mat screen0(Mat src);

Mat screen45(Mat src);

Mat screen15(Mat src);

Mat screen75(Mat src);

uchar minimum(uchar a, uchar b);

cv::Mat rgb2cmyk(cv::Mat& rgb);

cv::Mat bgr2cmyk(cv::Mat& bgr);

#endif // IMGUTILS_H
