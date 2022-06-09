#pragma once

#include <afxext.h>         // MFC 익스텐션
#include <afxwin.h> 

#pragma comment(lib, "gdiplus.lib")
#include <GdiPlus.h>
using namespace Gdiplus;


//#ifdef _akGraph
//#include "resource.h"
//#endif


#ifndef _AKGRAPH_EXPORTS

	#undef _AUTOLIBNAME
	#undef _AKPROJECTNAME
	#undef _AKVCVER
	#undef _AKDEBUG

	#define _AKPROJECTNAME "Graph"
	
	#ifdef _DEBUG
		#define _AKDEBUG "d"
	#else
		#define _AKDEBUG ""
	#endif

	#ifdef WIN64 
		#define _AKX64 "_x64"
	#elif _WIN64 
		#define _AKX64 "_x64"
	#else        
		#define _AKX64 ""
	#endif
	
	#if(_MSC_VER == 1200) //vs6.0
		#error 해당버전은 지원하지 않습니다.
	#elif(_MSC_VER == 1300) //vs2003 vc7
		#error 해당버전은 지원하지 않습니다.
	#elif(_MSC_VER == 1400) //vs2005 vc8
		#define _AKVCVER "_vc8"
	#elif(_MSC_VER == 1500) //vs2008 vc9
		#define _AKVCVER "_vc9"
	#elif(_MSC_VER == 1600) //vs2010
		#define _AKVCVER "_vc10"
	#elif(_MSC_VER == 1700) //vs2012
		#error 해당버전은 지원하지 않습니다.
	#elif(_MSC_VER == 1800) //vs2013
		#error 해당버전은 지원하지 않습니다.
	#elif(_MSC_VER >= 1910 && _MSC_VER < 1950) //vs2017 15.0
		#define _AKVCVER "_vc15"
	#else   
		//#define _AKVCVER ""
		#pragma message( "해당버전은 지원하지 않습니다. " _MSC_VER )
		#error _MSC_VER 해당버전은 지원하지 않습니다.
	#endif

	#define _AUTOLIBNAME _AKPROJECTNAME""_AKDEBUG""_AKX64""_AKVCVER".lib"
	
	// You may turn off this include message by defining _NOPSAUTOLIB
	#ifndef _NOPSAUTOLIBMSG
	#pragma message( ">>Kim Tae Hyun - Graph<< Will automatically link with " _AUTOLIBNAME )
	#endif
	#pragma comment(lib, _AUTOLIBNAME)
#endif

