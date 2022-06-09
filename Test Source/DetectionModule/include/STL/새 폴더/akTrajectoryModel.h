#pragma once


#include "akLinker.h"
#include "akSTL.h"
#include "akDE.h"
#include <vector>
namespace akSTL
{
	struct _Waypoint
	{
		double x;
		double y;
		double z;
		double speed;
	};

	class akSTL_DLLSPEC CakTrajcetoryModel
	{
	public:
		CakTrajcetoryModel(void);
		~CakTrajcetoryModel(void);

	public:
		virtual int calculate(float dTime); //더이상 계산할 경로점이 없을 경우 2를 반환
		void setCurrentByWaypoint(int nWaypointIndex); //현재값을 경로점 기준으로 셋팅

	

	public:
		//현재 값
		khVector3d m_position; //위치
		khVector3d m_rotate; //자세
		double m_velocity; //속도
		double m_acc;	//가속도
		khVector3d m_rotateAcc; //각속도

		//부여 받은 명령 설정
		khVector3d m_TaskPosition;
		double m_TaskVelocity;
		double m_TaskAcc;

		//객체 정보 관리
		double mass; //무게
		double maxSpeed; //최대 속도
		double minSpeed; //최저 속도
		double maxAcceleration; //가속도
		double maxDeceleration; //감속도

		khVector3d m_rotateVelRate;

		//경로점 관리
	public:
		int m_nWaypointType; //0 마지막 경로점이면 끝낸다, 1경로점 순회, 2경로점 왕복
		int m_nWaypointSet; //현재 설정된 경로점
		std::vector<_Waypoint> m_vecWaypoint;


	protected:
		void getAngle3d(double x1, double y1, double z1, double x2, double y2, double z2, double* heading, double* pitch);
		double getHeading(double x , double y);
	protected:
		CakDE m_movementmodel;
	};

}