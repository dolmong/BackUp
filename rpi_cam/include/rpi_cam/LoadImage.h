#ifndef RPI_CAM_LOADIMAGE_H
#define RPI_CAM_LOADIMAGE_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "rpi_cam/LoadData.h"


namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

class CLoadImage
{
public:
	CLoadImage( ros::NodeHandle &n );
	~CLoadImage();

	void LoadCallback( const rpi_cam::LoadData &msg );
private:
	ros::NodeHandle	nh_;
	ros::Subscriber sub_;
	image_transport::ImageTransport	it_;
	image_transport::Publisher	image_pub_;

	cv_bridge::CvImagePtr	cvImage_ptr;
};

#endif
