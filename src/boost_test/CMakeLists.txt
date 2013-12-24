cmake_minimum_required(VERSION 2.8.3)
project(boost_test)

find_package(
	catkin REQUIRED
	COMPONENTS roscpp
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES boost_test
	CATKIN_DEPENDS roscpp
	DEPENDS system_lib
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)

add_executable(
	boost_test
	src/test.cpp
)

target_link_libraries(
	boost_test
	${catkin_LIBRARIES}
)
