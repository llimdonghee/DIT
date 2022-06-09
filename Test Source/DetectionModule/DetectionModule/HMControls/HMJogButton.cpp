// HMJogButton.cpp : implementation file
//

#include "stdafx.h"
#include "HMJogButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMJogButton

CHMJogButton::CHMJogButton()
{
	m_strMessageDown	= _T("");
	m_strMessageUp		= _T("");

	m_nMessageDown		= -1;
	m_nMessageUp		= -1;
}

CHMJogButton::~CHMJogButton()
{
}


BEGIN_MESSAGE_MAP(CHMJogButton, CButton)
	//{{AFX_MSG_MAP(CHMJogButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMJogButton message handlers

void CHMJogButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nMessageDown == -1)
		m_pJ2M->SendMessageToParent((LPCTSTR)m_strMessageDown);
	else
		m_pJ2M->SendMessageToParent(m_nMessageDown);

	CButton::OnLButtonDown(nFlags, point);
}

void CHMJogButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nMessageUp == -1)
		m_pJ2M->SendMessageToParent((LPCTSTR)m_strMessageUp);
	else
		m_pJ2M->SendMessageToParent(m_nMessageUp);
	
	CButton::OnLButtonUp(nFlags, point);
}
