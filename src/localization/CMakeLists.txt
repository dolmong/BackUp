cmake_minimum_required(VERSION 2.8.3)
project(localization)

find_package(
	catkin REQUIRED
	COMPONENTS cv_bridge roscpp sensor_msgs
)
find_package(
	OpenCV REQUIRED
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES localization
	CATKIN_DEPENDS cv_bridge roscpp sensor_msgs
	DEPENDS OpenCV system_lib
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${OPENCV_INCLUDE_DIRS}
)

add_executable(
	localization_node
	src/filesystem.cpp
	src/improc.cpp
	src/matching.cpp
	src/surf.cpp
	src/viewer.cpp
	src/main.cpp
)

target_link_libraries(
	localization_node
	${catkin_LIBRARIES}
	${OPENCV_LIBRARIES}
	/usr/lib/uv4l/uv4lext/armv6l/libuv4lext.so
)
