#pragma once
#include "afxwin.h"

//20141208 윤정민
//다이얼로그 컨트롤 및 배경색 바꾸고 싶을때 상속받아 쓴다.
//다이얼로그마다 선언하기 귀찮아서 만듦
class CGradientStatic;
class CBGSetDlg :
	public CDialog
{
public:
	CBGSetDlg(void);
	virtual ~CBGSetDlg(void);
	CBGSetDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	
	void	SetBGColor(COLORREF cl) { m_clBGColor = cl; }
	void	SetCtrlColor(COLORREF cl) { m_clCtrlColor = cl; }
	void	SetBtnColor(COLORREF clBtn = RGB(114,109,82)
					, COLORREF clBtn2 = RGB(74,69,42)
					, COLORREF clText = RGB(255, 255, 255)
					, COLORREF clSelected = RGB(225, 226, 148)
					, COLORREF clFocus = RGB(242, 148, 54)
					, COLORREF clOut1 = RGB(255, 255, 255)
					, COLORREF clOut2 = RGB(160, 160, 160));
	void	SetTitleBar(CGradientStatic *pstaticbar, CString strTitle
					, COLORREF clTitle1 = RGB(155,117,78)
					, COLORREF clTitle2 = RGB(85,47,8)
					, COLORREF clText = RGB(255,255,255)
					, COLORREF clOut1 = RGB(0,0,0)
					, COLORREF clOut2 = RGB(70,50,10));

protected:
	long	m_clBGColor;
	long	m_clCtrlColor;
	long	m_clBtnColor;
	long	m_clBtnColor2;
	long	m_clBtnTextColor;
	long	m_clBtnSelectedColor;
	long	m_clBtnFocusColor;
	long	m_clBtnOutColor1;
	long	m_clBtnOutColor2;
	void	DrawButton(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
