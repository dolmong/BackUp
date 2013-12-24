cmake_minimum_required(VERSION 2.8.3)
project(position)

find_package(
	catkin REQUIRED
	COMPONENTS cv_bridge roscpp sensor_msgs
)
find_package(
	OpenCV REQUIRED
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES position
	CATKIN_DEPENDS cv_bridge roscpp sensor_msgs
	DEPENDS OpenCV system_lib
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${OPENCV_INCLUDE_DIRS}
)

add_executable(
	position_node
	src/filesystem.cpp
	src/improc.cpp
	src/matching.cpp
	src/surf.cpp
	src/main.cpp
)

target_link_libraries(
	position_node
	${catkin_LIBRARIES}
	${OPENCV_LIBRARIES}
)
