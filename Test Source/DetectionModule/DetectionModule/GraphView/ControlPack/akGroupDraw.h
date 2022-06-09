#pragma once

#include "afxcmn.h"


struct _GroupDrawOption
{
	_GroupDrawOption()
	{
		nDrawStyle=0;

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
		nFontSize = 30;

		bTextShadow = FALSE;
		nTextShadowX = nTextShadowY=1;

		nTitleHeight = 30;
	}
	int nDrawStyle;//0:

	COLORREF clrTitle;
	COLORREF clrBack;
	COLORREF clrOutline;
	COLORREF clrTransparency;
	COLORREF clrMark;

	int nTitleHeight;
	//글자출력관련 태현[2016/9/19]
	bool bTextEnable;
	CString strFontName;
	int nFontSize;
	int nTextPosX, nTextPosY; //좌상단기준 기준으로 글자표시 OFFSET

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//폰트 그림자 출력 태현[2016/9/19]
	bool bTextShadow;
	int nTextShadowX, nTextShadowY;

	COLORREF clrReserved[16];//그리는 스타일에 따라서 알아서!!
};


class CakGroupDraw : public CStatic
{
public:
	CakGroupDraw(void);
	~CakGroupDraw(void);
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	void clear();

	void setGroupDrawOption(_GroupDrawOption Option);
	_GroupDrawOption getGroupDrawOption(){ return m_Option;};

	void setFont( char* fontName, int fontSize );
protected:
	void drawGroup(CDC* pDC, CRect rect, _GroupDrawOption option);
	void drawText(CDC* pDC, CRect rect, _GroupDrawOption option);

	COLORREF getBrightColor(COLORREF color, double dBright);
	void fillRectGradientT2B(CDC* pDC, CRect rect, COLORREF colorTop, COLORREF colorBottom);
protected:
	_GroupDrawOption m_Option;
	CFont m_fontTitle;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnPaint();
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
