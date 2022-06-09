// XIcon.h  Version 1.0 - see article at CodeProject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XICON_H
#define XICON_H


//=============================================================================
class CXIcon : public CButton
//=============================================================================
{
// Construction
public:
	CXIcon(BOOL bCheckType = false);
	virtual ~CXIcon();

// Attributes
public:	
	CXIcon& SetAutoResize(BOOL bAutoResize, BOOL bRedraw = TRUE);
	CXIcon& SetBold(BOOL bBold, BOOL bRedraw = TRUE);
	CXIcon& SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	CXIcon& SetFont(LOGFONT * pLogFont, BOOL bRedraw = TRUE);
	CXIcon& SetFont(LPCTSTR lpszFaceName, int nPointSize, BOOL bRedraw = TRUE);
	CXIcon& SetIcon(HICON hIcon, UINT nIconSize = 16, BOOL bRedraw = TRUE);
	CXIcon& SetIcon(UINT nIconId, UINT nIconSize = 16, BOOL bRedraw = TRUE);
	CXIcon& SetIconSpacing(int nIconSpacing, BOOL bRedraw = TRUE);
	CXIcon& SetMargins(int nXMargin, int nYMargin, BOOL bRedraw = TRUE);
	CXIcon& SetTextColor(COLORREF cr, BOOL bRedraw = TRUE);
	CXIcon& SetWindowText(LPCTSTR lpszText, BOOL bRedraw = TRUE);
	CXIcon& SetColor(COLORREF cr, BOOL bRedraw = TRUE);
	CXIcon& SetHover(bool bHover, COLORREF crHover, BOOL bRedraw = TRUE);
	CXIcon& SetClick(COLORREF crClick, BOOL bRedraw = TRUE);
	CXIcon& SetMultiLineText(BOOL bMultiLine, BOOL bRedraw = TRUE);
	enum class iconLocation { top, left, right };
	CXIcon& SetIconLocation(iconLocation loc);


	BOOL GetAutoResize() { return m_bAutoResize; }
	BOOL GetBold();
	CFont* GetFont() { return &m_font; }
	BOOL GetFont(LOGFONT *pLF);
	CString GetFontFaceName();
	int GetFontPointSize();
	int GetIconSpacing() { return m_nIconSpacing; }
	void GetMargins(int& nXMargin, int& nYMargin)
	{ nXMargin = m_nXMargin; nYMargin = m_nYMargin; }
	COLORREF GetTextColor() { return m_crText; }

	int GetCheck() const { return m_bCheckBoxFlag; };
	void SetCheck(int nCheck);
	void SetUpdateStateNotify(HWND hParent) { m_hNotifyParent = hParent; };

// Operations
public:
	void Resize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXIcon)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont		m_font;						// font used for text
	COLORREF	m_crText;					// text color
	int			m_nXMargin, m_nYMargin;		// space in pixels between border
											// and icon/text
	int			m_nIconSpacing;				// horizontal space in pixels 
											// between icon and text
	HICON		m_hIcon;					// icon handle
	UINT		m_nIconSize;				// size of icon, default to 16
	BOOL		m_bAutoResize;				// TRUE = XIcon will resize itself
											// to content
	BOOL		m_bDestroyIcon;				// TRUE = icon will be destroyed
											// by XIcon (internal only)
	COLORREF	m_crBk;						// Background color
	COLORREF	m_crHover;					// Hover Color
	COLORREF	m_crClick;
	COLORREF	m_crDisable;

	BOOL m_bMultiLine;

	bool m_bHover;
	bool m_bEnableHover;
	bool m_bCheckBoxType;
	bool m_bCheckBoxFlag;
	HWND m_hNotifyParent;

	iconLocation m_iconLocation;

	

	int			DrawIcon(CDC *pDC, CRect& rect);
	int			DrawText(CDC *pDC, CRect& rect);
	int			GetFontHeight(int nPointSize);
	int			GetFontPointSize(int nHeight);
	CFont *		GetSafeFont();

	// Generated message map functions
	//{{AFX_MSG(CXIcon)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
public:
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XICON_H
