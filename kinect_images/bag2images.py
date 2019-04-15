#!/usr/bin/python

# Extract images from a bag file.

#PKG = 'beginner_tutorials'
import roslib;   #roslib.load_manifest(PKG)
import rosbag
import rospy
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from cv_bridge import CvBridgeError

# Reading bag filename from command line or roslaunch parameter.
#import os
#import sys


class ImageCreator():


    def __init__(self):
        self.bridge = CvBridge()
        with rosbag.Bag('second.bag', 'r') as bag:  
            for topic,msg,t in bag.read_messages():
                if topic == "/kinect2/qhd/image_color": 
                        try:
                            cv_image = self.bridge.imgmsg_to_cv2(msg,"bgr8")
                        except CvBridgeError as e:
                            print e
                        timestr = "%.6f" %  msg.header.stamp.to_sec()
                        
                        image_name = timestr+ ".png"
                        cv2.imwrite(image_name, cv_image)

if __name__ == '__main__':

    #rospy.init_node(PKG)

    try:
        image_creator = ImageCreator()
    except rospy.ROSInterruptException:
        pass


