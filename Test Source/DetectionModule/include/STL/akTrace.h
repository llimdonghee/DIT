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
	//0�� �޸𸮿� ������ ���� �ϴ� ���, 1�� ���Ͽ� �����͸� ���� �ϴ� ���
	virtual void setMode(Mode nMode);

	//TRACE
	virtual void setTrace(char* format, ...);




	//////////////////////////////////////////////////////////////////////////
	//���ϸ�� �ϰ�� ��� �Լ�
	//////////////////////////////////////////////////////////////////////////
	
	//������ ���� �̸� ����
	bool setTraceFileName(char* filename, bool bClear = true);




	//////////////////////////////////////////////////////////////////////////
	//�޸� ��� �ϰ�� ��� �Լ�
	//////////////////////////////////////////////////////////////////////////
	//������� �ʱ�ȭ
	void clear(); 
	//������ ���� ȹ��
	int getTraceNum();
	//����� ������ ȹ��
	char* getTraceFront();
	//�� �տ� �����͸� ȹ���ϰ� ����
	void getTracePop(char* pBuffer);
	//�޸𸮿� �ִ� �����͸� ���Ϸ� ����
	void setWriteFile(char* filename, bool bAdd = true);
	

protected:
	std::queue<char*> m_vecTraceData;
	char* m_pTraceFileName;
	Mode m_nMode;
	CRITICAL_SECTION	m_csTrace;
	FILE* m_pf;
};
