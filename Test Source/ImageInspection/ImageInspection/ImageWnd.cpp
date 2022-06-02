#include "StdAfx.h"
#include "ImageWnd.h"

#include "DDMAlgorithm.h"
#include "ImageInspectionDlg.h"

#define CALC_MIN	2

#define ADD(a, b)	(a+b)
#define SUB(a, b)	((a - b < 0) ? 0 : (a-b))

IMPLEMENT_DYNAMIC(CImageViewWnd, CWnd)
//IMPLEMENT_DYNCREATE(CImageViewWnd, CWnd)

CImageViewWnd::CImageViewWnd(void)
{
	m_bBoundingBoxView = FALSE;
	m_ptBoxDraw = CPoint(0, 0);
	m_bBoxDraw = FALSE;
	m_bCalcRect = FALSE;

	m_fZoom = 0.5f;

	m_bCutting = m_bCutStart = m_bCutted = FALSE;

	m_iEdgeType = 0;
	m_iEdgeThreshold = 0;
	m_iEdgeSubCutting = 0;

	m_bProfile = FALSE;
	m_bProfileStart = FALSE;

	m_rtTracker.m_rect = CRect(0, 0, 0, 0);
	m_rtTracker.m_nStyle = CRcTracker::dottedLine | CRcTracker::resizeOutside;
	
	m_bPaintDefect = TRUE;
	m_bCalcRealFOV = FALSE;
	m_pDDMAlgorithm = NULL;

	m_ptPixel = CPoint(0, 0);
	m_ptPanStart = CPoint(0, 0);
	m_bPanImage = TRUE;
	m_bPanStart = FALSE;

	m_ptSaveGrayStart = CPoint(0, 0);
	m_ptSaveGrayEnd = CPoint(0, 0);
	m_rtSaveGrayProfile = CRect(0,0,0,0);
	m_bNormalizeGray = FALSE;
	m_bIsDisplayRoiCoordinate = FALSE;
}

CImageViewWnd::~CImageViewWnd(void)
{
}

BOOL CImageViewWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	BOOL bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	OnUpdate(NULL, 0, NULL);

	GetClientRect(m_rtDrawView);
	int max = max(m_rtDrawView.Width(), m_rtDrawView.Height());	
	max = max + (int)(m_fZoom-fmod((float)max,m_fZoom));
	m_rtDrawView.bottom = max;
	m_rtDrawView.right = max;

	return bRet;
	
	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CImageViewWnd::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(m_DrawImage.IsValidImage())
	{
		m_DrawImage.Destroy();
	}

	if(m_OriginImage.IsValidImage())
	{
		m_OriginImage.Destroy();
	}

	if(m_CuttedImage.IsValidImage())
	{
		m_CuttedImage.Destroy();
	}

	return __super::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CImageViewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_PROFILE, &CImageViewWnd::OnProfile)
	ON_COMMAND(ID_DRAW_DEFECT, &CImageViewWnd::OnDrawDefect)
	ON_UPDATE_COMMAND_UI(ID_PROFILE, &CImageViewWnd::OnUpdateProfile)
	ON_UPDATE_COMMAND_UI(ID_DRAW_DEFECT, &CImageViewWnd::OnUpdateDrawDefect)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PAN_IMAGE, &CImageViewWnd::OnPanImage)
	ON_UPDATE_COMMAND_UI(ID_PAN_IMAGE, &CImageViewWnd::OnUpdatePanImage)
	ON_COMMAND(ID_NORMALIZE_GRAY, &CImageViewWnd::OnNormalizeGray)
	ON_UPDATE_COMMAND_UI(ID_NORMALIZE_GRAY, &CImageViewWnd::OnUpdateNormalizeGray)
END_MESSAGE_MAP()

void CImageViewWnd::ResetImage()
{
	if(m_OriginImage.IsValidImage())
	{
		m_DrawImage = m_OriginImage;
	}
}

void CImageViewWnd::DrawImage(FCObjImage* pDrawImage, float fZoom, CPoint ptScrollPos)
{
	if(m_DrawImage.IsValidImage())
	{
		m_DrawImage.Destroy();
	}

	if(m_OriginImage.IsValidImage())
	{
		m_OriginImage.Destroy();
	}

	if(m_CuttedImage.IsValidImage())
	{
		m_CuttedImage.Destroy();
	}

	m_DrawImage = *pDrawImage;
	m_OriginImage = *pDrawImage;

	m_szImageSize = CSize(m_DrawImage.Width(), m_DrawImage.Height());
	m_fZoom = fZoom;

	ModifyStyle(0, WS_HSCROLL|WS_VSCROLL, SWP_DRAWFRAME);
	
	GetClientRect(m_rtDrawView);
	int max = max(m_rtDrawView.Width(), m_rtDrawView.Height());
	int margin_x = 0, margin_y = 0;
	if(m_rtDrawView.Width() > m_rtDrawView.Height())
		margin_y = m_rtDrawView.Width() - m_rtDrawView.Height();
	if(m_rtDrawView.Width() < m_rtDrawView.Height())
		margin_x = m_rtDrawView.Height() - m_rtDrawView.Width();

	max = max + (int)(m_fZoom-fmod((float)max,m_fZoom));
	m_rtDrawView.right = max;
	m_rtDrawView.bottom = max;

	CSize szScroll;
	szScroll.cx = (int)((m_szImageSize.cx*m_fZoom - m_rtDrawView.Width()) - 1) + margin_x + (int)m_fZoom;
	szScroll.cy = (int)((m_szImageSize.cy*m_fZoom - m_rtDrawView.Height()) - 1) + margin_y + (int)m_fZoom;
	szScroll.cx = szScroll.cx + (int)(m_fZoom-fmod((float)szScroll.cx, m_fZoom));
	szScroll.cy = szScroll.cy + (int)(m_fZoom-fmod((float)szScroll.cy, m_fZoom));
	SetScrollRange(SB_HORZ, 0, szScroll.cx);
	SetScrollRange(SB_VERT, 0, szScroll.cy);
	
	SetScrollPos(SB_HORZ, ptScrollPos.x);
	SetScrollPos(SB_VERT, ptScrollPos.y);
	
	Invalidate(TRUE);

	ModifyIndicator_ImageSize(m_szImageSize, CSize(m_rtDrawView.Width(), m_rtDrawView.Height()));
	ModifyIndicator_Zoom(m_fZoom);
}

double CImageViewWnd::Calc_MeanGray(int startX, int startY, int width, int height)
{
	if (!m_DrawImage.IsValidImage())
		return -1;

	BYTE *pImage = m_DrawImage.GetMemStart();

	if (startX < 0 || startY < 0 || startX + width > m_DrawImage.Width()
		|| startY + height > m_DrawImage.Height()) return -1;

	register int x, y, posy, nCount = 0;
	double fSum = 0;
	startY = m_DrawImage.Height() - startY - 1;
	for (y = startY - height; y < startY; y++)
	{
		posy = y * m_DrawImage.Width();
		for (x = startX; x < startX + width; x++)
		{
			fSum += pImage[posy + x];
			nCount++;
		}
	}

	return (fSum / nCount);
}

double CImageViewWnd::Calc_StdGray(double fMean, int startX, int startY, int width, int height)
{
	if (!m_DrawImage.IsValidImage())
		return -1;

	BYTE *pImage = m_DrawImage.GetMemStart();

	if (startX < 0 || startY < 0 || startX + width > m_DrawImage.Width()
		|| startY + height > m_DrawImage.Height()) return -1;

	register int x, y, posy, nCount = 0;
	double fSum = 0;
	startY = m_DrawImage.Height() - startY - 1;
	for (y = startY - height; y < startY; y++)
	{
		posy = y * m_DrawImage.Width();
		for (x = startX; x < startX + width; x++)
		{
			fSum += (fMean - pImage[posy + x]) * (fMean - pImage[posy + x]);
			nCount++;
		}
	}

	return sqrt(fSum / nCount);
}


BOOL CImageViewWnd::Histogram(int startX, int startY, int width, int height, int *pBuffer, int *nMinGray, int *nMaxGray, int *nMaxCount, int *MaxCountGray, double* dblContrast)
{
	if (!m_DrawImage.IsValidImage())
		return FALSE;

	BYTE *pImage = m_DrawImage.GetMemStart();

	if (startX < 0 || startY < 0 || startX + width > m_DrawImage.Width()
		|| startY + height > m_DrawImage.Height()) 
	{
		return FALSE;
	}
	
	memset(pBuffer, 0, sizeof(int) * 256);

	*nMaxGray = 0;
	*nMinGray = 256;
	*nMaxCount = 0;
	*MaxCountGray = 0;

	register int x, y, posy, nCount = 0;
	double fSum = 0;
	startY = m_DrawImage.Height() - startY - 1;
	unsigned char ucMin, ucMax;

	for (y = startY - height; y < startY; y++)
	{
		posy = y * m_DrawImage.Width();
		for (x = startX; x < startX + width; x++)
		{
			pBuffer[pImage[posy + x]]++;
		
			if (pImage[posy + x] < *nMinGray) *nMinGray = pImage[posy + x];
			if (pImage[posy + x] > *nMaxGray) *nMaxGray = pImage[posy + x];

			if (pBuffer[pImage[posy + x]] > *nMaxCount) 
			{
				*nMaxCount = pBuffer[pImage[posy + x]];
				*MaxCountGray = pImage[posy + x];
			}
		}
	}

	///////////////////////////////////////////////////////////////////////

	if(m_bNormalizeGray == TRUE)
	{
		// 상, 하위 1% 제거
		int iTotalPixelCount = 0, iMinPixelCount = 0, iMaxPixelCount = 0;

		iTotalPixelCount = width*height;
		iMaxPixelCount = iMinPixelCount = iTotalPixelCount * 0.01;

		int iTempCount = 0;
		BOOL bMinCalc = TRUE;
		BOOL bMaxCalc = TRUE;
		int iMin = 0, iMax = 0;

		for (int i = 0 ; i < 256 ; i++)
		{
			iMinPixelCount = iMinPixelCount - pBuffer[i];
			iMaxPixelCount = iMaxPixelCount - pBuffer[255 - i];

			if (bMinCalc)
			{
				if (iMinPixelCount > 0)
				{
					if (pBuffer[i] - iMinPixelCount < 0)
						pBuffer[i] = 0;
				}
				if (pBuffer[i] != 0 )
				{
					iMin = (double)i + 1;
					bMinCalc = FALSE;
				}
			}

			if (bMaxCalc)
			{
				if (iMaxPixelCount > 0)
				{
					if (pBuffer[255-i] - iMaxPixelCount < 0)
						pBuffer[255-i] = 0 ;
				}
				if (pBuffer[255-i] != 0)
				{
					iMax = (double)(255-i) - 1;
					bMaxCalc = FALSE;
				}
			}
		}

		*nMaxGray = iMax;
		*nMinGray = iMin;
	}
	///////////////////////////////////////////////////////////////////////

	if(*nMaxGray + *nMinGray == 0) 
		*dblContrast = 0;
	else
		*dblContrast = (double)(*nMaxGray - *nMinGray) / (double)(*nMaxGray + *nMinGray) ; 

	return TRUE;
}

