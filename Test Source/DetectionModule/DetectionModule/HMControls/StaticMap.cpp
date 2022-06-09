// StaticMap.cpp : implementation file
//

#include "stdafx.h"
// #include "	\ add additional includes here"
#include "StaticMap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_BLINK		9001

/////////////////////////////////////////////////////////////////////////////
// CStaticMap

CStaticMap::CStaticMap()
{
	m_pParentWnd			= NULL;
	m_nMapWidth				= 0;
	m_nMapHeight			= 0;

	m_OriginDir				= OD_LeftBottom;
	m_nUnitSize				= 0;

	m_bShowShot				= FALSE;

	m_hDrawingSurface		= NULL;
	m_RectDrawingSurface	= CRect(0, 0, 0, 0);
	memset(&m_BMIH, 0, sizeof(BITMAPINFOHEADER));
	m_pDrawingSurfaceBits	= NULL;
	
	m_dRatio				= 1.0;
	m_RectWnd.SetRect(0, 0, 0, 0);
	m_RectMap.SetRect(0, 0, 0, 0);
	m_PointWindowStart.x	= 0;
	m_PointWindowStart.y	= 0;
	m_bDragMove				= FALSE;
	m_bIsMiniMap			= FALSE;
	m_PointGlassStart.x		= 0;
	m_PointGlassStart.y		= 0;
	m_PointGlassEnd.x		= 0;
	m_PointGlassEnd.y		= 0;
	m_nCoordGabX			= 2;
	m_nCoordGabY			= 2;

	m_nCCutSize				= 20;
	m_clBGColor				= RGB(0,0,0);
	m_clPatColor			= RGB(20,220,20);
}

CStaticMap::~CStaticMap()
{
	DeinitGlass();
}

BEGIN_MESSAGE_MAP(CStaticMap, CStatic)
	//{{AFX_MSG_MAP(CStaticMap)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//ON_CONTROL_RANGE(BN_CLICKED, _MENU_DEFECT_LIST_ID_, _MENU_DEFECT_LIST_ID_+3, OnMenuChangeViewMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticMap message handlers
void CStaticMap::InitGlass(CWnd* pParent, int nMapWidth, int nMapHeight, int nMapMargin)
{
	if (!pParent || nMapWidth <= 0 || nMapHeight <= 0)
		return;

	m_pParentWnd = pParent;
	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;

	GetClientRect(m_RectWnd);

	m_nUnitSize = m_nMapWidth / m_RectWnd.Width();

	float fMapWperH = static_cast<float>(m_nMapWidth) / static_cast<float>(m_nMapHeight);
	float fWndWperH = static_cast<float>(m_RectWnd.Width()) / static_cast<float>(m_RectWnd.Height());
	if (fMapWperH > fWndWperH)
	{
		m_nMapTotalWidth = m_nMapWidth + nMapMargin * 2;
		m_nMapTotalHeight = static_cast<int>(static_cast<float>(m_nMapTotalWidth) * fWndWperH);

		m_nMapMarginWidth = nMapMargin;
		m_nMapMarginHeight = (m_nMapTotalHeight - m_nMapHeight) / 2;
	}
	else
	{
		m_nMapTotalHeight = m_nMapHeight + nMapMargin * 2;
		m_nMapTotalWidth = static_cast<int>(static_cast<float>(m_nMapTotalHeight) * fWndWperH);
		
		m_nMapMarginHeight = nMapMargin;
		m_nMapMarginWidth = (m_nMapTotalWidth - m_nMapWidth) / 2;
	}
	m_RectMap.SetRect(m_nMapMarginWidth, m_nMapMarginHeight, m_nMapMarginWidth + m_nMapWidth, m_nMapMarginHeight + m_nMapHeight);

	// Review Map 저장을 위한 준비
	GetWindowRect(&m_RectDrawingSurface);
	ScreenToClient(&m_RectDrawingSurface);
	CDC* pDC = GetDC();
	if(pDC != NULL)
	{
		m_BMIH.biSize = sizeof(BITMAPINFOHEADER);
		m_BMIH.biBitCount = 24;
		m_BMIH.biPlanes = 1;
		m_BMIH.biCompression = BI_RGB;
		m_BMIH.biWidth = m_RectDrawingSurface.Width();
		m_BMIH.biHeight = m_RectDrawingSurface.Height();
		m_BMIH.biSizeImage = ((((m_BMIH.biWidth * m_BMIH.biBitCount) + 31) & ~31) >> 3) * m_BMIH.biHeight;
		DeinitGlass();
		m_hDrawingSurface = CreateDIBSection(pDC->GetSafeHdc(), (CONST BITMAPINFO*)&m_BMIH, DIB_RGB_COLORS
										, (void**)&m_pDrawingSurfaceBits, NULL, 0);
		ReleaseDC(pDC);
	}
}

