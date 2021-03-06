#include "camera/capture.h"

CCapture::CCapture(
	ros::NodeHandle &n
	)
:	nh_( n ),
	it_( nh_ )
{
	sub_ = nh_.subscribe( "Capture_In", 1, &CCapture::CaptureCallback, this );
	image_pub_ = it_.advertise( "Capture_Out", 1 );
	cvImage_ptr.reset( new cv_bridge::CvImage );
	cv::namedWindow( WINDOW );
}

CCapture::~CCapture()
{
	cv::destroyWindow( WINDOW );
}

void CCapture::CaptureCallback(
	const std_msgs::BoolPtr &msg
	)
{
	CvCapture *capture = 0;
	cv::Mat frame;
	capture = cvCreateCameraCapture( -1 );
	// 0=defalt, -1=any camera, 1~99=your camera

	if( !capture )
		ROS_ERROR( "No camera detected!" );
	else
	{
		ROS_INFO( "In capture ..." );

		while( 1 )
		{
			IplImage *ipImg = cvQueryFrame( capture );
			frame = ipImg;
			if( frame.empty() )
				break;
			if( ipImg->origin==IPL_ORIGIN_TL )
				frame.copyTo( cvImage_ptr->image );
			else
				cv::flip( frame, cvImage_ptr->image, 0 );

			if( cv::waitKey( 10 )>=0 )
				cvReleaseCapture( &capture );
		}

		cv::waitKey( 0 );
	}
}
