#pragma once

#define _AKTRACE

#if defined _AKTRACE
#define AKTRACEEXT(fmt,...) CakTraceExt::setTraceWithDateTime(fmt,##__VA_ARGS__) 
#else
#define AKTRACEEXT(fmt,...)
#endif

#if defined _AKTRACE
#define AKTRACE(fmt,...) CakTraceExt::getInstance()->setTrace(fmt,##__VA_ARGS__) 
#else
#define AKTRACE(fmt,...)
#endif


#include "akTrace.h"
#include "akTextExt.h"
#include <Windows.h>
class akSTL_DLLSPEC CakTraceExt
{
protected:
	CakTraceExt(void);
	virtual ~CakTraceExt(void);

public:
	//////////////////////////////////////////////////////////////////////////
	static CakTrace *getInstance() {return &m_Instance;};	
	
	static void setTraceWithDateTime(char* format, ...)
	{
		char text[512]={};
		va_list ap;
		va_start(ap, format);
		vsprintf(text, format, ap);
		va_end(ap);

		m_Instance.setTrace("%s%s : %s", AKTEXT->getDate(), AKTEXT->getTime(), text);
	};

	

protected:
	static CakTrace m_Instance;
	
};
