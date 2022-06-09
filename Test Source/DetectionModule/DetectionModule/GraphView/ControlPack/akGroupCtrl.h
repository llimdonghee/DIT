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
	int nStyle;//0:이미지 기반, 1:드로우 기반
	int nStyleSub;//

	//이미지 기반 변수들
	int nResourceID;
	CRect rectMargin;


	COLORREF clrTitle;
	COLORREF clrBack;
	COLORREF clrOutline;
	COLORREF clrTransparency;
	COLORREF clrMark;


	//글자출력관련 태현[2016/9/19]
	bool bTextEnable;
	CString strFontName;
	int nFontSize;
	int nTextPosX, nTextPosY; //정가운데 기준으로 글자표시 OFFSET

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//폰트 그림자 출력 태현[2016/9/19]
	bool bTextShadow;
	int nTextShadowX, nTextShadowY;

	int nTitleHeight;
	COLORREF clrReserved[16];//그리는 스타일에 따라서 알아서!!
};

class CakGroupCtrl
{
public:
	CakGroupCtrl(void);
	~CakGroupCtrl(void);

	void clear(); //DestroyWindow()에서 clear호출해야됨!!

	void setCtrlOption(_GroupCtrlOption CtrlOption);
	_GroupCtrlOption getCtrlOption(){return m_CtrlOption;};

	void addControl(int nIDCtl, CWnd* pParent);


	CStatic* GetDlgItem(int nIDCtl);

protected:
	std::map<int,void*> m_mapControls;
	_GroupCtrlOption m_CtrlOption;
};
