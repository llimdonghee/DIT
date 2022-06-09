#pragma once

#include "akLinker.h"


namespace akSTL
{
	class akSTL_DLLSPEC khFileUtil
	{
	public:
		khFileUtil(void);
		~khFileUtil(void);

		static bool CopytoFile(char* srcfilename, char* cpyfilename);
		//상대경로가 '/', '\\'로 시작하면 안됨(나중에 수정하도록 하자..API문제인듯)
		static bool CopytoFolder(char* srcfilename, char* cpyFolder); 

		//상대경로가 '/', '\\'로 시작하면 안됨, 와일드 카드 사용가능
		static bool RemoveFile(char* filename);
	};
}
