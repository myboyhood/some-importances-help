////  a = hight
////  b = width
////  e = hight from 90point to circlepoint
////  f = width from 90point to circlepoint
////  d = 2 * radius
////  aerfa = the angle between hight and width
//// }



#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <cv.h>


using namespace std;
using namespace cv;
vector<float> param;
int thrd_val,dilate_val,erode_val,cir_dilate_size,cir_erode_size,contour_linewidth,circle_minR,circle_maxR;
float scale_coeffi;
Mat imgsrc;

vector<Mat> imageshub;
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


int dilateval;
int houghparam2;
int n1 = 6;//horizon devide number
int n2 = 5;//verticle devide number
vector<Vec4i> lines;
vector<Vec4i> lls;
vector<Vec4i> linehoris;
vector<Vec4i> linevertis;
vector<Vec4i> lineslopes;

float circleRadius;
Point2f angle90;
Point2f toppoint;
Point2f rightpoint;
Point2f circlepoint;
vector<Point> slopepoint1;
vector<Point> slopepoint2;
int Yhori, Xverti;
int vertiflag = 0;
int horiflag = 0;
vector<float> ah, bh, mh;
vector<float> av, bv, mv;
vector<float> as, bs, ms;
float asavg = 0, bsavg = 0, msavg = 0;
float avavg = 0, bvavg = 0, mvavg = 0;
float ahavg = 0, bhavg = 0, mhavg = 0;

////READ CSV paramter
void csvParser(string filename, vector<float> &lineArray)
{
    ifstream in;
    in.open(filename);
    while(in.good())
    {   cout << "start reading param" << endl;
        string lineStr;
        while(getline(in,lineStr))
        {
            stringstream ss(lineStr);
            string str;
            while(getline(ss,str,','))
            {
                float num = stof(str);
                lineArray.push_back(num);
            }
        }
    }
    std::cout << "param read successful" << std::endl;
    in.close();
}

void on_measure(int i)
{
    float a,b,d,e,f,line1,line2,lo9,lobp;   //lo9 is the distance from circlepoint to angle90 point
    double c,aerfa,cos_theta_1,sin_theta_1;
    Point2f bp;       //lobp is distance from circlepoint to the point of bp
    line1 = abs(sqrt(pow((toppoint.y - angle90.y),2)+pow((toppoint.x - angle90.x),2)));
    line2 = abs(sqrt(pow((rightpoint.y - angle90.y),2)+pow((rightpoint.x - angle90.x),2)));
    if(line1 > line2)
    {
        a = line2;
        b = line1;
        bp = toppoint;
        ////calculate e f (circle_point to a line  and b line)
        cout << "ahvag " << ahavg <<"   bhavg " << bhavg << "   mhavg " << mhavg << endl;
        if(horiflag == 1)
        {
            e = abs(Yhori - circlepoint.y);
        }
        else
        {
            e = abs(ahavg*circlepoint.x+bhavg*circlepoint.y-mhavg)/sqrt(ahavg*ahavg+bhavg*bhavg);
        }

        if(vertiflag == 1)
        {
            f = abs(Xverti-circlepoint.x);
        }
        else
        {
            f = abs(avavg*circlepoint.x+bvavg*circlepoint.y-mvavg)/sqrt(avavg*avavg+bvavg*bvavg);
        }

    }
    else
        {
           a = line1;
           b = line2;
           bp = rightpoint;
            ////calculate e f (circle_point to a line  and b line)
            if(horiflag == 1)
            {
                f = abs(Yhori - circlepoint.y);
            }
            else
            {
                f = abs(ahavg*circlepoint.x+bhavg*circlepoint.y-mhavg)/sqrt(ahavg*ahavg+bhavg*bhavg);
            }

            if(vertiflag == 1)
            {
                e = abs(Xverti-circlepoint.x);
            }
            else
            {
                e = abs(avavg*circlepoint.x+bvavg*circlepoint.y-mvavg)/sqrt(avavg*avavg+bvavg*bvavg);
            }
        }

    lo9 = sqrt(pow((circlepoint.y - angle90.y),2)+pow((circlepoint.x - angle90.x),2));
    lobp = sqrt(pow((bp.y - circlepoint.y),2)+pow((bp.x - circlepoint.x),2));
    cos_theta_1 = (lo9*lo9 + b*b - lobp*lobp)/(2*b*lo9);
    sin_theta_1 = sqrt(1 - cos_theta_1*cos_theta_1);
//    e = lo9*cos_theta_1;
//    f = lo9*sin_theta_1;
    d = 2*circleRadius;
    c = sqrt(pow((toppoint.x-rightpoint.x),2)+pow((toppoint.y-rightpoint.y),2)); //the length of slope
    aerfa = acos((pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b));
    aerfa = aerfa*180/CV_PI;
    cout << "cos_theta_1 = " << cos_theta_1 << "   sin_theta_1 = " << sin_theta_1 << endl;
    cout << "c = " << c << "   aerfa = " << aerfa << endl;
    ofstream result;
    result.open("/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/result.txt",ios::app);
    result << "  " << endl;
    result << " the result of measurement "+ to_string(i) << endl;
    result << "a = " << a/scale_coeffi << endl;
    result << "b = " << b/scale_coeffi << endl;
    result << "aerfa = " << aerfa << endl;
    result << "d = " << d/scale_coeffi << endl;
    result << "e = " << e/scale_coeffi << endl;
    result << "f = " << f/scale_coeffi << endl;
    result.close();

}

