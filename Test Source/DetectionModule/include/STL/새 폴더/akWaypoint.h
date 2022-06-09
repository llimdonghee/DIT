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
		void pointInsert(unsigned int index, khVector3d point);//범위를 넘어가면 젤 뒤에 추가
		void pointDelete(unsigned int index);
		
		int size(){return m_nPointsNum;};
		
		khVector3d& operator [](unsigned int nindex){return m_pPoints[nindex];};
		CakWaypoint& operator= (CakWaypoint& waypoint);

	private:
		void secureMemory();

	private:
		int m_nMakePoints; //확보 메모리 사이즈
		int m_nPointsNum; //현재 입력된 Point 갯수
		khVector3d* m_pPoints; //데이터 변수


	};
}