void CStaticMap::DeinitGlass()
{
	if (m_hDrawingSurface != NULL)
	{
		DeleteObject(m_hDrawingSurface);
		m_hDrawingSurface = NULL;
	}
}

void CStaticMap::SetMappingMode(CDC *pDC)
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	if (m_OriginDir == OD_LeftTop)
	{
		pDC->SetWindowExt(m_nMapTotalWidth, m_nMapTotalHeight);
		pDC->SetViewportOrg(m_PointWindowStart.x, m_PointWindowStart.y);
	}
	else if (m_OriginDir == OD_LeftBottom)
	{
		pDC->SetWindowExt(m_nMapTotalWidth, -1 * m_nMapTotalHeight);
		pDC->SetViewportOrg(m_PointWindowStart.x, m_RectWnd.bottom - m_PointWindowStart.y);
	}
	else if (m_OriginDir == OD_RightTop)
	{
		pDC->SetWindowExt(-1 * m_nMapTotalWidth, m_nMapTotalHeight);
		pDC->SetViewportOrg(m_RectWnd.right - m_PointWindowStart.x, m_PointWindowStart.y);
	}
	else if (m_OriginDir == OD_RightBottom)
	{
		pDC->SetWindowExt(-1 * m_nMapTotalWidth, -1 * m_nMapTotalHeight);
		pDC->SetViewportOrg(m_RectWnd.right - m_PointWindowStart.x, m_RectWnd.bottom - m_PointWindowStart.y);
	}

	pDC->SetViewportExt(static_cast<int>(m_RectWnd.right * m_dRatio), static_cast<int>(m_RectWnd.bottom * m_dRatio));
}

void CStaticMap::IncreaseRatio()
{
	if (!m_bIsMiniMap)
	{
		m_dRatio = m_dRatio + 0.5;
		if (m_dRatio > 64.0)
			m_dRatio = 64.0;

		// 가운데를 기준으로 옮긴다.
		m_PointWindowStart.x = static_cast<int>((m_RectWnd.right * m_dRatio - m_RectWnd.Width()) / 2) * -1;
		m_PointWindowStart.y = static_cast<int>((m_RectWnd.bottom * m_dRatio - m_RectWnd.Height()) / 2) * -1;

		// 영역이 벗어나지 않도록 예외처리.
		if (m_PointWindowStart.x > 0)
			m_PointWindowStart.x = 0;
		else if (m_PointWindowStart.x < m_RectWnd.right * m_dRatio * -1 + m_RectWnd.Width())
			m_PointWindowStart.x = static_cast<int>(m_RectWnd.right * m_dRatio) * -1 + m_RectWnd.Width();
		if (m_PointWindowStart.y > 0)
			m_PointWindowStart.y = 0;
		else if (m_PointWindowStart.y < m_RectWnd.bottom * m_dRatio * -1 + m_RectWnd.Height())
			m_PointWindowStart.y = static_cast<int>(m_RectWnd.bottom * m_dRatio) * -1 + m_RectWnd.Height();

		m_PointGlassStart = SetWindowToGlass(CPoint(0, 0));
		m_PointGlassEnd = SetWindowToGlass(CPoint(m_RectWnd.right, m_RectWnd.bottom));

		if (m_pParentWnd)
			m_pParentWnd->PostMessage(UM_MAP_CHANGED, 0, 0);

		Invalidate();
	}
}