void showimage(String winname, InputArray image)
{
    namedWindow(winname, CV_WINDOW_NORMAL);
    resizeWindow(winname, 800, 450);
    imshow(winname, image);
}

void on_convert() {
    drawingthrdcvt = drawingthrd.clone();
    drawingthrdcvt = Mat::zeros(drawingthrd.size(), CV_8UC1);
    for (int i = 0; i < drawingthrd.rows; ++i) {
        for (int j = 0; j < imgerode.cols; ++j) {
            if (drawingthrd.at<float>(i, j) <= 1)
                drawingthrdcvt.at<float>(i, j) = 255;
            if (drawingthrd.at<float>(i, j) >= 254)
                drawingthrdcvt.at<float>(i, j) = 0;
        }
    }
}

void on_CornerHarris(InputArray image) {
    Mat dstimage;
    Mat normimage;
    Mat scaledimage;
    dstimage = Mat::zeros(image.size(), CV_32FC1);
//    showimage(dstimage,"dstorigin");
    cornerHarris(image, dstimage, 2, 3, 0.04, BORDER_DEFAULT);
    normalize(dstimage, normimage, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(normimage, scaledimage);

//    ofstream cornervalue;
//    cornervalue.open("/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/cornervalue.txt");
    for (int i = 0; i < dstimage.rows; ++i) {
        for (int j = 0; j < dstimage.cols; ++j) {
//            cornervalue << dstimage.at<float>(i,j) << "    ";
            if (normimage.at<float>(i, j) > 80) {
                circle(dstimage, Point(i, j), 3, Scalar(0, 10, 30), 1, 8, 0);
                circle(scaledimage, Point(i, j), 5, Scalar(0, 10, 30), 1, 8, 0);
            }

        }
//        cornervalue << endl;
    }
//    cornervalue.close();
//    showimage(dstimage,"dstimage");
//    showimage(scaledimage,"scaledimage");
}

void on_houghcircle(int j) {
    ////dilate_size=5,erode_size=20
    Mat dilatesize = getStructuringElement(MORPH_ELLIPSE, Size(cir_dilate_size, cir_dilate_size));
    Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(cir_erode_size,cir_erode_size ));
    erode(drawing, imgerode, erodesize);
    dilate(imgerode, imgdilate, dilatesize);

//    HoughCircles(imgerode,pcircles,CV_HOUGH_GRADIENT,1,1000,100,houghparam2,100,1000);
    drawing = imgdilate;
//    GaussianBlur(drawing,drawing,3);
    vector<Vec3f> pcircles;
    HoughCircles(drawing, pcircles, CV_HOUGH_GRADIENT, 1, 1000, 100, 30, circle_minR, circle_maxR);
    imghuoghcircle = drawing.clone();
    cvtColor(imghuoghcircle, colorhough, CV_GRAY2BGR);
    for (size_t i = 0; i < pcircles.size(); i++) {
        Vec3f circles = pcircles[i];
        circle(colorhough, Point(circles[0], circles[1]), circles[2], Scalar(20, 10, 200), 3, LINE_AA);
        circlepoint.x = circles[0];
        circlepoint.y = circles[1];
        circleRadius = circles[2];
    }
    showimage("houghcircle"+to_string(j), colorhough);
}

