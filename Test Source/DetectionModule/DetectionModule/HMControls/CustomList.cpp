#include "StdAfx.h"
#include "CustomList.h"

BEGIN_MESSAGE_MAP(CCustomList, CListCtrl)
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CCustomList::CCustomList(void)
{
	SetEvenBKColor(RGB(114,109,82));
	SetEvenTextColor(RGB(255,255,255));
	SetOddBKColor(RGB(144,139,112));
	SetOddTextColor(RGB(255,255,255));
}

CCustomList::~CCustomList(void)
{
}

void CCustomList::OnCustomDrawList( NMHDR* pNMHDR, LRESULT* pResult )
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;		
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); 
		if(nItem % 2 == 0)
		{
			pLVCD->clrText = m_EvenTextColor;
			pLVCD->clrTextBk = m_EvenBkColor;
		}
		else
		{
			pLVCD->clrText = m_OddTextColor;
			pLVCD->clrTextBk = m_OddBkColor;
		}	

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;	
	}	
}

BOOL CCustomList::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	CCustomList::GetClientRect(rect);

	POINT mypoint;  

	CBrush brush0(m_EvenBkColor);
	CBrush brush1(m_OddBkColor);

	int chunk_height=GetCountPerPage();
	pDC->FillRect(&rect,&brush1);

	for (int i=0;i<=chunk_height;i++)
	{
		GetItemPosition(i,&mypoint);
		rect.top=mypoint.y ;
		GetItemPosition(i+1,&mypoint);
		rect.bottom =mypoint.y;
		pDC->FillRect(&rect,i %2 ? &brush1 : &brush0);
	}

	brush0.DeleteObject();
	brush1.DeleteObject();

	return FALSE;
}