
// ImageInspection.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageInspectionApp:
// �� Ŭ������ ������ ���ؼ��� ImageInspection.cpp�� �����Ͻʽÿ�.
//

class CImageInspectionApp : public CWinAppEx
{
public:
	CImageInspectionApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageInspectionApp theApp;