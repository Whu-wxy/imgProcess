#ifndef IMGUTILS_H
#define IMGUTILS_H

#include <QImage>
#include <QObject>

#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include "opencv2/highgui/highgui.hpp"

#include "iostream"
#include <math.h>

#include <QPolygon>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std;
using namespace cv;

Mat convolution(Mat src, Mat kernel, bool bPadding = false,
                bool bToGray = false, bool useCVConv = false, bool compare2CVConv=true);

void showMatImage(Mat image, QString win_name="image", int width=800, int height=600);

cv::Mat QImageToMat(QImage image);

QImage MatToQImage(cv::Mat mtx);

Mat screen0(Mat src);

Mat screen45(Mat src);

Mat screen15(Mat src);

Mat screen75(Mat src);

Mat FrequenceM(Mat src);
Mat ErrR(Mat src);

double getPSNR(const Mat& I1, const Mat& I2);
Scalar getMSSIM(const Mat& i1, const Mat& i2);

uchar minimum(uchar a, uchar b);

cv::Mat rgb2cmyk(cv::Mat& rgb);

cv::Mat bgr2cmyk(cv::Mat& bgr);

Mat bgr2gray(Mat src, bool average); //bgr2gray



typedef struct structPolygon
{
    QPolygon polygon;
    QPolygon cornerPts;
    QPoint   midPt;
    QPoint   LMidPt;
    QPoint   RMidPt;
    double   horDis;
}PolygonObject;

typedef QList<PolygonObject> PolygonObjList;

bool processGeoJson(QString filePath, PolygonObjList& polys, int &maxX, int &maxY); //解析geojson数据
void sortPolygons(PolygonObjList& polys, int idx1, int idx2);  //冒泡排序

#endif // IMGUTILS_H
