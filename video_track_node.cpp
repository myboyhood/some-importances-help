#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv-3.3.1-dev/opencv/cv.hpp>
#include<vector>
//#include </home/l/software/opencv-2.4.11/modules/stitching/include/opencv2/stitching/stitcher.hpp>   //这个只有opencv2才有，所以安装一个，然后用绝对路径

using namespace std;
using namespace cv;
bool try_use_gpu = false;
//static const std::string OPENCV_WINDOW = "Image window";
Mat img1;
//Mat img2;
vector<Mat> imgs;
string result_name = "dst1.jpg";
UMat frame0;
UMat frame1;
UMat ptr0;
UMat ptr1;
UMat frameCopy;
Mat mask_temp;
int points = 1000;//specify points count [GoodFeatureToTrack]
vector<cv::Point2f> pts(points);
vector<cv::Point2f> nextPts(points);
vector<unsigned char> status(points);
vector<float> err;

class ImageConverter {
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub1_;
//image_transport::Subscriber image_sub2_;
//image_transport::Publisher image_pub_;


public:

    ImageConverter()
            : it_(nh_) {
// Subscrive to input video feed and publish output video feed
//        image_sub1_ = it_.subscribe("/image_publish", 1, &ImageConverter::imageCb1, this);
        image_sub1_ = it_.subscribe("/usb_cam/image_raw", 1, &ImageConverter::imageCb1, this);
//image_sub2_ = it_.subscribe("/2/usb_cam/image_raw", 1, &ImageConverter::imageCb2, this);
//image_pub_ = it_.advertise("/image_converter/output_video", 1);

//namedWindow(OPENCV_WINDOW);
    }

