#pragma once


#include "akLinker.h"
#include <vector>

//���� ����
//float version;
//size_t structTotalSize;//����ü �� ũ��
//size_t structNum;//����ü ����
//char rev[128]; //����� �ӽð���
//size_t nSize[] ������ ����ü ũ��
//void* pData //���� ������ ���� ����

namespace akSTL
{
	struct akSTL_DLLSPEC _akFileMgrHeader
	{
		float version;

		size_t structTotalSize;//����ü �� ũ��
		size_t structNum;//
		char rev[128];
	};

	class akSTL_DLLSPEC CakFileMgrAdv
	{
	public:
		CakFileMgrAdv(void);
		~CakFileMgrAdv(void);

	public:
		//���� ����
		bool writeFile(char* pFileName, float fVer=1.0);

		//���� �б�
		//0�� ���� 1�� ����
		//10001 �о�� ����ü ũ�Ⱑ ���Ϻ��� ŭ(struct size > file size)
		//10002 �о�� ����ü ũ�Ⱑ ���Ϻ��� ŭ(struct size > file size)
		//10003 �о�� ����ü�� ����ũ�Ⱑ ����. ������ ����ü ������ �ٸ�(struct size == file size)
		int readFile(char* filepath, bool bErrorCheck = false);

		//������/ũ�� ����(������ ���������� �о �� �����ͷ� ����)
		bool editFile(char* filepath, int nIndex, void* pData, size_t nSize);


		//������/ũ�� �ʱ�ȭ
		void clear();
		//������/ũ�� �߰�
		void addDataSize(void* pData, size_t nSize);
		//������/ũ�� ����
		void setDataSize(int nIndex, void* pData, size_t nSize);

		_akFileMgrHeader getFileHeaderInfo(char* pFileName);
		
	
		
		
	private:
		struct _datasize
		{
			_datasize(void* pData1, size_t nSize1)
			{
				pData = pData1;
				nSize = nSize1;
			}
			void* pData;
			size_t  nSize;
		};
		std::vector<_datasize> m_vecData;

		
	};
}