void CStaticMap::DecreaseRatio()
{
	if (!m_bIsMiniMap)
	{
		m_dRatio = m_dRatio - 0.5;
		if (m_dRatio < 1)
			m_dRatio = 1.0;

		// 가운데를 기준으로 옮긴다.
		m_PointWindowStart.x = static_cast<int>((m_RectWnd.right * m_dRatio - m_RectWnd.Width()) / 2) * -1;
		m_PointWindowStart.y = static_cast<int>((m_RectWnd.bottom * m_dRatio - m_RectWnd.Height()) / 2) * -1;

		// 영역이 벗어나지 않도록 예외처리.
		if (m_PointWindowStart.x > 0)
			m_PointWindowStart.x = 0;
		else if (m_PointWindowStart.x < m_RectWnd.right * m_dRatio * -1 + m_RectWnd.Width())
			m_PointWindowStart.x = static_cast<int>(m_RectWnd.right * m_dRatio) * -1 + m_RectWnd.Width();
		if (m_PointWindowStart.y > 0)
			m_PointWindowStart.y = 0;
		else if (m_PointWindowStart.y < m_RectWnd.bottom * m_dRatio * -1 + m_RectWnd.Height())
			m_PointWindowStart.y = static_cast<int>(m_RectWnd.bottom * m_dRatio) * -1 + m_RectWnd.Height();

		m_PointGlassStart = SetWindowToGlass(CPoint(0, 0));
		m_PointGlassEnd = SetWindowToGlass(CPoint(m_RectWnd.right, m_RectWnd.bottom));

		if (m_pParentWnd)
			m_pParentWnd->PostMessage(UM_MAP_CHANGED, 0, 0);

		Invalidate();
	}
}

void CStaticMap::ResetRatio()
{
	if (!m_bIsMiniMap)
	{
		m_dRatio = 1.0;

		m_PointWindowStart.x = 0;
		m_PointWindowStart.y = 0;

		m_PointGlassStart = SetWindowToGlass(CPoint(0, 0));
		m_PointGlassEnd = SetWindowToGlass(CPoint(m_RectWnd.right, m_RectWnd.bottom));

		if (m_pParentWnd)
			m_pParentWnd->PostMessage(UM_MAP_CHANGED, 0, 0);

		Invalidate();
	}
}

void CStaticMap::SetDisplayRect(CPoint& PointStart, CPoint& PointEnd)
{
	m_PointGlassStart = PointStart;
	m_PointGlassEnd = PointEnd;
}

void CStaticMap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	/*
	// Glass 그리기.
	CPen PenGlass, *pOldPen;
	CBrush BrushGlass, *pOldBrush;
	PenGlass.CreatePen(PS_SOLID, 0, m_clBGColor);
	BrushGlass.CreateSolidBrush(m_clBGColor);
	pOldBrush = (CBrush*)memdc.SelectObject(&BrushGlass);
	pOldPen = (CPen*)memdc.SelectObject(&PenGlass);

	dc.Rectangle(m_RectWnd);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
*/
	DrawGlass(&dc);

	// 보여주기 영역 사각형 그리기.
	if (m_bIsMiniMap)
	{
		CBrush BrushDisplay;
  		CPen PenShow(PS_SOLID, 1, RGB(255,0,0));
 		CPen* pOldPen = (CPen*)dc.SelectObject(&PenShow);
		BrushDisplay.CreateStockObject(NULL_BRUSH);
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&BrushDisplay);
		dc.Rectangle(CRect(m_PointGlassStart.x + m_RectMap.left, m_PointGlassStart.y + m_RectMap.top
						, m_PointGlassEnd.x + m_RectMap.left, m_PointGlassEnd.y + m_RectMap.top));
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}

	// Do not call CStatic::OnPaint() for painting messages
}

void CStaticMap::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	MouseLBtnDblClk(point);

	CStatic::OnLButtonDblClk(nFlags, point);
}

void CStaticMap::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bDragMove = TRUE;
	m_PointDragStart = point;
	SetFocus();
	MouseLBtnDwn(point);

	CStatic::OnLButtonDown(nFlags, point);
}

void CStaticMap::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bDragMove = FALSE;
	MouseLBtnUp(point);

	CStatic::OnLButtonUp(nFlags, point);
}

void CStaticMap::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	MouseRBtnDblClk(point);

	CStatic::OnRButtonDblClk(nFlags, point);
}

void CStaticMap::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	MouseRBtnDwn(point);

	CStatic::OnRButtonDown(nFlags, point);
}

void CStaticMap::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	MouseRBtnUp(point);

	CStatic::OnRButtonUp(nFlags, point);
}

