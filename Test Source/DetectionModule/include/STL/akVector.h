#pragma once

#include "akSTLLinker.h"
#include "akVectorT.h"

//#define CakPointToCPoint(akPoint) CPoint(akPoint.x, akPoint.y)
//#define CPointToCakPoint(point) CakPoint(point.x, point.y)


class AKSTL_DLLSPEC akVector3d : public CakVectorT<double>
{
public:

	akVector3d(){};
	virtual ~akVector3d(){};

	akVector3d(const double& X, const double& Y, const double& Z) 
	{
		x=X,y=Y,z=Z;
	};

	akVector3d operator*(double& a)
	{
		akVector3d returnval;

		returnval.x = x * a;
		returnval.y = y * a;
		returnval.z = z * a;

		return returnval;
	};

	akVector3d operator *(akVector3d &vec)
	{
		akVector3d vc;
		vc.x = y*vec.z - z*vec.y;
		vc.y = z*vec.x - x*vec.z;
		vc.z = x*vec.y - y*vec.x;
		return vc;
	}

	
	

};
