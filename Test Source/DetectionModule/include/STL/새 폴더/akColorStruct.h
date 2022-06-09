#pragma once

#include "khLinker.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace KimHeart
{
	struct KIMHEART_DLLSPEC khVector3d
	{
		double pos[3];

		khVector3d();
		khVector3d(double x,double y, double z);
		inline void set(double x,double y, double z);
		inline double x(){return pos[0];};
		inline double y(){return pos[1];};
		inline double z(){return pos[2];};

		double& operator()(int index){return pos[index];};

		inline khVector3d operator+(const khVector3d& vec3d);
		inline khVector3d operator-(const khVector3d& vec3d);
		inline khVector3d& operator=(const khVector3d& vec3d);
		inline khVector3d& operator+=(const khVector3d& vec3d);
		inline khVector3d& operator-=(const khVector3d& vec3d);
		khVector3d operator*(float& a)
		{
			khVector3d returnval;

			returnval.pos[0] = pos[0] * a;
			returnval.pos[1] = pos[1] * a;
			returnval.pos[2] = pos[2] * a;

			return returnval;
		};

		khVector3d operator *(khVector3d &vec)
		{
			khVector3d vc;
			vc.pos[0] = pos[1]*vec.pos[2] - pos[2]*vec.pos[1];
			vc.pos[1] = pos[2]*vec.pos[0] - pos[0]*vec.pos[2];
			vc.pos[2] = pos[0]*vec.pos[1] - pos[1]*vec.pos[0];
			return vc;
		}

		float Dot(khVector3d vec)
		{
			return (float)(vec.pos[0] * pos[0] + vec.pos[1] * pos[1] + vec.pos[2] * pos[2]);
		};
		float Mag()
		{
			return (float)sqrt(pos[0]*pos[0]+pos[1]*pos[1]);
		};
		void Normalize()
		{
			float length;

			//������ ���̸� ����Ѵ�.
			length = (float)sqrt((pos[0]*pos[0]) + (pos[1]*pos[1]) +(pos[2]*pos[2]));
			// ���̰� 0�� ����� ���Ϳ��� ������ ���� �״��Ͽ� ���α׷��� �������� �ʵ��� �Ѵ�.
			if(length == 0.0f) length = 1.0f;

			// �� ������ ������ ���̷� ������ ���� ���Ͱ� �ȴ�.
			pos[0] = pos[0] / length;
			pos[1] = pos[1] / length;
			pos[2] = pos[2] / length;

		};
	};

	struct KIMHEART_DLLSPEC khVector2d
	{
		double pos[2];

		khVector2d();
		khVector2d(double x,double y);
		inline void set(double x,double y);
		inline double x(){return pos[0];};
		inline double y(){return pos[1];};

		double& operator()(int index){return pos[index];};

		inline khVector2d operator+(const khVector2d& vec3d);
		inline khVector2d operator-(const khVector2d& vec3d);
		inline khVector2d& operator=(const khVector2d& vec3d);
		inline khVector2d& operator+=(const khVector2d& vec3d);
		inline khVector2d& operator-=(const khVector2d& vec3d);
		
	};

	struct KIMHEART_DLLSPEC khColor4f
	{
		float color[4];

		khColor4f();
		khColor4f(float R, float G, float B, float A=0.0f);
		void set(float R, float G, float B, float A=0.0f);
		inline float R(){return color[0];};
		inline float G(){return color[1];};
		inline float B(){return color[2];};
		inline float A(){return color[3];};
		inline khColor4f& operator=(const khColor4f& ksgcolor);
	};

	struct KIMHEART_DLLSPEC khRect
	{
		double pos[4];

		khRect();
		khRect(double left, double top, double right, double bottom);
		void set(double left, double top, double right, double bottom);
		inline khRect& operator=(const khRect& rect);
		inline double Left(){return pos[0];};
		inline double Top(){return pos[1];};
		inline double Right(){return pos[2];};
		inline double Bottom(){return pos[3];};
		inline double Width(){return pos[2]-pos[0];};
		inline double Height(){return pos[3]-pos[1];};
		inline void SetAlign(); //�������� ����, ��, ū���� ������ �Ʒ��� ���� �Ѵ�.
		inline bool CheckAreaIn(double x, double y); //�簢�� ������ x,y����Ʈ�� �ִ��� �˻�
		inline bool CheckWidthIn(double p1); //x����Ʈ ��� ���� �ִ��� �˻�
		inline bool CheckHeightIn(double p1); //y����Ʈ ��� ���� �ִ��� �˻�


	};
};
