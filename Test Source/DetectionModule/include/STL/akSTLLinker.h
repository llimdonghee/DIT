#pragma once

//#pragma warning(disable:4251)
// 
// typedef unsigned int uint;
// typedef unsigned char byte;

#ifdef AKSTL_EXPORTS
#define AKSTL_DLLSPEC    __declspec( dllexport )
#else
#define AKSTL_DLLSPEC    __declspec( dllimport )
#endif



#ifndef AKSTL_EXPORTS

	#undef _AUTOLIBNAME
	#undef _AKPROJECTNAME
	#undef _AKVCVER
	#undef _AKDEBUG
	#undef _AKWINDOWSDK

	#define _AKPROJECTNAME "STL"


	#ifdef _DEBUG
	#define _AKDEBUG "d"
	#else
	#define _AKDEBUG ""
	#endif

	#if(_MSC_VER < 1910)
	#define _AKWINDOWSDK ""
	#else
	#define _AKWINDOWSDK "_WS10"
	#endif   


	#ifdef WIN64 
	#define _AKX64 "_x64"
	#elif _WIN64 
	#define _AKX64 "_x64"
	#else        
	#define _AKX64 ""
	#endif

	#define _AUTOLIBNAME _AKPROJECTNAME""_AKDEBUG""_AKWINDOWSDK""_AKX64".lib"

	// You may turn off this include message by defining _NOPSAUTOLIB
	#ifndef _NOPSAUTOLIBMSG
		//#pragma message( ">>Advance Kim Templet Library<< Will automatically link with " _AUTOLIBNAME )
		#pragma message( ">>Kim Tae Hyun - STL<< Will automatically link with " _AUTOLIBNAME )
	#endif
		#pragma comment(lib, _AUTOLIBNAME)
#endif
