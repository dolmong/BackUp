#include <boost/thread.hpp>

#include "main.h"
#include "filesystem.h"
#include "global.h"
#include "improc.h"
#include "matching.h"
#include "surf.h"
#include "viewer.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>


CMain::CMain()
{
}


CMain::~CMain()
{
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

/*
void SurfIn()
{
	while( 1 )
	{
		if( imgCap.size()!=0 )
		{
			imgsrIn.img = imgCap;
			imgsrIn.sr.ipSurf.clear();
			imgsrIn.sr.Surf( imgCap, imgsrIn.sr.ipSurf );
			imgsrIn.bFlag = TRUE;
		}
	}
}


void SurfRef()
{
	while( 1 )
	{
		if( 
	}
}


void Matching()
{
	CFileSystem filesystem;
	CSURF surfRef;
	CSURF surfIn;
	CMatching matching;

	sensor_msgs::Image imgRef;
	std::string strRefPath;
	nh.getParam( "/Main/RefPath", strRefPath );
	std::string strRefName;
	nh.getParam( "/Main/RefName", strRefName );
	std::string strRefFile=filesystem.MergeFileName( strRefPath.c_str(), strRefName.c_str() );
	if( !filesystem.ReadImage( imgRef, strRefFile.c_str() ) )
	{
		ROS_ERROR( "Main: ReadImage_ReferenceImage Fail!" );
		return 0;
	}
	else
	{
		surfRef.ipSurf.clear();
		std::string strAdd="_surf.txt";
                std::string strRefSurfName=filesystem.AddFileName( strRefName.c_str(), strAdd.c_str() );
                std::string strRefSurfFile=filesystem.MergeFileName( strRefPath.c_str(), strRefSurfName.c_str() );

		if( !filesystem.ReadSurf(surfRef.ipSurf, strRefSurfFile.c_str()) )
		{
			surfRef.Surf( imgRef, surfRef.ipSurf );

			if( !filesystem.WriteSurf(surfRef.ipSurf, strRefSurfFile.c_str()) )
			{
				ROS_ERROR( "Main: WriteSurf Fail!" );
				imgRef.data.clear();
				return 0;
			}
		}

		sensor_msgs::Image imgIn;
		std::string strInPath;
		nh_.getParam( "/Main/InPath", strInPath );
		std::string strInName;
		nh_.getParam( "/Main/InName", strInName );
		std::string strInFile=filesystem.MergeFileName( strInPath.c_str(), strInName.c_str() );
		if( !filesystem.ReadImage(imgIn, strInFile.c_str()) )
		{
			ROS_ERROR( "Main: ReadImage_InputImage Fail!" );
		}
		surfIn.Surf( imgIn, surfIn.ipSurf );

		strAdd = "_HEIGHT.bmp";
		std::string strRefHeightName=filesystem.AddFileName( strRefName.c_str(), strAdd.c_str() );
		std::string strRefHeightFile=filesystem.MergeFileName( strRefPath.c_str(), strRefHeightName.c_str() );
		
		matching.Matching( imgRef, imgIn, surfRef.ipSurf, surfIn.ipSurf, strRefHeightFile, matching.ipPairMat, matching.spMeter );
		imgRef.data.clear();
		imgIn.data.clear();
	}
}
*/

void threadImage()
{
	cv::namedWindow( "Image", CV_WINDOW_AUTOSIZE );
	cvimgCap.reset( new cv_bridge::CvImage );
	cvimgGray.reset( new cv_bridge::CvImage );

	CViewer viewer;
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
		}
	}
	cv::destroyWindow( "Image" );
	cvimgCap->image.release();
	cvimgGray->image.release();
	imgCap.data.clear();
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "Capture" );
	ros::NodeHandle nh;

	boost::thread thimage( &threadImage );
//	boost::thread thMatching( &threadMatching );

	CMain main;
	main.Capture( 1280, 720 );

	ros::shutdown();
	thimage.join();
	return 0;
}
