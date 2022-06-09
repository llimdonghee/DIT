#include "StdAfx.h"
#include "akMemDC.h"
#include "akStaticGroup.h"


BEGIN_MESSAGE_MAP(CakStaticGroup, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


CakStaticGroup::CakStaticGroup(void)
{
	m_nTitlePosX = 17;
	m_nTitlePosY = 26;

	m_nDrawtype = 0;
	m_bImageLoad = false;
	m_bTransparency = false;
	m_TransparencyColor = RGB(255,255,254);
	m_rectMargin.SetRect(50,50,10,10);

	m_colorTitle = RGB(255,255,255);
	m_nTitleTextAlign = TA_LEFT|TA_BOTTOM;


}

CakStaticGroup::~CakStaticGroup(void)
{
	clear();
}


void CakStaticGroup::clear()
{
	m_bImageLoad = false;

	m_bitImage.DeleteObject();
}


void CakStaticGroup::setImage( UINT bitmapid, int drawType /*=0*/ )
{
	

	m_bitImage.DeleteObject();
	m_bitImage.LoadBitmap(bitmapid);

	m_bitImage.GetBitmap(&m_bitImageInfo);
	
	
	ModifyStyle(NULL, WS_CLIPSIBLINGS | SS_OWNERDRAW ,	0);
	SetWindowPos(&wndBottom, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	
	m_bImageLoad = true;
	GetClientRect(&m_rectClient);

	Invalidate(TRUE);


}

void CakStaticGroup::setTransparency( bool bflag, COLORREF color /*= RGB(255,255,255)*/ )
{
	m_bTransparency = bflag;
	m_TransparencyColor = color;
}

BOOL CakStaticGroup::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//if(m_bImageLoad == true) return true;
	//return CStatic::OnEraseBkgnd(pDC);
}


void CakStaticGroup::SmartBitmapScale(CDC* pDC, CRect rectDC/*그려질곳의 위치*/, CBitmap* bitmap)
{
	if(pDC == NULL) return;

	CDC memDC;
	CBitmap* pOldBitmap;
	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = memDC.SelectObject(bitmap);

	BITMAP bitInfo;
	bitmap->GetBitmap(&bitInfo);

	int sideL,sideR, sideT, sideB;
	sideL=sideR=sideT=sideB=10;
	sideL = m_rectMargin.left;
	sideR = m_rectMargin.right;
	sideT = m_rectMargin.top;
	sideB = m_rectMargin.bottom;

	//pDC->FillSolidRect(rectDC.left, rectDC.top, rectDC.Width(), rectDC.Height(), RGB(0,0,255));
	
	//위1
	pDC->BitBlt(rectDC.left, rectDC.top, sideL, sideT, &memDC, 0,0, SRCCOPY);
	//위2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.top, rectDC.Width()-sideL-sideR, sideT, &memDC, sideL, 0, bitInfo.bmWidth-sideL-sideR, sideT, SRCCOPY);
	//위3
	pDC->BitBlt(rectDC.right-sideR, rectDC.top, sideR, sideT, &memDC, bitInfo.bmWidth-sideR,0, SRCCOPY);

	//가운데 1
	pDC->TransparentBlt(rectDC.left, rectDC.top+sideT, sideL, rectDC.Height()-sideT-sideB, &memDC, 0, sideT, sideL, bitInfo.bmHeight-sideT-sideB, SRCCOPY);
	//가운데 2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.top+sideT, rectDC.Width()-sideL-sideR, rectDC.Height()-sideT-sideB, &memDC, sideL, sideT, bitInfo.bmWidth-sideL-sideR, bitInfo.bmHeight-sideT-sideB, SRCCOPY);
	//가운데 3
	pDC->TransparentBlt(rectDC.left+rectDC.Width()-sideR, rectDC.top+sideT, sideR, rectDC.Height()-sideT-sideB, &memDC, bitInfo.bmWidth-sideR, sideT, sideR, bitInfo.bmHeight-sideT-sideB, SRCCOPY);


	//아래1
	pDC->BitBlt(rectDC.left, rectDC.bottom-sideB, sideL, sideB, &memDC, 0,bitInfo.bmHeight-sideB, SRCCOPY);
	//아래2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.bottom-sideB, rectDC.Width()-sideL-sideR, sideB, &memDC, sideL, bitInfo.bmHeight-sideB, bitInfo.bmWidth-sideL-sideR, sideB, SRCCOPY);
	//아래3
	pDC->BitBlt(rectDC.right-sideR, rectDC.bottom-sideB, sideR, sideB, &memDC, bitInfo.bmWidth-sideR,bitInfo.bmHeight-sideB, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return;
	//

}


void CakStaticGroup::SmartBitmapScaleTransparent(CDC* pDC, CRect rectDC/*그려질곳의 위치*/, CBitmap* bitmap,  COLORREF trColor)
{
	if(pDC == NULL) return;


	CDC memDC;
	CBitmap* pOldBitmap;
	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = memDC.SelectObject(bitmap);

	BITMAP bitInfo;
	bitmap->GetBitmap(&bitInfo);

	int sideL,sideR, sideT, sideB;
	sideL=sideR=sideT=sideB=10;
	sideL = m_rectMargin.left;
	sideR = m_rectMargin.right;
	sideT = m_rectMargin.top;
	sideB = m_rectMargin.bottom;
	//pDC->FillSolidRect(rectDC.left, rectDC.top, rectDC.Width(), rectDC.Height(), RGB(0,0,255));

	//위1
	pDC->TransparentBlt(rectDC.left, rectDC.top, sideL, sideT, &memDC, 0,0,sideL,sideT, trColor);
	//위2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.top, rectDC.Width()-sideL-sideR, sideT, &memDC, sideL, 0, bitInfo.bmWidth-sideL-sideR, sideT,trColor);
	//위3
	pDC->TransparentBlt(rectDC.right-sideR, rectDC.top, sideR, sideT, &memDC, bitInfo.bmWidth-sideR,0, sideR, sideT,  trColor);

	//가운데 1
	pDC->TransparentBlt(rectDC.left, rectDC.top+sideT, sideL, rectDC.Height()-sideT-sideB, &memDC, 0, sideT, sideL, bitInfo.bmHeight-sideT-sideB, trColor);
	//가운데 2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.top+sideT, rectDC.Width()-sideL-sideR, rectDC.Height()-sideT-sideB, &memDC, sideL, sideT, bitInfo.bmWidth-sideL-sideR, bitInfo.bmHeight-sideT-sideB, trColor);
	//가운데 3
	pDC->TransparentBlt(rectDC.left+rectDC.Width()-sideR, rectDC.top+sideT, sideR, rectDC.Height()-sideT-sideB, &memDC, bitInfo.bmWidth-sideR, sideT, sideR, bitInfo.bmHeight-sideT-sideB, trColor);

	//아래1
	pDC->TransparentBlt(rectDC.left, rectDC.bottom-sideB, sideL , sideB, &memDC, 0,bitInfo.bmHeight-sideB,  sideL, sideB, trColor);
	//아래2
	pDC->TransparentBlt(rectDC.left+sideL, rectDC.bottom-sideB, rectDC.Width()-sideL-sideR, sideB, &memDC, sideL, bitInfo.bmHeight-sideB, bitInfo.bmWidth-sideL-sideR, sideB, trColor);
	//아래3
	pDC->TransparentBlt(rectDC.right-sideR, rectDC.bottom-sideB, sideR, sideB, &memDC, bitInfo.bmWidth-sideR,bitInfo.bmHeight-sideB, sideR, sideB,  trColor);

	//memDC.DeleteDC();

}


void CakStaticGroup::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}

void CakStaticGroup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.

	//return;
	CDC *mydc=&dc;
	mydc->SetBkMode(TRANSPARENT);
	

	CakMemDC memDC(mydc);
	memDC.SetBkMode(TRANSPARENT);
	memDC->SetStretchBltMode(COLORONCOLOR);
	CakMemDC* pMemDC = &memDC;

	CFont* pOldFont = (CFont*)pMemDC->SelectObject(m_fontTitle);

	memDC->FillSolidRect(m_rectClient, RGB(0,0,0));


	if(m_bTransparency)
	{
		CDC* pParentDC = GetParent()->GetDC();
		CRect rectClient;
		GetWindowRect(&rectClient);
		GetParent()->ScreenToClient(&rectClient);
		pMemDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), pParentDC, rectClient.left, rectClient.top, SRCCOPY);
		SmartBitmapScaleTransparent(pMemDC, m_rectClient, &m_bitImage, m_TransparencyColor);
		//ReleaseDC(pParentDC);

	}
	else
	{
		SmartBitmapScale(pMemDC, m_rectClient, &m_bitImage);
	}


	CString strTitle;	GetWindowText(strTitle);
	{
		CRect rect;
		GetClientRect(&rect);

	

		UINT oldAlign = pMemDC->SetTextAlign(TA_TOP | TA_LEFT);
		CFont *pOldFont;
		
		pOldFont = pMemDC->SelectObject(GetFont());
		pMemDC->SetTextColor(m_colorTitle);

		pMemDC->SetBkMode(TRANSPARENT);

		pMemDC->TextOut(m_nTitlePosX, m_nTitlePosY, strTitle);


		pMemDC->SetTextAlign(oldAlign);
		pMemDC->SelectObject(pOldFont);
	}
}

BOOL CakStaticGroup::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID))
	{
		
		return true;

	}

	return false;
}

void CakStaticGroup::setFont( char* fontName, int fontSize )
{

	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);
	
	
	if(fontName && strlen(fontName)>0)
	{
		strcpy(lf.lfFaceName, fontName);
	}

	if(fontSize)
	{
		lf.lfHeight = -fontSize;
	}

	m_fontTitle.DeleteObject();
	m_fontTitle.CreateFontIndirect(&lf); 

	SetFont(&m_fontTitle);
}
BOOL CakStaticGroup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_LBUTTONDOWN
		||pMsg->message == WM_LBUTTONDBLCLK)
	{
		return TRUE;
	}
	return CStatic::PreTranslateMessage(pMsg);
}
