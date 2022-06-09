#if !defined(AFX_HMLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_HMLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMLabel window
enum FlashType {None, Text, Background };

class AFX_EXT_CLASS CHMLabel : public CStatic
{
// Construction
public:
	CHMLabel();
	CHMLabel& SetBkColor(COLORREF crBkgnd);
	CHMLabel& SetTextColor(COLORREF crText);
	CHMLabel& SetText(const CString& strText);
	CHMLabel& SetFontBold(BOOL bBold);
	CHMLabel& SetFontName(const CString& strFont);
	CHMLabel& SetFontUnderline(BOOL bSet);
	CHMLabel& SetFontItalic(BOOL bSet);
	CHMLabel& SetFontSize(int nSize);
	CHMLabel& SetSunken(BOOL bSet);
	CHMLabel& SetBorder(BOOL bSet);
	CHMLabel& FlashText(BOOL bActivate);
	CHMLabel& FlashBackground(BOOL bActivate);
	CHMLabel& SetLink(BOOL bLink);
	CHMLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:
protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;
			// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHMLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
