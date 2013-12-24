#include "main.h"
#include <sstream>

CMain::CMain()
{
}


CMain::~CMain()
{
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "Main" );

	CMain main;

	sensor_msgs::Image image;
	std::string strPath="/home/pi/picture/Database";
	std::string strName="(0,1).bmp";
	std::string strFile=main.filesystem.MergeFileName( strPath.c_str(), strName.c_str() );
	if( !main.filesystem.ReadImage( image, strFile.c_str() ) )
	{
		ROS_ERROR( "Main: ReadImage Fail!" );
		return 0;
	}

	main.surf.ipSurf.clear();
	std::string strAdd="_surf.txt";
	std::string strSurfName = main.filesystem.AddFileName( strName.c_str(), strAdd.c_str() );
	std::string strSurfFile = main.filesystem.MergeFileName( strPath.c_str(), strSurfName.c_str() );
	if( !main.filesystem.ReadSurf(main.surf.ipSurf, strSurfFile.c_str()) )
	{
		main.surf.Surf( image, main.surf.ipSurf );
		main.filesystem.WriteSurf( main.surf.ipSurf, strSurfFile.c_str() );
	}
	image.data.clear();
	ROS_INFO( "surf test end" );
	return 0;
}
