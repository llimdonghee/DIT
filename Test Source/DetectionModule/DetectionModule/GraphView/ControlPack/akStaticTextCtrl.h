#pragma once
#include "afxcmn.h"
#include <map>

struct _StaticTextCtrlOption
{
	_StaticTextCtrlOption()
	{
		nStyle=1;
		nStyleSub=0;

		{
			nResourceID = 136;
			rectMargin.SetRect(50,50,10,10);
		}

		nTextAlign = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
		//clrTitle = RGB(175,210,255);
		clrTitle = RGB(83,156,255);
		clrBack = RGB(227, 239, 255);
		clrOutline = RGB(104, 147, 207);

		clrFontNormal = RGB(0,0,0);
		clrFontPush = RGB(0,0,0);
		clrFontMouseOver = RGB(128, 128, 128);
		clrFontDisable = RGB(150,150,150);
		clrTransparency = 0xFFFFFE;

		bTextEnable = TRUE;
		nTextPosX = 0;
		nTextPosY = 0;
		strFontName = "";
		nFontSize = 0;

		bTextShadow = FALSE;
		nTextShadowX = nTextShadowY=1;

		nDrawOutlineType = 1;
		
	}
	int nStyle;//0:이미지 기반, 1:드로우 기반
	int nStyleSub;//

	//이미지 기반 변수들
	int nResourceID;
	CRect rectMargin;


	COLORREF clrTitle; //FontColor
	COLORREF clrBack;
	COLORREF clrOutline;
	COLORREF clrTransparency;

	bool bTextEnable;
	CString strFontName;
	int nFontSize;
	int nTextPosX, nTextPosY; //정가운데 기준으로 글자표시 OFFSET
	UINT nTextAlign;

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//폰트 그림자 출력 태현[2016/9/19]
	bool bTextShadow;
	int nTextShadowX, nTextShadowY;

	int nDrawOutlineType;

	COLORREF clrReserved[16];//그리는 스타일에 따라서 알아서!!
};

class CakStaticTextCtrl
{
public:
	CakStaticTextCtrl(void);
	~CakStaticTextCtrl(void);

	void clear(); //DestroyWindow()에서 clear호출해야됨!!

	void setCtrlOption(_StaticTextCtrlOption CtrlOption);
	_StaticTextCtrlOption getCtrlOption(){return m_CtrlOption;};

	void addControl(int nIDCtl, CWnd* pParent);


	CStatic* GetDlgItem(int nIDCtl);

protected:
	std::map<int,void*> m_mapControls;
	_StaticTextCtrlOption m_CtrlOption;
};
