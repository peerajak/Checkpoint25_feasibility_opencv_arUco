cmake_minimum_required(VERSION 2.8)
project( MarkerDetect )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( MarkerDetect marker_detector.cpp )
target_link_libraries( MarkerDetect ${OpenCV_LIBS} )

add_executable( PoseEstimation marker_detector_pose_estimation.cpp )
target_link_libraries( PoseEstimation ${OpenCV_LIBS} )