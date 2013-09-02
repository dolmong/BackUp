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


CLoadImage::CLoadImage( ros::NodeHandle &n )
: nh_( n ),
it_( nh_ )
{
	sub_ = nh_.subscribe( "Image_Load", 1, &CLoadImage::LoadCallback, this );
	image_pub_ = it_.advertise( "Image_Out", 1 );

	cvImage_ptr.reset( new cv_bridge::CvImage );

	cv::namedWindow( WINDOW );
}


CLoadImage::~CLoadImage()
{
	cv::destroyWindow( WINDOW );
}


void CLoadImage::LoadCallback(
	const rpi_cam::LoadData &msg
	)
{
	cvImage_ptr->image = cv::imread( msg.strPath );

	if( !cvImage_ptr->image.data )
	{
		ROS_ERROR( "Could not open or find the image" );
		return;
	}

	ROS_INFO( "File Open" );
	if( msg.bView!=false )
	{
		cv::imshow( WINDOW, cvImage_ptr->image );
		cv::waitKey( msg.nTime );
	}

	image_pub_.publish( cvImage_ptr->toImageMsg() );

}
