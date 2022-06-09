#pragma once

#include "akLinker.h"

class akSTL_DLLSPEC CakText
{
public:
	CakText(void);
	virtual ~CakText(void);
	
	//////////////////////////////////////////////////////////////////////////
	//시간 반환 함수
	char* getTime();
	char* getTime(char* pSeparator);
	char* getTime(char* pSeparator1, char* pSeparator2, char* pSeparator3);

	//////////////////////////////////////////////////////////////////////////
	//날자 반환 함수
	char* getDate();
	char* getDate(char* pSeparator);
	char* getDate(char* pSeparator1, char* pSeparator2, char* pSeparator3);
	
	//////////////////////////////////////////////////////////////////////////
	//시스템 정보 반환
	char* getModuleFileName(); //실행되는 파일 이름 반환
	char* getCurrentDirectory(); //현재 작업 폴더 반환


	//////////////////////////////////////////////////////////////////////////
	//버퍼 관리 함수
	//내부적으로 많은 양의 크기를 요구하는 문자열의 경우
	//자동으로 크기를 재 할당하므로 별도의 버퍼크기를 조절 하지 않아도 괜찮아요~
public:
	char* getBuffer(int index){return m_pBuffer[index];};
	int getBufferSize(){return m_nBuffersize;};
	int getBufferNum(){return m_nBufferNum;};
	void setBufferSize(int nNum, int nSize);
	
private:
	char** m_pBuffer; //m_nBufferNum*m_nBuffersize
	int m_nBufferNum; //기본 버퍼갯수8개
	int m_nBuffersize; //기본크기 64
	int m_nBufferCurIndex;
};


