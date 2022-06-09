#pragma once

#include "afxcmn.h"
#include <vector>
#include <process.h>

class CakStaticText : public CStatic
{
public:
	CakStaticText(void);
	virtual ~CakStaticText(void);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

private:
	CBrush m_brush;
	COLORREF m_backcolor;
	COLORREF m_textcolor;

	bool m_bColorUpdate;

	CFont m_font;
	UINT m_nFormat;

	bool m_bFastMode;

	int m_nDrawOutline;
	COLORREF m_clrOutline;
	COLORREF m_clrOutline2;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//MFC의 도움을 받지 않고.. 직접 텍스트를 그린다(정렬값 설정해줘야됨. 기본으로 좌측 위에 표시가됨)
	void SetFastMode();
	void setColor(COLORREF textcolor,COLORREF backcolor);
	void setTextColor(COLORREF textcolor);
	void setBkColor(COLORREF backcolor);
	void setFont(char* fontName);
	void setFontSize(int size);
	void setOutLine(int nOutlineType, COLORREF color);

	//정렬(기본설정:DT_SINGLELINE | DT_LEFT | DT_TOP)
	//DT_TOP            //DT_SINGLELINE
	//DT_LEFT           //DT_EXPANDTABS
	//DT_CENTER         //DT_TABSTOP
	//DT_RIGHT          //DT_NOCLIP
	//DT_VCENTER        //DT_EXTERNALLEADING
	//DT_BOTTOM         //DT_CALCRECT
	//DT_WORDBREAK      //DT_NOPREFIX
	//DT_INTERNAL
	void SetAlign(UINT format);
};
