// HeadCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HeadCtrl.h"
#include "CellCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NORML		0x00000000
#define MERGE		0x00000001
#define UNUSE		0x00000002

#define SetFlag(n)  DWORD(BYTE(n)<<16)
#define GetFlag(d)  int((d & 0x00FF0000)>>16)
#define SetDown(n)  DWORD(BYTE(n)<<8)
#define GetDown(d)  int((d & 0x0000FF00)>>8)
#define SetRight(n) DWORD(BYTE(n))
#define GetRight(d) int((d & 0x000000FF))
/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl

CHeadCtrl::CHeadCtrl()
{
	m_bclick=FALSE;
	m_count=1;
	m_Info.SetSize(1);
}

CHeadCtrl::~CHeadCtrl()
{
	m_Info.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHeadCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeadCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl message handlers

int  CHeadCtrl::GetHeadHeight()
{
	CRect rect;
	GetItemRect(0, &rect);
	return rect.Height();
}

void CHeadCtrl::SetCount(int nCount, CFont* pFont)
{
	LOGFONT logfont;
	pFont->GetLogFont(&logfont);
	m_count=nCount;
	FontChange(logfont);
	m_Info.SetSize(nCount);
}

void CHeadCtrl::DeleteColumn(int nCol)
{
	int nSize = static_cast<int>(m_Info.GetSize());
	for( int i=0; i<nSize; i++)
	{
		if(m_Info[i].GetSize()>nCol)
			m_Info[i].RemoveAt(nCol);
	}
}

void CHeadCtrl::FontChange(LOGFONT logfont)
{
	if(m_font.GetSafeHandle())
		m_font.DeleteObject();

	CFont  font;

	m_font.CreateFontIndirect(&logfont);
	
	logfont.lfHeight-=m_count;
	logfont.lfHeight*=m_count;

	font.CreateFontIndirect(&logfont);
	SetFont(&font);
	
	font.DeleteObject();
}

void CHeadCtrl::InsertColumn(int nCol)
{
	for( int i=0; i<m_count; i++)
	{
		CHeadCell hCell;
		m_Info[i].SetAtGrow(nCol, hCell);
	}
}

BOOL CHeadCtrl::SetItemText(int nItem, int nSubItem, LPCSTR lpcstr)
{
	if(m_Info.GetSize()<=nItem)
		return 0;
	if(m_Info[nItem].GetSize()<=nSubItem)
		return 0;

	m_Info[nItem][nSubItem].string=lpcstr;
	return 1;
}

CString CHeadCtrl::GetItemText(int nItem, int nSubItem)
{
	if(m_Info.GetSize()<=nItem)
		return "";
	if(m_Info[nItem].GetSize()<=nSubItem)
		return "";

	return m_Info[nItem][nSubItem].string;
}

BOOL CHeadCtrl::GetRect(int nItem, int nSubItem, CRect& rect)
{
	CRect rectTemp;
	int down=0;
	int right=0;
	int nHeight=0;
	int nWidth=0;
	UINT nFlag=GetItemState(nItem, nSubItem, down, right);

	if(nFlag==UNUSE)
		return 0;

	GetItemRect(nSubItem, &rect);
	nHeight = rect.Height()/m_count;
	rect.top   = nHeight * nItem ;
	rect.bottom= rect.top + ( nHeight * (down+1));

	for( int i=nSubItem+1; i<= nSubItem + right; i++)
	{
		GetItemRect(i, &rectTemp);
		nWidth += rectTemp.Width();
	}
	rect.right = rect.right + nWidth;

	return 1;
}

BOOL CHeadCtrl::SetItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem)
{
	if(m_Info.GetSize()<=neItem)	return 0;
	if(m_Info[neItem].GetSize()<=neSubItem)return 0;

	for( int i=nsItem; i<=neItem; i++)
	{
		for( int j=nsSubItem; j<=neSubItem; j++)
		{
			if(i==nsItem && j==nsSubItem)
				m_Info[i][j].state= SetFlag(MERGE) | 
									SetDown(neItem-nsItem) | 
									SetRight(neSubItem-nsSubItem);
			else
				m_Info[i][j].state=SetFlag(UNUSE);
		}
	}
	return 1;
}

UINT CHeadCtrl::GetItemState(int nItem, int nSubItem, int& down, int& right) 
{
	if(m_Info.GetSize()<=nItem)
		return 0;
	if(m_Info[nItem].GetSize()<=nSubItem)
		return 0;
	DWORD dw = m_Info[nItem][nSubItem].state;
	down =GetDown(dw);
	right=GetRight(dw);

	return GetFlag(dw);
}

void CHeadCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CFont* pOldFont = NULL;
	dc.SetBkMode(TRANSPARENT   );

	if(!m_font.GetSafeHandle())
	{
		pOldFont = dc.SelectObject(&m_font);
	}
	dc.SetBkColor(RGB(255,0,0));
	for( int i=0; i<m_count; i++)
	{
		for( int j=0; j<GetItemCount(); j++)
		{
			CRect rect;
			if(!GetRect(i, j, rect ))
				continue;
			if(!IsDraw(rect))
				continue;
			CString strText = GetItemText(i, j);

			if( rect.left<m_pnt.x && rect.right>m_pnt.x &&
				rect.top<m_pnt.y && rect.bottom>m_pnt.y && m_bclick)
			{
				dc.DrawEdge( &rect, EDGE_SUNKEN, BF_RECT | BF_SOFT);
			}
			else
			{
				dc.DrawEdge( &rect, EDGE_RAISED, BF_RECT | BF_SOFT);
			}

			BOOL bMulti = GetTextRect(&dc, strText, rect);
			if(bMulti)
			{
				dc.DrawText( strText, -1, rect, 
					DT_CENTER | DT_VCENTER );
			}
			else
			{
				dc.DrawText( strText, -1, rect, 
					DT_CENTER | DT_SINGLELINE | DT_VCENTER );
			}
		}
	}
	if(pOldFont)
		dc.SelectObject(pOldFont);
}

BOOL CHeadCtrl::GetTextRect(CDC* pDC, CString strText, CRect& rect)
{
	int nCount=1, nStart=0;
	TEXTMETRIC metrics;
	while(1)
	{
		nStart = strText.Find('\n', nStart)+1;
		if(nStart < 1)	break;
		nCount++;
	}

	if(nCount==1)	
		return 0;

	pDC->GetOutputTextMetrics(&metrics) ;
	rect.top += ((rect.Height() - metrics.tmHeight * nCount)/2);
	return 1;
}

BOOL CHeadCtrl::IsDraw(CRect rect)
{
	CRect rectCC;
	CCellCtrl* pCC=(CCellCtrl*)GetParent();
	pCC->GetClientRect(&rectCC);

	int nPos = pCC->GetScrollPos(SB_HORZ);

	if(nPos>rect.right)
		return 0;
	if((nPos+rectCC.Width())<rect.left)
		return 0;
	return 1;
}

void CHeadCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	Invalidate();
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
}

void CHeadCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bclick)
	{
		m_bclick=TRUE;
		m_pnt=point;
	}
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CHeadCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bclick)
	{
		m_bclick=FALSE;
		Invalidate();
	}
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}