void CImageViewWnd::GetGrayStatistics(int left, int right, int top, int bottom, double &fMaxGray, double &fMinGray, double &fMeanGray, double &fStdev)
{
	if (!m_OriginImage.IsValidImage())
		return;

	int i = 0, j = 0 ;
	int Histogram[256] ;

	BYTE *Img = m_OriginImage.GetMemStart() ;
	BYTE *ImgTemp = NULL;

	int xCnt = right - left ;
	int yCnt = bottom - top ;

	fMaxGray = 0;
	fMinGray = 0;
	fMeanGray = 0;
	fStdev = 0;

	int aaa ;
	ZeroMemory(Histogram, sizeof(int)*256) ;

	for (i = 0; i < yCnt; i++)
	{
		//ImgTemp = &(Img[(top+i)*m_OriginImage.Width()+left]) ;

		for (j = 0; j < xCnt; j++) 
		{
			//aaa = static_cast<int>(*ImgTemp) ;
			aaa = m_OriginImage.GetPixelData(left + j, top + i);
			Histogram[aaa]++ ;
			fMeanGray += aaa ;
			fStdev += (aaa*aaa) ;
			//ImgTemp++ ;
		}
	}

	fMeanGray /= (yCnt*xCnt) ;

	fStdev = sqrt(fStdev / (yCnt * xCnt) - fMeanGray) ;

	double dRate = 0.01;
	int MaxCnt = static_cast<int>(yCnt * xCnt * dRate + 0.5) ;
	int MinCnt = MaxCnt ;

	for (i = 0; i < 256; i++) 
	{
		if (MaxCnt > 0) 
		{
			if (MaxCnt > Histogram[255-i]) 
			{
				fMaxGray += ((255-i) * Histogram[255-i]) ;
				MaxCnt -= Histogram[255-i] ;
			}
			else 
			{
				fMaxGray += (255-i) * MaxCnt ;
				MaxCnt = 0 ;
			}
		}

		if (MinCnt > 0) 
		{
			if (MinCnt > Histogram[i]) 
			{
				fMinGray += (i * Histogram[i]) ;
				MinCnt -= Histogram[i] ;
			}
			else 
			{
				fMinGray += (i * MinCnt) ;
				MinCnt = 0 ;
			}
		}

		if (MinCnt == 0 && MaxCnt == 0)
			break ;
	}

	MaxCnt = static_cast<int>(yCnt * xCnt * dRate + 0.5) ;

	fMinGray /= MaxCnt ;
	fMaxGray /= MaxCnt ;

	/*
	////////////////////////////////////////////////////////////////////////////////
	double fTempMinGray = 0.0, fTempMaxGray = 0.0;
	fTempMinGray = fMinGray;
	fTempMaxGray = fMaxGray;

	int iTotalPixelCount = 0, iMinPixelCount = 0, iMaxPixelCount = 0;
	iTotalPixelCount = xCnt * yCnt;
	iMaxPixelCount = iMinPixelCount = iTotalPixelCount * dRate;

	int iTempCount = 0;
	BOOL bMinCalc = TRUE, bMaxCalc = TRUE;

	for(i = 0; i < 256; i++)
	{
		iMinPixelCount = iMinPixelCount - Histogram[i];
		iMaxPixelCount = iMaxPixelCount - Histogram[255 - i];

		if(bMinCalc)
		{
			if(iMinPixelCount > 0)
			{
				if(Histogram[i] - iMinPixelCount < 0)
				{
					Histogram[i] = 0;
				}
				if(Histogram[i] != 0)
				{
					fMinGray = (double)i + 1.0;
					bMinCalc = FALSE;
				}
			}
		}

		if(bMaxCalc)
		{
			if(iMaxPixelCount > 0)
			{
				if(Histogram[255-i] - iMaxPixelCount < 0)
				{
					Histogram[255-i] = 0;
				}
				if(Histogram[255-i] != 0)
				{
					fMaxGray = (double)(255-i) - 1.0;
					bMaxCalc = FALSE;
				}
			}
		}
	}

	if(bMinCalc)
	{
		fMinGray = fTempMinGray;
	}
	if(bMaxCalc)
	{
		fMaxGray = fTempMaxGray;
	}
	////////////////////////////////////////////////////////////////////////////////
	*/
}

void CImageViewWnd::PaintDefect(CDC *pDC)
{
	if(m_bPaintDefect == TRUE && m_pDDMAlgorithm != NULL)
	{		
		CPoint po;
		for(int iDefect = 0; iDefect < m_pDDMAlgorithm->m_Merge.iDefectCnt; iDefect++)
		{
			if(m_pDDMAlgorithm->m_Merge.DefectList[iDefect].bDelete == TRUE)
				continue;

			for(int iPixel = 0; iPixel < m_pDDMAlgorithm->m_Merge.DefectList[iDefect].iPixelCnt; iPixel++)
			{
				po.x = m_pDDMAlgorithm->m_Merge.DefectList[iDefect].PixelList[iPixel].iX;
				po.y = m_pDDMAlgorithm->m_Merge.DefectList[iDefect].PixelList[iPixel].iY;				

				CPoint pt((int)((float)(po.x)*m_fZoom) - GetScrollPos(SB_HORZ), (int)((float)(po.y)*m_fZoom) - GetScrollPos(SB_VERT));
				CRect rect = CRect(pt.x, pt.y, pt.x+(int)(1.0f*m_fZoom), pt.y+(int)(1.0f*m_fZoom));
				pDC->FrameRect(rect, &CBrush(RGB(255, 0, 0)));
			}
		}
	}
}
void CImageViewWnd::PaintROI(CDC *pDC)
{
	
}
void CImageViewWnd::PaintPixel(CDC* pDC)
{
	if(m_fZoom >= 4.0f)
	{
		CPoint pt((int)(m_ptPixel.x*m_fZoom) - GetScrollPos(SB_HORZ), (int)(m_ptPixel.y*m_fZoom) - GetScrollPos(SB_VERT));

		CRect rect = CRect(pt.x, pt.y, pt.x+(int)(1.0f*m_fZoom), pt.y+(int)(1.0f*m_fZoom));

		//pDC->FrameRect(rect, &CBrush(RGB(0, 255, 255)));
		
		CPen pen, *pOldpen = NULL;
		pen.CreatePen(PS_SOLID, 1 + (int)(m_fZoom/32.0f), RGB(255,255,0));
		pOldpen = pDC->SelectObject(&pen);		
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.left, rect.bottom));
		pDC->LineTo(rect.BottomRight());
		pDC->LineTo(CPoint(rect.right, rect.top));
		pDC->LineTo(rect.TopLeft());
		pDC->SelectObject(pOldpen);
		pen.DeleteObject();		
	}
}

void CImageViewWnd::PaintGrid(CDC* pDC)
{
	return;

	if(m_fZoom >= 64.0f)
	{
		CRect rect;	
		int pitch = (int)(m_rtDrawView.Width()/m_fZoom);
		if(pitch < 0)
			return;

		pitch = m_rtDrawView.Width()/pitch;

		for(int y = 0; y < m_rtDrawView.Height(); y += pitch)
		{
			for(int x = 0; x < m_rtDrawView.Width(); x += pitch)
			{
				rect = CRect(x, y, x+pitch, y+pitch);
				pDC->FrameRect(rect, &CBrush(RGB(255, 255, 0)));
			}
		}
	}
}

void CImageViewWnd::MoveToPoint(CPoint point, BOOL bBoxDraw, BOOL bMovePosition)
{
	m_ptBoxDraw.x = (int)((float)(point.x) * m_fZoom);
	m_ptBoxDraw.y = (int)((float)(point.y) * m_fZoom);
	m_bBoxDraw = bBoxDraw;
	int iPos = 0;

	if (bMovePosition && m_ptBoxDraw.x >= 0 && m_ptBoxDraw.y >= 0)
	{
		if (m_DrawImage.IsValidImage())
		{
			CRect rect;
			//rect = m_rtDrawView;
			GetClientRect(&rect);

			if ((int)((float)(m_DrawImage.Width()) * m_fZoom) > rect.Width())
			{
				iPos = (int)(m_ptBoxDraw.x - (int)((float)(rect.Width()) / 2.0f));
				iPos  = iPos + (int)(m_fZoom-fmod((float)iPos, m_fZoom));
				SetScrollPos(SB_HORZ, iPos);				
			}
			if ((int)((float)(m_DrawImage.Height()) * m_fZoom) > rect.Height())
			{
				iPos = (int)(m_ptBoxDraw.y - (int)((float)(rect.Height()) / 2.0f));
				iPos  = iPos + (int)(m_fZoom-fmod((float)iPos, m_fZoom));
				SetScrollPos(SB_VERT, iPos);
			}
		}
	}

	Invalidate(FALSE);
}

void CImageViewWnd::ShowFindedEdge(int nType, CPoint ptRef, int nThreshold, int nSubCutting)
{
	if (!m_DrawImage.IsValidImage()) return;

	if (nType <= 0 && nType > 4) return;

	switch (nType)
	{
	case 1:	// Left
		{
			m_rtEdge.left = 0;
			for (int x = 0; x < m_DrawImage.Width() / 2 - 4; x++)			
			{
				double mean1 = Calc_MeanGray(x, ptRef.y - 5, 2, 10);
				double mean2 = Calc_MeanGray(x + 2, ptRef.y - 5, 2, 10);
				if ((int)fabs(mean1 - mean2) > nThreshold)
				{
					m_rtEdge.left = x + 2 + nSubCutting;
					break;
				}
			}
		}
		break;
	case 2:	// Top
		{
			m_rtEdge.top = 0;
			for (int y = 0; y < m_DrawImage.Height() / 2 - 4; y++)
			{
				double mean1 = Calc_MeanGray(ptRef.x - 5, y, 10, 2);
				double mean2 = Calc_MeanGray(ptRef.x - 5, y + 2, 10, 2);
				if ((int)fabs(mean1 - mean2) > nThreshold)
				{
					m_rtEdge.top = y + 2 + nSubCutting;
					break;
				}
			}
		}
		break;
	case 3:	// Right
		{
			m_rtEdge.right = m_DrawImage.Width() - 1;
			for (int x = m_DrawImage.Width() - 1; x > m_DrawImage.Width() / 2 - 4; x--)
			{
				double mean1 = Calc_MeanGray(x - 2, ptRef.y - 5, 2, 10);
				double mean2 = Calc_MeanGray(x - 4, ptRef.y - 5, 2, 10);
				if ((int)fabs(mean1 - mean2) > nThreshold)
				{
					m_rtEdge.right = x - 2 - nSubCutting;
					break;
				}
			}
		}
		break;
	case 4:	// Bottom
		{
			m_rtEdge.bottom = m_DrawImage.Height() - 1;
			for (int y = m_DrawImage.Height() - 1; y > m_DrawImage.Height() / 2 - 4; y--)
			{
				double mean1 = Calc_MeanGray(ptRef.x - 5, y - 2, 10, 2);
				double mean2 = Calc_MeanGray(ptRef.x - 5, y - 4, 10, 2);
				if ((int)fabs(mean1 - mean2) > nThreshold)
				{
					m_rtEdge.bottom = y - 2 - nSubCutting;
					break;
				}
			}
		}
		break;
	}
}

