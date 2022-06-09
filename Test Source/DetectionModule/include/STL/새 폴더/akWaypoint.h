#pragma once


#include "akSTLLinker.h"
#include "akSTLVector.h"

namespace akSTL
{
	class AKSTL_DLLSPEC CakWaypoint
	{
	public:
		CakWaypoint(void);
		~CakWaypoint(void);

	public:
		void clear();
		void pointAdd(khVector3d point);
		void pointInsert(unsigned int index, khVector3d point);//������ �Ѿ�� �� �ڿ� �߰�
		void pointDelete(unsigned int index);
		
		int size(){return m_nPointsNum;};
		
		khVector3d& operator [](unsigned int nindex){return m_pPoints[nindex];};
		CakWaypoint& operator= (CakWaypoint& waypoint);

	private:
		void secureMemory();

	private:
		int m_nMakePoints; //Ȯ�� �޸� ������
		int m_nPointsNum; //���� �Էµ� Point ����
		khVector3d* m_pPoints; //������ ����


	};
}