//void on_threshold(int, void *) {
//    threshold(imgsrc01, imgthrd, thrdval, 255, CV_THRESH_BINARY_INV);
//    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
//    dilate(imgthrd, imgdilate, element);
//    imshow("threshold and dilate", imgdilate);
//
//}
//
//void on_dilate(int, void *) {
////    Mat element = getStructuringElement(MORPH_RECT, Size(dilateval, dilateval));
//    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
//    dilate(imgthrd, imgdilate, element);
////    imshow("dilate",imgdilate);
//}

void on_line() {
    Mat kernelHori = getStructuringElement(MORPH_RECT, Size(imgerode.cols / 20, 1), Point(-1, -1));
    erode(drawing, imghori, kernelHori, Point(-1, -1));
    dilate(drawing, imghori, kernelHori, Point(-1, -1));
//    morphologyEx(drawing, imgline, MORPH_OPEN, kernelHori,Point(-1,-1));
//    imshow("imgline",imgline);
    showimage("drawinghori", drawing);
}

//output the average of vector for ax+by = m
float on_avgabm(vector<float> &abm) {
    float abmsum = 0.0;
    float abmavg = 0.0;
    for (int n = 0; n < abm.size(); ++n) {
        abmsum += abm[n];
    }
    if(abm.empty())
        return 0.0;
    abmavg = abmsum / abm.size();
    return abmavg;
}

Point2f crosspoint_solver(float &a, float &b, float &c, float &d, float &m, float &n) {    //// ay+bx=m
    ////cy+dx=n
    ////y=(md-bn)/(ad-bc)
    ////x=(an-mc)/(ad-bc)
    Point2f crosspoint;
    crosspoint.x = (m * d - b * n) / (a * d - b * c);
    crosspoint.y = (a * n - m * c) / (a * d - b * c);
    return crosspoint;
}
void coutlines(vector<Vec4i> &coutlines)
{
    for (int n = 0; n < coutlines.size(); ++n)
    {
        cout << " x1= "<<coutlines[n][0] << "  y1= "<<coutlines[n][1]<<"    x2= "<<coutlines[n][2]<< "  y2= "<<coutlines[n][3] << endl;
    }
}

void read_images_in_folder(String pattern)
{
    vector<String> fn;
    glob(pattern,fn,false);
    size_t count = fn.size();
    for (int i = 0; i < count ; ++i)
    {
        imageshub.emplace_back(imread(fn[i]));
        //imshow("img", imread(fn[i]));
    }

}

