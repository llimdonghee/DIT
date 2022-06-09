#pragma once


#include "akLinker.h"
//#include "akSTLVector.h"

//namespace akSTL
//{
#include <vector>




	template <typename T> 
	class CakTrajectory
	{
	public:
		CakTrajectory();
		~CakTrajectory();
		
		
		void clear(); //메모리 삭제는 안됨
		void setMemSize(unsigned int memsize); //memsize가 0일때 메모리 삭제
		
		void pointAdd(T point);
		void pointSet( unsigned int nindex , T point );
		void getPoints(T* pPointBuffer, unsigned int nBufferLength);
		
		int size(){return m_nPointNum;};
		T& operator [](unsigned int nindex);
		CakTrajectory& operator= (CakTrajectory& traj);

		static unsigned int s_TrajectorySize;


		//가능하면 아래의 멤버 변수에 직접 수정은 하지 말것!!
	public:
		int m_nMemSize; //확보 메모리 사이즈
		int m_nPointIndex; //현재 인덱스
		int m_nPointNum; //현재 입력된 Point 갯수
		T* m_pPoints; //데이터 변수
	};



	
	//}

	




	
	template <typename T> 
	unsigned int  CakTrajectory<T>::s_TrajectorySize = 100;


	template <typename T>
	 CakTrajectory<T>::CakTrajectory(void)
	{
		m_pPoints = NULL;
		m_nMemSize = 0;

		clear();
		setMemSize(s_TrajectorySize);
	};


	template <typename T>
	CakTrajectory<T>::~CakTrajectory(void)
	{
		setMemSize(0);
	};

	template <typename T> 
	void CakTrajectory<T>::clear() //메모리 삭제는 안됨
	{
		m_nMemSize; //확보 메모리 사이즈
		m_nPointIndex = -1; //현재 인덱스
		m_nPointNum = 0; //현재 입력된 Point 갯수
		//m_pPoints; //데이터 변수
	}

	template <typename T> 
	void CakTrajectory<T>::setMemSize(unsigned int memsize) //memsize가 0일때 메모리 삭제
	{
		//clear
		if(m_pPoints != NULL)
		{
			clear();
			m_nMemSize = 0;
			delete [] m_pPoints;
			m_pPoints = NULL;
		}

		if(memsize > 0)
		{
			m_nMemSize = memsize;

			m_pPoints = new T [m_nMemSize];
		}

	}

	template <typename T> 
	void CakTrajectory<T>::pointAdd(T point)
	{
		m_nPointIndex++;
		if(m_nPointIndex >= m_nMemSize)
		{
			m_nPointIndex = 0;
		}

		m_nPointNum++;
		if(m_nPointNum >= m_nMemSize)
		{
			m_nPointNum = m_nMemSize;
		}

		m_pPoints[m_nPointIndex] = point;
	}

	template <typename T>
	void CakTrajectory<T>::pointSet( unsigned int nindex , T point )
	{
		int pointindex = m_nPointIndex-nindex;

		if(pointindex<0) pointindex = m_nPointNum + pointindex;

		m_pPoints[pointindex] = point;
	}

	template <typename T> 
	T& CakTrajectory<T>::operator[]( unsigned int nindex )
	{
		int pointindex = m_nPointIndex-nindex;

		if(pointindex<0) pointindex = m_nPointNum + pointindex;

		return m_pPoints[pointindex];
	}

	template <typename T> 
	CakTrajectory<T>& CakTrajectory<T>::operator=( CakTrajectory<T>& traj )
	{
		if(m_nMemSize != traj.m_nMemSize)
		{
			if(m_pPoints)
			{
				delete [] m_pPoints;
			}

			m_pPoints = new T[traj.m_nMemSize];
		}


		m_nMemSize		= traj.m_nMemSize; 
		m_nPointIndex	= traj.m_nPointIndex;
		m_nPointNum		= traj.m_nPointNum; 

		memcpy(m_pPoints, traj.m_pPoints, sizeof(khVector3d)*traj.m_nMemSize);






		return *this;
	}