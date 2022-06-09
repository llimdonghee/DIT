#if !defined(AFX_HEADCTRL_H__E483AE98_F737_4DBF_8508_8E934A8F6988__INCLUDED_)
#define AFX_HEADCTRL_H__E483AE98_F737_4DBF_8508_8E934A8F6988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeadCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl window
#ifndef __AFXTEMPL_H
#include <afxtempl.h>
#endif

class CHeadCtrl : public CHeaderCtrl
{
// Construction
public:
	struct HEADCELL
	{
		DWORD   state;
		CString string;
	};
	class CHeadCell : public HEADCELL
	{
	public:
		CHeadCell()
		{
			state=0;
			string="";
		}
		CHeadCell(HEADCELL& hcell)
		{
			*this=hcell;
		}
		CHeadCell operator=(HEADCELL& hcell)
		{
			state=hcell.state;
			string=hcell.string;
			return *this;
		}
	};

	typedef CArray<CHeadCell,CHeadCell&> CHeadItem;
	typedef CArray<CHeadItem,CHeadItem&> CHeadInfo;

public:
	CHeadCtrl();
	void SetCount(int nCount, CFont* pFont);
	int  GetCount(){return m_count;}
	int  GetHeadHeight();


	void InsertColumn(int nCol);
	void DeleteColumn(int nCol);
	void FontChange(LOGFONT logfont);
	BOOL SetItemText(int nItem, int nSubItem, LPCSTR lpcstr);
	CString GetItemText(int nItem, int nSubItem);
	BOOL SetItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem); 
	UINT GetItemState(int nItem, int nSubItem, int& down, int& right);
	BOOL GetRect(int nItem, int nSubItem, CRect& rect);
protected:
	BOOL m_bclick;
	CPoint m_pnt;

	CFont m_font;
	int m_count;
	CHeadInfo m_Info;
	BOOL IsDraw(CRect rect);
	BOOL GetTextRect(CDC* pDC, CString strText, CRect& rect);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeadCtrl)
	virtual void OnPaint();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeadCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeadCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADCTRL_H__E483AE98_F737_4DBF_8508_8E934A8F6988__INCLUDED_)
