#pragma once

#include "akLinker.h"

namespace akSTL
{
	class akSTL_DLLSPEC CakSyncObject
	{
	public:
		CakSyncObject(void);
		virtual ~CakSyncObject(void);

	public:
		//읽기 지정을 하였을때... 이미 어디서 쓰고 있다면.. 기다렸다가 다음을 수행한다.
		//bWait = false 일경우.. 어딘가에서 쓰고 있다면 false를 반환하고 끝내버린다.
		bool SetRead(bool bWait = true);  
		//쓰기 지정을 하였을때... 이미 어디서 읽고 있다면.. 기다렸다가 다음을 수행한다.
		bool SetWrite(bool bWait = true);
		int SetReadRelease(); //현재 남아있는 읽기 카운트 반환
		void SetWriteRelease();

		int GetReadCount(){return m_nReadCount;}; //읽기 호출한 횟수 반환
		bool GetWriteState(){return m_bWrite;}; //0이 될때까지 그리는 루틴은 무한 루프가 돈다.
		

	protected:
		
	private:
		bool m_bWrite; 
		int m_nReadCount;
		
	};
}