// HMButton.cpp : implementation file
//

#include "stdafx.h"
#include "HMButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMButton

CHMButton::CHMButton()
{
	m_bHM = FALSE;
	m_bTracking = FALSE;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);
	m_Font.CreateFontIndirect(&m_lf);

	m_RGBTextNormal = RGB(10, 10, 10);
	m_RGBTextDisable = RGB(80, 80, 80);
}

CHMButton::~CHMButton()
{
	m_Font.DeleteObject();
}
IMPLEMENT_DYNAMIC(CHMButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CHMButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHMButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHOver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
//	CHMButton message handlers

void CHMButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

BOOL CHMButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

// Set the tooltip with a string resource
void CHMButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetToolTipText(&sText, bActivate);

}

// Set the tooltip with a CString
void CHMButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CHMButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip

// Activate the tooltip
void CHMButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

void CHMButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(mydc);

	CBitmap * pOldBitmap;
	pOldBitmap = pMemDC -> SelectObject(&m_Bitmap);
	
	CPoint point(0,0);	
		
	if(lpDrawItemStruct->itemState & ODS_SELECTED)	// Click
	{
		m_bIsDisable = FALSE;
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,m_ButtonSize.cy * 2,SRCCOPY);
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED && m_uiImageCount > 3)	// 이미지가 네 개 이상이면 네 번째가 Disable
	{
		m_bIsDisable = TRUE;
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,m_ButtonSize.cy * 3,SRCCOPY);
	}
	else
	{
		m_bIsDisable = FALSE;
		if(m_bHM)	// OverMouse
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,m_ButtonSize.cy,SRCCOPY);
		}
		else	// 일반
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
		}	
	}
	fPaint(mydc);

	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
	pMemDC = NULL;
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/HM
BOOL CHMButton::LoadBitmap(UINT bitmapid, UINT uiImageCount)
{
	m_uiImageCount = uiImageCount;

	m_Bitmap.Detach();
	m_Bitmap.Attach(::LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	m_Bitmap.GetBitmap(&bitmapbits);
	m_ButtonSize.cy = bitmapbits.bmHeight / uiImageCount;
	m_ButtonSize.cx = bitmapbits.bmWidth;

	SetWindowPos(NULL, 0, 0, m_ButtonSize.cx, m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER);

	return TRUE;
}

LRESULT CHMButton::OnMouseHOver(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHM=TRUE;
	Invalidate();
	return 0;
}

LRESULT CHMButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHM=FALSE;
	Invalidate();
	return 0;
}

void CHMButton::SetWindowText(CString strText) 
{
	m_strText = strText;
	Invalidate();
}

void CHMButton::fPaint(CDC* pDC)
{
	if(m_strText.GetLength())
	{
		if (!m_bIsDisable)
			pDC->SetTextColor(m_RGBTextNormal);
		else
			pDC->SetTextColor(m_RGBTextDisable);

		RECT Rect;
		GetClientRect(&Rect);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(m_Font);
	    pDC->DrawText(m_strText,&Rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void CHMButton::SetFontSize(int nSize)
{
	m_lf.lfHeight = nSize;
	
	m_Font.DeleteObject();
	
	BOOL bCreated = m_Font.CreateFontIndirect(&m_lf);

	Invalidate();
}

void CHMButton::SetTextColor(COLORREF rgbNormal, COLORREF rgbDisable)
{
	m_RGBTextNormal = rgbNormal;
	m_RGBTextDisable = rgbDisable;
}

void CHMButton::SetHMButtonStyle(UINT uID, int nImgCount)
{
	LoadBitmap(uID, 4);
	Invalidate();
}

void CHMButton::SetHMButtonStyle(UINT uID, int nImgCount, CString spTipText)
{
	LoadBitmap(uID, 4);

	if (!spTipText.IsEmpty())
		SetToolTipText(&spTipText);
	Invalidate();
}

void CHMButton::SetHMButtonStyle(UINT uID, int nImgCount, CString spTipText, CString spBtnText, int nFontSize)
{
	LoadBitmap(uID, 4);

	if (!spTipText.IsEmpty())
		SetToolTipText(&spTipText);
	if (nFontSize > 0 && !spBtnText.IsEmpty())
	{
		SetFontSize(nFontSize);
		SetWindowText(spBtnText);
	}
	Invalidate();
}