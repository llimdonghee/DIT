#pragma once


#include "akSTLLinker.h"

	template <typename T> 
	class CakQueueCircle
	{
	public:
		CakQueueCircle();
		~CakQueueCircle();
		
		
		void clear(); //�޸� ������ �ȵ�
		void setMemSize(unsigned int memsize); //memsize�� 0�϶� �޸� ����
		
		void pushData(T point);
		void setData( unsigned int nindex , T point );
		void getData(T* pPointBuffer, unsigned int nBufferLength);
		
		int size(){return m_nPointNum;};

		T& operator [](unsigned int nindex);
		CakQueueCircle& operator= (CakQueueCircle& traj);

		//static unsigned int s_TrajectorySize;


		//�����ϸ� �Ʒ��� ��� ������ ���� ������ ���� ����!!
	public:
		int m_nMemSize; //Ȯ�� �޸� ������
		int m_nPointIndex; //���� �ε���
		int m_nPointNum; //���� �Էµ� Point ����
		T* m_pPoints; //������ ����
	public:
		unsigned long m_nPushCount;
	};


	template <typename T>
	 CakQueueCircle<T>::CakQueueCircle(void)
	{
		m_pPoints = NULL;
		m_nMemSize = 0;
		m_nPushCount = 0;

		clear();
		setMemSize(100);
	};


	template <typename T>
	CakQueueCircle<T>::~CakQueueCircle(void)
	{
		setMemSize(0);
	};

	template <typename T> 
	void CakQueueCircle<T>::clear() //�޸� ������ �ȵ�
	{
		m_nMemSize; //Ȯ�� �޸� ������
		m_nPointIndex = -1; //���� �ε���
		m_nPointNum = 0; //���� �Էµ� Point ����
		//m_pPoints; //������ ����
		m_nPushCount = 0;
	}

	template <typename T> 
	void CakQueueCircle<T>::setMemSize(unsigned int memsize) //memsize�� 0�϶� �޸� ����
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
	void CakQueueCircle<T>::pushData(T point)
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
		m_nPushCount++;
	}

	template <typename T>
	void CakQueueCircle<T>::setData( unsigned int nindex , T point )
	{
		int pointindex = m_nPointIndex-nindex;

		if(pointindex<0) pointindex = m_nPointNum + pointindex;

		m_pPoints[pointindex] = point;
	}

	template <typename T> 
	T& CakQueueCircle<T>::operator[]( unsigned int nindex )
	{
		int pointindex = m_nPointIndex-nindex;

		if(pointindex<0) pointindex = m_nPointNum + pointindex;

		return m_pPoints[pointindex];
	}

	template <typename T> 
	CakQueueCircle<T>& CakQueueCircle<T>::operator=( CakQueueCircle<T>& queuecircle )
	{
		if(m_nMemSize != queuecircle.m_nMemSize)
		{
			if(m_pPoints)
			{
				delete [] m_pPoints;
			}

			m_pPoints = new T[queuecircle.m_nMemSize];
		}


		m_nMemSize		= queuecircle.m_nMemSize; 
		m_nPointIndex	= queuecircle.m_nPointIndex;
		m_nPointNum		= queuecircle.m_nPointNum; 

		memcpy(m_pPoints, queuecircle.m_pPoints, sizeof(T)*queuecircle.m_nMemSize);



		return *this;
	}