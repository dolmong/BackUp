#ifndef MAIN_H
#define MAIN_H

// Header
	#include <ros/ros.h>
	#include <opencv/cv.h>
	#include <sensor_msgs/Image.h>
	#include <sensor_msgs/image_encodings.h>
	#include <cv_bridge/cv_bridge.h>
	#include "surf.h"

// Structual
	typedef struct _ImgSurf
	{
		CSURF sr;
		bool bFlag;
		sensor_msgs::Image img;
	}imagesurf;

// Variable
	sensor_msgs::Image imgCap;
	cv_bridge::CvImagePtr cvimgCap;
	cv_bridge::CvImagePtr cvimgGray;
	imagesurf imgsrIn;
	imagesurf imgsrRef;

// Thread Function
	void threadImage();

// Function
	void Matching();

class CMain
{
public:
	// Construct and Destruct
	CMain();
	~CMain();

private:
	ros::NodeHandle nh;

public:
	// Function
	void Capture( int width, int height );
};

#endif