int main(int argc, char *argv[])
{
    String paramname = "/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/param.txt";
    ifstream fin(paramname);
    if(!fin.good())
    {
        cout << "can not find param.txt, need to input package path first" << endl;
    }
    csvParser("/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/param.txt",param);
    thrd_val = param[0];
    dilate_val = param[1];
    erode_val = param[2];
    cir_dilate_size = param[3];
    cir_erode_size = param[4];
    contour_linewidth = param[5];
    circle_minR = param[6];
    circle_maxR = param[7];
    scale_coeffi = param[8];

    String pattern = "/home/wzy/softwares/CLion-2019.3/projects/workpiecemeasure/*.jpg";
    read_images_in_folder(pattern);

    //// for loop to calculate each image
    for (int j = 0; j < imageshub.size(); ++j)
    {
        cout << "j =  " << j << endl;
        imgsrc = imageshub[j];

        if (!imgsrc.data || imgsrc.channels() != 1)
        {
            cvtColor(imgsrc, imggray, COLOR_BGR2GRAY);
        //showimage("imggray"+to_string(j), imggray);
            imgsrc = imggray;
        }

        showimage("imgsrc"+to_string(j), imgsrc);

        ////threshold thresh(default) = 150;
        threshold(imgsrc, imgthrd, thrd_val, 255, CV_THRESH_BINARY);
        showimage("imgthrd"+to_string(j),imgthrd);

        ////03.jpg default dialate_val=10,erode_val=5;
        Mat dilatesize = getStructuringElement(MORPH_ELLIPSE, Size(dilate_val, dilate_val));
        dilate(imgthrd, imgdilate, dilatesize);
        Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(erode_val, erode_val));
        erode(imgdilate, imgerode, erodesize);
        //showimage("img_dilate&&erode", imgerode);

        //// extract contours
        drawing = Mat::zeros(imgthrd.size(), CV_8UC1);
        for (int k = 0; k < drawing.rows; ++k) {
            uchar *data = drawing.ptr<uchar>(k);
            for (int i = 0; i < drawing.cols; ++i) {
                data[i] = 255;
            }
        }
        std::vector<std::vector<Point>> contours(10000);
        std::vector<Vec4i> hierarchy;
        findContours(imgerode, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (int i = 0; i < contours.size(); ++i) {
            drawContours(drawing, contours, i, Scalar(0, 0, 0), contour_linewidth, 18, hierarchy, 0, Point());
        }
        threshold(drawing, drawingthrd, 40, 255, CV_THRESH_BINARY_INV);

        ////extract cirecle
        on_houghcircle(j);
        cout << "circleX = " << circlepoint.x << "  circleY = " << circlepoint.y << "  circleRadius = " << circleRadius << endl;

        ////dilate and erode
        ////01.bmp
        Mat drawkernel = getStructuringElement(MORPH_RECT, Size(20, 20));
        dilate(drawingthrd, drawdialte, drawkernel, Point(-1, -1));
        erode(drawdialte, drawerode, drawkernel, Point(-1, -1));
        drawingthrd_open = drawerode;
        ////03.jpg
        drawingthrd_open = drawingthrd;

        HoughLinesP(drawingthrd_open, lines, 100, CV_PI / 180, 800, 300, 40);

        cvtColor(drawingthrd_open, drawinglinecolor, CV_GRAY2BGR);
        cout << "rows = " << drawingthrd_open.rows << "   cols =  " << drawingthrd_open.cols << endl;

        for (size_t i = 0; i < lines.size(); i++) {
//        cout << "contours_size = " << contours.size() << endl;

            if ((lines[i][0] > 5) && (lines[i][2] > 5) && (lines[i][0] < drawingthrd_open.cols - 5) &&
                (lines[i][2] < drawingthrd_open.cols - 5) && (lines[i][1] > 5) && (lines[i][3] > 5) &&
                (lines[i][1] < drawingthrd_open.rows - 5) && (lines[i][3] < drawingthrd_open.rows - 5))
            {

                int x1, y1, x2, y2;
                x1 = lines[i][0];
                y1 = lines[i][1];
                x2 = lines[i][2];
                y2 = lines[i][3];
                cout << "x  " << x1 << " ,  " << x2 << "        y " << y1 << "  , " << y2 << endl;
                line(drawinglinecolor, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
                     Scalar(18, 88, 255), 7, LINE_AA);

                lls.push_back(lines[i]);
            }
        }
        cout << "lines_number = " << lls.size() << endl;
        for (auto & ll : lls)
        {   //vertical is 20 times longer than horizon ,so it is a vertical line
            if (abs(ll[1] - ll[3]) > abs(ll[0] - ll[2]) * 7)
                linevertis.push_back(ll);

            //horizon is 20 times longer than vertical ,so it is a horizon line
            if (abs(ll[0] - ll[2]) > abs(ll[1] - ll[3]) * 5)
                linehoris.push_back(ll);

            //calculate the ratio of (y1-y2)/(x1-x2) in range and enough length
            if ((abs(ll[1] - ll[3]) > 0.5* abs(ll[0] - ll[2])) &&
                (abs(ll[1] - ll[3]) <  4.0* abs(ll[0] - ll[2])) &&
                (abs(ll[0] - ll[2]) + abs(ll[1] - ll[3])) > 600)
                lineslopes.push_back(ll);

        }
        //// cout different kinds of lines
        cout << "linehoris" << endl;
        coutlines(linehoris);
        cout << "lineverti" << endl;
        coutlines(linevertis);
        cout << "lineslope" << endl;
        coutlines(lineslopes);


        ////horizon line
        //// initialize a b m
         asavg = 0; bsavg = 0; msavg = 0;
         avavg = 0; bvavg = 0; mvavg = 0;
         ahavg = 0; bhavg = 0; mhavg = 0;

         horiflag = 0;


        for (auto & linehori : linehoris)
        {
            int x1, x2, y1, y2;
            x1 = linehori[0];
            y1 = linehori[1];
            x2 = linehori[2];
            y2 = linehori[3];
            if (y1 == y2)
            {
                Yhori = y1;
                horiflag = 1;
            }
            if (y1 != y2)
            {
                //ax+by=m  first workout a b m for horizonline
                ah.push_back((y1 - y2)*1.0/(x1 - x2));
                bh.push_back(-1);
                mh.push_back(x1*1.0 * (y1 - y2)/(x1 - x2) - y1);

            }
        }
        ahavg = on_avgabm(ah);
        bhavg = on_avgabm(bh);
        mhavg = on_avgabm(mh);

        ////-----------------------------------------------vertical line----------------------------------------------------------
        vertiflag = 0;

        for (auto & lineverti : linevertis)
        {
            int x1, x2, y1, y2;
            x1 = lineverti[0];
            y1 = lineverti[1];
            x2 = lineverti[2];
            y2 = lineverti[3];
            if (x1 == x2) {
                Xverti = x1;
                vertiflag = 1;
            }
            if (x1 != x2) {
                //ax+by=m  first workout a b m for verticle line
                av.push_back((y1 - y2)*1.0/(x1 - x2));
                bv.push_back(-1);
                mv.push_back(x1*1.0 * (y1 - y2)/(x1 - x2) - y1);

            }
        }
        avavg = on_avgabm(av);
        bvavg = on_avgabm(bv);
        mvavg = on_avgabm(mv);

        ////slope line

//    Point point1,point2;
        for (int k = 0; k < lineslopes.size(); ++k)
        {
            int x1, x2, y1, y2;
            x1 = lineslopes[k][0];
            y1 = lineslopes[k][1];
            x2 = lineslopes[k][2];
            y2 = lineslopes[k][3];

            as.push_back((y1-y2)*1.0/(x1-x2));
            bs.push_back(-1);
            ms.push_back(x1*1.0 * (y1 - y2)/(x1 - x2) - y1);
//        cout << " as= " << as[k] << " bs= " << bs[k] << " ms= "<< ms[k] << endl;

        }
        asavg = on_avgabm(as);
        bsavg = on_avgabm(bs);
        msavg = on_avgabm(ms);
        cout << "vertiflag = " << vertiflag << "   horiflag = " << horiflag << endl;



////three conditions to calculate cross 90 point
        if (vertiflag == 1 && horiflag == 1) {
            angle90.x = Xverti;
            angle90.y = Yhori;
            cout << "easyright  angle90.x = " << angle90.x << "      " << "angle90.y= " << angle90.y << endl;
        }

        if (vertiflag == 1 && horiflag == 0)
        {
            angle90.x = Xverti;
            float anglepointsumy = 0.0;
            for (auto & linehori : linehoris)
            {
                angle90.y = ((linehori[1]-linehori[3])*1.0/(linehori[0]-linehori[2]))*(Xverti - linehori[0]) + linehori[1];
                anglepointsumy +=angle90.y;
//            cout << "toppoint.y =  " << toppoint.y<<endl;
            }
            angle90.y = anglepointsumy/linehoris.size();
            cout << "easytop angle90.x = " << angle90.x << "   angle90.y = " << angle90.y << endl;
        }

        if(vertiflag == 0 && horiflag == 1)
        {
            angle90.y = Yhori;
            float anglepointsumx = 0.0;
            for (auto & lineverti : linevertis)
            {
                angle90.x = (Yhori - lineverti[1])/((lineverti[1]-lineverti[3])*1.0/(lineverti[0]-lineverti[2])) + lineverti[0];
                anglepointsumx +=angle90.x;
//            cout << "toppoint.y =  " << toppoint.y<<endl;
            }
            angle90.x = anglepointsumx/linevertis.size();
            cout << "easytop angle90.x = " << angle90.x << "   angle90.y = " << angle90.y << endl;
        }

        if (vertiflag == 0 && horiflag == 0)
        {
            //// ay+bx=m
            ////cy+dx=n
            ////y=(md-bn)/(ad-bc)
            ////x=(an-mc)/(ad-bc)
            float aa = ahavg;
            float bb = bhavg;
            float mm = mhavg;

            float cc = avavg;
            float dd = bvavg;
            float nn = mvavg;

            angle90 = crosspoint_solver(aa, bb, cc, dd, mm, nn);
            cout << "using matrix angle90.x = " << angle90.x << "      " << "angle90.y" << angle90.y << endl;
        }

////calculate top point
        if(vertiflag == 1)
        {
            toppoint.x = Xverti;
            float toppointsumy = 0.0;
            for (auto & lineslope : lineslopes)
            {
                toppoint.y = ((lineslope[1]-lineslope[3])*1.0/(lineslope[0]-lineslope[2]))*(Xverti - lineslope[0]) + lineslope[1];
                toppointsumy +=toppoint.y;
//            cout << "toppoint.y =  " << toppoint.y<<endl;
            }
            toppoint.y = toppointsumy/lineslopes.size();
            cout << "easytop toppoint.x = " << toppoint.x << "   toppoint.y = " << toppoint.y << endl;
        }
        if(vertiflag == 0 )
        {
            //// ay+bx=m
            ////cy+dx=n
            ////y=(md-bn)/(ad-bc)
            ////x=(an-mc)/(ad-bc)
            float aa = asavg;
            float bb = bsavg;
            float mm = msavg;

            float cc = avavg;
            float dd = bvavg;
            float nn = mvavg;

            toppoint = crosspoint_solver(aa, bb, cc, dd, mm, nn);
            cout << "using matrix toppoint.x = " << toppoint.x << "      " << "toppoint.y" << toppoint.y << endl;
        }

////calculate right point
        if(horiflag == 1)
        {
            rightpoint.y = Yhori;
            float rightpointsumx = 0.0;
            for (int i = 0; i < lineslopes.size(); ++i)
            {
                rightpoint.x = (ms[i]-bs[i]*rightpoint.y)/as[i];
                rightpointsumx += rightpoint.x;
            }
            rightpoint.x = rightpointsumx/lineslopes.size();
            cout << "easyright rightpoint.x = " << rightpoint.x << "   rightpoint.y = " << rightpoint.y << endl;
        }
        if(horiflag == 0)
        {
            //// ay+bx=m
            ////cy+dx=n
            ////y=(md-bn)/(ad-bc)
            ////x=(an-mc)/(ad-bc)
            float aa = asavg;
            float bb = bsavg;
            float mm = msavg;

            float cc = ahavg;
            float dd = bhavg;
            float nn = mhavg;

            rightpoint = crosspoint_solver(aa, bb, cc, dd, mm, nn);
            cout << "using matrix rightpoint.x = " << rightpoint.x << "     rightpoint.y = " << rightpoint.y << endl;
        }


        circle(drawinglinecolor,angle90,10,Scalar(0,0,255));
        circle(drawinglinecolor,toppoint,10,Scalar(0,0,255));
        circle(drawinglinecolor,rightpoint,10,Scalar(0,0,255));
        circle(drawinglinecolor,circlepoint,10,Scalar(0,0,255));
//        line(drawinglinecolor, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);

        showimage("imglinecolor"+to_string(j), drawinglinecolor);
        on_measure(j);
        ////clear vector
        linehoris.clear();
        lines.clear();
        lls.clear();
        linehoris.clear();
        linevertis.clear();
        lineslopes.clear();
        ah.clear();
        bh.clear();
        mh.clear();
        av.clear();
        bv.clear();
        mv.clear();
        as.clear();
        bs.clear();
        ms.clear();
    }

    waitKey(0);
    return 0;
}
//    if (!imgsrc.data || imgsrc.channels() != 1)
//    {
//        cvtColor(imgsrc, imggray, COLOR_BGR2GRAY);
//        showimage("imggray", imggray);
//    }

