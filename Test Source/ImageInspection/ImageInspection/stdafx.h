
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����



#include <vector>
#include <afxcontrolbars.h>
#include <afxwin.h>

#define		MAX_CAMERA_CCD_SIZE		16384
#define		MAX_FRAME_HEIGHT		8192
#define		CHANNEL_COUNT	4
#define		ERR_NONE	0
#define		MAX_VERTEX	4

typedef struct tag_fPoint
{
	double x;
	double y;
} fPoint;

typedef struct tag_fLine
{
	tag_fPoint p1;
	tag_fPoint p2;

} fLine;

typedef struct
{
	int  iType;
	RECT rcROI;
} SaveROI;

typedef struct
{
	int  iType;
	RECT rcROI;
} GraySaveROI;
/*
typedef struct
{
	RECT rcROI;

	int iMergeDis;
	int iThreshold;
	int iMinSize;
	int iMargin;
} EdgeROI;
/*/
typedef struct
{
	RECT rcROI;

	int iMergeDis;
	int iThreshold;
	int iMinSize;
	int iOffset;
	int iThresholdH;
} EdgeROI;

//*/
typedef struct
{
	int	   iDefCount;
	double fTime_FOV;
	double fTime_PM;
	double fTime_Mask;
	double fTime_PixelInsp;
	double fTime_Merge;
	double fTime_PAD;
	double fTime_Macro;
} TimeInfo;

template <typename T>
void Swap(T &a, T &b)
{
	T c;
	c = a;
	a = b;
	b = c;
}

template <typename T>
int Round(T a)
{
	if (a < 0)
	{
		return static_cast<int>(-a + 0.5)*-1;
	}
	return static_cast<int>(a + 0.5);
}



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


