#pragma once

#include "akLinker.h"

class akSTL_DLLSPEC CakText
{
public:
	CakText(void);
	virtual ~CakText(void);
	
	//////////////////////////////////////////////////////////////////////////
	//�ð� ��ȯ �Լ�
	char* getTime();
	char* getTime(char* pSeparator);
	char* getTime(char* pSeparator1, char* pSeparator2, char* pSeparator3);

	//////////////////////////////////////////////////////////////////////////
	//���� ��ȯ �Լ�
	char* getDate();
	char* getDate(char* pSeparator);
	char* getDate(char* pSeparator1, char* pSeparator2, char* pSeparator3);
	
	//////////////////////////////////////////////////////////////////////////
	//�ý��� ���� ��ȯ
	char* getModuleFileName(); //����Ǵ� ���� �̸� ��ȯ
	char* getCurrentDirectory(); //���� �۾� ���� ��ȯ


	//////////////////////////////////////////////////////////////////////////
	//���� ���� �Լ�
	//���������� ���� ���� ũ�⸦ �䱸�ϴ� ���ڿ��� ���
	//�ڵ����� ũ�⸦ �� �Ҵ��ϹǷ� ������ ����ũ�⸦ ���� ���� �ʾƵ� �����ƿ�~
public:
	char* getBuffer(int index){return m_pBuffer[index];};
	int getBufferSize(){return m_nBuffersize;};
	int getBufferNum(){return m_nBufferNum;};
	void setBufferSize(int nNum, int nSize);
	
private:
	char** m_pBuffer; //m_nBufferNum*m_nBuffersize
	int m_nBufferNum; //�⺻ ���۰���8��
	int m_nBuffersize; //�⺻ũ�� 64
	int m_nBufferCurIndex;
};


