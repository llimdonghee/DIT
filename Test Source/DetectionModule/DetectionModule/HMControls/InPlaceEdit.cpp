// InPlaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceEdit.h"
#include "HMListCtrlExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(int iItem, int iSubItem, CString sInitText) : m_sInitText(sInitText)
{
	m_iItem = iItem ;
	m_iSubItem = iSubItem ;
	m_bESC = FALSE ;

}

CInPlaceEdit::~CInPlaceEdit()
{			

}


BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers


void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	DestroyWindow() ;
	
}

void CInPlaceEdit::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	
	// TODO: Add your message handler code here

}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if(nChar == VK_ESCAPE)
			m_bESC = TRUE ;
		GetParent()->SetFocus() ;
		return ;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CString str ;

	GetWindowText(str) ;
	CWindowDC dc(this) ;
	CFont *pFont = GetParent()->GetFont() ;
	CFont *pFontDC = dc.SelectObject(pFont) ;
	CSize size = dc.GetTextExtent(str) ;
	dc.SelectObject(pFontDC) ;
	size.cx += 5 ;

	CRect rect, parentrect = CRect(0,0,0,0) ;
	GetClientRect(&rect) ;
	GetParent()->ScreenToClient(&rect) ;

	if(size.cx > rect.Width())
	{
		if (size.cx + rect.left < parentrect.right)
			rect.right = rect.left + size.cx ;
		else 
			rect.right = parentrect.right ;
		MoveWindow(&rect) ;
	}
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	long style;
//	style = GetWindowLong(this->m_hWnd,GWL_STYLE);
//	style |= ES_AUTOVSCROLL;
//	SetWindowLong(this->m_hWnd,GWL_STYLE,style);

	CFont* font = GetParent()->GetFont() ;
	SetFont(font) ;

	SetWindowText(m_sInitText) ;
	SetFocus() ;
	SetSel(0, -1) ;

	return 0;
}

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| GetKeyState(VK_CONTROL))
		{
			::TranslateMessage(pMsg) ;
			::DispatchMessage(pMsg) ;

			if(pMsg->wParam == VK_RETURN)
			{
				CString str ;
				GetWindowText(str) ;

				((CHMListCtrlExt*)GetParent())->InsertListItem(m_iItem,m_iSubItem,(LPSTR)(LPCSTR)str);
			}
			return TRUE ;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}



