// LogCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LogCtrl.h"
CPrintDC::CPrintDC()
{
	m_pDC = new CDC;
}

CPrintDC::~CPrintDC()
{
	//hDC를 삭제하지 않아도 CDC클래스의 소멸자에서 자동 소멸된다.
	//HDC hDC = m_pDC->Detach();
	//::DeleteDC(hDC);
	if (m_pDC)
	{
		delete m_pDC;
		m_pDC = NULL;
	}
	
}

CDC* CPrintDC::GetPrintDC(int Orientation, BOOL bDialog)
{
	HDC hDC=m_pDC->Detach();
	DEVMODE* pDM;
	if(hDC) ::DeleteDC(hDC);

	CPrintDialog pntDlg(FALSE);

	if(bDialog)
	{
		if(pntDlg.DoModal()!=IDOK)
			return 0;
		hDC=pntDlg.GetPrinterDC();
	}
	else
	{
		pntDlg.GetDefaults();
		hDC=pntDlg.GetPrinterDC();
	}

	if(Orientation!=0)
	{
		//pDM = return (LPDEVMODE)::GlobalLock(m_pd.hDevMode);
		pDM = pntDlg.GetDevMode();
		pDM->dmOrientation = Orientation;
		::ResetDC(hDC, pDM);
		::GlobalUnlock(pntDlg.m_pd.hDevMode);
	}
	m_pDC->Attach(hDC);
	return m_pDC;
}

CLogCell::CLogCell()
{
	InitCell();	
}

CLogCell::CLogCell(LPCSTR lpstr, CRect r, int nCellFormat,
					COLORREF textColor,COLORREF backColor, CFont* pf)
{
	m_nLine=LCS_RECT;
	m_nState=0;
	m_txtclr=textColor;
	m_bkgclr=backColor;
	m_nFormat=nCellFormat;

	m_strTxt = lpstr;
	m_rect=r;
	m_pfont=pf;
	m_pBitmap=0;
}

void CLogCell::operator=(const CLogCell& log)
{
	m_nLine=log.m_nLine;
	m_nState=log.m_nState;
	m_txtclr=log.m_txtclr;
	m_bkgclr=log.m_bkgclr;
	m_nFormat=log.m_nFormat;

	m_strTxt=log.m_strTxt;
	m_rect=log.m_rect;
	m_pfont=log.m_pfont;
	m_pBitmap=log.m_pBitmap;
}

void CLogCell::InitCell()
{
	m_nLine=LCS_RECT;
	m_nState=0;
	m_txtclr=0X00000000;
	m_bkgclr=0X00FFFFFF;
	m_nFormat=0;
	m_strTxt="";
	m_pfont=0;
	m_pBitmap=0;
	memset(&m_rect, NULL, sizeof(m_rect));
}

void CLogCell::Draw(CDC* pDC)
{
//	ASSERT(pDC);
	if(GetFlag(m_nState)==LCS_UNUSE)
		return;
	CRect rect=m_rect;

	CFont* pOldFont=0;
	int nMode=pDC->SetBkMode(TRANSPARENT);
	COLORREF color=pDC->SetTextColor(m_txtclr);
	if(m_pfont) 
		pOldFont=pDC->SelectObject(m_pfont);

	pDC->FillRect(rect, &CBrush(m_bkgclr));
	if(	m_nLine & LCS_TLINE)
	{
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right,rect.top);
	}
	if(	m_nLine & LCS_BLINE)
	{
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(rect.right,rect.bottom);
	}
	if(	m_nLine & LCS_LLINE)
	{
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom);
	}
	if(	m_nLine & LCS_RLINE)
	{
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.right, rect.bottom);
	}
	//1mm이동
	rect.left+=(pDC->GetDeviceCaps(HORZSIZE)/10);
	rect.right-=(pDC->GetDeviceCaps(HORZSIZE)/10);

	if(m_pBitmap)
		pDC->DrawState( rect.TopLeft(), rect.Size(), m_pBitmap, DSS_NORMAL, &CBrush(m_bkgclr));

	pDC->DrawText(m_strTxt, -1, rect, m_nFormat);

	if(pOldFont)
		pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
	pDC->SetBkMode(nMode);
}

CLogPage::CLogPage()
{
	m_startPnt=CPoint(0,0);
	RemoveAll();
}

CLogPage::CLogPage(CPoint startPnt)
{
	m_startPnt=startPnt;
	RemoveAll();
}

CLogPage::~CLogPage()
{
	RemoveAll();
}

void CLogPage::RemoveAll()
{
	m_ColCnt=0;
	m_RowCnt=0;
	m_liAry.RemoveAll();
	m_startPnt=CPoint(0,0);
}