//    showimage("imgsrc", imghub[0]);

//    on_CornerHarris(imgsrc);
//    threshold(imgsrc,imgthrd,thrdval,255,CV_THRESH_BINARY);
//    showimage(imgthrd,"imgthrd");
//    namedWindow("threshold and dilate",CV_WINDOW_NORMAL);
//    resizeWindow("threshold and dilate",800,450);
//    createTrackbar("value", "threshold and dilate", &thrdval, 255, on_threshold,0);
//    namedWindow("dilate",CV_WINDOW_NORMAL);
//    resizeWindow("dilate",800,450);
//    createTrackbar("value", "dilate", &dilateval, 255, on_dilate,0);



////01.bmp
//    Mat dilatesize = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
//    dilate(imgthrd, imgdilate, dilatesize);
//    Mat erodesize = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
//    erode(imgdilate, imgerode, erodesize);
//    showimage("imgerode", imgerode);

////04.jpg
//    imgerode = imgthrd;


//    showimage("drawingthrd", drawingthrd);


//    showimage("threshold and dilate and erode",imgerode);
//    imgerodecvt = on_convert();
//    namedWindow("hough",CV_WINDOW_NORMAL);
//    resizeWindow("hough",800,450);
//    createTrackbar("value", "hough", &houghparam2, 255, on_houghcircle,0);

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
//    showimage("drawingthrd", drawingthrd);



//    imshow("drawingthrd_open", drawingthrd_open);
////01.bmp
//    HoughLinesP(drawingthrd_open, lines, 100, CV_PI / 180, 1500, 400, 40);
////03.jpg





