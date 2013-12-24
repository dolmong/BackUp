#ifndef SURF_TEST_PROJECT_MAIN_H
#define SURF_TEST_PROJECT_MAIN_H

#include <string>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include "surf.h"
#include "filesystem.h"

class CMain
{
public:
	CMain();
	~CMain();

public:
	CSURF surf;
	CFileSystem filesystem;
};

#endif