BOOL CLogPage::SetPageRect(CDC* pDC, CRect page, CRect device)
{
	ASSERT(pDC);

	pDC->SetMapMode(MM_ISOTROPIC);

	//페이지 크기 설정
	pDC->SetWindowOrg(page.TopLeft());
	pDC->SetWindowExt(page.right,page.bottom);

	//시작위치,마지막 위치
	pDC->SetViewportOrg( device.TopLeft() );
	pDC->SetViewportExt( device.right, device.bottom);
	for( int i=0; i<m_RowCnt; i++)
	{
		for( int j=0; j<m_ColCnt; j++)
			pDC->LPtoDP(&m_liAry[i][j].m_rect);
	}
	m_IsViewPort=true;
	pDC->SetMapMode(MM_TEXT);
	return 1;
}

BOOL CLogPage::SetColWidth(int nCol, int nWidth)
{
	LOGCHECK0(nCol>=m_ColCnt);
	for( int i=0; i<m_RowCnt; i++)
	{
		if(nCol==0)
		{
			m_liAry[i][0].m_rect.left =m_startPnt.x;
			m_liAry[i][0].m_rect.right=m_startPnt.x + nWidth;
		}
		else
		{
			m_liAry[i][nCol].m_rect.left =
				m_liAry[i][nCol-1].m_rect.right;
			m_liAry[i][nCol].m_rect.right=
				m_liAry[i][nCol-1].m_rect.right + nWidth;
		}
	}
	return 1;
}

BOOL CLogPage::SetRowHeight(int nRow, int nHeight)
{
	LOGCHECK0(nRow>=m_RowCnt);
	for( int i=0; i<m_ColCnt; i++)
	{
		if(nRow==0)
		{
			m_liAry[0][i].m_rect.top =m_startPnt.y;
			m_liAry[0][i].m_rect.bottom=m_startPnt.y + nHeight;
		}
		else
		{
			m_liAry[nRow][i].m_rect.top =
				m_liAry[nRow-1][i].m_rect.bottom;

			m_liAry[nRow][i].m_rect.bottom =
				m_liAry[nRow-1][i].m_rect.bottom+nHeight;
		}
	}
	return 1;
}

void CLogPage::SetColCount(int nCount)
{
	m_ColCnt=nCount;
	int nSize=static_cast<int>(m_liAry.GetSize());
	for( int i=0; i<nSize; i++)
		m_liAry[i].SetSize(nCount);
}

void CLogPage::SetRowCount(int nCount)
{
	m_RowCnt=nCount;
	m_liAry.SetSize(nCount);
	for( int i=0; i<nCount; i++)
		m_liAry[i].SetSize(m_ColCnt);
}

BOOL CLogPage::RemoveRow(int nRow)
{
	LOGCHECK0(nRow>=m_RowCnt);
	m_liAry.RemoveAt(nRow);
	return 1;
}

//장표 출력위치
void CLogPage::SetStartPoint(CPoint startPnt)
{
	m_startPnt=startPnt;
}

CPoint CLogPage::GetStartPoint() const 
{ 
	return m_startPnt;
}

//행,열 갯수를 리턴한다.
int CLogPage::GetColCount() const
{
	return m_ColCnt;
}
int CLogPage::GetRowCount() const 
{
	return m_RowCnt;
}

//폰트를 설정한다.
BOOL CLogPage::SetCellFont(int nRow, int nCol, CFont* pf)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_pfont = pf;
	return 1;
}

BOOL CLogPage::SetCellFont(int nsRow, int nsCol, int neRow, int neCol, CFont* pf)
{
	LOGCHECK0(neRow>=m_RowCnt || neCol>=m_ColCnt);
	for( int i=nsRow; i<=neRow; i++)
	{
		for( int j=nsCol; j<=neCol; j++)
			m_liAry[i][j].m_pfont = pf;
	}
	return 1;
}

CFont* CLogPage::GetCellFont(int nRow, int nCol)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_pfont;
}

//배경색을 설정한다.
BOOL CLogPage::SetCellBkgColor(int nRow, int nCol, COLORREF color)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_bkgclr = color;
	return 1;
}
COLORREF CLogPage::GetCellBkgColor(int nRow, int nCol)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_bkgclr;
}

//글자색을 설정한다.
BOOL CLogPage::SetCellTxtColor(int nRow, int nCol, COLORREF color)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_txtclr=color;
	return 1;
}
COLORREF CLogPage::GetCellTxtColor(int nRow, int nCol)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_txtclr;
}

//글자위치 설정
BOOL CLogPage::SetCellFormat(int nRow, int nCol, int nFormat)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_nFormat = nFormat;
	return 1;
}

int CLogPage::GetCellFormat(int nRow, int nCol)
{
	LOGCHECK1(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_nFormat;
}

//Rect을 변경한다.
BOOL CLogPage::SetCellRect(int nRow, int nCol, CRect rect)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_rect=rect;
	return 1;
}

BOOL CLogPage::GetCellRect(int nRow, int nCol, CRect& rect)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	rect = m_liAry[nRow][nCol].m_rect;
	return 1;
}

//BITMAP을 설정한다.
BOOL CLogPage::SetCellBmp(int nRow, int nCol, CBitmap* pBmp)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_pBitmap=pBmp;
	return 1;
}

