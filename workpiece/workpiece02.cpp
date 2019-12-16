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
Mat drawing;
Mat imghuoghcircle;
Mat colorhough;
Mat drawingline;
Mat drawinglinecolor;
Mat drawingthrd;
Mat drawinggray;
Mat drawingthrdcvt;

Mat drawingthrd_open;
Mat drawdialte;
Mat drawerode;
Mat imghori;
Mat imgveti;
Mat edges;
//select circle area to execute some specific erode ,in order to get correct circle

int thrdval;
int dilateval;
int houghparam2;
int n1 = 6;//horizon devide number
int n2 = 5;//verticle devide number
vector<Vec4i>lines;




void showimage(String winname,InputArray image)
{
    namedWindow(winname,CV_WINDOW_NORMAL);
    resizeWindow(winname,800,450);
    imshow(winname,image);
}

void on_convert()
{
    drawingthrdcvt =drawingthrd.clone();
    drawingthrdcvt = Mat::zeros(drawingthrd.size(),CV_8UC1);
    for (int i = 0; i < drawingthrd.rows; ++i)
    {
        for (int j = 0; j < imgerode.cols; ++j)
        {
            if(drawingthrd.at<float>(i,j) <= 1 )
                drawingthrdcvt.at<float>(i,j) = 255;
            if(drawingthrd.at<float>(i,j) >= 254 )
                drawingthrdcvt.at<float>(i,j) = 0;
        }
    }
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
    Mat dilatesize = getStructuringElement(MORPH_ELLIPSE, Size(70, 70));
    Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(70, 70));
    erode(drawing, imgerode, erodesize);
    dilate(imgerode, imgdilate, dilatesize);

//    HoughCircles(imgerode,pcircles,CV_HOUGH_GRADIENT,1,1000,100,houghparam2,100,1000);
    drawing = imgdilate;
//    GaussianBlur(drawing,drawing,3);
    vector<Vec3f> pcircles;
    HoughCircles(drawing,pcircles,CV_HOUGH_GRADIENT,1,1000,100,30,100,1000);
    imghuoghcircle=drawing.clone();
    cvtColor(imghuoghcircle,colorhough,CV_GRAY2BGR);
    for (size_t i = 0; i < pcircles.size(); i++)
    {
        Vec3f circles = pcircles[i];
        circle(colorhough,Point(circles[0],circles[1]),circles[2],Scalar(20,10,200),3,LINE_AA);
    }
    showimage("houghcircle",colorhough);
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
//    imshow("dilate",imgdilate);
}

void on_line()
{
    Mat kernelHori = getStructuringElement(MORPH_RECT, Size(imgerode.cols/20, 1), Point(-1, -1));
    erode(drawing,imghori,kernelHori,Point(-1,-1));
    dilate(drawing,imghori,kernelHori,Point(-1,-1));
//    morphologyEx(drawing, imgline, MORPH_OPEN, kernelHori,Point(-1,-1));
//    imshow("imgline",imgline);
    showimage("drawinghori",drawing);
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

////threshold
    threshold(imgsrc,imgthrd,18,255,CV_THRESH_BINARY);

    Mat dilatesize = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
    dilate(imgthrd, imgdilate, dilatesize);
    Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
    erode(imgdilate, imgerode, erodesize);
    showimage("imgerode",imgdilate);



//// extract contours
    drawing = Mat::zeros(imgthrd.size(), CV_8UC1);
    for (int k = 0; k <drawing.rows ; ++k)
    {
        uchar* data = drawing.ptr<uchar>(k);
        for (int i = 0; i < drawing.cols; ++i)
        {
            data[i] = 255;
        }
    }

    std::vector<std::vector<Point>> contours(5000);
    std::vector<Vec4i> hierarchy;
    findContours(imgerode,contours,hierarchy,CV_RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
    for (int i = 0; i < contours.size(); ++i)
    {
        drawContours(drawing, contours, i, Scalar(0,0,0), 2, 18, hierarchy, 0, Point());
    }
    showimage("drawing",drawing);
    threshold(drawing,drawingthrd,40,255,CV_THRESH_BINARY_INV);
    showimage("drawingthrd",drawingthrd);


//    showimage("threshold and dilate and erode",imgerode);
//    imgerodecvt = on_convert();
//    namedWindow("hough",CV_WINDOW_NORMAL);
//    resizeWindow("hough",800,450);
//    createTrackbar("value", "hough", &houghparam2, 255, on_houghcircle,0);
////extract cirecle
     on_houghcircle();

////extract line
//    Mat kernelHori = getStructuringElement(MORPH_RECT, Size(imgerode.cols/n, 1), Point(-1, -1));
//    morphologyEx(imgerode, imgline, MORPH_OPEN, kernelHori,Point(-1,-1));
//    showimage("imgline",imgline);

//    namedWindow("imgline",CV_WINDOW_NORMAL);
//    resizeWindow("imgline",800,450);
//    createTrackbar("value", "imgline", &n, 300, on_line,0);
//// on_line
//    on_line();
////HoughLine

////Canny
    //Canny(drawingthrd,edges,50,150);
//    showimage("edges",edges);

////    on_convert();
//    drawingthrdcvt = Mat::zeros(drawingthrd.size(),drawingthrd.type());
//    cout << drawingthrd.type() << endl;
//    for (int i = 0; i < drawingthrd.rows; ++i)
//    {
//
//        for (int j = 0; j < drawingthrd.cols; ++j)
//        {
//
//            //drawingthrdcvt.at<int>(i,j) = abs(255 - drawingthrd.at<int>(i,j));
////            if(drawingthrd.at<float>(i,j) == 0 )
////                drawingthrdcvt.at<float>(i,j) = 255;
////            if(drawingthrd.at<float>(i,j) == 255 )
////                drawingthrdcvt.at<float>(i,j) = 0;
//        }
//    }
    showimage("drawingthrd",drawingthrd);

////dilate and erode
    Mat drawkernel = getStructuringElement(MORPH_RECT, Size(10, 10));

    dilate(drawingthrd,drawdialte,drawkernel,Point(-1,-1));
    erode(drawdialte,drawerode,drawkernel,Point(-1,-1));


    drawingthrd_open = drawerode;
    imshow("drawingthrd_open",drawingthrd_open);

    HoughLinesP(drawingthrd_open, lines, 100, CV_PI / 180, 700, 100, 40);
    cvtColor(drawingthrd_open,drawinglinecolor,CV_GRAY2BGR);
    for (size_t i = 0; i < lines.size(); i++)
    {
//        cout << "contours_size = " << contours.size() << endl;
        Vec4i l = lines[i];
        if((l[0] > 5) && (l[2] > 5) && (l[0] < drawingthrd_open.rows-5) && (l[2] < drawingthrd_open.rows-5) && (l[1] > 5) && (l[3] > 5) && (l[1] < drawingthrd_open.cols-5) && (l[3] < drawingthrd_open.cols-5))
        {
            line(drawinglinecolor, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(18, 88, 255), 3, LINE_AA);
            cout << "contours_size = " << contours.size() << endl;
        }


//        line(drawinglinecolor, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);
    }
    imshow("imglinecolor",drawinglinecolor);
    waitKey(0);
    return 0;
}
