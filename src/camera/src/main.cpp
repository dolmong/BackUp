#include <boost/thread.hpp>
#include "main.h"


CMain::CMain()
{
	cvimgCap.reset( new cv_bridge::CvImage );
	cvimgGray.reset( new cv_bridge::CvImage );
}


CMain::~CMain()
{
	cvimgCap->image.release();
	cvimgGray->image.release();
	imgCap.data.clear();
}


void CMain::Capture(
	int width,
	int height
	)
{
	cv::VideoCapture cap( -1 );
	if( !cap.isOpened() )
	{
		ROS_ERROR( "Cann't open camera" );
		return;
	}

	cap.set( CV_CAP_PROP_FRAME_WIDTH, width );
	cap.set( CV_CAP_PROP_FRAME_HEIGHT, height );

	cvimgCap->image.release();
	cvimgGray->image.release();
	while( 1 )
	{
		bool bSuccess=cap.read( cvimgCap->image );
		if( !bSuccess )
		{
			ROS_ERROR( "Cann't read a frame from camera" );
			break;
		}
	}
	ROS_INFO( "Capture End" );
}

void threadImage()
{
	while( 1 )
	{
		if( cvimgCap->image.empty()!=true )
		{
			if( cvimgCap->image.type()==16 )
				cv::cvtColor( cvimgCap->image, cvimgGray->image, CV_RGB2GRAY );
			else
				cvimgCap->image.copyTo( cvimgGray->image );

			cvimgGray->encoding = sensor_msgs::image_encodings::MONO8;
			cvimgGray->toImageMsg( imgCap );
			ROS_INFO( "thread" );
		}
	}
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "Capture" );

	boost::thread thimage( &threadImage );

	CMain main;

	main.Capture( 1280, 720 );

	ros::shutdown();
	thimage.join();
	return 0;
}
