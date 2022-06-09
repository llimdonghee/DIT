#pragma once




#include "akLinker.h"
#include <queue>
#include <windows.h>

class akSTL_DLLSPEC CakTrace
{
public:
	CakTrace(void);
	virtual ~CakTrace(void);

	enum Mode
	{
		TM_MEMORY = 0,
		TM_FILE
	};
	//0은 메모리에 데이터 관리 하는 모드, 1은 파일에 데이터를 관리 하는 모드
	virtual void setMode(Mode nMode);

	//TRACE
	virtual void setTrace(char* format, ...);




	//////////////////////////////////////////////////////////////////////////
	//파일모드 일경우 사용 함수
	//////////////////////////////////////////////////////////////////////////
	
	//저장할 파일 이름 지정
	bool setTraceFileName(char* filename, bool bClear = true);




	//////////////////////////////////////////////////////////////////////////
	//메모리 모드 일경우 사용 함수
	//////////////////////////////////////////////////////////////////////////
	//저장공간 초기화
	void clear(); 
	//데이터 갯수 획득
	int getTraceNum();
	//저장된 데이터 획득
	char* getTraceFront();
	//젤 앞에 데이터를 획득하고 삭제
	void getTracePop(char* pBuffer);
	//메모리에 있는 데이터를 파일로 저장
	void setWriteFile(char* filename, bool bAdd = true);
	

protected:
	std::queue<char*> m_vecTraceData;
	char* m_pTraceFileName;
	Mode m_nMode;
	CRITICAL_SECTION	m_csTrace;
	FILE* m_pf;
};
