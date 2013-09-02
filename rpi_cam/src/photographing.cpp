#include <ros/ros.h>
#include <sstream>

#include "rpi_cam/CamData.h"

class CPhotographing
{
public:
	CPhotographing( ros::NodeHandle &n );

private:
	typedef struct _Argument
	{
		bool bMode;
		int nWidth;
		int nHeight;
		int nTime;
		std::string strName;
		std::string strPath;
	} arg;

private:
	ros::NodeHandle nh_;
	ros::ServiceServer server_;
	
	arg argDib;

private:
	bool camCallback( rpi_cam::CamData::Request &req, rpi_cam::CamData::Response &res );

	void Video();
	void Photo();
};


CPhotographing::CPhotographing(
	ros::NodeHandle &n
	)
{
	nh_ = n;
	
	server_ = nh_.advertiseService( "photographing", &CPhotographing::camCallback, this );
}


void CPhotographing::Video()
{
	std::string strMessage;
	std::stringstream ss;

	if( argDib.nWidth>=1920 )
		argDib.nWidth = 1920;
	if( argDib.nHeight>=1080 )
		argDib.nHeight = 1080;
	
	if( argDib.strName.length()!=0 )
	{
		if( argDib.strPath.length()!=0 )
			ss << "raspivid" << " -fps 30"  << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t " << argDib.nTime << " -o " << argDib.strPath.c_str() << "/" << argDib.strName.c_str();
		else
			ss << "raspivid" << " -fps 30"  << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t " << argDib.nTime << " -o ~/Jeong_ws/src/rpi_cam/video/" << argDib.strName.c_str();
	}
	else
		ss << "raspivid" << " -fps 30" << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t " << argDib.nTime;

	strMessage = ss.str();

	ROS_INFO( strMessage.c_str() );
	system( strMessage.c_str() );
}


void CPhotographing::Photo()
{
	std::string strMessage;
        std::stringstream ss;

	if( argDib.strName.length()!=0 )
	{
		if( argDib.strPath.length()!=0 )
			ss << "raspistill" << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t 0" << " -o " << argDib.strPath.c_str() << "/" << argDib.strName.c_str() << ".bmp";
		else
			ss << "raspistill" << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t 0" << " -o ~/Jeong_ws/src/rpi_cam/picture/" << argDib.strName.c_str() << ".bmp";
	}
	else
		ss << "raspistill" << " -w " << argDib.nWidth << " -h " << argDib.nHeight << " -t 0";

        strMessage = ss.str();

        ROS_INFO( strMessage.c_str() );
        system( strMessage.c_str() );
}


bool CPhotographing::camCallback(
	rpi_cam::CamData::Request	&req,
	rpi_cam::CamData::Response	&res
	)
{
	argDib.nWidth = req.nWidth;
	argDib.nHeight = req.nHeight;
	argDib.nTime = req.nTime;
	argDib.strName = req.strName;
	argDib.strPath = req.strPath;
	argDib.bMode = req.bMode;

	if( argDib.bMode!=true )	// Photo Mode
		Photo();
	else
		Video();

	return true;
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "photographing" );
	ros::NodeHandle n;
	
	CPhotographing potographing( n );

	ROS_INFO( "photographing start" );

	ros::spin();

	return 0;
}