void CStaticMap::PopupMenu(int nBaseID, CPoint point)
{
	// TODO: Add your message handler code here
	CString strText;
	CMenu menu;
	VERIFY(menu.CreatePopupMenu());

	menu.AppendMenu(MF_STRING, nBaseID, (LPCTSTR)"Register Nodefect(&R)");	

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

void CStaticMap::OnMenuChangeViewMode(UINT nID)
{
	//nID -= _MENU_DEFECT_LIST_ID_;
// 	switch(nID)
// 	{
// 	case 0:RegistNoDefect(); break;
// 	}
}

void CStaticMap::OnMouseMove(UINT nFlags, CPoint point) 
{	
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_LBUTTON && m_bDragMove && m_dRatio > 1.0)
	{
		if (m_OriginDir == OD_LeftTop)
		{
			m_PointWindowStart.x -= (m_PointDragStart.x - point.x);
			m_PointWindowStart.y -= (m_PointDragStart.y - point.y);
		}
		else if (m_OriginDir == OD_LeftBottom)
		{
			m_PointWindowStart.x -= (m_PointDragStart.x - point.x);
			m_PointWindowStart.y += (m_PointDragStart.y - point.y);
		}
		else if (m_OriginDir == OD_RightTop)
		{
			m_PointWindowStart.x += (m_PointDragStart.x - point.x);
			m_PointWindowStart.y -= (m_PointDragStart.y - point.y);
		}
		else if (m_OriginDir == OD_RightBottom)
		{
			m_PointWindowStart.x += (m_PointDragStart.x - point.x);
			m_PointWindowStart.y += (m_PointDragStart.y - point.y);
		}
		//TRACE("%d %d\n", m_PointWindowStart.x, m_PointWindowStart.y);
		// 영역이 벗어나지 않도록 예외처리.
		if (m_PointWindowStart.x > 0)
			m_PointWindowStart.x = 0;
		else if (m_PointWindowStart.x < m_RectWnd.right * m_dRatio * -1 + m_RectWnd.Width())
			m_PointWindowStart.x = static_cast<int>(m_RectWnd.right * m_dRatio) * -1 + m_RectWnd.Width();
		if (m_PointWindowStart.y > 0)
			m_PointWindowStart.y = 0;
		else if (m_PointWindowStart.y < m_RectWnd.bottom * m_dRatio * -1 + m_RectWnd.Height())
			m_PointWindowStart.y = static_cast<int>(m_RectWnd.bottom * m_dRatio) * -1 + m_RectWnd.Height();
		m_PointDragStart = point;

		// 현재 보여지는 영역 알아내기.
		m_PointGlassStart = SetWindowToGlass(CPoint(0, 0));
		m_PointGlassEnd = SetWindowToGlass(CPoint(m_RectWnd.right, m_RectWnd.bottom));

		if (m_pParentWnd)
			m_pParentWnd->PostMessage(UM_MAP_CHANGED, 0, 0);

		Invalidate();
	}
	MouseLBtnMove(point);

	CStatic::OnMouseMove(nFlags, point);
}

void CStaticMap::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Timer(static_cast<UINT>(nIDEvent));

	CStatic::OnTimer(nIDEvent);
}

CPoint CStaticMap::SetWindowToGlass(CPoint pt)
{
	if (!GetSafeHwnd())
		return pt;
	
	//넘겨받은 물리 좌표를 논리좌표로 변환하여 현재 화면에 나타나는 좌표로 변환하여 반환한다.
	CPaintDC dc(this);

	SetMappingMode(&dc);

	dc.DPtoLP(&pt);

	return pt;
}

CPoint CStaticMap::SetGlassToWindow(CPoint pt)
{
	if (!GetSafeHwnd())
		return pt;
	
	CPaintDC dc(this);

	SetMappingMode(&dc);

	dc.LPtoDP(&pt);

	return pt;
}

void CStaticMap::SaveMapToFile(CString strFileName) 
{
	//Create a new file for writing
	FILE *pFile;
	fopen_s(&pFile, strFileName, "wb");
	if(pFile == NULL)
		return;

	BITMAPFILEHEADER bmfh;	
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + m_BMIH.biSize; 
	LONG lImageSize = m_BMIH.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	bmfh.bfType = 'B'+('M'<<8);			
	bmfh.bfOffBits = nBitsOffset;		
	bmfh.bfSize = lFileSize;				
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	//Write the bitmap file header
	UINT nWrittenFileHeaderSize = static_cast<UINT>(fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile));
	//And then the bitmap info header
	UINT nWrittenInfoHeaderSize = static_cast<UINT>(fwrite(&m_BMIH, 1, sizeof(BITMAPINFOHEADER), pFile));
	//Finally, write the image data itself -- the data represents our drawing
	UINT nWrittenDIBDataSize = static_cast<UINT>(fwrite(m_pDrawingSurfaceBits, 1, lImageSize, pFile));
	
	fclose(pFile);
}

