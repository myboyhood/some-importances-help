cmake_minimum_required(VERSION 3.13)
project(detect)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})



add_executable(detect main.cpp)
target_link_libraries( detect ${OpenCV_LIBS} )