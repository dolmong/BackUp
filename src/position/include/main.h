#ifndef MAIN_H
#define MAIN_H

// Header
	#include <ros/ros.h>
	#include <cv_bridge/cv_bridge.h>
	#include <sensor_msgs/Image.h>
	#include "surf.h"
	#include "matching.h"

// Structual
	typedef struct _SurfDetection
	{
		CSURF surf;
		CMatching mat;
		std::string strOrg;
		sensor_msgs::Image imgOrg;
		std::string strHeight;
		sensor_msgs::Image imgHeight;
		std::string strSurf;
		std::vector<Interestpoint> ipSurf;
	}sd;

// Variable
	sd sdRef[9];
	sd sdInput;

// Funtion
	void SurfIn( std::string strPath, std::string strName );
	void SurfRef( std::string strPath, std::string strName, int num );
	void Matching( sd &sdInTmp, sd *sdRefTmp, int nRefNum );

#endif