void CImageViewWnd::ZoomImage(float fZoomFactor, CPoint pt)
{
	if(fZoomFactor < 0.03125f)
		return;

	if(fZoomFactor > 64.0f)
		return;

	CPoint scrollPosition;
	scrollPosition.x = (int)((GetScrollPos(SB_HORZ) + pt.x)/m_fZoom);
	scrollPosition.y = (int)((GetScrollPos(SB_VERT) + pt.y)/m_fZoom);

	m_fZoom = fZoomFactor;

	GetClientRect(m_rtDrawView);
	int max = max(m_rtDrawView.Width(), m_rtDrawView.Height());	
	int margin_x = 0, margin_y = 0;
	if(m_rtDrawView.Width() > m_rtDrawView.Height())
		margin_y = m_rtDrawView.Width() - m_rtDrawView.Height();
	if(m_rtDrawView.Width() < m_rtDrawView.Height())
		margin_x = m_rtDrawView.Height() - m_rtDrawView.Width();

	max = max + (int)(m_fZoom-fmod((float)max,m_fZoom));	
	m_rtDrawView.right = max;
	m_rtDrawView.bottom = max;

	CSize szScroll;
	szScroll.cx = (int)((m_szImageSize.cx*m_fZoom - m_rtDrawView.Width()) - 1) + margin_x + (int)m_fZoom;
	szScroll.cy = (int)((m_szImageSize.cy*m_fZoom - m_rtDrawView.Height()) - 1) + margin_y + (int)m_fZoom;
	szScroll.cx = szScroll.cx + (int)(m_fZoom-fmod((float)szScroll.cx, m_fZoom));
	szScroll.cy = szScroll.cy + (int)(m_fZoom-fmod((float)szScroll.cy, m_fZoom));
	SetScrollRange(SB_HORZ, 0, szScroll.cx);
	SetScrollRange(SB_VERT, 0, szScroll.cy);

	szScroll.cx = (int)(scrollPosition.x*m_fZoom) - pt.x;
	szScroll.cy = (int)(scrollPosition.y*m_fZoom) - pt.y;
	szScroll.cx = szScroll.cx + (int)(m_fZoom-fmod((float)szScroll.cx, m_fZoom));
	szScroll.cy = szScroll.cy + (int)(m_fZoom-fmod((float)szScroll.cy, m_fZoom));
	SetScrollPos(SB_HORZ, szScroll.cx);
	SetScrollPos(SB_VERT, szScroll.cy);
	
	Invalidate(FALSE);

	ModifyIndicator_ImageSize(m_szImageSize, CSize(m_rtDrawView.Width(), m_rtDrawView.Height()));
	ModifyIndicator_Zoom(m_fZoom);
}

CPoint CImageViewWnd::GetScrollPosition()
{
	CPoint scrollPosition;
	scrollPosition.x = GetScrollPos(SB_HORZ);
	scrollPosition.y = GetScrollPos(SB_VERT);

	return scrollPosition;
}

void CImageViewWnd::ImageCutting(BOOL bCutting)
{	
	if(m_CuttedImage.IsValidImage())
		m_CuttedImage.Destroy();

	m_bCutting = bCutting;
	m_bCutStart = m_bCutted = FALSE;
	m_rtTracker.m_rect = CRect(0, 0, 0, 0);
	m_rtCutting = CRect(0, 0, 0, 0);

	Invalidate(FALSE);
}

BOOL CImageViewWnd::SaveCuttingImage()
{
	if(m_rtTracker.m_rect.Width() > 0 && m_rtTracker.m_rect.Height() > 0)
	{
		if(m_CuttedImage.IsValidImage())
		{
			m_CuttedImage.Destroy();

			CPoint scrollPosition;
			scrollPosition.x = GetScrollPos(SB_HORZ);
			scrollPosition.y = GetScrollPos(SB_VERT);

			CRect rcCutting = m_rtCutting;
			rcCutting.left = (int)((float)(rcCutting).left / m_fZoom);
			rcCutting.top = (int)((float)(rcCutting.top) / m_fZoom);
			rcCutting.right = (int)((float)(rcCutting.right) / m_fZoom);
			rcCutting.bottom = (int)((float)(rcCutting.bottom) / m_fZoom);

			m_DrawImage.GetSubBlock(&m_CuttedImage, rcCutting);

			m_CuttedImage.Save("D:\\DDMDemo\\CuttingImage\\cutting_image.bmp");

			return TRUE;
		}
	}

	return FALSE;
}

CRect CImageViewWnd::GetCuttingROIRect()
{
	return m_rtCutting;
}

double CImageViewWnd::CalcRealFOV(CDC *pDC)
{
	if(m_bCalcRealFOV == TRUE && m_pDDMAlgorithm != NULL)
	{
		m_bCalcRealFOV = FALSE;

		CRect rcCutting;

		if(m_rtTracker.m_rect.Width() > 0 && m_rtTracker.m_rect.Height() > 0)
		{
			if(m_CuttedImage.IsValidImage())
			{
				m_CuttedImage.Destroy();

				rcCutting = m_rtCutting;
				rcCutting.left = (int)((float)(rcCutting.left) / m_fZoom);
				rcCutting.top = (int)((float)(rcCutting.top) / m_fZoom);
				rcCutting.right = (int)((float)(rcCutting.right) / m_fZoom);
				rcCutting.bottom = (int)((float)(rcCutting.bottom) / m_fZoom);

				m_DrawImage.GetSubBlock(&m_CuttedImage, rcCutting);
			}
			else
				return 0.0;
		}
		else
			return 0.0;

		double dblPixelSize = m_pDDMAlgorithm->m_Param.m_iHDist*m_pDDMAlgorithm->m_Param.m_fFOV;
		double dblDefaultFOV = m_pDDMAlgorithm->m_Param.m_fFOV;
		
		IplImage* image = cvCreateImage(cvSize(m_CuttedImage.Width(), m_CuttedImage.Height()), IPL_DEPTH_8U, 1);
		memcpy(image->imageData, m_CuttedImage.GetMemStart(), m_CuttedImage.Width()*m_CuttedImage.Height());
		cvFlip(image, image, 0);

		BYTE* pByteImage = (BYTE*)image->imageData;

		CvPoint	ptFindPos = cvPoint(0, 0);

		double fFOVPerPixel = 0.0, min = 0.0, max = 0.0;
		
		int i = 0, j = 0, h = 0 ;

		int iFindCnt = 0 ;

		int width = image->widthStep ;

		//int iDistH = (int)(dblPixelSize * 90 / dblDefaultFOV / 100) ; // 90 % 크기만 템플리트로 설정
		int iDistH = (int)(dblPixelSize / dblDefaultFOV) ; // 100 % 템플리트로 설정

		BYTE* pImage = NULL;
		BYTE* pTemplate = NULL;

		//pImage = new BYTE [iDistH * 2 * nHeight * 4] ; // 위아래로 4배, 좌우로 2배
		pImage = new BYTE [iDistH * 2 * rcCutting.Height()] ; // 좌우로 2배
		pTemplate = new BYTE [iDistH * rcCutting.Height()] ;

		for (i = 0; i < rcCutting.Height(); i++) 
		{
			CopyMemory(&(pTemplate[i*iDistH]), &(pByteImage[i*width]), iDistH) ;
		}

		BYTE cr ;

		for (i = 0; i < rcCutting.Height(); i++) 
		{
			for (h = 0; h < iDistH; h++) 
			{
				cr = pTemplate[i * iDistH + h];
			}
		}

		long sum = 0 ;
		long dif ;

		double fSum = 0. ;

		j = 0 ;

		int	iLastFindPos = 0 ;
		int	iFirstFindPos = -1 ;

		int nNextFindPos = 0 ;

		int nFindCount = 0 ;

		int nBeforePos = 0 ;

		double dTotal = 255. * 255. * rcCutting.Height() * iDistH ;

		CPen pen, *pOldPen = NULL;
		pen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
		pOldPen = pDC->SelectObject(&pen);

		//while (rcCutting.left + iLastFindPos + iDistH * 2 < rcCutting.right)
		while (iLastFindPos + iDistH * 2 < rcCutting.Width())
		{
			nBeforePos = iLastFindPos;

			for (i = 0; i < rcCutting.Height(); i++) 
			{
				CopyMemory(&(pImage[i * iDistH * 2]), &(pByteImage[i * width + nNextFindPos]), iDistH * 2) ;
			}

			min = 99999999. ;

			for (int k = 0; k < iDistH; k++) 
			{
				sum = 0 ;

				for (int v = 0; v < rcCutting.Height(); v++)
				{
					for (int h = 0; h < iDistH; h++) 
					{
						dif = pImage[v * (iDistH * 2) + h + k] - pTemplate[v*iDistH+h];
						sum += (dif * dif) ;
					}
				}

				fSum = double(sum / (rcCutting.Height() * iDistH)) ;

				if (fSum < min) 
				{
					min = fSum ;

					iLastFindPos = nNextFindPos + k ;
				}

				if (min == 0.) {
					break ;
				}
			}

			int final = iLastFindPos ;

			CPoint pt;
			pt.x = (int)((float)(rcCutting.left + iLastFindPos)*m_fZoom) - GetScrollPos(SB_HORZ);
			pt.y = (int)((float)(rcCutting.top)*m_fZoom) - GetScrollPos(SB_VERT);
			pDC->MoveTo(pt);
			pt.y = (int)((float)(rcCutting.top+rcCutting.Height())*m_fZoom) - GetScrollPos(SB_VERT);
			pDC->LineTo(pt);

			if (iFirstFindPos == -1) 
			{
				iFirstFindPos = iLastFindPos ;
			}
			else
			{
				if (abs(iLastFindPos - nBeforePos) > (iDistH / 2)) {
					nFindCount++ ;
				}
			}

			nNextFindPos = iLastFindPos + (iDistH / 2) ;
		}

		if (nBeforePos == iLastFindPos || iFirstFindPos == -1) 
		{
			fFOVPerPixel = 0. ;
		}
		else 
		{
			fFOVPerPixel = dblPixelSize * double(nFindCount) / ((double)iLastFindPos - (double)iFirstFindPos) ;
		}

		delete[] pImage ;
		delete[] pTemplate ;

		cvReleaseImage(&image);

		int nOldROP2 = pDC->SetROP2(R2_NOT);
		int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clrText = pDC->SetTextColor(RGB(0, 255, 255));

		CString str;
		str.Format("FOV = %.3lf", fFOVPerPixel);		
		pDC->TextOut((int)((float)(rcCutting.left + 5)*m_fZoom) - GetScrollPos(SB_HORZ), 
					 (int)((float)(rcCutting.top+5)*m_fZoom) - GetScrollPos(SB_VERT), str);

		pDC->SetTextColor(clrText);
		pDC->SetBkMode(nOldBkMode);
		pDC->SetROP2(nOldROP2);

		pen.DeleteObject();
		pDC->SelectObject(pOldPen);

		return fFOVPerPixel ;
	}

	return 0.0;
}

void CImageViewWnd::ModifyIndicator_ImageSize(CSize szImage, CSize szView)
{
	CString str;
	str.Format(" Image Size : %d X %d", szImage.cx, szImage.cy);
	m_pDlgInspection->SetDlgItemText(IDC_STATIC_IMAGE_SIZE, str);
	m_pDlgInspection->m_iImageSizeX = szImage.cx;
	m_pDlgInspection->m_iImageSizeY = szImage.cy;
}

void CImageViewWnd::ModifyIndicator_Zoom(float fZoom)
{
	CString str;
	str.Format(" Zoom : %.2f", fZoom);
	m_CSZoomRate = str;
	m_pDlgInspection->SetDlgItemText(IDC_STATIC_IMAGE_ZOOM, str);
	m_pDlgInspection->ZoomRateValue(fZoom);
}

void CImageViewWnd::ModifyIndicator_Position(CPoint ptPos, CPoint ptPixel, BYTE btUp, BYTE btImage, BYTE btDown, BYTE btLeft, BYTE btRight, BYTE btDiff, BYTE btThreshold)
{
	CString str;
	//str.Format("Image X : %d, Y : %d == Pixel X : %d, PY : %d == Gray Vert : %d-%d-%d == Gray Hori : %d-%d-%d == Gray Diff : %d == Threshold : %d", 
	//				ptPos.x, ptPos.y, ptPixel.x, ptPixel.y, btUp, btImage, btDown, btLeft, btImage, btRight, btDiff, btThreshold);

	str.Format(" [Image X : %d, Y : %d] == [Gray Vert : %d-%d-%d] == [Gray Hori : %d-%d-%d] == [Gray Diff : %d] == [Threshold : %d]", 
		ptPos.x, ptPos.y, btUp, btImage, btDown, btLeft, btImage, btRight, btDiff, btThreshold);

	m_pDlgInspection->SetDlgItemText(IDC_STATIC_IMAGE_POSITION, str);
}