CBitmap* CLogPage::GetCellBmp(int nRow, int nCol)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_pBitmap;
}

//사각형 그릴때 어느 위치를 그릴 것인가 설정한다. 
BOOL CLogPage::SetCellLine(int nRow, int nCol, int nState)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_nLine=nState;
	return 1;
}

int CLogPage::GetCellLine(int nRow, int nCol)
{
	LOGCHECK1(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_nLine;
}

void CLogPage::operator=(CLogPage& page)
{
	m_RowCnt=page.m_RowCnt;
	m_ColCnt=page.m_ColCnt;
	m_startPnt=page.m_startPnt;
	m_liAry.SetSize(m_RowCnt);
	for( int i=0; i<m_RowCnt; i++)
	{
		m_liAry[i].SetSize(m_ColCnt);
		for( int j=0; j<m_ColCnt; j++)
			m_liAry[i][j]=page.m_liAry[i][j];
	}
}

BOOL CLogPage::SetItemText(int nRow, int nCol, LPCTSTR lpstr, int nFormat)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol].m_strTxt=lpstr;
	m_liAry[nRow][nCol].m_nFormat = nFormat;
	return 1;
}

LPCSTR CLogPage::GetItemText(int nRow, int nCol)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return m_liAry[nRow][nCol].m_strTxt;
}

BOOL CLogPage::SetCell(int nRow, int nCol, CLogCell lc)
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	m_liAry[nRow][nCol]=lc;
	return 1;
}

CLogCell* CLogPage::GetCell(int nRow, int nCol) 
{
	LOGCHECK0(nRow>=m_RowCnt || nCol>=m_ColCnt);
	return &m_liAry[nRow][nCol];
}

void CLogPage::Draw(CDC* pDC, int nPageNO)
{
	LOGCHECK(m_RowCnt==0 || m_ColCnt==0);
	for( int i=0; i<m_RowCnt; i++)
	{
		for( int j=0; j<m_ColCnt; j++)
			m_liAry[i][j].Draw(pDC);
	}

	int nVertres = pDC->GetDeviceCaps(VERTRES);
	int nHorzres = pDC->GetDeviceCaps(HORZRES);
	int nHorzSize= pDC->GetDeviceCaps(HORZSIZE);
	int nVertSize= pDC->GetDeviceCaps(VERTSIZE);

	if(nPageNO)
	{
		CString str="";
		CPoint pntOrg;
		str.Format("%d", nPageNO);
		CRect rect( 0, 0, nHorzSize, nVertSize);

		pntOrg=pDC->SetViewportOrg(nHorzres/2, nVertres-nVertSize);
		pDC->DrawText(str, -1, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		pDC->SetViewportOrg(pntOrg.x, pntOrg.y);
	}
}

BOOL CLogPage::CellMerge(int nsRow, int nsCol, int neRow, int neCol)
{
	LOGCHECK0(neRow>=m_RowCnt || neCol>=m_ColCnt);
	for( int i=nsRow; i<=neRow; i++)
	{
		for( int j=nsCol; j<=neCol; j++)
		{
			if(i==nsRow && j==nsCol)
			{
				m_liAry[i][j].m_nState=	SetFlag(LCS_MERGE) | 
										SetDown(neRow-nsRow) | 
										SetRight(neCol-nsCol);

				CRect rect = m_liAry[i][j].m_rect;
				rect =m_liAry[neRow][neCol].m_rect;

				m_liAry[i][j].m_rect.bottom=m_liAry[neRow][neCol].m_rect.bottom;
				m_liAry[i][j].m_rect.right=m_liAry[neRow][neCol].m_rect.right;
			}
			else
			{
				m_liAry[i][j].m_nState=SetFlag(LCS_UNUSE);
			}
		}
	}
	return 1;
}

CLogCtrl::CLogCtrl()
{
	m_log.RemoveAll(); 
}

CLogCtrl::~CLogCtrl()		
{ 
	m_log.RemoveAll(); 
}

int CLogCtrl::GetLogPageCount()
{ 
	return static_cast<int>(m_log.GetSize()); 
}

void CLogCtrl::AddLogPage(CLogPage& lp)
{ 
	m_log.Add(lp); 
}

CLogPage* CLogCtrl::GetLogPage(int nPage)
{ 
	LOGCHECK0(m_log.GetSize()<=nPage);
	return &m_log[nPage]; 
}

void CLogCtrl::DelLogPage(int nPage)
{
	LOGCHECK(m_log.GetSize()<=nPage);
	m_log.RemoveAt(nPage);
}

void CLogCtrl::Draw(CDC* pDC, DOCINFO* pDocInfo)
{
	ASSERT(pDC);

	int nSize = static_cast<int>(m_log.GetSize());
	LOGCHECK(nSize<1);
	
	pDC->StartDoc(pDocInfo);
	for( int i=0; i<nSize; i++)
	{
		pDC->StartPage();
		m_log[i].Draw(pDC,i+1);
		pDC->EndPage();
	}
	pDC->EndDoc();
}
