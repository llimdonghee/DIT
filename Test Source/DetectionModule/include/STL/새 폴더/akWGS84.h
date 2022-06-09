#pragma once

#include "akSTLLinker.h"

class AKSTL_DLLSPEC CakWGS84
{
public:
	CakWGS84(void);
	~CakWGS84(void);

	//- 중부원점: N38, E127
	static void getPosition(double lat1 , double lon1 , double x , double y , double &lat2 , double &lon2);
	static void getRange(double lat1 , double lon1 , double lat2 , double lon2 , double &x , double &y);

	static void _TM2Bessel(double tm_x, double tm_y, double lonOrg, double latOrg, double *lon, double *lat);
	static void _Bessel2TM(double lon, double lat, double lonOrg, double latOrg, double *tm_x, double *tm_y);
	static void Bessel2TM (double alt, double lon, double lat, double lonOrg, double latOrg, double *tm_x, double *tm_y);

	//태현추가
	static double getBearing(double lon, double lat, double lonOrg, double latOrg);
	static void getRangePosition(double bearing, double range, double lonOrg, double latOrg, double *lon, double *lat);
};
