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
		//����ΰ� '/', '\\'�� �����ϸ� �ȵ�(���߿� �����ϵ��� ����..API�����ε�)
		static bool CopytoFolder(char* srcfilename, char* cpyFolder); 

		//����ΰ� '/', '\\'�� �����ϸ� �ȵ�, ���ϵ� ī�� ��밡��
		static bool RemoveFile(char* filename);
	};
}