void CImageViewWnd::ModifyIndicator_Image_Position(CPoint ptPos, BYTE btImage, BYTE btImageUp, BYTE btImageDown, BYTE btImageLeft, BYTE btImageRight,
	BYTE btUp, BYTE btDown, BYTE btLeft, BYTE btRight, BYTE btVertDiff, BYTE btHoriDiff,
	BYTE btMultiUp, BYTE btMultiDown, BYTE btMultiLeft, BYTE btMultiRight, BYTE btMultiVertDiff, BYTE btMultiHoriDiff,
	BYTE btThirdUp, BYTE btThirdDown, BYTE btThirdLeft, BYTE btThirdRight, BYTE btThirdVertDiff, BYTE btThirdHoriDiff, BYTE btThreshold)
{
	CString str = "";
	//str.Format("[Image X : %05d, Y : %05d]    [Gray Vert : %03d-%03d-%03d-(%03d)-%03d-%03d-%03d == Vert Gray Diff : %03d-%03d-%03d]    [Gray Hori : %03d-%03d-%03d-(%03d)-%03d-%03d-%03d == Hori Gray Diff : %03d-%03d-%03d]    [Adjacent Gray : V(%03d-%03d), H(%03d-%03d)]    [Threshold : %03d]",
	//	ptPos.x, ptPos.y,
	//	btThirdUp, btMultiUp, btUp, btImage, btDown, btMultiDown, btThirdDown,
	//	btVertDiff, btMultiVertDiff, btThirdVertDiff,
	//	btThirdLeft, btMultiLeft, btLeft, btImage, btRight, btMultiRight, btThirdRight,
	//	btHoriDiff, btMultiHoriDiff, btThirdHoriDiff,
	//	btImageUp, btImageDown, btImageLeft, btImageRight,
	//	btThreshold);

	str.Format("[Gray Vert : %03d-%03d-%03d-(%03d)-%03d-%03d-%03d == Vert Gray Diff : %03d-%03d-%03d]    [Gray Hori : %03d-%03d-%03d-(%03d)-%03d-%03d-%03d == Hori Gray Diff : %03d-%03d-%03d]    [Adjacent Gray : V(%03d-%03d), H(%03d-%03d)]",
		btThirdUp, btMultiUp, btUp, btImage, btDown, btMultiDown, btThirdDown,
		btVertDiff, btMultiVertDiff, btThirdVertDiff,
		btThirdLeft, btMultiLeft, btLeft, btImage, btRight, btMultiRight, btThirdRight,
		btHoriDiff, btMultiHoriDiff, btThirdHoriDiff,
		btImageUp, btImageDown, btImageLeft, btImageRight);

	m_pDlgInspection->SetDlgItemText(IDC_STATIC_IMAGE_INFO, str);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CImageViewWnd::OnPaint()
{	
	CRect rcClient;
	rcClient = m_rtDrawView;

	CPaintDC pdc(this);
	CUserMemDC dc(&pdc);
	
	FCObjImage *pImage = NULL;
	
	if(m_bPaintDefect == TRUE)
		pImage = &m_DrawImage;
	else
		pImage = &m_OriginImage;
	
	dc.FillSolidRect(rcClient, RGB(0, 0, 0));

	if (pImage->IsValidImage())
	{		
		CPoint scrollPosition;
		scrollPosition.x = GetScrollPos(SB_HORZ);
		scrollPosition.y = GetScrollPos(SB_VERT);
		
		CRect rr = CRect(CPoint(scrollPosition.x, scrollPosition.y), rcClient.Size());
		CRect rr2 = CRect((int)((float)(rr.left) / m_fZoom), (int)((float)(rr.top) / m_fZoom), 
        		        (int)((float)(rr.right) / m_fZoom), (int)((float)(rr.bottom) / m_fZoom));
		
		FCWin32::DrawImage(*pImage, dc.GetSafeHdc(), rcClient, rr2);
		
		if (m_bBoxDraw)
		{
			int width = rcClient.Width();
			int height = rcClient.Height();

			m_ptBoxDraw.x -= scrollPosition.x;
			m_ptBoxDraw.y -= scrollPosition.y;

			int left = (int)(m_ptBoxDraw.x - 10.0*m_fZoom);
			if(left < 0)
				left = 0;
			int top = (int)(m_ptBoxDraw.y - 10.0*m_fZoom);
			if(top < 0)
				top = 0;
			int right = (int)(m_ptBoxDraw.x + 10.0*m_fZoom);
			if(right > width)
				right = width;
			int bottom = (int)(m_ptBoxDraw.y + 10.0*m_fZoom);
			if(bottom > height)
				bottom = height;

			CRect r2 = CRect(left, top, right, bottom);	
			dc.FrameRect(r2, &CBrush(RGB(0, 255, 0)));
		}

		//if (m_bProfile && m_bProfileStart)
		if (m_bProfile)
		{
			if (m_rtProfile.Width() > 0 || m_rtProfile.Height() > 0)
			{
				CRect rcProfile = m_rtProfile;
				CRect rcProfileN = rcProfile;

				rcProfileN.NormalizeRect();

				CRect rcDrawN = rcProfileN;
				rcDrawN.OffsetRect(-scrollPosition);

				if ((int)((float)rcProfileN.right / m_fZoom) >= pImage->Width()
					|| (int)((float)rcProfileN.bottom / m_fZoom) >= pImage->Height())
					return;

				CRect rcProfileO = CRect((int)((float)rcProfile.left / m_fZoom), m_DrawImage.Height() - (int)((float)rcProfile.bottom / m_fZoom) - 1,
										 (int)((float)rcProfile.right / m_fZoom), m_DrawImage.Height() - (int)((float)rcProfile.top / m_fZoom) - 1);

				CRect rcDraw0 = rcProfileO;
				rcDraw0.OffsetRect(-scrollPosition);

				CPen pen;
				pen.CreatePen(PS_SOLID, 0, RGB(0, 255, 255));
				//pen.CreatePen(PS_SOLID, 0, RGB(200, 255, 0));
				CPen *pOldPen = dc.SelectObject(&pen);

				//dc.MoveTo(rcProfile.TopLeft());
				//dc.LineTo(rcProfile.BottomRight());
				dc.MoveTo(CPoint(rcProfile.TopLeft() - scrollPosition));
				dc.LineTo(CPoint(rcProfile.BottomRight() - scrollPosition));

				register int x, y, nVal, min = 256, max = 0;
				float fMean = 0;
				BYTE *pImageBuf = m_DrawImage.GetMemStart();
				m_rtSaveGrayProfile = rcProfileO;
				int nCount[256];

				if (rcProfileO.Width() > rcProfileO.Height())
				{
					ZeroMemory(nCount, sizeof(int)*256);

					for (x = rcProfileO.left; x < rcProfileO.right; x++)
					{
						y = (rcProfileO.top - rcProfileO.bottom) * (x - rcProfileO.left) / (rcProfileO.right - rcProfileO.left) + rcProfileO.bottom;
						nVal = pImageBuf[y * pImage->GetPitch() + x];
						
						nCount[nVal]++;

						if (nVal < min) min = nVal;
						if (nVal > max) max = nVal;
						fMean += nVal;
					}
					fMean /= abs(rcProfileO.right - rcProfileO.left);

					////////////////////////////////////////////////////////////////////////////////////////////////
					// 상, 하위 1% 제거
					if(m_bNormalizeGray == TRUE)
					{
						int iTotalPixelCount = 0, iMinPixelCount = 0, iMaxPixelCount = 0;
						iTotalPixelCount = abs(rcProfileO.right-rcProfileO.left);
						int iRatio = int(iTotalPixelCount * 0.01);
						//if(iRatio <= 0) iRatio = 1;
						iMaxPixelCount = iMinPixelCount = iRatio;

						int iTempCount = 0;
						BOOL bMinCalc = TRUE;
						BOOL bMaxCalc = TRUE;
						int iMin = 0, iMax = 0;

						for (int i = 0 ; i < 256 ; i++)
						{
							iMinPixelCount = iMinPixelCount - nCount[i];
							iMaxPixelCount = iMaxPixelCount - nCount[255 - i];

							if (bMinCalc)
							{
								if (iMinPixelCount > 0)
								{
									if (nCount[i] - iMinPixelCount < 0)
										nCount[i] = 0;
								}
								if (nCount[i] != 0 )
								{
									iMin = (double)i + 1;
									bMinCalc = FALSE;
								}
							}

							if (bMaxCalc)
							{
								if (iMaxPixelCount > 0)
								{
									if (nCount[255-i] - iMaxPixelCount < 0)
										nCount[255-i] = 0 ;
								}
								if (nCount[255-i] != 0)
								{
									iMax = (double)(255-i) - 1;
									bMaxCalc = FALSE;
								}
							}
						}

						min = iMin;
						max = iMax;
					}
					///////////////////////////////////////////////////////////////////////

					////////////////////////////////////////////////////////////////////////////////////////////////

					for (x = rcProfileO.left; x < rcProfileO.right; x++)
					{
						y = (rcProfileO.top - rcProfileO.bottom) * (x - rcProfileO.left) / (rcProfileO.right - rcProfileO.left) + rcProfileO.bottom;
						y = (pImageBuf[y * pImage->GetPitch() + x] - min) * 50 / ((max == min) ? 1 : (max - min));

						if (x == rcProfileO.left)
							dc.MoveTo(CPoint(CPoint(rcProfile.left + (int)((float)(x - rcProfileO.left) * m_fZoom), rcProfileN.bottom + (50 - y) + 5) - scrollPosition));
						dc.LineTo(CPoint(CPoint(rcProfile.left + (int)((float)(x - rcProfileO.left) * m_fZoom), rcProfileN.bottom + (50 - y) + 5) - scrollPosition));
					}
				}
				else
				{
					ZeroMemory(nCount, sizeof(int)*256);

					for (y = rcProfileO.top; y < rcProfileO.bottom; y++)
					{
						x = (rcProfileO.right - rcProfileO.left) * (y - rcProfileO.bottom) / (rcProfileO.top - rcProfileO.bottom) + rcProfileO.left;
						nVal = pImageBuf[y * pImage->GetPitch() + x];

						nCount[nVal]++;

						if (nVal < min) min = nVal;
						if (nVal > max) max = nVal;
						fMean += nVal;
					}
					fMean /= abs(rcProfileO.bottom - rcProfileO.top);

					////////////////////////////////////////////////////////////////////////////////////////////////
					// 상, 하위 1% 제거
					if(m_bNormalizeGray == TRUE)
					{
						int iTotalPixelCount = 0, iMinPixelCount = 0, iMaxPixelCount = 0;

						iTotalPixelCount = abs(rcProfileO.bottom-rcProfileO.top);
						int iRatio = int(iTotalPixelCount * 0.01);
						//if(iRatio <= 0) iRatio = 1;
						iMaxPixelCount = iMinPixelCount = iRatio;

						int iTempCount = 0;
						BOOL bMinCalc = TRUE;
						BOOL bMaxCalc = TRUE;
						int iMin = 0, iMax = 0;

						for (int i = 0 ; i < 256 ; i++)
						{
							iMinPixelCount = iMinPixelCount - nCount[i];
							iMaxPixelCount = iMaxPixelCount - nCount[255 - i];

							if (bMinCalc)
							{
								if (iMinPixelCount > 0)
								{
									if (nCount[i] - iMinPixelCount < 0)
										nCount[i] = 0;
								}
								if (nCount[i] != 0 )
								{
									iMin = (double)i + 1;
									bMinCalc = FALSE;
								}
							}

							if (bMaxCalc)
							{
								if (iMaxPixelCount > 0)
								{
									if (nCount[255-i] - iMaxPixelCount < 0)
										nCount[255-i] = 0 ;
								}
								if (nCount[255-i] != 0)
								{
									iMax = (double)(255-i) - 1;
									bMaxCalc = FALSE;
								}
							}
						}

						min = iMin;
						max = iMax;
					}
					////////////////////////////////////////////////////////////////////////////////////////////////

					for (y = rcProfileO.top; y < rcProfileO.bottom; y++)
					{
						x = (rcProfileO.right - rcProfileO.left) * (y - rcProfileO.bottom) / (rcProfileO.top - rcProfileO.bottom) + rcProfileO.left;
						x = (pImageBuf[y * pImage->GetPitch() + x] - min) * 50 / ((max == min) ? 1 : (max - min));

						if (y == rcProfileO.top)
							dc.MoveTo(CPoint(CPoint(rcProfile.right + 5 + x, rcProfileN.bottom - (int)((float)(y - rcProfileO.top) * m_fZoom)) - scrollPosition));
						dc.LineTo(CPoint(CPoint(rcProfile.right + 5 + x, rcProfileN.bottom - (int)((float)(y - rcProfileO.top) * m_fZoom)) - scrollPosition));
					}
				}

				int nOldBkMode = dc.SetBkMode(TRANSPARENT);
				COLORREF crOldTextColor = dc.SetTextColor(RGB(0, 255, 0));
				int nOldAlign = dc.SetTextAlign(TA_BOTTOM);

				int nYExtra = (rcProfileO.Width() > rcProfileO.Height()) ? 50 : 0;

				CString str;				
				str.Format("min = %d, max = %d", min, max);
				dc.TextOut(rcProfileN.left + 3 - scrollPosition.x,	(rcProfileN.top - 60 - scrollPosition.y > 0) ? rcProfileN.top - 60 - scrollPosition.y : rcProfileN.bottom + 15 + nYExtra - scrollPosition.y, str);

				str.Format("(max-min) = %d", max - min);
				dc.TextOut(rcProfileN.left + 3 - scrollPosition.x,	(rcProfileN.top - 60 - scrollPosition.y > 0) ? rcProfileN.top - 45 - scrollPosition.y : rcProfileN.bottom + 30 + nYExtra - scrollPosition.y, str);

				str.Format("mean = %0.2f", fMean);
				dc.TextOut(rcProfileN.left + 3 - scrollPosition.x,	(rcProfileN.top - 60 - scrollPosition.y > 0) ? rcProfileN.top - 30 - scrollPosition.y : rcProfileN.bottom + 45 + nYExtra - scrollPosition.y, str);

				//str.Format("S/N(dB):Max = %0.2f", 20 * log10f((max == 0 ? fMean : max) / fMean));
				//dc.TextOut(rcProfileN.left + 3 - scrollPosition.x,	(rcProfileN.top - 60 - scrollPosition.y > 0) ? rcProfileN.top - 15 - scrollPosition.y : rcProfileN.bottom + 60 + nYExtra - scrollPosition.y, str);

				//str.Format("S/N(dB):Min = %0.2f", 20 * log10f((min == 0 ? fMean : min) / fMean));
				//dc.TextOut(rcProfileN.left + 3 - scrollPosition.x,	(rcProfileN.top - 60 - scrollPosition.y > 0) ? rcProfileN.top - scrollPosition.y : rcProfileN.bottom + 75 + nYExtra - scrollPosition.y, str);

				dc.SetTextAlign(nOldAlign);
				dc.SetTextColor(crOldTextColor);
				dc.SetBkMode(nOldBkMode);

				dc.SelectObject(pOldPen);
				pen.DeleteObject();
			}
		}
		else if (abs(m_rtCalcRect.Width()) >= CALC_MIN && abs(m_rtCalcRect.Height()) >= CALC_MIN)
		{
			CRect rcCalc = m_rtCalcRect;

			rcCalc.NormalizeRect();

			CRect rcDraw = rcCalc;
			rcDraw.OffsetRect(-scrollPosition);

			CRect rcCalcO = rcCalc;
			CRect rcDraw0 = rcDraw;

			rcCalcO.left = (int)((float)rcCalcO.left / m_fZoom);
			rcCalcO.top = (int)((float)rcCalcO.top / m_fZoom);
			rcCalcO.right = (int)((float)rcCalcO.right / m_fZoom);
			rcCalcO.bottom = (int)((float)rcCalcO.bottom / m_fZoom);

			if(m_bIsDisplayRoiCoordinate)
			{
				m_pDlgInspection->SetRectCoordinate(rcCalcO);
			}

			int nOldROP2 = dc.SetROP2(R2_NOT);
			
			//dc.FrameRect(rcDraw, &CBrush(RGB(0, 255, 255)));
			dc.FrameRect(rcDraw, &CBrush(RGB(255, 255, 0)));

			double fAverage = Calc_MeanGray(rcCalcO.left, rcCalcO.top, rcCalcO.Width(), rcCalcO.Height());
			double fStd = Calc_StdGray(fAverage, rcCalcO.left, rcCalcO.top, rcCalcO.Width(), rcCalcO.Height());

			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			//COLORREF clrText = dc.SetTextColor(RGB(0, 255, 255));
			COLORREF clrText = dc.SetTextColor(RGB(255, 255, 0));

			CString str;
			str.Format("avg:%0.2f", fAverage);			
			dc.TextOut(rcDraw.left + 3, rcDraw.top + 3, str);

			str.Format("std:%0.2f", fStd);			
			dc.TextOut(rcDraw.left + 3, rcDraw.top + 18, str);

			str.Format("(dx:%d, dy:%d)  Measure FOV = %f um", rcCalcO.Width(), rcCalcO.Height(), rcCalcO.Width() / m_pDlgInspection->m_dPixelsize);
			dc.TextOut(rcDraw.left + 3, rcDraw.top + 33, str);

			int nCount[256], nMinGray, nMaxGray, nMaxCount, nMaxCountGray;
			double dblContrast = 0.0;
			if (Histogram(rcCalcO.left, rcCalcO.top, rcCalcO.Width(), rcCalcO.Height(), nCount, &nMinGray, &nMaxGray, &nMaxCount, &nMaxCountGray, &dblContrast))
			{
				CPen pen;
				pen.CreatePen(PS_SOLID, 0, RGB(0, 255, 255));				
				CPen *pOldPen = dc.SelectObject(&pen);

				for (int i = nMinGray; i <= nMaxGray; i++)
				{
					if (nCount[i] > 0)
					{
						dc.MoveTo(rcDraw0.right + 3 + i - nMinGray, rcDraw0.bottom);
						dc.LineTo(rcDraw0.right + 3 + i - nMinGray, rcDraw0.bottom - nCount[i] * 100 / nMaxCount);
					}
				}

				dc.SelectObject(pOldPen);
				
				//CPoint ptPos(rcDraw0.right + 5 + nMaxGray - nMinGray, rcDraw0.bottom - 100);
				CPoint ptPos(rcDraw0.right + 5*m_fZoom, rcDraw0.bottom - 100);
				if(ptPos.x < 0) ptPos.x = 10;
				if(ptPos.x > rcClient.right) ptPos.x = rcClient.right - 200;
				if(ptPos.y < 0) ptPos.y = 10;
				if(ptPos.y > rcClient.bottom) ptPos.y = rcClient.bottom - 200;

				str.Format("Max Count Gray: %d(%d)", nMaxCountGray, nMaxCount);
				dc.TextOut(ptPos.x, ptPos.y, str);

				str.Format("Min Gray: %d", nMinGray);				
				dc.TextOut(ptPos.x, ptPos.y + 15, str);

				str.Format("Max Gray: %d", nMaxGray);				
				dc.TextOut(ptPos.x, ptPos.y + 30, str);

				str.Format("Contrast: %.3lf", dblContrast);				
				dc.TextOut(ptPos.x, ptPos.y + 45, str);

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				double dblMax = 0.0, dblMin = 0.0, dblMean = 0.0, dblStd = 0.0;
				CRect gRect = m_rtCalcRect;
				gRect.NormalizeRect();
				gRect.left = (int)((float)gRect.left / m_fZoom);
				gRect.top = (int)((float)gRect.top / m_fZoom);
				gRect.right = (int)((float)gRect.right / m_fZoom);
				gRect.bottom = (int)((float)gRect.bottom / m_fZoom);

				GetGrayStatistics(gRect.left, gRect.right, gRect.top, gRect.bottom, dblMax, dblMin, dblMean, dblStd);

				str.Format("Area Min Gray: %.2lf", dblMin);
				dc.TextOut(ptPos.x, ptPos.y + 60, str);

				str.Format("Area Max Gray: %.2lf", dblMax);
				dc.TextOut(ptPos.x, ptPos.y + 75, str);

				str.Format("Area Mean Gray: %.2lf", dblMean);
				dc.TextOut(ptPos.x, ptPos.y + 90, str);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

			dc.SetTextColor(clrText);
			dc.SetBkMode(nOldBkMode);
			dc.SetROP2(nOldROP2);
		}

		if (m_bCutting && m_bCutStart)
		{
			CRect rect = m_rtCutting;
			rect.OffsetRect(-scrollPosition);

			dc.FrameRect(rect, &CBrush(RGB(0, 255, 255)));
		}

		//if(m_bCutting)
		if(m_bCutted)
		{
			if(m_rtTracker.m_rect.Width() > 0 && m_rtTracker.m_rect.Height() > 0)
			{
				CRect rect = m_rtCutting;
				rect.OffsetRect(-scrollPosition);
				m_rtTracker.m_rect = rect;
				m_rtTracker.Draw(&dc);
			}
		}

		PaintDefect(dc);
		PaintROI(dc);
		PaintPixel(dc);
		PaintGrid(dc);
		CalcRealFOV(dc);
	}
}

void CImageViewWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	SetFocus();
	
	m_bBoxDraw = FALSE;
	m_bIsDisplayRoiCoordinate = FALSE;

	CPoint scrollPosition;
	scrollPosition.x = GetScrollPos(SB_HORZ);
	scrollPosition.y = GetScrollPos(SB_VERT);

	int left = scrollPosition.x + point.x;
	int top = scrollPosition.y + point.y;
	
	if(m_bCutting)
	{
		if(m_bCutted)
		{
			if(m_rtTracker.m_rect.Width() > 0 && m_rtTracker.m_rect.Height() > 0)
			{
				if(m_rtTracker.HitTest(point) < 0)
				{
					CRcTracker track = m_rtTracker;
					if(track.TrackRubberBand(this, point, TRUE))
					{
						track.m_rect.NormalizeRect();
						m_rtTracker.m_rect = track.m_rect;
						m_rtTracker.m_rect.OffsetRect(scrollPosition);
						m_rtCutting = m_rtTracker.m_rect;
					}
					else
					{
						m_rtTracker.m_rect = CRect(0, 0, 0, 0);
						m_bCutted = FALSE;
					}
				}
				else
				{
					if(m_rtTracker.Track(this, point, TRUE))					
					{
						m_rtTracker.m_rect.NormalizeRect();
						m_rtTracker.m_rect.OffsetRect(scrollPosition);
						m_rtCutting = m_rtTracker.m_rect;
					}
					else
					{
						m_rtTracker.m_rect = CRect(0, 0, 0, 0);
						m_bCutted = FALSE;
					}
				}
			}
		}
		else
		{
			if(!m_bCutStart)
			{
				m_bCutStart = TRUE;
				m_rtCutting = CRect(left, top, left, top);
				m_bCutted = FALSE;
			}
			else
			{
				m_bCutStart = FALSE;
				//m_rtCutting.right = left;
				//m_rtCutting.right -= m_rtCutting.Width() % 4;
				//m_rtCutting.bottom = top;

				CRect rcCutting = m_rtCutting;
				rcCutting.left = (int)((float)(rcCutting.left) / m_fZoom);
				rcCutting.top = (int)((float)(rcCutting.top) / m_fZoom);
				rcCutting.right = (int)((float)(rcCutting.right) / m_fZoom);
				rcCutting.bottom = (int)((float)(rcCutting.bottom) / m_fZoom);

				if(m_CuttedImage.IsValidImage())
				{
					m_CuttedImage.Destroy();
				}

				m_DrawImage.GetSubBlock(&m_CuttedImage, rcCutting);
				
				m_bCutted = TRUE;
				
				CRect rect = m_rtCutting;
				m_rtTracker.m_rect = rect;
			}
		}
	}
	else if(m_bProfile)
	{
		m_rtProfile = CRect(left, top, left, top);
		m_bProfileStart = TRUE;
		m_rtSaveGrayProfile = CRect(0,0,0,0);
	}
	else if(m_bPanImage)
	{
		m_bPanStart = TRUE;
		m_ptPanStart = point;
	}
	else
	{
		m_bCalcRect = TRUE;
		m_rtCalcRect = CRect(left, top, left, top);	
		if(nFlags == MK_CONTROL + MK_LBUTTON)
		{
			m_bIsDisplayRoiCoordinate = TRUE;
		}
	}

	m_ptSaveGrayStart = CPoint(left/m_fZoom, top/m_fZoom);
	m_ptSaveGrayEnd = CPoint(0, 0);

	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CImageViewWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CPoint pt;
	pt.x = GetScrollPos(SB_HORZ) + point.x;
	pt.y = GetScrollPos(SB_VERT) + point.y;
					
	if (m_bProfile)
	{
		m_rtProfile.right = pt.x;
		m_rtProfile.bottom = pt.y;
		m_bProfileStart = FALSE;
	}
	else if(m_bPanStart)
	{		
		m_bPanStart = FALSE;
	}
	else
	{
		m_bCalcRect = FALSE;
	}

	m_ptSaveGrayEnd = CPoint(pt.x/m_fZoom, pt.y/m_fZoom);

	Invalidate(FALSE);

	CWnd::OnLButtonUp(nFlags, point);
}

void CImageViewWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (!m_iEdgeType == 0)
	{
		m_bBoxDraw = FALSE;

		ShowFindedEdge(m_iEdgeType, point, m_iEdgeThreshold, m_iEdgeSubCutting);
	}

	Invalidate(FALSE);

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CImageViewWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int x = GetScrollPos(SB_HORZ) + point.x;
	int y = GetScrollPos(SB_VERT) + point.y;

	if (m_DrawImage.IsValidImage())
	{
		CRect rtROI = m_pDDMAlgorithm->m_Param.m_rtROI;
		int iVDist = m_pDDMAlgorithm->m_Param.m_iVDist, iHDist = m_pDDMAlgorithm->m_Param.m_iHDist;

		int iMVDist = iVDist * 2, iMHDist = iHDist * 2;
		int iTVDist = iVDist * 3, iTHDist = iHDist * 3;

		if(rtROI.right > m_DrawImage.Width())
			rtROI.right = m_DrawImage.Width();
		if(rtROI.bottom > m_DrawImage.Height())
			rtROI.bottom = m_DrawImage.Height();

		/*
		if(m_pDDMAlgorithm->m_Param.m_bCellOffsetZero == FALSE)
		{
			rtROI.top += iVDist;
			rtROI.bottom -= iVDist;
			rtROI.left += iHDist;
			rtROI.right -= iHDist;
		}
		*/
		
		CPoint po((int)(x / m_fZoom), (int)(y / m_fZoom));
		CPoint p1(x, y);

		m_ptPixel = CPoint((int)((float)(GetScrollPos(SB_HORZ) + point.x) / m_fZoom), (int)((float)(GetScrollPos(SB_VERT) + point.y) / m_fZoom));

		if (po.x >= 0 && po.x < (int)m_DrawImage.Width() && po.y >= 0 && po.y < (int)m_DrawImage.Height())
		{
			DWORD dwUp = 0, dwImage = 0, dwDown = 0;
			DWORD dwLeft = 0, dwRight = 0;
			DWORD dwDiff = 0, dwThreshold = 0;

			DWORD dwImageUp = 0, dwImageDown = 0, dwImageLeft = 0, dwImageRight = 0;
			DWORD dwVertDiff = 0, dwHoriDiff = 0;
			DWORD dwMultiUp = 0, dwMultiDown = 0, dwMultiLeft = 0, dwMultiRight = 0;
			DWORD dwMultiVertDiff = 0, dwMultiHoriDiff = 0;
			DWORD dwThirdUp = 0, dwThirdDown = 0, dwThirdLeft = 0, dwThirdRight = 0;
			DWORD dwThirdVertDiff = 0, dwThirdHoriDiff = 0;
			DWORD dwLU = 0, dwLD = 0, dwRU = 0, dwRD = 0, dwMultiLU = 0, dwMultiLD = 0, dwMultiRU = 0, dwMultiRD = 0, dwThirdLU = 0, dwThirdLD = 0, dwThirdRU = 0, dwThirdRD = 0;
			DWORD dwLL = 0, dwLR = 0, dwRL = 0, dwRR = 0, dwMultiLL = 0, dwMultiLR = 0, dwMultiRL = 0, dwMultiRR = 0, dwThirdLL = 0, dwThirdLR = 0, dwThirdRL = 0, dwThirdRR = 0;


			dwImage = m_OriginImage.GetPixelData(po.x, po.y);
			dwImageUp = m_OriginImage.GetPixelData(po.x, po.y - 1);
			dwImageDown = m_OriginImage.GetPixelData(po.x, po.y + 1);
			dwImageLeft = m_OriginImage.GetPixelData(po.x - 1, po.y);
			dwImageRight = m_OriginImage.GetPixelData(po.x + 1, po.y);


			if(po.y - iVDist < 0)
			{
				if(po.y + 2*iVDist <= rtROI.bottom-1)
				{
					dwUp = m_OriginImage.GetPixelData(po.x, po.y + iVDist);
					dwDown = m_OriginImage.GetPixelData(po.x, po.y + 2*iVDist);
				}
			}
			else if(po.y + iVDist > rtROI.bottom-1)
			{
				if(po.y - 2*iVDist >= 0)
				{
					dwUp = m_OriginImage.GetPixelData(po.x, po.y - iVDist);
					dwDown = m_OriginImage.GetPixelData(po.x, po.y - 2*iVDist);
				}
			}
			else
			{
				dwUp = m_OriginImage.GetPixelData(po.x, po.y - iVDist);
				dwDown = m_OriginImage.GetPixelData(po.x, po.y + iVDist);
			}

			if(po.x - iHDist < 0)
			{
				if(po.x + 2*iHDist <= rtROI.right-1)
				{
					dwLeft = m_OriginImage.GetPixelData(po.x + iHDist, po.y);
					dwRight = m_OriginImage.GetPixelData(po.x + 2*iHDist, po.y);

					dwLU = m_OriginImage.GetPixelData(po.x + iHDist, po.y - 1);
					dwLD = m_OriginImage.GetPixelData(po.x + iHDist, po.y + 1);
					dwLL = m_OriginImage.GetPixelData(po.x + iHDist - 1, po.y);
					dwLR = m_OriginImage.GetPixelData(po.x + iHDist + 1, po.y);

					dwRU = m_OriginImage.GetPixelData(po.x + 2 * iHDist, po.y - 1);
					dwRD = m_OriginImage.GetPixelData(po.x + 2 * iHDist, po.y + 1);
					dwRL = m_OriginImage.GetPixelData(po.x + 2 * iHDist - 1, po.y);
					dwRR = m_OriginImage.GetPixelData(po.x + 2 * iHDist + 1, po.y);
				}
			}
			else if(po.x + iHDist > rtROI.right - 1)
			{
				if(po.x - 2*iHDist >= 0)
				{
					dwLeft = m_OriginImage.GetPixelData(po.x - iHDist, po.y);
					dwRight = m_OriginImage.GetPixelData(po.x - 2*iHDist, po.y);

					dwLU = m_OriginImage.GetPixelData(po.x - iHDist, po.y - 1);
					dwLD = m_OriginImage.GetPixelData(po.x - iHDist, po.y + 1);
					dwLL = m_OriginImage.GetPixelData(po.x - iHDist - 1, po.y);
					dwLR = m_OriginImage.GetPixelData(po.x - iHDist + 1, po.y);

					dwRU = m_OriginImage.GetPixelData(po.x - 2 * iHDist, po.y - 1);
					dwRD = m_OriginImage.GetPixelData(po.x - 2 * iHDist, po.y + 1);
					dwRL = m_OriginImage.GetPixelData(po.x - 2 * iHDist - 1, po.y);
					dwRR = m_OriginImage.GetPixelData(po.x - 2 * iHDist + 1, po.y);
				}
			}
			else
			{
				dwLeft = m_OriginImage.GetPixelData(po.x - iHDist, po.y);
				dwRight = m_OriginImage.GetPixelData(po.x + iHDist, po.y);

				dwLU = m_OriginImage.GetPixelData(po.x - iHDist, po.y - 1);
				dwLD = m_OriginImage.GetPixelData(po.x - iHDist, po.y + 1);
				dwLL = m_OriginImage.GetPixelData(po.x - iHDist - 1, po.y);
				dwLR = m_OriginImage.GetPixelData(po.x - iHDist + 1, po.y);

				dwRU = m_OriginImage.GetPixelData(po.x + iHDist, po.y - 1);
				dwRD = m_OriginImage.GetPixelData(po.x + iHDist, po.y + 1);
				dwRL = m_OriginImage.GetPixelData(po.x + iHDist - 1, po.y);
				dwRR = m_OriginImage.GetPixelData(po.x + iHDist + 1, po.y);
			}

			dwVertDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwUp), SUB((BYTE)dwUp, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwDown), SUB((BYTE)dwDown, (BYTE)dwImage)));
			dwHoriDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwLeft), SUB((BYTE)dwLeft, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwRight), SUB((BYTE)dwRight, (BYTE)dwImage)));

			if (po.y - iMVDist < 0)
			{
				if (po.y + 2 * iMVDist <= rtROI.bottom - 1)
				{
					dwMultiUp = m_OriginImage.GetPixelData(po.x, po.y + iMVDist);
					dwMultiDown = m_OriginImage.GetPixelData(po.x, po.y + 2 * iMVDist);
				}
			}
			else if (po.y + iMVDist > rtROI.bottom - 1)
			{
				if (po.y - 2 * iMVDist >= 0)
				{
					dwMultiUp = m_OriginImage.GetPixelData(po.x, po.y - iMVDist);
					dwMultiDown = m_OriginImage.GetPixelData(po.x, po.y - 2 * iMVDist);
				}
			}
			else
			{
				dwMultiUp = m_OriginImage.GetPixelData(po.x, po.y - iMVDist);
				dwMultiDown = m_OriginImage.GetPixelData(po.x, po.y + iMVDist);
			}

			if (po.x - iMHDist < 0)
			{
				if (po.x + 2 * iMHDist <= rtROI.right - 1)
				{
					dwMultiLeft = m_OriginImage.GetPixelData(po.x + iMHDist, po.y);
					dwMultiRight = m_OriginImage.GetPixelData(po.x + 2 * iMHDist, po.y);

					dwMultiLU = m_OriginImage.GetPixelData(po.x + iMHDist, po.y - 1);
					dwMultiLD = m_OriginImage.GetPixelData(po.x + iMHDist, po.y + 1);
					dwMultiLL = m_OriginImage.GetPixelData(po.x + iMHDist - 1, po.y);
					dwMultiLR = m_OriginImage.GetPixelData(po.x + iMHDist + 1, po.y);

					dwMultiRU = m_OriginImage.GetPixelData(po.x + 2 * iMHDist, po.y - 1);
					dwMultiRD = m_OriginImage.GetPixelData(po.x + 2 * iMHDist, po.y + 1);
					dwMultiRL = m_OriginImage.GetPixelData(po.x + 2 * iMHDist - 1, po.y);
					dwMultiRR = m_OriginImage.GetPixelData(po.x + 2 * iMHDist + 1, po.y);
				}
			}
			else if (po.x + iMHDist > rtROI.right - 1)
			{
				if (po.x - 2 * iMHDist >= 0)
				{
					dwMultiLeft = m_OriginImage.GetPixelData(po.x - iMHDist, po.y);
					dwMultiRight = m_OriginImage.GetPixelData(po.x - 2 * iMHDist, po.y);

					dwMultiLU = m_OriginImage.GetPixelData(po.x - iMHDist, po.y - 1);
					dwMultiLD = m_OriginImage.GetPixelData(po.x - iMHDist, po.y + 1);
					dwMultiLL = m_OriginImage.GetPixelData(po.x - iMHDist - 1, po.y);
					dwMultiLR = m_OriginImage.GetPixelData(po.x - iMHDist + 1, po.y);

					dwMultiRU = m_OriginImage.GetPixelData(po.x - 2 * iMHDist, po.y - 1);
					dwMultiRD = m_OriginImage.GetPixelData(po.x - 2 * iMHDist, po.y + 1);
					dwMultiRL = m_OriginImage.GetPixelData(po.x - 2 * iMHDist - 1, po.y);
					dwMultiRR = m_OriginImage.GetPixelData(po.x - 2 * iMHDist + 1, po.y);
				}
			}
			else
			{
				dwMultiLeft = m_OriginImage.GetPixelData(po.x - iMHDist, po.y);
				dwMultiRight = m_OriginImage.GetPixelData(po.x + iMHDist, po.y);

				dwMultiLU = m_OriginImage.GetPixelData(po.x - iMHDist, po.y - 1);
				dwMultiLD = m_OriginImage.GetPixelData(po.x - iMHDist, po.y + 1);
				dwMultiLL = m_OriginImage.GetPixelData(po.x - iMHDist - 1, po.y);
				dwMultiLR = m_OriginImage.GetPixelData(po.x - iMHDist + 1, po.y);

				dwMultiRU = m_OriginImage.GetPixelData(po.x + iMHDist, po.y - 1);
				dwMultiRD = m_OriginImage.GetPixelData(po.x + iMHDist, po.y + 1);
				dwMultiRL = m_OriginImage.GetPixelData(po.x + iMHDist - 1, po.y);
				dwMultiRR = m_OriginImage.GetPixelData(po.x + iMHDist + 1, po.y);
			}

			dwMultiVertDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwMultiUp), SUB((BYTE)dwMultiUp, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwMultiDown), SUB((BYTE)dwMultiDown, (BYTE)dwImage)));
			dwMultiHoriDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwMultiLeft), SUB((BYTE)dwMultiLeft, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwMultiRight), SUB((BYTE)dwMultiRight, (BYTE)dwImage)));

			if (po.y - iTVDist < 0)
			{
				if (po.y + 2 * iTVDist <= rtROI.bottom - 1)
				{
					dwThirdUp = m_OriginImage.GetPixelData(po.x, po.y + iTVDist);
					dwThirdDown = m_OriginImage.GetPixelData(po.x, po.y + 2 * iTVDist);
				}
			}
			else if (po.y + iTVDist > rtROI.bottom - 1)
			{
				if (po.y - 2 * iTVDist >= 0)
				{
					dwThirdUp = m_OriginImage.GetPixelData(po.x, po.y - iTVDist);
					dwThirdDown = m_OriginImage.GetPixelData(po.x, po.y - 2 * iTVDist);
				}
			}
			else
			{
				dwThirdUp = m_OriginImage.GetPixelData(po.x, po.y - iTVDist);
				dwThirdDown = m_OriginImage.GetPixelData(po.x, po.y + iTVDist);
			}

			if (po.x - iTHDist < 0)
			{
				if (po.x + 2 * iTHDist <= rtROI.right - 1)
				{
					dwThirdLeft = m_OriginImage.GetPixelData(po.x + iTHDist, po.y);
					dwThirdRight = m_OriginImage.GetPixelData(po.x + 2 * iTHDist, po.y);

					dwThirdLU = m_OriginImage.GetPixelData(po.x + iTHDist, po.y - 1);
					dwThirdLD = m_OriginImage.GetPixelData(po.x + iTHDist, po.y + 1);
					dwThirdLL = m_OriginImage.GetPixelData(po.x + iTHDist - 1, po.y);
					dwThirdLR = m_OriginImage.GetPixelData(po.x + iTHDist + 1, po.y);

					dwThirdRU = m_OriginImage.GetPixelData(po.x + 2 * iTHDist, po.y - 1);
					dwThirdRD = m_OriginImage.GetPixelData(po.x + 2 * iTHDist, po.y + 1);
					dwThirdRL = m_OriginImage.GetPixelData(po.x + 2 * iTHDist - 1, po.y);
					dwThirdRR = m_OriginImage.GetPixelData(po.x + 2 * iTHDist + 1, po.y);
				}
			}
			else if (po.x + iTHDist > rtROI.right - 1)
			{
				if (po.x - 2 * iTHDist >= 0)
				{
					dwThirdLeft = m_OriginImage.GetPixelData(po.x - iTHDist, po.y);
					dwThirdRight = m_OriginImage.GetPixelData(po.x - 2 * iTHDist, po.y);

					dwThirdLU = m_OriginImage.GetPixelData(po.x - iTHDist, po.y - 1);
					dwThirdLD = m_OriginImage.GetPixelData(po.x - iTHDist, po.y + 1);
					dwThirdLL = m_OriginImage.GetPixelData(po.x - iTHDist - 1, po.y);
					dwThirdLR = m_OriginImage.GetPixelData(po.x - iTHDist + 1, po.y);

					dwThirdRU = m_OriginImage.GetPixelData(po.x - 2 * iTHDist, po.y - 1);
					dwThirdRD = m_OriginImage.GetPixelData(po.x - 2 * iTHDist, po.y + 1);
					dwThirdRL = m_OriginImage.GetPixelData(po.x - 2 * iTHDist - 1, po.y);
					dwThirdRR = m_OriginImage.GetPixelData(po.x - 2 * iTHDist + 1, po.y);
				}
			}
			else
			{
				dwThirdLeft = m_OriginImage.GetPixelData(po.x - iTHDist, po.y);
				dwThirdRight = m_OriginImage.GetPixelData(po.x + iTHDist, po.y);

				dwThirdLU = m_OriginImage.GetPixelData(po.x - iTHDist, po.y - 1);
				dwThirdLD = m_OriginImage.GetPixelData(po.x - iTHDist, po.y + 1);
				dwThirdLL = m_OriginImage.GetPixelData(po.x - iTHDist - 1, po.y);
				dwThirdLR = m_OriginImage.GetPixelData(po.x - iTHDist + 1, po.y);

				dwThirdRU = m_OriginImage.GetPixelData(po.x + iTHDist, po.y - 1);
				dwThirdRD = m_OriginImage.GetPixelData(po.x + iTHDist, po.y + 1);
				dwThirdRL = m_OriginImage.GetPixelData(po.x + iTHDist - 1, po.y);
				dwThirdRR = m_OriginImage.GetPixelData(po.x + iTHDist + 1, po.y);
			}

			dwThirdVertDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwThirdUp), SUB((BYTE)dwThirdUp, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwThirdDown), SUB((BYTE)dwThirdDown, (BYTE)dwImage)));
			dwThirdHoriDiff = MIN(MAX(SUB((BYTE)dwImage, (BYTE)dwThirdLeft), SUB((BYTE)dwThirdLeft, (BYTE)dwImage)), MAX(SUB((BYTE)dwImage, (BYTE)dwThirdRight), SUB((BYTE)dwThirdRight, (BYTE)dwImage)));



			dwDiff = MAX(MIN(SUB((BYTE)dwImage, (BYTE)dwUp), SUB((BYTE)dwImage, (BYTE)dwDown)), MIN(SUB((BYTE)dwUp, (BYTE)dwImage), SUB((BYTE)dwDown, (BYTE)dwImage)));
			dwThreshold = (DWORD)((double)((m_pDDMAlgorithm->m_Param.m_iThresholdHigh - m_pDDMAlgorithm->m_Param.m_iThresholdLow)) / 256.0 * (BYTE)dwImage + m_pDDMAlgorithm->m_Param.m_iThresholdLow);
			ModifyIndicator_Position(po, p1, (BYTE)dwUp, (BYTE)dwImage, (BYTE)dwDown, (BYTE)dwLeft, (BYTE)dwRight, (BYTE)dwDiff, (BYTE)dwThreshold);

			ModifyIndicator_Image_Position(po, (BYTE)dwImage, (BYTE)dwImageUp, (BYTE)dwImageDown, (BYTE)dwImageLeft, (BYTE)dwImageRight,
				(BYTE)dwUp, (BYTE)dwDown, (BYTE)dwLeft, (BYTE)dwRight, (BYTE)dwVertDiff, (BYTE)dwHoriDiff,
				(BYTE)dwMultiUp, (BYTE)dwMultiDown, (BYTE)dwMultiLeft, (BYTE)dwMultiRight, (BYTE)dwMultiVertDiff, (BYTE)dwMultiHoriDiff,
				(BYTE)dwThirdUp, (BYTE)dwThirdDown, (BYTE)dwThirdLeft, (BYTE)dwThirdRight, (BYTE)dwThirdVertDiff, (BYTE)dwThirdHoriDiff,
				(BYTE)dwThreshold);
		}
		
		if(m_fZoom >= 4.0f)
			Invalidate(FALSE);
	}

	if(m_bPanImage)
	{
		if(m_bPanStart)
		{
			int pos_x = 0, pos_y = 0;
			int offset_x = 0, offset_y = 0;
			offset_x = m_ptPanStart.x - point.x;
			offset_y = m_ptPanStart.y - point.y;

			if(abs(offset_x) > 2)
			{
				pos_x = (GetScrollPos(SB_HORZ) + offset_x);
				pos_x = pos_x + (int)(m_fZoom-fmod((float)pos_x, m_fZoom));
				SetScrollPos(SB_HORZ, pos_x, TRUE);
				
				m_ptPanStart.x = point.x;
			}

			if(abs(offset_y) > 2)
			{
				pos_y = (GetScrollPos(SB_VERT) + offset_y);
				pos_y  = pos_y + (int)(m_fZoom-fmod((float)pos_y, m_fZoom));
				SetScrollPos(SB_VERT, pos_y, TRUE);

				m_ptPanStart.y = point.y;
			}

			Invalidate(FALSE);
		}
	}

	if (m_bCalcRect)
	{
		m_rtCalcRect.right = x;
		m_rtCalcRect.bottom = y;

		Invalidate(FALSE);
	}

	if (m_bCutting && m_bCutStart)
	{
		m_rtCutting.right = x;
		m_rtCutting.bottom = y;

		Invalidate(FALSE);
	}

	if (m_bProfile && m_bProfileStart)
	{
		m_rtProfile.right = x;
		m_rtProfile.bottom = y;

		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CImageViewWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if(m_DrawImage.IsValidImage())
	{
		CPoint pt = point;
		ClientToScreen(&pt);

		CMenu		menu;
		CMenu*		pSubMenu = NULL;
		menu.LoadMenu(IDR_CONTEXTMENU); // 메뉴 리소스 로드		
		pSubMenu = menu.GetSubMenu(0);

		if(pSubMenu == NULL)
		{
			CWnd::OnRButtonDown(nFlags, point);
			return;
		}

		pSubMenu->CheckMenuItem(ID_PROFILE, m_bProfile ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
		pSubMenu->CheckMenuItem(ID_DRAW_DEFECT, m_bPaintDefect ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
		pSubMenu->CheckMenuItem(ID_PAN_IMAGE, m_bPanImage ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);
		pSubMenu->CheckMenuItem(ID_NORMALIZE_GRAY, m_bNormalizeGray ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND);

		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);		
	}

	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CImageViewWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	int iVDist = m_pDDMAlgorithm->m_Param.m_iVDist, iHDist = m_pDDMAlgorithm->m_Param.m_iHDist;

	int nMaxSB = 0, nSBPos = 0;
	CRect rect;
	GetClientRect(rect);

	int max = max(rect.Width(), rect.Height());
	int margin_x = 0, margin_y = 0;
	if(rect.Width() > rect.Height())
		margin_y = rect.Width() - rect.Height();
	if(rect.Width() < rect.Height())
		margin_x = rect.Height() - rect.Width();



	if(m_DrawImage.IsValidImage())
	{
		if(nFlags == MK_CONTROL)
		{
			nMaxSB = (int)(m_szImageSize.cy*m_fZoom - max) + margin_y + (int)m_fZoom;
			nMaxSB = nMaxSB + (int)(m_fZoom-fmod((float)nMaxSB, m_fZoom));

			if(zDelta <= 1) // Wheel Down
			{
				if ((nSBPos = GetScrollPos(SB_VERT) + (int)(iVDist*m_fZoom)) > nMaxSB)
					nSBPos = nMaxSB;
			}
			else			// Wheel up
			{
				if ((nSBPos = GetScrollPos(SB_VERT) - (int)(iVDist*m_fZoom)) < 0)
					nSBPos = 0;
			}
			if (nSBPos >= 0) 
			{	
				SetScrollPos(SB_VERT, nSBPos);
				Invalidate(FALSE);
			}
		}
		else if(nFlags == MK_SHIFT)
		{
			nMaxSB = (int)(m_szImageSize.cx*m_fZoom - max) + margin_x + (int)m_fZoom;
			nMaxSB = nMaxSB + (int)(m_fZoom-fmod((float)nMaxSB, m_fZoom));

			if(zDelta <= 1) // Wheel Down
			{
				if ((nSBPos = GetScrollPos(SB_HORZ) + (int)(iHDist*m_fZoom)) > nMaxSB)
					nSBPos = nMaxSB;
			}
			else			// Wheel up
			{
				if ((nSBPos = GetScrollPos(SB_HORZ) - (int)(iHDist*m_fZoom)) < 0)
					nSBPos = 0;
			}
			
			if (nSBPos >= 0) 
			{	
				SetScrollPos(SB_HORZ, nSBPos);		
				Invalidate(FALSE);
			}
		}
		else
		{
			ScreenToClient(&pt);

			if (m_rtDrawView.PtInRect(pt))
			{
				if (zDelta == -120)
				{
					if(m_fZoom <= 1.0f)
						ZoomImage(m_fZoom-0.05f, pt);
					else
						ZoomImage(m_fZoom-1.0f, pt);
				}
				
				if(zDelta == 120)
				{
					if(m_fZoom < 1.0f)
						ZoomImage(m_fZoom+0.05f, pt);
					else
						ZoomImage(m_fZoom+1.0f, pt);			
				}

				SetFocus();
			}
		}
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CImageViewWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if(m_DrawImage.IsValidImage())
	{
		ZoomImage(1.0, point);
	}

	CWnd::OnMButtonDown(nFlags, point);
}


void CImageViewWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_DrawImage.IsValidImage() == false)
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	
	int iVDist = m_pDDMAlgorithm->m_Param.m_iVDist, iHDist = m_pDDMAlgorithm->m_Param.m_iHDist;

	int nMaxSB = 0, nSBPos = 0;

	CRect rect;
	GetClientRect(rect);
	int max = max(rect.Width(), rect.Height());
	int margin_x = 0, margin_y = 0;
	if(rect.Width() > rect.Height())
		margin_y = rect.Width() - rect.Height();
	if(rect.Width() < rect.Height())
		margin_x = rect.Height() - rect.Width();

	nMaxSB = (int)(m_szImageSize.cx*m_fZoom - max) + margin_x + (int)m_fZoom;
	nMaxSB = nMaxSB + (int)(m_fZoom-fmod((float)nMaxSB, m_fZoom));

	switch (nSBCode)
	{
	case SB_LINEUP :
		if ((nSBPos = GetScrollPos(SB_HORZ) - (int)(iHDist*m_fZoom)) < 0)
			nSBPos = 0;
		break;

	case SB_LINEDOWN :
		if ((nSBPos = GetScrollPos(SB_HORZ) + (int)(iHDist*m_fZoom)) > nMaxSB)
			nSBPos = nMaxSB;
		break;

	case SB_PAGEUP :
		if ((nSBPos = GetScrollPos(SB_HORZ) - (int)(iHDist*m_fZoom)) < 0)
			nSBPos = 0;
		break;

	case SB_PAGEDOWN :
		if ((nSBPos = GetScrollPos(SB_HORZ) + (int)(iHDist*m_fZoom)) > nMaxSB)
			nSBPos = nMaxSB;
		break;

	case SB_THUMBPOSITION :
		{
			//nSBPos = nPos;

			SCROLLINFO si = {0};
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_HORZ, &si, SIF_ALL);			

			int pos  = si.nTrackPos + (int)(m_fZoom-fmod((float)si.nTrackPos, m_fZoom));

			if(pos < 0)
				pos = 0;
			if(pos > nMaxSB)
				pos = nMaxSB;

			SetScrollPos(SB_HORZ, pos, TRUE);

			Invalidate(FALSE);
		}
		break;

	case SB_THUMBTRACK :
		{
			//nSBPos = nPos;

			SCROLLINFO si = {0};
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_HORZ, &si, SIF_ALL);			

			int pos  = si.nTrackPos + (int)(m_fZoom-fmod((float)si.nTrackPos, m_fZoom));

			if(pos < 0)
				pos = 0;
			if(pos > nMaxSB)
				pos = nMaxSB;

			SetScrollPos(SB_HORZ, pos, TRUE);

			Invalidate(FALSE);
		}
		break;

	default :
		nSBPos = -1;
		break;
	}

	if (nSBPos >= 0 && nSBCode < SB_THUMBPOSITION) 
	{	
		SetScrollPos(SB_HORZ, nSBPos);
		
		Invalidate(FALSE);
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageViewWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_DrawImage.IsValidImage() == false)
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	int iVDist = m_pDDMAlgorithm->m_Param.m_iVDist, iHDist = m_pDDMAlgorithm->m_Param.m_iHDist;

	int nMaxSB = 0, nSBPos = 0;
	CRect rect;
	GetClientRect(rect);
	int max = max(rect.Width(), rect.Height());
	int margin_x = 0, margin_y = 0;
	if(rect.Width() > rect.Height())
		margin_y = rect.Width() - rect.Height();
	if(rect.Width() < rect.Height())
		margin_x = rect.Height() - rect.Width();

	nMaxSB = (int)(m_szImageSize.cy*m_fZoom - max) + margin_y + (int)m_fZoom;
	nMaxSB = nMaxSB + (int)(m_fZoom-fmod((float)nMaxSB, m_fZoom));

	switch (nSBCode)
	{
	case SB_LINEUP :
		if ((nSBPos = GetScrollPos(SB_VERT) - (int)(iVDist*m_fZoom)) < 0)
			nSBPos = 0;
		break;

	case SB_LINEDOWN :
		if ((nSBPos = GetScrollPos(SB_VERT) + (int)(iVDist*m_fZoom)) > nMaxSB)
			nSBPos = nMaxSB;
		break;

	case SB_PAGEUP :
		if ((nSBPos = GetScrollPos(SB_VERT) - (int)(iVDist*m_fZoom)) < 0)
			nSBPos = 0;
		break;

	case SB_PAGEDOWN :
		if ((nSBPos = GetScrollPos(SB_VERT) + (int)(iVDist*m_fZoom)) > nMaxSB)
			nSBPos = nMaxSB;
		break;

	case SB_THUMBPOSITION :
		{
			//nSBPos = nPos;

			SCROLLINFO si = {0};
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si, SIF_ALL);			

			int pos  = si.nTrackPos + (int)(m_fZoom-fmod((float)si.nTrackPos, m_fZoom));
			
			if(pos < 0)
				pos = 0;
			if(pos > nMaxSB)
				pos = nMaxSB;

			SetScrollPos(SB_VERT, pos, TRUE);

			Invalidate(FALSE);
		}
		break;

	case SB_THUMBTRACK:
		{
			//nSBPos = nPos;

			SCROLLINFO si = {0};
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si, SIF_ALL);			
			
			int pos  = si.nTrackPos + (int)(m_fZoom-fmod((float)si.nTrackPos, m_fZoom));
			
			if(pos < 0)
				pos = 0;
			if(pos > nMaxSB)
				pos = nMaxSB;			

			SetScrollPos(SB_VERT, pos, TRUE);

			Invalidate(FALSE);
		}
		break;

	default :			 
		nSBPos = -1;
		break;
	}

	if (nSBPos >= 0 && nSBCode < SB_THUMBPOSITION) 
	{	
		SetScrollPos(SB_VERT, nSBPos);
		Invalidate(FALSE);
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImageViewWnd::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSize szSize = CSize(12000, 12000);
	if (m_DrawImage.IsValidImage())
		szSize = CSize((int)((float)m_DrawImage.Width() * m_fZoom), (int)((float)m_DrawImage.Height() * m_fZoom));

	Invalidate();
}

