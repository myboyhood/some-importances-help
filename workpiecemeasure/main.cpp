#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include<iostream>
#include<string>
using namespace std;
using namespace cv;
Mat imgsrc;
Mat imggray;
Mat imgthrd;
Mat imgdilate;
Mat imgerode;
Mat imgerodecvt;
Mat imghuoghcircle;
Mat colorhough;
Mat imageline;

//select circle area to execute some specific erode ,in order to get correct circle

int thrdval;
int dilateval;
int houghparam2;


void showimage(String winname,InputArray image)
{
    namedWindow(winname,CV_WINDOW_NORMAL);
    resizeWindow(winname,800,450);
    imshow(winname,image);
}

Mat on_convert()
{
    Mat imgcvt=imgerode.clone();
    imgcvt = Mat::zeros(imgerode.size(),CV_8UC1);
    for (int i = 0; i < imgerode.rows; ++i)
    {
        for (int j = 0; j < imgerode.cols; ++j)
        {
            if(imgerode.at<float>(i,j) <= 1 )
                imgcvt.at<float>(i,j) = 255;
            if(imgerode.at<float>(i,j) >= 255 )
                imgcvt.at<float>(i,j) = 0;
        }
    }
    return imgcvt;
}

void on_CornerHarris(InputArray image)
{
    Mat dstimage;
    Mat normimage;
    Mat scaledimage;
    dstimage = Mat::zeros(image.size(),CV_32FC1);
//    showimage(dstimage,"dstorigin");
    cornerHarris(image,dstimage,2,3,0.04,BORDER_DEFAULT);
    normalize(dstimage, normimage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(normimage, scaledimage);

//    ofstream cornervalue;
//    cornervalue.open("/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/cornervalue.txt");
    for (int i = 0; i < dstimage.rows; ++i)
    {
        for (int j = 0; j < dstimage.cols; ++j)
        {
//            cornervalue << dstimage.at<float>(i,j) << "    ";
            if(normimage.at<float>(i,j) > 80)
            {
                circle(dstimage,Point(i,j),3,Scalar(0,10,30),1,8,0);
                circle(scaledimage,Point(i,j),5,Scalar(0,10,30),1,8,0);
            }

        }
//        cornervalue << endl;
    }
//    cornervalue.close();
//    showimage(dstimage,"dstimage");
//    showimage(scaledimage,"scaledimage");
}

void on_houghcircle()
{
    vector<Vec3f> pcircles;
//    HoughCircles(imgerode,pcircles,CV_HOUGH_GRADIENT,1,1000,100,houghparam2,100,1000);
    HoughCircles(imgerode,pcircles,CV_HOUGH_GRADIENT,1,1000,100,20,100,1000);
    imghuoghcircle=imgerode.clone();
    cvtColor(imghuoghcircle,colorhough,CV_GRAY2BGR);
    for (size_t i = 0; i < pcircles.size(); i++)
    {
        Vec3f circles = pcircles[i];
        circle(colorhough,Point(circles[0],circles[1]),circles[2],Scalar(20,10,200),30,LINE_AA);
    }
    showimage("hough",colorhough);
}

void on_threshold(int,void*)
{
    threshold(imgsrc,imgthrd,thrdval,255,CV_THRESH_BINARY_INV);
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    dilate(imgthrd, imgdilate, element);
    imshow("threshold and dilate",imgdilate);

}

void on_dilate(int,void*)
{
//    Mat element = getStructuringElement(MORPH_RECT, Size(dilateval, dilateval));
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    dilate(imgthrd, imgdilate, element);
    imshow("dilate",imgdilate);
}

int main(int argc, char*argv[])
{
    std::cout << "Hello, World!" << std::endl;
    imgsrc = imread(argv[1],IMREAD_GRAYSCALE);
    if(!imgsrc.data||imgsrc.channels()!=1)
    {
        cvtColor(imgsrc,imggray,COLOR_BGR2GRAY);
        showimage("imggray",imggray);
    }

    showimage("imgsrc",imgsrc);
//    on_CornerHarris(imgsrc);
//    threshold(imgsrc,imgthrd,thrdval,255,CV_THRESH_BINARY);
//    showimage(imgthrd,"imgthrd");
//    namedWindow("threshold and dilate",CV_WINDOW_NORMAL);
//    resizeWindow("threshold and dilate",800,450);
//    createTrackbar("value", "threshold and dilate", &thrdval, 255, on_threshold,0);
//    namedWindow("dilate",CV_WINDOW_NORMAL);
//    resizeWindow("dilate",800,450);
//    createTrackbar("value", "dilate", &dilateval, 255, on_dilate,0);
    threshold(imgsrc,imgthrd,18,255,CV_THRESH_BINARY);
    Mat dilatesize = getStructuringElement(MORPH_RECT, Size(10, 10));
    dilate(imgthrd, imgdilate, dilatesize);
    Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
    dilate(imgdilate, imgerode, erodesize);
    showimage("threshold and dilate and erode",imgerode);
//    imgerodecvt = on_convert();
//    namedWindow("hough",CV_WINDOW_NORMAL);
//    resizeWindow("hough",800,450);
//    createTrackbar("value", "hough", &houghparam2, 255, on_houghcircle,0);
////extract cirecle
     on_houghcircle();

////extract line
    Mat kernelHori = getStructuringElement(MORPH_RECT, Size(20, 1), Point(-1, -1));
    morphologyEx(imgerode, imgline, MORPH_OPEN, kernelHori,Point(-1,-1));
    waitKey(0);
    return 0;
}
