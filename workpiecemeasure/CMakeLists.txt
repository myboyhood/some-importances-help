cmake_minimum_required(VERSION 3.15)
project(workpiecemeasure)

set(CMAKE_CXX_STANDARD 14)
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable(workpiecemeasure main.cpp)
target_link_libraries( workpiecemeasure ${OpenCV_LIBS} )