#include "imgutils.h"
#include <QPixmap>

void showMatImage(Mat image, QString win_name, int width, int height)
{
    namedWindow(win_name.toStdString(), WINDOW_NORMAL);
    resizeWindow(win_name.toStdString(), width, height);
    imshow(win_name.toStdString(), image);
}

cv::Mat QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage MatToQImage(cv::Mat mtx)
{
    switch (mtx.type())
    {
    case CV_8UC1:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
        return img;
    }
        break;
    case CV_8UC3:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 3, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
        break;
    case CV_8UC4:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
        return img;
    }
        break;
    default:
    {
        QImage img;
        return img;
    }
        break;
    }
}


Mat convolution(Mat src, Mat kernel, bool bPadding
                 , bool bToGray, bool useCVConv, bool compare2CVConv)
{
    if(src.data == NULL)
        return src;

    if(bToGray)
        cvtColor(src, src, CV_BGR2GRAY);
    int kernelH = kernel.rows;
    int kernelW = kernel.cols;

    if(compare2CVConv)
    {
        Mat compareRes;
        filter2D(src,compareRes,src.depth(),kernel, Point(-1,-1), 0, BORDER_DEFAULT); //BORDER_ISOLATED
        namedWindow("compareRes");
        imshow("compareRes", compareRes);
    }

    if(useCVConv)
    {
        filter2D(src, src ,src.depth(),kernel, Point(-1,-1), 0, BORDER_DEFAULT); //BORDER_ISOLATED
        return src;
    }

    Mat pdSrc;
    if(bPadding)
    {
        copyMakeBorder(src, pdSrc, kernelH/2, kernelH/2, kernelW/2, kernelW/2, BORDER_CONSTANT, cvScalarAll(0));
    }
    else
    {
        pdSrc = src.clone();
    }

    //Conv
    if(bToGray)
    {
        for(int i=kernelH/2; i<pdSrc.rows-kernelH/2; i++)
        {
            for(int j=kernelW/2; j<pdSrc.cols-kernelW/2; j++)
            {
                int value = 0;
                for(int a = -kernelH/2; a<=kernelH/2; a++)
                {
                    for(int b = -kernelW/2; b<=kernelW/2; b++)
                    {
                        value += pdSrc.at<uchar>(i+a, j+b) * kernel.at<char>(a+kernelH/2, b+kernelW/2);
                    }
                }
                if(value>255)
                    value=255;
                if(value<0)
                    value=0;
                if(bPadding)
                    src.at<uchar>(i-kernelH/2, j-kernelW/2) = value;
                else
                    src.at<uchar>(i, j) = value;

            }
        }
    }
    else
    {
        for(int i=kernelH/2; i<pdSrc.rows-kernelH/2; i++)
        {
            for(int j=kernelW/2; j<pdSrc.cols-kernelW/2; j++)
            {
                int valueB = 0;
                int valueG = 0;
                int valueR = 0;
                for(int a = -kernelH/2; a<=kernelH/2; a++)
                {
                    for(int b = -kernelW/2; b<=kernelW/2; b++)
                    {
                        valueB += pdSrc.at<Vec3b>(i+a, j+b)[0] * kernel.at<char>(a+kernelH/2, b+kernelW/2);
                        valueG += pdSrc.at<Vec3b>(i+a, j+b)[1] * kernel.at<char>(a+kernelH/2, b+kernelW/2);
                        valueR += pdSrc.at<Vec3b>(i+a, j+b)[2] * kernel.at<char>(a+kernelH/2, b+kernelW/2);
                    }
                }
                if(valueR>255)
                    valueR=255;
                if(valueR<0)
                    valueR=0;
                if(valueG>255)
                    valueG=255;
                if(valueG<0)
                    valueG=0;
                if(valueB>255)
                    valueB=255;
                if(valueB<0)
                    valueB=0;

                if(bPadding)
                {
                    src.at<Vec3b>(i-kernelH/2, j-kernelW/2)[0] = valueB;
                    src.at<Vec3b>(i-kernelH/2, j-kernelW/2)[1] = valueG;
                    src.at<Vec3b>(i-kernelH/2, j-kernelW/2)[2] = valueR;
                }
                else
                {
                    src.at<Vec3b>(i, j)[0] = valueB;
                    src.at<Vec3b>(i, j)[1] = valueG;
                    src.at<Vec3b>(i, j)[2] = valueR;
                }

            }
        }
    }

    return src;
}

