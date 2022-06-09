#pragma once


#include "akLinker.h"
#include <vector>




namespace akSTL
{
	class akSTL_DLLSPEC CakFileMgrB
	{
	public:
		CakFileMgrB(void);
		~CakFileMgrB(void);

		//���� ����
		void SetDataSize(unsigned int HeadSize, unsigned int BodySize);
		bool SetHead(void* head);
		bool AddBody(void* body);
		void ClearWriteBuffer(); //�����ҷ��� ������ ������ ��� ����

		//�б� ����
		void MakeFile(char* filepath);
		bool OpenFile(char* filepath);
		void* GetBodyData();
		void* GetHeadData();
		int MoveNext();
		void ClearReadBuffer();

	private:
		int m_headSize, m_bodySize;
		void* m_WriteHead;
		std::vector<void*> m_vecWriteBody;
		int m_readindex;
		void* m_ReadHead;
		std::vector<void*> m_vecReadBody;
	};
}