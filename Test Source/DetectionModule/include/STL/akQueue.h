#pragma once


#include "akSTLLinker.h"
#include <memory.h>

	class AKSTL_DLLSPEC CakQueue
	{
	public:
		CakQueue(void);
		~CakQueue(void);

	public:
		bool empty()
		{	
			return m_nDataSize == 0 ? true : false;
		}

		size_t size()
		{
			return m_nDataSize;
		}

		char* front()
		{
			return &m_pQueue[m_stPoint];
		}

		char* back()
		{
			return &m_pQueue[m_stPoint+m_nDataSize-1];
		}

		void push(char _Val)
		{
			if(m_stPoint+m_nDataSize >= m_nMemSize)
			{
				size_t newsize = m_nMemSize*2;
				char* pNewQueue = new char [newsize];
				memcpy(pNewQueue, &m_pQueue[m_stPoint], sizeof(char)*(m_nMemSize-m_stPoint));
				m_nMemSize = newsize;
				m_stPoint = 0;
				char* pTemp = m_pQueue;
				m_pQueue = pNewQueue;
				delete [] pTemp;

			}

			
			m_pQueue[m_stPoint+m_nDataSize] = _Val;
			m_nDataSize++;
		}

		void push(char* pVal, int nSize)
		{
			if(m_stPoint+m_nDataSize + nSize >= m_nMemSize)
			{
				size_t newsize = m_nMemSize+nSize*2;
				char* pNewQueue = new char [newsize];
				memcpy(pNewQueue, &m_pQueue[m_stPoint], sizeof(char)*(m_nMemSize-m_stPoint));
				m_nMemSize = newsize;
				m_stPoint = 0;
				char* pTemp = m_pQueue;
				m_pQueue = pNewQueue;
				delete [] pTemp;

			}

			//m_enPoint++;
			memcpy(&m_pQueue[m_stPoint+m_nDataSize], pVal, sizeof(char)*nSize);
			m_nDataSize+=nSize;
		}

		void pop()
		{
			m_stPoint++;
			m_nDataSize--;

			if(m_nDataSize == 0)
			{
				m_stPoint=m_nDataSize=0;
			}
		}
		void pop(int nSize)
		{
			m_stPoint += nSize;
			m_nDataSize -= nSize;
			
			if(m_nDataSize == 0)
			{
				m_stPoint=m_nDataSize=0;
			}
		}
	private:
		size_t m_stPoint; //메모리 시작위치
		size_t m_nDataSize; //메모리 끝 위치
		size_t m_nMemSize; //할당된 메모리 사이즈
		char*  m_pQueue;
		
	};