    ~ImageConverter() {
//destroyWindow(OPENCV_WINDOW);
    }
//    static void drawArrows(UMat& _frame, const vector<Point2f>& prevPts, const vector<Point2f>& nextPts, const vector<uchar>& status,
//                           Scalar line_color = Scalar(0, 0, 255))
//    {
//        Mat frame = _frame.getMat(ACCESS_WRITE);
//        for (size_t i = 0; i < prevPts.size(); ++i)
//        {
//            if (status[i])
//            {
//                int line_thickness = 1;
//
//                Point p = prevPts[i];
//                Point q = nextPts[i];
//
//                double angle = atan2((double) p.y - q.y, (double) p.x - q.x);
//
//                double hypotenuse = sqrt( (double)(p.y - q.y)*(p.y - q.y) + (double)(p.x - q.x)*(p.x - q.x) );
//
//                if (hypotenuse < 1.0)
//                    continue;
//
//                // Here we lengthen the arrow by a factor of three.
//                q.x = (int) (p.x - 3 * hypotenuse * cos(angle));
//                q.y = (int) (p.y - 3 * hypotenuse * sin(angle));
//
//                // Now we draw the main line of the arrow.
//                line(frame, p, q, line_color, line_thickness);
//
//                // Now draw the tips of the arrow. I do some scaling so that the
//                // tips look proportional to the main line of the arrow.
//
//                p.x = (int) (q.x + 9 * cos(angle + CV_PI / 4));
//                p.y = (int) (q.y + 9 * sin(angle + CV_PI / 4));
//                line(frame, p, q, line_color, line_thickness);
//
//                p.x = (int) (q.x + 9 * cos(angle - CV_PI / 4));
//                p.y = (int) (q.y + 9 * sin(angle - CV_PI / 4));
//                line(frame, p, q, line_color, line_thickness);
//            }
//        }
//    }


///////1/
    void imageCb1(const sensor_msgs::ImageConstPtr &msg)
    {
        cv_bridge::CvImagePtr cv_ptr1;
        Mat src;
        Mat gray_image;
        Mat mask_dst_gray;
        Mat mask_dst;

        static int n=0;



        try {
            cv_ptr1 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
//imshow("1111",cv_ptr1->image);
        src = cv_ptr1->image;
        waitKey(1);
        namedWindow("src", WINDOW_AUTOSIZE);
        imshow("src", src);
        //img1.convertTo(src, -1, 1, 0);//not use until now
        cvtColor(src, gray_image, COLOR_BGR2GRAY);
//        balck_back.create(gray_image.dims,gray_image.size,gray_image.type());
//        cvtColor(balck_back, mask_dst, COLOR_BGR2GRAY);
//threshold by gray value
        for (int i = 0; i < gray_image.rows; i++)
        {
            uchar *data = gray_image.ptr<uchar>(i);
            for (int j = 0; j < gray_image.cols; j++)
            {
                if (data[j] > 220)
                    data[j] = 255;
                else
                    data[j] = 0;
            }
        }
        matchTemplate(gray_image,mask_temp,mask_dst,0);//0 is one method mode for matching
        normalize(mask_dst,mask_dst,1,0);
//        cvtColor(mask_dst,mask_dst_gray,COLOR_BGR2GRAY);
//        minMaxLoc(mask_dst,min_value,max_value,&minPoint,&maxPoint);

        //cv::namedWindow("threshold", WINDOW_AUTOSIZE);
        imshow("mask_dst", mask_dst);
        imshow("mask_tmp", mask_temp);


            if (n == 0)
            {
//                gray_image.copyTo(frame0);
                mask_dst.copyTo(frame0);
                //convert Mat to UMat
            }

            else
            {
                cout << "first" << n << endl;
                if (n % 2 == 1) {
//                    gray_image.copyTo(frame1);
                    mask_dst.copyTo(frame1);
                    ptr0 = frame0;
                    ptr1 = frame1;
                } else {
//                    gray_image.copyTo(frame0);
                    mask_dst.copyTo(frame0);
                    ptr0 = frame1;
                    ptr1 = frame0;
                }
//                cv::namedWindow("ptr0", WINDOW_AUTOSIZE);
//                imshow("ptr0",frame0);
//                cv::namedWindow("ptr1", WINDOW_AUTOSIZE);
//                imshow("ptr1",ptr1);

                pts.clear();
//                imshow("ptr0", ptr0);
//                imshow("ptr1", ptr1);
                goodFeaturesToTrack(ptr0, pts, points, 0.01, 0.0);

                if (pts.size() != 0 && !ptr0.empty() && !ptr1.empty()) {
                    calcOpticalFlowPyrLK(ptr0, ptr1, pts, nextPts, status, err);
                    cout << "Points count : " << nextPts[n].x << endl << endl;
                }


//                if (n % 2 == 1)
//                    frame1.copyTo(frameCopy);
//                else
//                    frame0.copyTo(frameCopy);

                //drawArrows(frameCopy, pts, nextPts, status, Scalar(255, 0, 0));
                //imshow("PyrLK [Sparse]", frameCopy);

//                vector<std::vector<Point>> contours(500);
//                std::vector<Vec4i> hierarchy;
//                cv::findContours(mask_dst, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//                for (int i = 0; i< contours.size(); i++)
//                {
//                    double area = cv::contourArea(Mat(contours[i]));
//                    cout << "area = " << area << endl;
//                    drawContours(img1, contours, i, Scalar(0,0,255), 1, 18, hierarchy, 0, Point());
//                    imshow("Contours",img1);
//                    double min_area = 23;
//                    double max_area = 80;
//                    double x_sum[contours.size()] = {0}, y_sum[contours.size()] = {0};
//                    double x_avg[contours.size()] = {0}, y_avg[contours.size()] = {0};
//                    Point coutours_sum[contours.size()] ;
//                    if(area > min_area && area < max_area)
//                    {
//                        for (int j = 0; j < contours[i].size(); ++j)
//                        {
//                            cout << "contours[i].size() = " << contours[i].size() <<endl;
//                            int x = contours[i][j].x;
//                            int y = contours[i][j].y;
//                            cout << "contours[i][j].x = " << x <<endl;
//                            cout << "contours[i][j].y = " << y <<endl;
//
//                            x_sum[i] += x;
//                            y_sum[i] += y;
//
//                        }
//                    }
//                    x_avg[i] = x_sum[i]/contours[i].size();
//                    y_avg[i] = y_sum[i]/contours[i].size();
//                    coutours_sum[i].x = (int)x_avg[i];
//                    coutours_sum[i].y = (int)y_avg[i];
//                    circle(gray_image,coutours_sum[i], 15, Scalar(0,0,255));
//
//                }
//                imshow("gray_image", gray_image);
                imshow("mask_dst",mask_dst);
            }
            n++;
//}
//        }
//2////////////////



///////2
//***
//void imageCb2(const sensor_msgs::ImageConstPtr& msg)
//{
//cv_bridge::CvImagePtr cv_ptr2;
//    try
//    {
//    cv_ptr2 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
//    }
//    catch (cv_bridge::Exception& e)
//    {
//    ROS_ERROR("cv_bridge exception: %s", e.what());
//    return;
//    }
////Mat img2;
////imshow("2222",img1);
//img2= cv_ptr2->image;
//waitKey(3);
//
////***/
//imshow("1111",img1);
//imshow("2222",img2);
//////////////////////
////**
//
/////**/
//}
    }
};
int main(int argc, char** argv)
{
ros::init(argc, argv, "video_stitching");
ImageConverter ic;
ros::Rate loop_rate(10);  //5/100
///////video_stitching
mask_temp = imread("/home/user/Pictures/one_marker.png",IMREAD_GRAYSCALE);
//imshow("1111",cv_ptr1->image);
/**
if (!img2.empty()) 
{
imshow("3333", img2);
} 
else
{
cout << "image3 is empty" << endl;
}
**/
ros::spin();

return 0;
}
