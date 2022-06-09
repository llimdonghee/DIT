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
		//������ �̷�� ������ ����
		double GetAngle3Point(double cX, double cY, double x1, double y1, double x2, double y2 );
		//��� ���������� ���� ���� �ϴ��� �˻�
		bool GetIsPointOnPolygon(khVector2d point, khVector2d* polygon, int length); 
		//�� ������ ���� �ִ��� �˻�
		bool GetLineOnPoint(khVector3d point, khVector3d line1, khVector3d line2);
		//2d ȸ�� ��ǥ(�߽������� 
		khVector2d GetRotatePosition(khVector2d center, khVector2d point1, double angle);

		//�μ��� ������ ���ϱ�(�������� ������ true ��ȯ)
		bool GetIntersectPoint(khVector2d a1, khVector2d a2, khVector2d b1, khVector2d b2, khVector2d* IP);
	};

}