Mat screen0(Mat src)
{
    if(src.channels() != 1)
        cvtColor(src, src, CV_RGB2GRAY);

    Mat img(src.rows*12, src.cols*12, CV_8UC1, Scalar(255, 255, 255));
    Mat kernel = (Mat_<uchar>(12, 12) <<144,140,132,122,107,63,54,93,106,123,133,142,
                  143,137,128,104,94,41,31,65,98,116,120,139,
                  135,131,114,97,61,35,24,55,80,103,113,125,
                  126,117,88,83,56,29,15,51,68,90,99,111,
                  109,100,81,77,48,22,8,28,47,76,85,96,
                  91,44,16,12,9,3,5,21,25,33,37,73,
                  59,58,30,18,10,1,2,4,11,19,34,42,
                  92,64,57,52,26,6,7,14,32,46,53,74,
                  101,95,70,67,38,13,20,36,50,75,82,108,
                  121,110,86,78,45,17,27,39,69,79,102,119,
                  134,129,112,89,49,23,43,60,71,87,115,127,
                  141,138,124,118,66,40,62,72,84,105,130,136);

    for(int i=0; i<img.rows; i++)
    {
        int k = i % kernel.rows;
        for(int j=0; j<img.cols; j++)
        {
            int l = j % kernel.cols;
            int pixelValue = ceil(src.at<uchar>(i/12, j/12) / 255.0*144);
            if(pixelValue <= kernel.at<uchar>(k,l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;

//            img.at<char>(i, j) = kernel.at<uchar>(k,l);

        }
    }


    Mat roi = img(Rect(12*50, 12*50, 24, 24));
    return img;
}

Mat screen45(Mat src)
{
    if(src.channels() != 1)
        cvtColor(src, src, CV_RGB2GRAY);

    Mat img(src.rows*12, src.cols*12, CV_8UC1, Scalar(255, 255, 255));
    Mat kernel = (Mat_<uchar>(8, 16) <<128,120,109,92,74,66,46,8,15,10,64,79,97,111,122,127,
                  123,116,87,69,62,38,6,39,42,3,19,55,86,105,115,119,
                  107,96,71,59,24,12,28,52,63,47,20,1,58,95,108,112,
                  84,73,56,2,18,23,48,78,82,67,35,5,31,61,91,101,
                  77,53,32,4,25,43,75,85,100,89,60,30,9,34,68,80,
                  51,41,21,27,40,70,94,102,110,103,93,57,26,11,37,65,
                  44,29,33,45,72,90,104,121,117,114,106,88,54,17,13,16,
                  14,36,49,76,83,98,118,126,125,124,113,99,81,50,22,7);

    for(int i=0; i<img.rows; i++)
    {
        int k = i % kernel.rows;
        int t=0;
        if(i/kernel.rows % 2 == 0)
            t = 0;
        else
            t = kernel.rows;
        for(int j=0; j<img.cols; j++)
        {
            int l = (j%img.cols + t) % kernel.cols;
            int pixelValue = ceil(src.at<uchar>(i/12, j/12) / 255.0*128);
            if(pixelValue <= kernel.at<uchar>(k,l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;

//            img.at<char>(i, j) = kernel.at<uchar>(k,l);

        }
    }


    Mat roi = img(Rect(12*50, 12*50, 24, 24));
    return img;
}


Mat screen75(Mat src)
{
  //  showMatImage(src, "before");
    if(src.channels() != 1)
        cvtColor(src, src, CV_RGB2GRAY);

    Mat img(src.rows*12, src.cols*12, CV_8UC1, Scalar(255, 255, 255));
    Mat kernel = (Mat_<uchar>(3, 51) << 153,148,120,77,53,28,26,60,87,122,131,135,132,124,116,104,73,47,23,6,56,66,85,57,51,39,
                  19,8,15,2,7,17,55,79,83,99,102,109,112,117,105,74,54,14,24,64,84,121,137,142,150,
                  145,139,101,69,48,11,34,68,100,128,138,143,147,141,125,97,71,43,13,30,62,90,107,110,96,91,
                  76,52,27,20,5,4,21,25,37,45,82,92,94,95,98,63,41,1,38,67,89,127,134,140,149,
                  136,126,88,59,31,12,46,75,114,130,146,151,152,144,136,86,61,40,18,49,70,103,119,123,115,111,
                  108,93,80,65,36,3,22,50,35,9,16,32,44,81,78,58,29,10,42,72,106,113,118,129,133);

    int q=4;
    for(int i=0; i<img.rows; i++)
    {
        int k = i % kernel.rows;
        int t = img.cols-(q*kernel.rows*(i/kernel.rows)) % img.cols;
        for(int j=0; j<img.cols; j++)
        {
            int l = (j%img.cols + t) % kernel.cols;
            int pixelValue = ceil(src.at<uchar>(i/12, j/12) / 255.0*153);
            if(pixelValue <= kernel.at<uchar>(k,l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;

        //    img.at<char>(i, j) = kernel.at<uchar>(k,l);

        }
    }


    Mat roi = img(Rect(12*50, 12*50, 24, 24));
    return img;
}


Mat screen15(Mat src)
{
    if(src.channels() != 1)
        cvtColor(src, src, CV_RGB2GRAY);

    Mat img(src.rows*12, src.cols*12, CV_8UC1, Scalar(255, 255, 255));
    Mat kernel = (Mat_<uchar>(3, 51) << 153,145,136,117,95,81,8,52,93,104,97,86,77,69,59,54,41,29,7,5,36,23,13,18,26,34,
                  46,64,67,72,79,25,50,66,90,103,122,128,130,137,134,118,102,82,16,51,96,115,132,147,152,
                  148,139,126,105,98,78,15,27,80,73,71,61,53,48,31,14,1,10,17,4,3,6,30,49,60,68,
                  75,84,89,106,83,37,35,85,107,119,131,138,146,142,140,129,109,92,32,39,91,111,124,141,144,
                  120,101,88,74,63,58,2,20,65,47,43,40,28,11,12,24,38,42,55,21,22,56,62,70,87,100,
                  114,121,127,113,99,45,9,57,110,123,135,143,151,150,149,133,112,94,44,19,76,108,116,125,136);

    int q=4;
    for(int i=0; i<img.rows; i++)
    {
        int k = i % kernel.rows;
        int t = q*kernel.rows*(i/kernel.rows) % img.cols;
        for(int j=0; j<img.cols; j++)
        {
            int l = (j%img.cols + t) % kernel.cols;
            int pixelValue = ceil(src.at<uchar>(i/12, j/12) / 255.0*153);
            if(pixelValue <= kernel.at<uchar>(k,l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;

//            img.at<char>(i, j) = kernel.at<uchar>(k,l);

        }
    }


    Mat roi = img(Rect(12*50, 12*50, 24, 24));
    return img;
}

Mat FrequenceM  (Mat src)
{
    if (src.channels() != 1)
            cvtColor(src, src, CV_RGB2GRAY);

        Mat img(src.rows * 12, src.cols * 12, CV_8UC1, Scalar(255, 255, 255));
        Mat pix = Mat::zeros(img.size(), CV_8UC1);
        double A1 = 7.0 / 16;
        double A2 = 3.0 / 16;
        double A3 = 5.0 / 16;
        double A4 = 1.0 / 16;

        for (int i = 0; i<img.rows; i++)
        {
            for (int j = 0; j<img.cols; j++)
            {
                pix.at<uchar>(i, j) = src.at<uchar>(i / 12, j / 12) * 144.0 / 255 + 0.5;
            }
        }
        float nError;
        for (int i = 0; i<img.rows - 1; i++)
        {
            for (int j = 1; j<img.cols - 1; j++)
            {
                if (pix.at<uchar>(i, j) <= 72)
                {
                    nError = pix.at<uchar>(i, j);
                    img.at<uchar>(i, j) = 0;
                }
                else
                {
                    nError = pix.at<uchar>(i, j) - 144;
                    img.at<uchar>(i, j) = 255;
                }
                pix.at<uchar>(i, j + 1) += nError*A1;
                pix.at<uchar>(i + 1, j - 1) += nError*A2;
                pix.at<uchar>(i + 1, j) += nError*A3;
                pix.at<uchar>(i + 1, j + 1) += nError*A4;

            }
        }
        Mat roi = img(Rect(12 * 50, 12 * 50, 24, 24));
        return img;
}

Mat ErrR(Mat src)
{
    if (src.channels() != 1)
        cvtColor(src, src, CV_RGB2GRAY);

    Mat img(src.rows * 8, src.cols * 8, CV_8UC1, Scalar(255, 255, 255));//放大4倍
    Mat kernel = (Mat_<uchar>(8, 8) <<
        0, 32, 8, 40, 2, 34, 10, 42,
        48, 16, 56, 42, 50, 18, 58, 26,
        12, 44, 4, 36, 14, 46, 6, 38,
        60, 28, 52, 20, 62, 30, 54, 22,
        3, 35, 11, 43, 1, 33, 9, 41,
        51, 19, 59, 27, 49, 17, 57, 25,
        15, 47, 7, 39, 13, 45, 5, 37,
        63, 31, 55, 23, 61, 29, 53, 21);

    int q = 8;

    for (int i = 0; i<img.rows; i++)
    {
        float k = i % q;
        //int t = q*kernel.rows*(i/kernel.rows) % img.cols;
        for (int j = 0; j<img.cols; j++)
        {
            // int l = (j%img.cols + t) % kernel.cols;
            float l = j % q;

            int pixelValue = ceil(src.at<uchar>(i/q, j/q) / 255.0 * 64);

            if (pixelValue <= kernel.at<uchar>(k, l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;
        }
    }
    Mat roi = img(Rect(12 * 50, 12 * 50, 24, 24));
    return img;

}


double getPSNR(const Mat& I1, const Mat& I2) {
    Mat s1;
    absdiff(I1, I2, s1);
    s1.convertTo(s1, CV_32F);
    s1 = s1.mul(s1);
    Scalar s = sum(s1);
    double sse = s.val[0] + s.val[1] + s.val[2];
    if (sse <= 1e-10)
        return 0;
    else {
        double mse = sse / (double)(I1.channels()*I1.total());
        double psnr = 10.0*log10(255 * 255 / mse);
        return psnr;
    }

}

Scalar getMSSIM(const Mat& i1, const Mat& i2) {
    const double C1 = 6.5025, C2 = 58.5225;
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);
    i2.convertTo(I2, d);
    Mat I2_2 = I2.mul(I2);     // I2^2
    Mat I1_2 = I1.mul(I1);      //I1^2
    Mat I1_I2 = I1.mul(I2);     // I1*I2

    Mat mu1, mu2;
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1_mu2 = mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);

    Mat ssim_map;
    divide(t3, t1, ssim_map);
    Scalar mssim = mean(ssim_map);
    return mssim;
}

uchar minimum(uchar a, uchar b)
{
    return a <= b ? a : b;
}

cv::Mat rgb2cmyk(cv::Mat& rgb)
{
    cv::Mat cmyk = cv::Mat::zeros(rgb.rows, rgb.cols, CV_8UC4);
    int pixel_num = rgb.rows * rgb.cols;
    for (int i = 0; i < pixel_num; i++)
    {
        uchar c = 255 - rgb.data[3 * i + 0];
        uchar m = 255 - rgb.data[3 * i + 1];
        uchar y = 255 - rgb.data[3 * i + 2];
        uchar K = minimum(minimum(c, m), y);
        uchar C = 0;
        uchar M = 0;
        uchar Y = 0;
        if (K == 255)
        {
            C = 0;
            M = 0;
            Y = 0;
        }
        else
        {
            C = (uchar)((c - K)*255.0 / (255 - K));
            M = (uchar)((m - K)*255.0 / (255 - K));
            Y = (uchar)((y - K)*255.0 / (255 - K));
        }
        cmyk.data[4 * i + 0] = C;
        cmyk.data[4 * i + 1] = M;
        cmyk.data[4 * i + 2] = Y;
        cmyk.data[4 * i + 3] = K;
    }
    return cmyk;
}


cv::Mat bgr2cmyk(cv::Mat& bgr)
{
    Mat rgb;
    cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
   // showMatImage(rgb, "rgb");
    cv::Mat cmyk = cv::Mat::zeros(rgb.rows, rgb.cols, CV_8UC4);
    int pixel_num = rgb.rows * rgb.cols;
    for (int i = 0; i < pixel_num; i++)
    {
        uchar c = 255 - rgb.data[3 * i + 0];
        uchar m = 255 - rgb.data[3 * i + 1];
        uchar y = 255 - rgb.data[3 * i + 2];
        uchar K = minimum(minimum(c, m), y);
        uchar C = 0;
        uchar M = 0;
        uchar Y = 0;
        if (K == 255)
        {
            C = 0;
            M = 0;
            Y = 0;
        }
        else
        {
            C = (uchar)((c - K)*255.0 / (255 - K));
            M = (uchar)((m - K)*255.0 / (255 - K));
            Y = (uchar)((y - K)*255.0 / (255 - K));
        }
        cmyk.data[4 * i + 0] = C;
        cmyk.data[4 * i + 1] = M;
        cmyk.data[4 * i + 2] = Y;
        cmyk.data[4 * i + 3] = K;
    }
    return cmyk;
}



Mat bgr2gray(Mat src, bool average)
{
    Mat dst(src.rows, src.cols, CV_8UC1);

    float R, G, B;
    for (int y = 0; y < src.rows; y++)
    {
        uchar* data = dst.ptr<uchar>(y);
        for (int x = 0; x < src.cols; x++)
        {
            B = src.at<Vec3b>(y, x)[0];
            G = src.at<Vec3b>(y, x)[1];
            R = src.at<Vec3b>(y, x)[2];
            if(average)
                data[x] = (int)(R + G + B )/3;
            else
                data[x] = (int)(R * 0.299 + G * 0.587 + B * 0.114);//利用公式计算灰度值（加权平均法）
        }
    }
    return dst;
}



bool processGeoJson(QString filePath, PolygonObjList& polys, int &maxX, int &maxY)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(jsonDoc.isEmpty())
        return false;

    double maxArea = 0;
    QRect maxRect(0,0,0,0);

    QJsonObject jsonRoot = jsonDoc.object();
    QJsonArray features = jsonRoot.value("features").toArray();
    for(int i=0; i<features.count(); i++)
    {

        QJsonObject feature = features.at(i).toObject();
        QJsonObject geometry = feature.value("geometry").toObject();
        QJsonObject properties = feature.value("properties").toObject();
        double color = properties.value("Segment").toDouble();

        QJsonArray polygons = geometry.value("coordinates").toArray();

        for(int j=0; j<polygons.count(); j++)
        {
            QJsonArray polygon = polygons.at(j).toArray();
            PolygonObject polyObj;
            for(int m=0; m<polygon.count(); m++)  //points
            {
                QJsonArray point = polygon.at(m).toArray();
                if(point.count() == 2)
                {
                    int x = point.at(0).toInt();
                    int y = point.at(1).toInt();
                    if(x > maxX)
                        maxX = x;
                    if(y>maxY)
                        maxY = y;
                    polyObj.polygon.append(QPoint(x, y));
                }
            }
            QRect boundRect = polyObj.polygon.boundingRect();
            polyObj.midPt = boundRect.center();
            if(boundRect.width()*boundRect.height() > maxArea)
            {
                maxArea = boundRect.width()*boundRect.height();
                maxRect = boundRect;
            }

            if(color == 255)
                continue;
            if(polyObj.polygon.count() >= 3)
                polys.append(polyObj);
        }
    }

    double aveHeight = 0.0;

    int count = polys.count();
    for(int j=0; j<count; j++)
    {
        double minArea = 9999999999;
        double maxArea = 0;
        double minArea2 = 9999999999;
        double maxArea2 = 0;
        int idx1=0, idx2=0, idx3=0, idx4=0;

        PolygonObject polyObj = polys.at(j);
        for(int i=0; i<polyObj.polygon.count(); i++)
        {
            QPoint pt = polyObj.polygon.at(i);
            double area = pt.x()+pt.y();
            if(area < minArea)
            {
                minArea = area;
                idx1=i;
            }
            if(area > maxArea)
            {
                maxArea = area;
                idx3=i;
            }
            //reset
            area = pt.x()+(maxRect.height()-pt.y());
            if(area < minArea2)
            {
                minArea2 = area;
                idx2=i;
            }
            if(area > maxArea2)
            {
                maxArea2 = area;
                idx4=i;
            }
        }
        //qDebug()<<idx1<<","<<idx2<<","<<idx3<<","<<idx4;
        polyObj.cornerPts.append(polyObj.polygon.at(idx1));
        polyObj.cornerPts.append(polyObj.polygon.at(idx2));
        polyObj.cornerPts.append(polyObj.polygon.at(idx3));
        polyObj.cornerPts.append(polyObj.polygon.at(idx4));

        double lAveX = 0.0, rAveX = 0.0;
        for(int m=idx1; m<=idx2; m++)  //pts
        {
            lAveX += polyObj.polygon.at(m).x();
        }
        lAveX = lAveX/(idx2-idx1+1);

        for(int m=idx3; m<=idx4; m++)  //pts
        {
            rAveX += polyObj.polygon.at(m).x();
        }
        rAveX = rAveX/(idx4-idx3+1);

        int lMidY = (polyObj.polygon.at(idx1).y()+polyObj.polygon.at(idx2).y()) / 2;
        int rMidY = (polyObj.polygon.at(idx3).y()+polyObj.polygon.at(idx4).y()) / 2;
        aveHeight += polyObj.polygon.at(idx2).y()-polyObj.polygon.at(idx1).y();
        aveHeight += polyObj.polygon.at(idx3).y()-polyObj.polygon.at(idx4).y();

        polyObj.LMidPt.setX(lAveX);
        polyObj.LMidPt.setY(lMidY);
        polyObj.RMidPt.setX(rAveX);
        polyObj.RMidPt.setY(rMidY);
        polyObj.horDis = rAveX-lAveX;

        polys.replace(j, polyObj);
    }
    aveHeight /= count*2;

    double lineY = polys.at(0).midPt.y();
    int idx1=0, idx2=0;
    for(int i=0; i<polys.count(); i++)
    {
        PolygonObject polyObj = polys.at(i);
        if(abs(polyObj.midPt.y()-lineY) > aveHeight/2.0 || i == polys.count()-1)
        {
            if(i == polys.count()-1)
                idx2++;
            sortPolygons(polys, idx1, idx2);
            qDebug()<<idx1<<","<<idx2;
            idx1 = idx2;
            lineY = polyObj.midPt.y();
        }

        idx2++;
    }

    return true;
}

void sortPolygons(PolygonObjList& polys, int idx1, int idx2)
{
    PolygonObjList sortPolys = polys.mid(idx1, idx2-idx1);
    for(int i=0; i<sortPolys.count(); i++)
    {
        for(int j=0; j<sortPolys.count()-i-1; j++)
        {
            if(sortPolys.at(j).midPt.x()>sortPolys.at(j+1).midPt.x())
            {
                sortPolys.swap(j+1, j);
            }
        }
    }
    for(int i=idx1,j=0; i<idx2; i++,j++)
    {
        polys.replace(i,sortPolys.at(j));
    }
}