BOOL CImageViewWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//CWnd::OnEraseBkgnd(pDC);
}

BOOL CImageViewWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_rtTracker.SetCursor(pWnd, nHitTest))
		return TRUE;

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CImageViewWnd::OnProfile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	DrawProfile(m_bProfile);
}

void CImageViewWnd::OnDrawDefect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	SetPaintDefect(m_bPaintDefect);
	Invalidate();
}

void CImageViewWnd::OnUpdateProfile(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	m_bProfile = !m_bProfile;
	pCmdUI->SetCheck(m_bProfile);

	if(m_bProfile == TRUE && m_bPanImage == TRUE)
		m_bPanImage = FALSE;
}

void CImageViewWnd::OnUpdateDrawDefect(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	m_bPaintDefect = !m_bPaintDefect;
	pCmdUI->SetCheck(m_bPaintDefect);
}


void CImageViewWnd::OnPanImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	SetPanImage(m_bPanImage);
}

void CImageViewWnd::OnUpdatePanImage(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	m_bPanImage = !m_bPanImage;
	pCmdUI->SetCheck(m_bPanImage);

	if(m_bPanImage == TRUE && m_bProfile == TRUE)
		m_bProfile = FALSE;
}

void CImageViewWnd::OnNormalizeGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CImageViewWnd::OnUpdateNormalizeGray(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	m_bNormalizeGray = !m_bNormalizeGray;
	pCmdUI->SetCheck(m_bNormalizeGray);
}
