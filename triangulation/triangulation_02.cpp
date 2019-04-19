#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
//Eigen头文件要在#include <opencv2/core/eigen.hpp>之前，否则报错
#include <Eigen/Dense>
#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>

// #include "extra.h" // used in opencv2
using namespace std;
using namespace cv;
using namespace Eigen;

void triangulation (
        const Point2d& uv_point_1,
        const Point2d& uv_point_2,
        const Mat& R1, const Mat& t1, const Mat& R2, const Mat& t2,
        vector<Point3d>& points );

// 像素坐标转相机归一化坐标
Point2f pixel2cam( const Point2d& p, const Mat& K );

int main ( int argc, char** argv )
{

//inverse_x_y_1__7 and 1__15        357,370             214,624
//inverse_x_y_2__13 and 2__18       166,486             371,600
//inverse_x_y_3__20 and 3__29       222,317             217,551
//inverse_x_y_4__16 and 4__23       246,641             365,467
//inverse_x_y_5__6  and 5__7        381,500             357,370
//inverse_x_y_6__11 and 6__12       163,281             175,418
//            7__12 and 7__13       418,175             486,166
//            8__14 and 8__15       526,173             624,214
//            9__21 and 9__22       426,221             568,218
    Point2d uv_points_1(526,173), uv_points_2(624,214);

//转换矩阵定义
    Mat t1;
    Mat t2;
    Mat R1;
    Mat R2;
//四元数定义
//1__7  0.9982746,-0.0060674, 0.0247354, 0.0529064
//1__15 0.9983897,-0.0171841, -0.0233101, 0.0487795

//2__13  0.9977246,-0.0310185, -0.0197670, 0.0565043
//2__18  0.9996939,0.0065591, -0.0113205, 0.0210007

//3__20  0.9950806,-0.0369820, 0.0106631, 0.0912861
//3__29  0.9974173,-0.0530036, -0.0295233, 0.0384411

//4__16  0.9986162,-0.0093842, -0.0235014, 0.0461021
//4__23  0.9988216,-0.0271060, -0.0015118, 0.0402285

//5__6   0.9984154, 0.0074380, 0.0277701, 0.0483769
//5__7  0.9982746,-0.0060674, 0.0247354, 0.0529064

//6__11  0.9957916,-0.0460886, 0.0010451, 0.0792077
//6__12  0.9974253,-0.0319022, -0.0160101, 0.0621993

//7__12  0.9974253,-0.0319022, -0.0160101, 0.0621993
//7__13  0.9977246,-0.0310185, -0.0197670, 0.0565043

//8__14  0.9980602,-0.0262029, -0.0238290, 0.0511998
//8__15  0.9983897,-0.0171841, -0.0233101, 0.0487795

//9__21  0.9973937,-0.0422470, -0.0196249, 0.0550987
//9__22  0.9980463,-0.0312341, -0.0301002, 0.0449661
    Quaterniond Qv1(0.9980602,-0.0262029, -0.0238290, 0.0511998);
    Quaterniond Qv2(0.9983897,-0.0171841, -0.0233101, 0.0487795);
    Matrix3d QvR1;
    Matrix3d QvR2;
    QvR1 = Qv1.matrix();
    QvR2 = Qv2.matrix();
//Eigen to CV
    eigen2cv(QvR1,R1);
    eigen2cv(QvR2,R2);

//1__7      0.3303327, 0.0300423, 0.0825348
//1__15     0.0817482, 0.1490154, 0.0288321
//2__13     0.2217423, 0.1957212, 0.0716526
//2__18     0.1070732, 0.0133353, 0.0568685
//4__16     0.0632782, 0.1229710, 0.0189963
//4__23     0.2445631, 0.0018060, 0.1570807
//5__6      0.1734087, 0.0017389, 0.0535460
//5__7      0.3303327, 0.0300423, 0.0825348
//6__11     0.4108765, 0.2087498, 0.1288482
//6__12     0.2921467, 0.1919154, 0.1017735
//7__12     0.2921467, 0.1919154, 0.1017735
//7__13     0.2217423, 0.1957212, 0.0716526
//8__14     0.1859159, 0.1883330, 0.0583820
//8__15     0.0817482, 0.1490154, 0.0288321
//9__21     0.2924721, 0.1281421, 0.1592880
//9__22     0.1606274, 0.1274972 ,0.0911838

    t1 = (Mat_<double> (3,1) << 0.0,0.0,0.0);
    t2 = (Mat_<double> (3,1) << 0.104,0.039,0.03);

    //-- 三角化
    vector<Point3d> points;
    triangulation(uv_points_1,uv_points_2,R1,t1,R2,t2,points);

    return 0;
}

void  triangulation (
        const Point2d& uv_point_1,
        const Point2d& uv_point_2,
        const Mat& R1, const Mat& t1, const Mat& R2, const Mat& t2,
        vector<Point3d>& points )
{
    Mat T1 = (Mat_<float> (3,4) <<
            R1.at<double>(0,0), R1.at<double>(0,1), R1.at<double>(0,2), t1.at<double>(0,0),
            R1.at<double>(1,0), R1.at<double>(1,1), R1.at<double>(1,2), t1.at<double>(1,0),
            R1.at<double>(2,0), R1.at<double>(2,1), R1.at<double>(2,2), t1.at<double>(2,0)
            );
    Mat T2 = (Mat_<float> (3,4) <<
            R2.at<double>(0,0), R2.at<double>(0,1), R2.at<double>(0,2), t2.at<double>(0,0),
            R2.at<double>(1,0), R2.at<double>(1,1), R2.at<double>(1,2), t2.at<double>(1,0),
            R2.at<double>(2,0), R2.at<double>(2,1), R2.at<double>(2,2), t2.at<double>(2,0)
            );

    Mat K = ( Mat_<double> ( 3,3 ) << 494.9, 0, 465.9, 0, 512.8, 314.3, 0, 0, 1 );
    vector<Point2f> pts_1, pts_2;

    // 将像素坐标转换至相机坐标
    pts_1.push_back( pixel2cam( uv_point_1, K)) ;
    pts_2.push_back( pixel2cam( uv_point_2, K)) ;
    cout << "pts_1 = " << pts_1 << endl;
    cout << "pts_2 = " << pts_2 << endl;

    Mat pts_4d;
    cv::triangulatePoints( T1, T2, pts_1, pts_2, pts_4d );

    // 转换成非齐次坐标
    for ( int i=0; i<pts_4d.cols; i++ )
    {
        Mat x = pts_4d.col(i);
        x /= x.at<float>(3,0); // 归一化
        Point3d p (
                x.at<float>(0,0),
                x.at<float>(1,0),
                x.at<float>(2,0)
        );
        points.push_back( p );
        cout << "Point3d p = " << p << endl;
        Point2d reproject_pt1;
        reproject_pt1.x = x.at<float>(0,0)/x.at<float>(2,0);
        reproject_pt1.y = x.at<float>(1,0)/x.at<float>(2,0);
        cout << "reproject_pt1.x = " << reproject_pt1.x << endl;
        cout << "reproject_pt1.y = " << reproject_pt1.y << endl;
    }

}
Point2f pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2f
            (
                    ( p.x - K.at<double>(0,2) ) / K.at<double>(0,0),
                    ( p.y - K.at<double>(1,2) ) / K.at<double>(1,1)
            );
}


