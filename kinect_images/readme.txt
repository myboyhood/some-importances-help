1.使用.py文件将ROS记录的qhd/kinect2/image_color话题bag文件转化为图像序列，记得先新建一个文件夹，将.py文件放入，最后生成的图像序列和.py文件在同一级目录。

2.使用name.cpp
g++ -o nameraw name.cpp
./nameraw
生成一个rgbraw.txt
这是一个未经过排序的图像文件名的txt文件

3.使用cutname.cpp
g++ -o namepure name
./namepure
生成一个rgb.txt
这是跑ORB-SLAM2所要求的文件名
然后把图像放到一个新建的文件夹，比如 imagesv2.0

kinect_v2标定文件是我在5x7 0.03的chess下各种倾斜角度下标定的qhd大小文件。
