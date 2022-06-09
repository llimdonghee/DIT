#include "stdafx.h"
#include "akWndArrange.h"


CakWndArrange::CakWndArrange(void)
{
	m_pWndParent = NULL;
	m_rectOriginal.SetRect(0,0,0,0);
}


CakWndArrange::~CakWndArrange(void)
{
	clear();
}

void CakWndArrange::clear()
{
	//m_pWndParent = NULL;
	//m_rectOriginal.SetRect(0,0,0,0);
	m_vecWndChild.clear();
}

void CakWndArrange::setParentWnd( CWnd* pWnd )
{
	m_pWndParent = pWnd;
	pWnd->GetClientRect(m_rectOriginal);
}

void CakWndArrange::addChildWnd( CWnd* pWnd, int nStyle )
{
	
	_WndData data;
	data.nStyle = nStyle;
	data.hWnd = pWnd->GetSafeHwnd();

	pWnd->GetWindowRect(&data.rectOrginal);
	m_pWndParent->ScreenToClient(&data.rectOrginal);

	m_vecWndChild.push_back(data);
}

void CakWndArrange::setChildStyle( CWnd* pWnd, int nStyle )
{
	HWND hWnd = pWnd->GetSafeHwnd();
	for(int i=0; i<m_vecWndChild.size(); i++)
	{
		if(m_vecWndChild[i].hWnd == hWnd)
		{
			m_vecWndChild[i].nStyle = nStyle;
		}
	}
}

void CakWndArrange::process( int nWidth, int nHeight )
{
	if(nWidth*nHeight <= 0) return;
	int nSize = m_vecWndChild.size();

	_WndData WndChild;
	CRect rectParentOrg = m_rectOriginal;
	CRect rectNew;
	for(int i=0; i<nSize; i++)
	{
		WndChild = m_vecWndChild[i];
		
		rectNew = WndChild.rectOrginal;
		if(WA_RIGHTTOP & WndChild.nStyle)
		{
			rectNew.left = nWidth - ((rectParentOrg.right)-WndChild.rectOrginal.left);
			rectNew.right = rectNew.left + WndChild.rectOrginal.Width();
		}
		else if(WA_LEFTBOTTOM & WndChild.nStyle)
		{
			rectNew.top = nHeight - ((rectParentOrg.bottom)-WndChild.rectOrginal.top);
			rectNew.bottom = rectNew.top + WndChild.rectOrginal.Height();
		}
		else if(WA_RIGHTBOTTOM & WndChild.nStyle)
		{
			rectNew.left = nWidth - ((rectParentOrg.right)-WndChild.rectOrginal.left);
			rectNew.right = rectNew.left + WndChild.rectOrginal.Width();
		}
		else if(WA_LEFTTOP & WndChild.nStyle)
		{

		}

		if(WA_RESIZE_WIDTH & WndChild.nStyle)
		{
			rectNew.right += (nWidth-rectParentOrg.Width());
		}
		if(WA_RESIZE_HEIGHT & WndChild.nStyle)
		{
			rectNew.bottom += (nHeight-rectParentOrg.Height());
		}
		
		MoveWindow(WndChild.hWnd, rectNew.left, rectNew.top, rectNew.Width(), rectNew.Height(), TRUE);
	}

}
