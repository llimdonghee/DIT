// InPlaceComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceComboBox.h"
#include "HMListCtrlExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceComboBox

CInPlaceComboBox::CInPlaceComboBox(int iItem,int iSubItem)
{
	m_iItem = iItem;
	m_iSubItem = iSubItem;
}

CInPlaceComboBox::~CInPlaceComboBox()
{
}


BEGIN_MESSAGE_MAP(CInPlaceComboBox, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceComboBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceComboBox message handlers

int CInPlaceComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* font = GetParent()->GetFont() ;
	SetFont(font) ;

	SetFocus() ;
	SetCurSel(0) ;
	// TODO: Add your specialized creation code here
	
	return 0;
}


void CInPlaceComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetLBText(GetCurSel(),str);
	((CHMListCtrlExt*)GetParent())->InsertListItem(m_iItem,m_iSubItem,(LPSTR)(LPCSTR)str);
	ShowWindow(SW_HIDE);
}

void CInPlaceComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
//	DestroyWindow();
}
