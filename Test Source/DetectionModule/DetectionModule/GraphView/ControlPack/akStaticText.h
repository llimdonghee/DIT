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
	//MFC�� ������ ���� �ʰ�.. ���� �ؽ�Ʈ�� �׸���(���İ� ��������ߵ�. �⺻���� ���� ���� ǥ�ð���)
	void SetFastMode();
	void setColor(COLORREF textcolor,COLORREF backcolor);
	void setTextColor(COLORREF textcolor);
	void setBkColor(COLORREF backcolor);
	void setFont(char* fontName);
	void setFontSize(int size);
	void setOutLine(int nOutlineType, COLORREF color);

	//����(�⺻����:DT_SINGLELINE | DT_LEFT | DT_TOP)
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
