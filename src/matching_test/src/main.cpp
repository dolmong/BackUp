#include "main.h"

CMain::CMain()
{
//	nh_.setParam( "Width", 1280 );
//	nh_.setParam( "Height", 720 );
//	nh_.setParam( "RefPath", "/home/pi/picture/Database" );
//	nh_.setParam( "RefName", "(0,0).bmp" );
//	nh_.setParam( "InPath", "/home/pi/picture/Database" );
//	nh_.setParam( "InName", "(0,1).bmp" );
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

	sensor_msgs::Image imgRef;
	std::string strRefPath;
	main.nh_.getParam( "/Main/RefPath", strRefPath );
	std::string strRefName;
	main.nh_.getParam( "/Main/RefName", strRefName );
	std::string strRefFile=main.filesystem.MergeFileName( strRefPath.c_str(), strRefName.c_str() );
	if( !main.filesystem.ReadImage( imgRef, strRefFile.c_str() ) )
	{
		ROS_ERROR( "Main: ReadImage_ReferenceImage Fail!" );
		return 0;
	}
	else
	{
		main.surfRef.ipSurf.clear();
		std::string strAdd="_surf.txt";
                std::string strRefSurfName=main.filesystem.AddFileName( strRefName.c_str(), strAdd.c_str() );
                std::string strRefSurfFile=main.filesystem.MergeFileName( strRefPath.c_str(), strRefSurfName.c_str() );

		if( !main.filesystem.ReadSurf(main.surfRef.ipSurf, strRefSurfFile.c_str()) )
		{
			main.surfRef.Surf( imgRef, main.surfRef.ipSurf );

			if( !main.filesystem.WriteSurf(main.surfRef.ipSurf, strRefSurfFile.c_str()) )
			{
				ROS_ERROR( "Main: WriteSurf Fail!" );
				imgRef.data.clear();
				return 0;
			}
		}

		sensor_msgs::Image imgIn;
		std::string strInPath;
		main.nh_.getParam( "/Main/InPath", strInPath );
		std::string strInName;
		main.nh_.getParam( "/Main/InName", strInName );
		std::string strInFile=main.filesystem.MergeFileName( strInPath.c_str(), strInName.c_str() );
		if( !main.filesystem.ReadImage(imgIn, strInFile.c_str()) )
		{
			ROS_ERROR( "Main: ReadImage_InputImage Fail!" );
		}
		main.surfIn.Surf( imgIn, main.surfIn.ipSurf );

		strAdd = "_HEIGHT.bmp";
		std::string strRefHeightName=main.filesystem.AddFileName( strRefName.c_str(), strAdd.c_str() );
		std::string strRefHeightFile=main.filesystem.MergeFileName( strRefPath.c_str(), strRefHeightName.c_str() );
		
		main.matching.Matching( imgRef, imgIn, main.surfRef.ipSurf, main.surfIn.ipSurf, strRefHeightFile, main.matching.ipPairMat, main.matching.spMeter );
		imgRef.data.clear();
		imgIn.data.clear();
	}

	ROS_INFO( "Matching test end" );

	return 0;
}
