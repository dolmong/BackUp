#include "ros/ros.h"
#include "rpi_cam/CamData.h"


class COrder
{

public:
	COrder( ros::NodeHandle &n );

public:
	void SetMode( bool bTmp );
	void SetWidth( int nTmp );
	void SetHeight( int nTmp );
	void SetTime( int nTmp );
	void SetName( std::string strTmp );
	void SetPath( std::string strTmp );
	
	bool SendSrv();

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
        ros::ServiceClient client_;
        rpi_cam::CamData srvCamdata;

	arg argDib;

};


COrder::COrder(
	ros::NodeHandle &n
	)
{
	nh_ = n;

	client_ = nh_.serviceClient< rpi_cam::CamData >( "photographing" );
	
	argDib.bMode = false;
        argDib.nWidth = 1920;
        argDib.nHeight = 1280;
        argDib.nTime = 5000;
	argDib.strName = std::string("");
	argDib.strPath = std::string("");
}


void COrder::SetMode(
	bool bTmp
	)
{
	argDib.bMode = bTmp;
}


void COrder::SetWidth(
	int nTmp
	)
{
	argDib.nWidth = nTmp;
}


void COrder::SetHeight(
	int nTmp
        )
{
	argDib.nHeight = nTmp;
}


void COrder::SetTime(
	int nTmp
	)
{
	argDib.nTime = nTmp;
}


void COrder::SetName(
	std::string strTmp
	)
{
	argDib.strName = strTmp;
}


void COrder::SetPath(
	std::string strTmp
	)
{
	argDib.strPath = strTmp;
}


bool COrder::SendSrv()
{
	srvCamdata.request.bMode	= argDib.bMode;
	srvCamdata.request.nWidth	= argDib.nWidth;
	srvCamdata.request.nHeight	= argDib.nWidth;
	srvCamdata.request.nTime	= argDib.nTime;
	srvCamdata.request.strPath	= argDib.strPath;
	srvCamdata.request.strName	= argDib.strName;

	if( client_.call( srvCamdata ) )
		return true;
	else
		return false;
}


int main(
	int argc,
	char **argv
	)
{
	ros::init( argc, argv, "rpi_cam_node" );

	ros::NodeHandle n;

	COrder order( n );

	for( int i=0; i<argc; i++ )
	{
		if( argv[i]==std::string("-help") || argv[i]==std::string("--h") )
		{
			ROS_INFO( "-m: Camera Mode (p: Photo, v: Video)" );
			ROS_INFO( "-w: Image Width" );
			ROS_INFO( "-h: Image Height" );
			ROS_INFO( "-t: Video Time" );
			ROS_INFO( "-n: Image Output & Name" );
			ROS_INFO( "-p: Image Path (Basic Path: rpi_cam/picture/)" );

                        return 0;
                }
                else if( argv[i]==std::string("-m") )
                {
                        ++i;
                        if( argv[i]==std::string("p") )
                        {
				ROS_INFO( "Camera Mode: photo" );
				order.SetMode( false );
                        }
                        else if( argv[i]==std::string("v") )
                        {
                                ROS_INFO( "Camera Mode: Video" );
                        	order.SetMode( true );
			}
                }
                else if( argv[i]==std::string("-w") )
                {
                        ROS_INFO( "Image Width: [%s]", argv[++i] );
                        order.SetWidth( atoll( argv[i] ) );
                }
                else if( argv[i]==std::string("-h") )
                {
                        ROS_INFO( "Image Height: [%s]", argv[++i] );
                        order.SetHeight( atoll( argv[i] ) );
                }
		else if( argv[i]==std::string("-n") )
		{
			ROS_INFO( "Image Name: [%s]", argv[++i] );
			order.SetName( argv[i] );
		}
		else if( argv[i]==std::string("-p") )
		{
			ROS_INFO( "Image Path: [%s]", argv[++i] );
			order.SetPath( argv[i] );
		}
	}

	if( order.SendSrv() )
	{
		ROS_INFO( "rpi_cam Complete" );
	}
	else
	{
		ROS_INFO( "Failed to call service rpi_cam" );
		return 1;
	}

	return 0;
}
