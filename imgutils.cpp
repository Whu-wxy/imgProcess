#include "imgutils.h"

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

    Mat img(src.cols*12, src.rows*12, CV_8UC1, Scalar(255, 255, 255));
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


//    Mat kernel = (Mat_<char>(51, 3) << 153,148,120,77,53,28,26,60,87,122,131,135,132,124,116,104,73,47,23,6,56,66,85,57,51,39,
//                  19,8,15,2,7,17,55,79,83,99,102,109,112,117,105,74,54,14,24,64,84,121,137,142,150,
//                  145,139,101,69,48,11,34,68,100,128,138,143,147,141,125,97,71,43,13,30,62,90,107,110,96,91,
//                  76,52,27,20,5,4,21,25,37,45,82,92,94,95,98,63,41,1,38,67,89,127,134,140,149,
//                  136,126,88,59,31,12,46,75,114,130,146,151,152,144,136,86,61,40,18,49,70,103,119,123,115,111,
//                  108,93,80,65,36,3,22,50,35,9,16,32,44,81,78,58,29,10,42,72,106,113,118,129,133);

    for(int i=0; i<img.rows; i++)
    {
        int k = i % kernel.rows;
        for(int j=0; j<img.cols; j++)
        {
            int l = j % kernel.cols;
            int pixelValue = ceil(src.at<uchar>(i/12, j/12) / 255.0*144);
            int kernelValue = kernel.at<uchar>(k,l);
            if(pixelValue > kernel.at<uchar>(k,l))
                img.at<char>(i, j) = 0;
            else
                img.at<char>(i, j) = 255;

        }
    }


    Mat roi = img(Rect(12*50, 12*50, 12, 12));
    return img;
}
