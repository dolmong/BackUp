#ifndef CAMERA_CLASS_CAPTURE_H
#define CAMERA_CLASS_CAPTURE_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <std_msgs/Bool.h>

namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

class CCapture
{
public:
	CCapture( ros::NodeHandle &n );
	~CCapture();

	void CaptureCallback( const std_msgs::BoolPtr &msg );
private:
	ros::NodeHandle nh_;
	ros::Subscriber sub_;
	image_transport::ImageTransport it_;
	image_transport::Publisher image_pub_;

	cv_bridge::CvImagePtr cvImage_ptr;
};

#endif
