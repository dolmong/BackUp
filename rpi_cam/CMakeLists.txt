cmake_minimum_required(VERSION 2.8.3)
project(rpi_cam)

## Find catkin and any catkin packages
find_package(
	catkin REQUIRED
	COMPONENTS roscpp std_msgs sensor_msgs message_generation
	image_transport REQUIRED
	cv_bridge REQUIRED
)
find_package(
	OpenCV REQUIRED
)

## Declare ROS messages and services
add_message_files(
	FILES LoadData.msg
)
add_service_files(
	FILES CamData.srv
)

# declare General message and services
generate_messages(
	DEPENDENCIES std_msgs sensor_msgs
)

#declare a catkin package
catkin_package(
	INCLUDE_DIRS include
	LIBRARIES rpi_cam
	CATKIN_DEPENDS std_msgs sensor_msgs message_runtime
	DEPENDS system_lib
)

include_directories(
	${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)


## rpi_cam library
add_executable(
	rpi_cam	src/photographing.cpp src/LoadImage.cpp
)
target_link_libraries(
	rpi_cam	${catkin_LIBRARIES} ${OpenCV_LIBRARIES}
)
add_dependencies(
	rpi_cam	${PROJECT_NAME}_gencpp
)


add_executable(
	order	src/order.cpp
)
target_link_libraries(
	order	${catkin_LIBRARIES}
)
add_dependencies(
	order	rpi_cam_gencpp
)
