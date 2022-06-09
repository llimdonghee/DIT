#pragma once

#include "akSTLLinker.h"
#include "akPointT.h"

#define CakPointToCPoint(akPoint) CPoint(akPoint.x, akPoint.y)
#define CPointToCakPoint(point) CakPoint(point.x, point.y)

class AKSTL_DLLSPEC CakPoint : public CakPointT<long>
{
public:

	CakPoint(){};
	virtual ~CakPoint(){};

	CakPoint(const long& X, const long& Y) 
	{
		x=X,y=Y;
	};

	void test();
};

class AKSTL_DLLSPEC CakPointf : public CakPointT<float>
{
public:

	CakPointf(){};
	virtual ~CakPointf(){};

	CakPointf(const float& X, const float& Y) 
	{
		x=X,y=Y;
	};
};

class AKSTL_DLLSPEC CakPointd : public CakPointT<double>
{
public:

	CakPointd(){};
	virtual ~CakPointd(){};

	CakPointd(const double& X, const double& Y) 
	{
		x=X,y=Y;
	};
};