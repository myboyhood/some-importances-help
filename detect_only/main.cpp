#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
#include<iostream>
#include<string>
using namespace std;
//输入图像
Mat img;
//灰度值归一化
Mat bgr;
//HSV图像
Mat hsv;
//色相
int hmin = 0;
int hmin_Max = 360;
int hmax = 360;
int hmax_Max = 360;
//饱和度
int smin = 0;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 106;
int vmin_Max = 255;
int vmax = 250;
int vmax_Max = 255;
//显示原图的窗口
string windowName = "src";
//输出图像的显示窗口
string dstName = "dst";
//输出图像
Mat dst;
int y_avg = 0;
int x_avg = 0;
int x_sum = 0;
int y_sum = 0;


//回调函数
void callBack(int, void*)
{
    //输出图像分配内存
    dst = Mat::zeros(img.size(), CV_32FC3);
    //掩码
    Mat mask;
    inRange(hsv, Scalar(340, 120 / float(smin_Max), 90 / float(vmin_Max)), Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
    //只保留
    for (int r = 0; r < bgr.rows; r++)
    {
        for (int c = 0; c < bgr.cols; c++)
        {
            if (mask.at<uchar>(r, c) == 255)
            {
                dst.at<Vec3f>(r, c) = bgr.at<Vec3f>(r, c);
            }
        }
    }
    //输出图像
    imshow(dstName, dst);
//图像转换
    Mat dst_convert;
    dst.convertTo(dst_convert, CV_8UC3, 255.0, 0);

//    cvtColor(dst,dst,COLOR_BGR2GRAY);
    //二值化后图像
    Mat threshold_result_binary;
    //阈值分割二值化
    threshold(dst_convert,threshold_result_binary,15,255,CV_THRESH_BINARY_INV);
    namedWindow("after_threshold",WINDOW_AUTOSIZE);
    imshow("after_threshold",threshold_result_binary);
    //二值图转灰度图
    Mat threshold_result_gray;
    cvtColor(threshold_result_binary,threshold_result_gray,COLOR_BGR2GRAY);

    //轮廓提取，原图像需要是8UC3
    std::vector<std::vector<Point>> contours(500);
    std::vector<Vec4i> hierarchy;
    findContours(threshold_result_gray, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    namedWindow("after_contours",WINDOW_AUTOSIZE);
    imshow("after_contours",threshold_result_gray);

    //提取轮廓
    Mat drawing = Mat::zeros(dst.size(), CV_8UC1);

    for (int k = 0; k <drawing.rows ; ++k)
    {
        uchar* data = drawing.ptr<uchar>(k);
        for (int i = 0; i < drawing.cols; ++i)
        {
            data[i] = 255;
        }
    }

    namedWindow("white_image",WINDOW_AUTOSIZE);
    imshow("white_image",drawing);

    for (int i = 0; i< contours.size(); i++)
    {
        cout << "contours_size = " << contours.size() << endl;
        drawContours(drawing, contours, i, Scalar(0,0,0), 1, 18, hierarchy, 0, Point());
        namedWindow("drawcontours",WINDOW_AUTOSIZE);
        imshow("drawcontours",drawing);
        double area = contourArea(Mat(contours[i]));
        cout << "area = " << area << endl;
        double min_area = 300;
        if(area > min_area && area < 10000)
        {
            for (int j = 0; j < contours[i].size(); ++j)
            {
                cout << "contours[i].size() = " << contours[i].size() <<endl;
                int x = contours[i][j].x;
                int y = contours[i][j].y;
                cout << "contours[i][j].x = " << x <<endl;
                cout << "contours[i][j].y = " << y <<endl;

                x_sum += x;
                y_sum += y;

            }

        }
        x_avg = x_sum/contours[1].size();
        y_avg = y_sum/contours[1].size();

    }
    //保存图像

//    imwrite("HSV_inRange.jpg", dst);
}
int main(int argc, char*argv[])
{
    //输入图像
    img = imread(argv[1], IMREAD_COLOR);
    if (!img.data || img.channels() != 3)
        return -1;
    imshow(windowName, img);
    //滤波
//    blur(img,img,Size(3,3));

    //彩色图像的灰度值归一化
    img.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
    //颜色空间转换
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    //定义输出图像的显示窗口
    namedWindow(dstName, WINDOW_GUI_EXPANDED);
    //调节色相 H
    createTrackbar("hmin", dstName, &hmin, hmin_Max, callBack);
    createTrackbar("hmax", dstName, &hmax, hmax_Max, callBack);
    //调节饱和度 S
    createTrackbar("smin", dstName, &smin, smin_Max, callBack);
    createTrackbar("smax", dstName, &smax, smax_Max, callBack);
    //调节亮度 V
    createTrackbar("vmin", dstName, &vmin, vmin_Max, callBack);
    createTrackbar("vmax", dstName, &vmax, vmax_Max, callBack);
    callBack(0, 0);


    cout <<"x_avg = " << x_avg << endl;
    cout <<"y_avg = " << y_avg << endl;
    waitKey(0);
    return 0;
}