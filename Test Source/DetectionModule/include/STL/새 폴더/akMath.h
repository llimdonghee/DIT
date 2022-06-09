#pragma once


#include "akLinker.h"
#include "akSTL.h"

namespace akSTL
{

	class akSTL_DLLSPEC CakMath
	{
	public:
		CakMath(void);
		~CakMath(void);

	public:
		//세점이 이루는 각도를 산출
		double GetAngle3Point(double cX, double cY, double x1, double y1, double x2, double y2 );
		//평면 폴리곤위에 점이 존재 하는지 검사
		bool GetIsPointOnPolygon(khVector2d point, khVector2d* polygon, int length); 
		//한 선위에 점이 있는지 검사
		bool GetLineOnPoint(khVector3d point, khVector3d line1, khVector3d line2);
		//2d 회전 좌표(중심점에서 
		khVector2d GetRotatePosition(khVector2d center, khVector2d point1, double angle);

		//두선의 교차점 구하기(교차점이 있으면 true 반환)
		bool GetIntersectPoint(khVector2d a1, khVector2d a2, khVector2d b1, khVector2d b2, khVector2d* IP);
	};

}