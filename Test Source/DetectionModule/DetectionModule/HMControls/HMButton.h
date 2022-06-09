#if !defined(AFX_HMBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
#define AFX_HMBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMButton by Niek Albers
// Thanks to some people for the tooltip.
// A cool CBitmapButton derived class with 3 states,
// Up/Down/HM.
class AFX_EXT_CLASS CHMButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CHMButton);

	// Construction
public:
	CHMButton();
	void SetToolTipText(CString* spText, BOOL bActivate = TRUE);
	void SetToolTipText(int nId, BOOL bActivate = TRUE);
	void SetWindowText(CString strText);
	void SetFontSize(int nSize);
	void SetTextColor(COLORREF rgbNormal, COLORREF rgbDisable);
	void SetHMButtonStyle(UINT uID, int nImgCount);
	void SetHMButtonStyle(UINT uID, int nImgCount, CString spTipText);
	void SetHMButtonStyle(UINT uID, int nImgCount, CString spTipText, CString spBtnText, int nFontSize);

	CString m_strText;
	CDC      m_gMemDC;

// Attributes
protected:
	void fPaint(CDC* pDC);
	void ActivateTooltip(BOOL bActivate = TRUE);
	BOOL m_bHM;						// indicates if mouse is over the button
	CSize m_ButtonSize;					// width and height of the button
	CBitmap m_Bitmap;
	BOOL m_bTracking;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMButton)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadBitmap(UINT bitmapid, UINT uiImageCount);
	virtual ~CHMButton();

	// Generated message map functions
protected:
	CToolTipCtrl m_ToolTip;
	void InitToolTip();
	//{{AFX_MSG(CHMButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHOver(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	UINT m_uiImageCount;
	CFont	m_Font;
	LOGFONT		m_lf;
	COLORREF m_RGBTextNormal;
	COLORREF m_RGBTextDisable;
	BOOL m_bIsDisable;	// Disable일 때는 글자색을 바꾸기 위해.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
