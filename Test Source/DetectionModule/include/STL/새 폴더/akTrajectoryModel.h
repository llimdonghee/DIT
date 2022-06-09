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
		virtual int calculate(float dTime); //���̻� ����� ������� ���� ��� 2�� ��ȯ
		void setCurrentByWaypoint(int nWaypointIndex); //���簪�� ����� �������� ����

	

	public:
		//���� ��
		khVector3d m_position; //��ġ
		khVector3d m_rotate; //�ڼ�
		double m_velocity; //�ӵ�
		double m_acc;	//���ӵ�
		khVector3d m_rotateAcc; //���ӵ�

		//�ο� ���� ��� ����
		khVector3d m_TaskPosition;
		double m_TaskVelocity;
		double m_TaskAcc;

		//��ü ���� ����
		double mass; //����
		double maxSpeed; //�ִ� �ӵ�
		double minSpeed; //���� �ӵ�
		double maxAcceleration; //���ӵ�
		double maxDeceleration; //���ӵ�

		khVector3d m_rotateVelRate;

		//����� ����
	public:
		int m_nWaypointType; //0 ������ ������̸� ������, 1����� ��ȸ, 2����� �պ�
		int m_nWaypointSet; //���� ������ �����
		std::vector<_Waypoint> m_vecWaypoint;


	protected:
		void getAngle3d(double x1, double y1, double z1, double x2, double y2, double z2, double* heading, double* pitch);
		double getHeading(double x , double y);
	protected:
		CakDE m_movementmodel;
	};

}