BOOL CStaticMap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta < 0)
	{
		ControlZoom(FALSE, pt);
	}
	else if(zDelta > 0)
	{
		ControlZoom(TRUE, pt);
	}	
	
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CStaticMap::ControlZoom(BOOL bZoomIn, CPoint ptMouse)
{
	CPoint ptCurPos(ptMouse);
	ScreenToClient(&ptCurPos);
	CPoint ptTemp(ptCurPos);

	if (m_OriginDir == OD_LeftTop)
	{
		;
	}
	else if (m_OriginDir == OD_LeftBottom)
	{
		ptCurPos.y = m_RectWnd.Height() - ptCurPos.y;
		ptTemp.y = m_RectWnd.Height() - ptTemp.y;
	}
	else if (m_OriginDir == OD_RightTop)
	{
		ptCurPos.x = m_RectWnd.Width() - ptCurPos.x;
		ptTemp.x = m_RectWnd.Width() - ptTemp.x;
	}
	else if (m_OriginDir == OD_RightBottom)
	{
		ptCurPos.x = m_RectWnd.Width() - ptCurPos.x;
		ptCurPos.y = m_RectWnd.Height() - ptCurPos.y;
		ptTemp.x = m_RectWnd.Width() - ptTemp.x;
		ptTemp.y = m_RectWnd.Height() - ptTemp.y;
	}

	ptCurPos -= m_PointWindowStart;

	ptCurPos.x = (LONG)(ptCurPos.x / m_dRatio);
	ptCurPos.y = (LONG)(ptCurPos.y / m_dRatio);

	double dOldRatio = m_dRatio;

	if(bZoomIn == FALSE)
	{
		m_dRatio = m_dRatio - 0.5;
		if (m_dRatio < 1)
			m_dRatio = 1.0;
	}
	else
	{
		m_dRatio = m_dRatio + 0.5;
		if (m_dRatio > 64.0)
			m_dRatio = 64.0;
	}

	ptCurPos.x = (LONG)(ptCurPos.x * m_dRatio);
	ptCurPos.y = (LONG)(ptCurPos.y * m_dRatio);

	//m_PointWindowStart.x = static_cast<int>(((m_RectWnd.right * m_dRatio - m_RectWnd.Width()) / 2 + (ptCurPos.x - m_RectWnd.Width() * m_dRatio / 2))) * -1;
	//m_PointWindowStart.y = static_cast<int>(((m_RectWnd.bottom * m_dRatio - m_RectWnd.Height()) / 2 + (ptCurPos.y - m_RectWnd.Height() * m_dRatio / 2))) * -1;

	m_PointWindowStart.x = (ptCurPos.x - ptTemp.x) * -1;
	m_PointWindowStart.y = (ptCurPos.y - ptTemp.y) * -1;

	// 영역이 벗어나지 않도록 예외처리.
	if (m_PointWindowStart.x > 0)
		m_PointWindowStart.x = 0;
	else if (m_PointWindowStart.x < m_RectWnd.right * m_dRatio * -1 + m_RectWnd.Width())
		m_PointWindowStart.x = static_cast<int>(m_RectWnd.right * m_dRatio) * -1 + m_RectWnd.Width();
	if (m_PointWindowStart.y > 0)
		m_PointWindowStart.y = 0;
	else if (m_PointWindowStart.y < m_RectWnd.bottom * m_dRatio * -1 + m_RectWnd.Height())
		m_PointWindowStart.y = static_cast<int>(m_RectWnd.bottom * m_dRatio) * -1 + m_RectWnd.Height();

	// 현재 보여지는 영역 알아내기.
	m_PointGlassStart = SetWindowToGlass(CPoint(0, 0));
	m_PointGlassEnd = SetWindowToGlass(CPoint(m_RectWnd.right, m_RectWnd.bottom));

	Invalidate(FALSE);
}