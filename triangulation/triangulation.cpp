//
// Created by ubuntu on 19-4-18.
//
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
//    if ( argc != 3 )
//    {
//        cout<<"usage: triangulation img1 img2"<<endl;
//        return 1;
//    }
//    -- 读取图像
//    Mat img_1 = imread ( argv[1], CV_LOAD_IMAGE_COLOR );
//    Mat img_2 = imread ( argv[2], CV_LOAD_IMAGE_COLOR );

    Point2d uv_points_1(818,334), uv_points_2(700,178);

//    vector<DMatch> matches;
//    find_feature_matches ( img_1, img_2, keypoints_1, keypoints_2, matches );
//    cout<<"一共找到了"<<matches.size() <<"组匹配点"<<endl;

//转换矩阵定义
    Mat t1;
    Mat t2;
    Mat R1;
    Mat R2;
//四元数定义
    Quaterniond Qv1(0.9999759,-0.0023747,0.0037205,0.0053638);
    Quaterniond Qv2(0.9988146,-0.0269420,-0.0218019,0.0341795);
    Matrix3d QvR1;
    Matrix3d QvR2;
    QvR1 = Qv1.matrix();
    QvR2 = Qv2.matrix();
//Eigen to CV
    eigen2cv(QvR1,R1);
    eigen2cv(QvR2,R2);

//    pose_estimation_2d2d ( keypoints_1, keypoints_2, matches, R, t );
    t1 = (Mat_<double> (3,1) << 0.0208036,-0.0011688,-0.0001212);
    t2 = (Mat_<double> (3,1) << 0.0754700,0.0742439,-0.0761950);


    //-- 三角化
    vector<Point3d> points;
    triangulation(uv_points_1,uv_points_2,R1,t1,R2,t2,points);

    //-- 验证三角化点与特征点的重投影关系
//    Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
//    for ( int i=0; i<matches.size(); i++ )
//    {
//        Point2d pt1_cam = pixel2cam( keypoints_1[ matches[i].queryIdx ].pt, K );
//        Point2d pt1_cam_3d(
//                points[i].x/points[i].z,
//                points[i].y/points[i].z
//        );
//
//        cout<<"point in the first camera frame: "<<pt1_cam<<endl;
//        cout<<"point projected from 3D "<<pt1_cam_3d<<", d="<<points[i].z<<endl;
//
//        // 第二个图
//        Point2f pt2_cam = pixel2cam( keypoints_2[ matches[i].trainIdx ].pt, K );
//        Mat pt2_trans = R*( Mat_<double>(3,1) << points[i].x, points[i].y, points[i].z ) + t;
//        pt2_trans /= pt2_trans.at<double>(2,0);
//        cout<<"point in the second camera frame: "<<pt2_cam<<endl;
//        cout<<"point reprojected from second frame: "<<pt2_trans.t()<<endl;
//        cout<<endl;
//    }

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


