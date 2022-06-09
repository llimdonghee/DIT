#pragma once
#include "afxcmn.h"
#include <map>

struct _GroupCtrlOption
{
	_GroupCtrlOption()
	{
		nStyle=1;
		nStyleSub=0;

		{
			nResourceID = 136;
			rectMargin.SetRect(50,50,10,10);
		}


		//clrTitle = RGB(175,210,255);
		clrTitle = RGB(83,156,255);
		clrBack = RGB(227, 239, 255);
		clrOutline = RGB(104, 147, 207);
		clrMark = RGB(64, 191, 79);

		clrFontNormal = RGB(0,0,0);
		clrFontPush = RGB(0,0,0);
		clrFontMouseOver = RGB(128, 128, 128);
		clrFontDisable = RGB(150,150,150);
		clrTransparency = 0xFFFFFE;

		bTextEnable = TRUE;
		nTextPosX = 20;
		nTextPosY = 6;
		strFontName = "";
		nFontSize = 20;

		bTextShadow = FALSE;
		nTextShadowX = nTextShadowY=1;

		nTitleHeight = 30;
	}
	int nStyle;//0:�̹��� ���, 1:��ο� ���
	int nStyleSub;//

	//�̹��� ��� ������
	int nResourceID;
	CRect rectMargin;


	COLORREF clrTitle;
	COLORREF clrBack;
	COLORREF clrOutline;
	COLORREF clrTransparency;
	COLORREF clrMark;


	//������°��� ����[2016/9/19]
	bool bTextEnable;
	CString strFontName;
	int nFontSize;
	int nTextPosX, nTextPosY; //����� �������� ����ǥ�� OFFSET

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//��Ʈ �׸��� ��� ����[2016/9/19]
	bool bTextShadow;
	int nTextShadowX, nTextShadowY;

	int nTitleHeight;
	COLORREF clrReserved[16];//�׸��� ��Ÿ�Ͽ� ���� �˾Ƽ�!!
};

class CakGroupCtrl
{
public:
	CakGroupCtrl(void);
	~CakGroupCtrl(void);

	void clear(); //DestroyWindow()���� clearȣ���ؾߵ�!!

	void setCtrlOption(_GroupCtrlOption CtrlOption);
	_GroupCtrlOption getCtrlOption(){return m_CtrlOption;};

	void addControl(int nIDCtl, CWnd* pParent);


	CStatic* GetDlgItem(int nIDCtl);

protected:
	std::map<int,void*> m_mapControls;
	_GroupCtrlOption m_CtrlOption;
};
