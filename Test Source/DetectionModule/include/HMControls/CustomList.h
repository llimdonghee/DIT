#pragma once
#include "afxcmn.h"

class AFX_EXT_CLASS CCustomList :
	public CListCtrl
{
public:
	CCustomList(void);
	~CCustomList(void);

protected:
	COLORREF		m_EvenBkColor;
	COLORREF		m_EvenTextColor;
	COLORREF		m_OddBkColor;
	COLORREF		m_OddTextColor;

public:
	void			SetEvenBKColor(COLORREF cl) { m_EvenBkColor = cl; }
	void			SetEvenTextColor(COLORREF cl) { m_EvenTextColor = cl; }
	void			SetOddBKColor(COLORREF cl) { m_OddBkColor = cl; }
	void			SetOddTextColor(COLORREF cl) { m_OddTextColor = cl; }

protected:
	afx_msg void OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};
