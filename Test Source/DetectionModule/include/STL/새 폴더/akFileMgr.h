#pragma once



#include "akLinker.h"
#include <vector>

struct akSTL_DLLSPEC _Attribute
{
	_Attribute()
	{
		memset(Title		, 0, sizeof(char)*128);
		memset(Attribute	, 0, sizeof(char)*128);
		memset(Value		, 0, sizeof(char)*128);
	}
	char Title[128];
	char Attribute[128];
	char Value[128];
};

struct akSTL_DLLSPEC _Title
{
	_Title(char* title)
	{
		strcpy(Title, title);
	};
	bool compare(char* title)
	{
		if(strcmp(title, Title) == 0 && strlen(title) == strlen(Title))
		{
			return true;
		}
		return false;
	};
	void clear()
	{
		memset(Title, 0, sizeof(char)*128);
	};
	char Title[128];
};

namespace akSTL
{
	class akSTL_DLLSPEC CakFileMgr
	{
	public:
		CakFileMgr(void);
		~CakFileMgr(void);

		//Ÿ��Ʋ ����(OpenFile()�Լ��� MakeFile()�Լ��� ȣ���ϸ� Ÿ��Ʋ������ Null�� �ȴ�)
		void SetTitle(char* title);

		//���� ����
		//Ÿ��Ʋ�� ���Ѵ�.
		bool SetAttribute(char* Attribute, int value);
		bool SetAttribute(char* Attribute, double value);
		bool SetAttribute(char* Attribute, char* value);
		bool SetAttribute(char* Attribute, float value);
		//���� ���ۿ� ����� �����͸� ���Ϸ� ����
		bool MakeFile(char* filepath);
		

		//�б� ����
		//���� ����(�����غ����̿��ٸ� ���� ���۴� �ʱ�ȭ �ȴ�)
		bool OpenFile(char* filepath);
		bool GetAttribute(char* Attribute, int& value);
		bool GetAttribute(char* Attribute, double& value);
		bool GetAttribute(char* Attribute, char* value);
		bool GetAttribute(char* Attribute, float& value);
		
	private:
		//���� ã�⸦ ���ؼ� ����� ������ ����
		void DataSort();
		char m_Title[128];
		std::vector<_Attribute> m_data;

		char* m_buffer;
		int m_fastIndex;
	};
}