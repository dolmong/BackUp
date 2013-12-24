#include <boost/thread.hpp>
#include <sstream>

#include "main.h"
#include "filesystem.h"
#include "improc.h"
#include "matching.h"
#include "surf.h"


void SurfIn(
	std::string strPath,
	std::string strName
	)
{
	CFileSystem fs;

	sdInput.strOrg = fs.MergeFileName( strPath.c_str(), strName.c_str() );
	sdInput.imgOrg.data.clear();
	if( !fs.ReadImage(sdInput.imgOrg, sdInput.strOrg.c_str()) )
		ROS_ERROR( "Main: ReadImage_InputImage Fail!" );

	sdInput.surf.Surf( sdInput.imgOrg, sdInput.ipSurf );
}


void SurfRef(
	std::string strPath,
	std::string strName,
	int num
	)
{
	CFileSystem fs;

	sdRef[num].strOrg = fs.MergeFileName( strPath.c_str(), strName.c_str() );

	if( !fs.ReadImage( sdRef[num].imgOrg, sdRef[num].strOrg.c_str() ) )
	{
		ROS_ERROR( "(Main) ReadImage_ReferenceImage Fail!" );
		sdRef[num].imgOrg.data.clear();
		sdRef[num].strOrg.clear();
		return;
	}

	sdRef[num].ipSurf.clear();

	std::string strAdd="_surf.txt";
	std::string strSurfName=fs.AddFileName( strName.c_str(), strAdd.c_str() );
	sdRef[num].strSurf = fs.MergeFileName( strPath.c_str(), strSurfName.c_str() );

	if( !fs.ReadSurf(sdRef[num].ipSurf, sdRef[num].strSurf.c_str()) )
	{
		sdRef[num].surf.Surf( sdRef[num].imgOrg, sdRef[num].ipSurf );

		if( !fs.WriteSurf(sdRef[num].ipSurf, sdRef[num].strSurf.c_str()) )
		{
			ROS_ERROR( "(SurfRef) WriteSurf Fail!" );
			sdRef[num].imgOrg.data.clear();
			return;
		}
	}

	strAdd = "_HEIGHT.bmp";
	std::string strHeightName=fs.AddFileName( strName.c_str(), strAdd.c_str() );
	sdRef[num].strHeight = fs.MergeFileName( strPath.c_str(), strHeightName.c_str() );
}


void Matching(
	sd &sdInTmp,
	sd *sdRefTmp,
	int nRefNum
	)
{
	for( int i=0; i<nRefNum; i++ )
	{
		if( sdRefTmp[i].imgOrg.data.size()!=0 )
			sdRefTmp[i].mat.Matching( sdRefTmp[i].imgOrg, sdInTmp.imgOrg, sdRefTmp[i].ipSurf, sdInTmp.ipSurf, sdRefTmp[i].strHeight, sdRefTmp[i].mat.ipPairMat, sdRefTmp[i].mat.spMeter );
	}
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "Position/Image" );

	ros::NodeHandle nh;

	std::string strInPath;
	nh.getParam( "/Position/InPath", strInPath );
	ROS_INFO( "(Main) Input Path: [%s]", strInPath.c_str() );

	std::string strInName;
	nh.getParam( "/Position/InName", strInName );
	ROS_INFO( "(Main) Input Name: [%s]", strInName.c_str() );

	ROS_INFO( "(Main) Input Image SURF" );
	SurfIn( strInPath.c_str(), strInName.c_str() );

	std::string strRefPath;
	nh.getParam( "/Position/RefPath", strRefPath );
	ROS_INFO( "(Main) Reference Path: [%s]", strRefPath.c_str() );

	int nFind=strInName.find( ")" );
	std::string strTmp=strInName.substr( 1, nFind-1 );
	std::string strX=strTmp.substr( 0, strTmp.length()/2 );
	nFind = strTmp.find( "," );
	std::string strY = strTmp.substr( nFind+1, strTmp.length()-1 );
	int nx=atoi( strX.c_str() );
	int ny=atoi( strY.c_str() );

	std::string strRefName;
	std::stringstream ss;

	ROS_INFO( "(Main) Reference Image SURF" );
	for( int j=0; j<3; j++ )
	{
		for( int i=0; i<3; i++ )
		{
			ss.str("");
			ss << "(" << ny+(j-1) << "," << nx+(i-1) << ").bmp";
			strRefName.empty();
			strRefName = ss.str();
			SurfRef( strRefPath.c_str(), strRefName.c_str(), j*3+i );
		}
	}
	for( int i=0; i<9; i++ )
	{
		ROS_INFO( "(Main) Ref[%d] File: [%s]", i, sdRef[i].strOrg.c_str() );
	}
	ROS_INFO( "(Main) Matching" );
	Matching( sdInput, sdRef, 9 );
}
