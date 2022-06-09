#pragma once

#include "afxcmn.h"


class CakStaticGroup : public CStatic
{
public:
	CakStaticGroup(void);
	~CakStaticGroup(void);
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	void clear();
	void setImage(UINT bitmapid, int drawType =0);
	void setTransparency(bool bflag, COLORREF color = RGB(255,255,254));

	void setFont(char* fontName, int fontSize);//Null이거나 -1이면 기존값 그대로 사용
public:
	void SmartBitmapScale(CDC* pDC, CRect rectDC/*그려질곳의 위치*/, CBitmap* bitmap);
	void SmartBitmapScaleTransparent(CDC* pDC, CRect rectDC/*그려질곳의 위치*/, CBitmap* bitmap,  COLORREF trColor);

public:
	CRect m_rectMargin;//smartbitmapscale 그리는 여백

	bool m_bTransparency;
	COLORREF m_TransparencyColor;

	CFont m_fontTitle;
	COLORREF m_colorTitle;

	UINT m_nTitleTextAlign; //TA_LEFT|TA_BOTTOM
	int m_nTitlePosX;
	int m_nTitlePosY;

	CRect m_rectClient;
protected:
	CBitmap m_bitImage;
	BITMAP	m_bitImageInfo;
	bool m_bImageLoad;
	int m_nDrawtype;




public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnPaint();
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
