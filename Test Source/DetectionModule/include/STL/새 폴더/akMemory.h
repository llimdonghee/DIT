#pragma once

#include "akLinker.h"

namespace akSTL
{
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)  { if(p) { delete [] (p);     (p)=NULL; } }

	class akSTL_DLLSPEC CakMemory
	{
	public:
		CakMemory(void);
		~CakMemory(void);
	};
}