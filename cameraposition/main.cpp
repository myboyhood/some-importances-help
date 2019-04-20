#include<ros/ros.h> //ros标准库头文件
#include <iostream>
#include<cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <geometry_msgs/PoseArray.h>
#include <Eigen/Geometry>
#include <Eigen/Core>
#include <fstream>
#include<chrono>

using namespace std;
using namespace chrono;

double camera_x,camera_y,camera_z;

void write_to_file()
{
    ros::Time time = ros::Time::now();
    int name_suffix = 2;
    ofstream markertraj;
    ostringstream oss;
    oss << name_suffix;
    string filename = "/home/ubuntu/cotex/cameraposition/dataset/markertruth_" + oss.str()+".txt";

    markertraj.open(filename.c_str(),ios::app);
    markertraj << time;
    markertraj << " ";
    markertraj << camera_x;
    markertraj << " ";
    markertraj << camera_y;
    markertraj << " ";
    markertraj << camera_z;
    markertraj << " ";
    markertraj << 0 << " " << 0 << " " << 0 << " " << 0;
    markertraj << endl;
    markertraj.close();

}

void submarkerrecall(visualization_msgs::MarkerArray state  )
{
    cout<<state.markers.at(4).pose.position.x<<endl;
    camera_x = state.markers.at(4).pose.position.x;
    camera_y = state.markers.at(4).pose.position.y;
    camera_z = state.markers.at(4).pose.position.z;

    write_to_file();
}



int main(int argc, char **argv) {
    ros::init(argc,argv,"cameraposition");
    ros::NodeHandle nh_;

    ros::Subscriber sub_marker_data = nh_.subscribe("/vis_markers",100000,submarkerrecall);


        ros::spin();

    return 0;
}
