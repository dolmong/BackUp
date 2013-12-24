#include "filesystem.h"
#include <sstream>

namespace enc=sensor_msgs::image_encodings;

CFileSystem::CFileSystem()
{
}


CFileSystem::~CFileSystem()
{
}


bool CFileSystem::WriteImage(
	sensor_msgs::Image &image,
	std::string strName
	)
{
	cv_bridge::CvImagePtr cv_ptr;
	try
	{
		cv_ptr = cv_bridge::toCvCopy( image, enc::MONO8 );
		image.data.clear();
	}
	catch( cv_bridge::Exception &e )
	{
		ROS_ERROR( "cv_bridge exception: [%s]", e.what() );
		return false;
	}

	cv::imwrite( strName.c_str(), cv_ptr->image );
	cv_ptr->image.release();

	return true;
}


bool CFileSystem::ReadImage(
	sensor_msgs::Image &image,
	std::string strName
	)
{
	cv_bridge::CvImagePtr cv_ptr, gray;
	cv_ptr.reset( new cv_bridge::CvImage );
	gray.reset( new cv_bridge::CvImage );

	cv_ptr->image = cv::imread( strName.c_str() );
	if( cv_ptr->image.empty()!=false )
	{
		ROS_ERROR( "CFileSystem(ReadImage): Image read fail!" );
		return false;
	}
	ROS_INFO( "Image File Open: [%s]", strName.c_str() );

	if( cv_ptr->image.type()==16 )	// RGB Image file
	{
		cv::cvtColor( cv_ptr->image, gray->image, CV_BGR2GRAY );
		gray->encoding = enc::MONO8;
	}	
	else	// Gray Image file
	{
		cv_ptr->image.copyTo( gray->image );
		gray->encoding = enc::MONO8;
	}

	image.data.clear();
	gray->toImageMsg( image );

	if( image.data.size()==0 )
	{
		ROS_ERROR( "(ReadImage) cv::Mat to sensor_msgs::Image Copy fail!" );
		cv_ptr->image.release();
		gray->image.release();
		return false;
	}

	cv_ptr->image.release();
	gray->image.release();

	return true;
}


bool CFileSystem::ReadSurf(
	std::vector<Interestpoint> &ipSurf,
	std::string strName
	)
{
	FILE *fileSurf=fopen( strName.c_str(), "r" );
	if( fileSurf==NULL )
	{
		ROS_ERROR( "FileSystem(ReadSurf): Cann't Surf File Open!" );
		return false;
	}
	else
	{
		ROS_INFO( "Text File Open: [%s]", strName.c_str() );
		if( ipSurf.size()!=0 )
			ipSurf.clear();

		Interestpoint ipTmp;
		while( fread(&ipTmp, sizeof(Interestpoint), 1, fileSurf)==1 )
			ipSurf.push_back( ipTmp );
	}

	fflush( fileSurf );
	fclose( fileSurf );

	return true;
}


bool CFileSystem::WriteSurf(
	std::vector<Interestpoint> &ipSurf,
	std::string strName
	)
{
	FILE *fileSurf=fopen( strName.c_str(), "w" );

	if( fileSurf==NULL )
	{
		ROS_ERROR( "CFileSystem(WriteSurf): Cann't Surf File Open!" );
		return false;
	}

	ROS_INFO( "Surf File Open: [%s]", strName.c_str() );

	for( unsigned int i=0; i<ipSurf.size(); i++ )
	{
		if( fwrite( &ipSurf[i], sizeof(Interestpoint), 1, fileSurf )!=1 )
		{
			ROS_ERROR( "FileSystem(WriteSurf): Interestpoint vector write fail!" );
			return false;
		}
	}

	fflush( fileSurf );
	fclose( fileSurf );

	return true;
}

/*
std::string CFileSystem::*SplitFileName(
	std::string strPath,
	std::string strName
	)
{
	std::string strFile=strPath;
	unsigned int found=strFile.find_last_of( "/\\" );
	strName = strFile.substr( found+1 );
	strPath = strFile.substr( 0, found );
	ROS_INFO( "File path: [%s], File name: [%s]", strPath.c_str(), strName.c_str() );

	return [strPath, strName];
}
*/

std::string CFileSystem::MergeFileName(
	std::string strPath,
	std::string strName
	)
{
	std::stringstream ss;
	ss << strPath.c_str() << "/" << strName.c_str();
	return ss.str();	
}


std::string CFileSystem::AddFileName(
	std::string strName,
	std::string strAdd
	)
{
	unsigned int found=strName.find_last_of( "." );
	std::string str=strName.substr( 0, found );
	std::stringstream ss;
	ss << str.c_str() << strAdd.c_str();
	return ss.str();
}
