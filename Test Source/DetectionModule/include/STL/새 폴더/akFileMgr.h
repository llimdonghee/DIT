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

		//타이틀 설정(OpenFile()함수와 MakeFile()함수를 호출하면 타이틀셋팅은 Null이 된다)
		void SetTitle(char* title);

		//쓰기 관련
		//타이틀을 정한다.
		bool SetAttribute(char* Attribute, int value);
		bool SetAttribute(char* Attribute, double value);
		bool SetAttribute(char* Attribute, char* value);
		bool SetAttribute(char* Attribute, float value);
		//쓰기 버퍼에 저장된 데이터를 파일로 저장
		bool MakeFile(char* filepath);
		

		//읽기 관련
		//파일 열기(쓰기준비중이였다면 쓰기 버퍼는 초기화 된다)
		bool OpenFile(char* filepath);
		bool GetAttribute(char* Attribute, int& value);
		bool GetAttribute(char* Attribute, double& value);
		bool GetAttribute(char* Attribute, char* value);
		bool GetAttribute(char* Attribute, float& value);
		
	private:
		//빠른 찾기를 위해서 저장된 데이터 정렬
		void DataSort();
		char m_Title[128];
		std::vector<_Attribute> m_data;

		char* m_buffer;
		int m_fastIndex;
	};
}