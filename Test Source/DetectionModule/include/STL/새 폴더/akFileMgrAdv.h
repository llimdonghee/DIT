#pragma once


#include "akLinker.h"
#include <vector>

//파일 구조
//float version;
//size_t structTotalSize;//구조체 총 크기
//size_t structNum;//구조체 갯수
//char rev[128]; //예약된 임시공간
//size_t nSize[] 각각의 구조체 크기
//void* pData //실제 데이터 저장 공간

namespace akSTL
{
	struct akSTL_DLLSPEC _akFileMgrHeader
	{
		float version;

		size_t structTotalSize;//구조체 총 크기
		size_t structNum;//
		char rev[128];
	};

	class akSTL_DLLSPEC CakFileMgrAdv
	{
	public:
		CakFileMgrAdv(void);
		~CakFileMgrAdv(void);

	public:
		//파일 쓰기
		bool writeFile(char* pFileName, float fVer=1.0);

		//파일 읽기
		//0은 실패 1은 정상
		//10001 읽어올 구조체 크기가 파일보다 큼(struct size > file size)
		//10002 읽어올 구조체 크기가 파일보다 큼(struct size > file size)
		//10003 읽어올 구조체와 파일크기가 같음. 하지만 구조체 구성이 다름(struct size == file size)
		int readFile(char* filepath, bool bErrorCheck = false);

		//데이터/크기 수정(기존의 파일정보를 읽어서 새 데이터로 수정)
		bool editFile(char* filepath, int nIndex, void* pData, size_t nSize);


		//데이터/크기 초기화
		void clear();
		//데이터/크기 추가
		void addDataSize(void* pData, size_t nSize);
		//데이터/크기 수정
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