// XIcon.cpp  Version 1.0 - see article at CodeProject.com
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XIcon implements CXIcon, XIcon displays text and/or icon in a control
//     that is suitable for use on a groupbox header, or any place where
//     the text/icon could dynamically change.
//
// History
//     Version 1.0 - 2008 May 4
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XIcon.h"
#include "XlconRadio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)			// disable bogus deprecation warning

#ifndef __noop
#if _MSC_VER < 1300
#define __noop ((void)0)
#endif
#endif

#undef TRACE
#define TRACE __noop
#undef TRACERECT
#define TRACERECT __noop

//=============================================================================
// if you want to see the TRACE output, uncomment this line:
//#include "XTrace.h"

//=============================================================================
// set resource handle (in case used in DLL)
//=============================================================================
#ifdef _USRDLL
#define AFXMANAGESTATE AfxGetStaticModuleState
#else
#define AFXMANAGESTATE AfxGetAppModuleState
#endif

//=============================================================================
BEGIN_MESSAGE_MAP(CXIcon, CButton)
//=============================================================================
	//{{AFX_MSG_MAP(CXIcon)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//=============================================================================
CXIcon::CXIcon(BOOL bCheckType) :
	m_crText(GetSysColor(COLOR_BTNTEXT)),
	m_hIcon(0),
	m_nXMargin(2),
	m_nYMargin(0),
	m_nIconSpacing(5),
	m_nIconSize(16),
	m_bAutoResize(TRUE),
	m_bDestroyIcon(FALSE),
	m_crBk(GetSysColor(CTLCOLOR_DLG)),
	m_crHover(m_crBk),
	m_crClick(m_crBk),
	m_crDisable(m_crBk),
	m_bMultiLine(FALSE),
	m_bHover(false),
	m_bEnableHover(false),
	m_bCheckBoxType(bCheckType),
	m_bCheckBoxFlag(false),
	m_hNotifyParent(NULL)
//=============================================================================
{
}

//=============================================================================
CXIcon::~CXIcon()
//=============================================================================
{
	if (m_font.GetSafeHandle())
		m_font.DeleteObject();
	if (m_hIcon && m_bDestroyIcon)
		::DestroyIcon(m_hIcon);
	m_hIcon = 0;
}

//=============================================================================
void CXIcon::PreSubclassWindow() 
//=============================================================================
{
	// check if BS_OWNERDRAW
	if ((GetStyle() & BS_TYPEMASK) != BS_OWNERDRAW)
	{
		TRACE(_T("ERROR - style BS_OWNERDRAW must be specified\n"));
		ASSERT(FALSE);
		return;
	}
	
	CFont* pFont = GetSafeFont();
	ASSERT(pFont);

	if (pFont)
	{
		ASSERT(pFont->GetSafeHandle());

		// create the font for this control
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		lf.lfCharSet = DEFAULT_CHARSET;
		if (m_font.GetSafeHandle())
			m_font.DeleteObject();
		VERIFY(m_font.CreateFontIndirect(&lf));
		//m_font.GetLogFont(&lf);
		//TRACE("font=%s\n", lf.lfFaceName);
	}

	CButton::PreSubclassWindow();
}

//=============================================================================
CXIcon& CXIcon::SetMargins(int nXMargin, int nYMargin, BOOL bRedraw /*= TRUE*/)
//=============================================================================
{ 
	m_nXMargin = nXMargin; 
	m_nYMargin = nYMargin; 
	if (m_bAutoResize)
		Resize();
	if (bRedraw) 
		RedrawWindow(); 
	return *this;
}

//=============================================================================
CXIcon& CXIcon::SetIcon(HICON hIcon, 
						UINT nIconSize /*= 16*/, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	m_nIconSize = nIconSize;

	if (m_hIcon && m_bDestroyIcon)
		::DestroyIcon(m_hIcon);

	m_hIcon = hIcon;

	m_bDestroyIcon = FALSE;

	if (m_bAutoResize)
		Resize();

	if (bRedraw)
		RedrawWindow();

	return *this;
}

//=============================================================================
CXIcon& CXIcon::SetIcon(UINT nIconId, 
						UINT nIconSize /*= 16*/, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	AFX_MANAGE_STATE(AFXMANAGESTATE());

	m_nIconSize = nIconSize;

	if (m_hIcon && m_bDestroyIcon)
		::DestroyIcon(m_hIcon);

	if (nIconId)
	{
		m_hIcon = (HICON) ::LoadImage(AfxGetInstanceHandle(), 
									MAKEINTRESOURCE(nIconId),
									IMAGE_ICON, m_nIconSize, m_nIconSize, 0);
		ASSERT(m_hIcon);
	}
	else
	{
		m_hIcon = 0;
	}

	m_bDestroyIcon = TRUE;

	if (m_bAutoResize)
		Resize();

	if (bRedraw)
		RedrawWindow();

	return *this;
}

//=============================================================================
CXIcon& CXIcon::SetIconSpacing(int nIconSpacing, BOOL bRedraw /*= TRUE*/)
//=============================================================================
{ 
	m_nIconSpacing = nIconSpacing;
	if (m_bAutoResize)
		Resize();
	if (bRedraw) 
		RedrawWindow(); 
	return *this; 
}

//=============================================================================
CXIcon& CXIcon::SetWindowText(LPCTSTR lpszText, BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	AFX_MANAGE_STATE(AFXMANAGESTATE());

	CString strText = _T("");

	// lpszText can be pointer to normal text string,
	// or it can be resource id constructed with
	// MAKEINTRESOURCE()

	if (lpszText)
	{
		// is this a text string or an id?
		if (HIWORD(lpszText) == 0)
		{
			// id
			UINT nId = LOWORD((UINT)(UINT_PTR)lpszText);
			VERIFY(strText.LoadString(nId));
		}
		else
		{
			// string
			strText = lpszText;
		}
	}

	CButton::SetWindowText(strText); 

	if (m_bAutoResize)
		Resize();

	if (bRedraw) 
		RedrawWindow();

	return *this;
}

CXIcon & CXIcon::SetColor(COLORREF cr, BOOL bRedraw)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	m_crBk = cr;
	m_crDisable = cr;

	return *this;
}

CXIcon & CXIcon::SetHover(bool bHover, COLORREF crHover, BOOL bRedraw)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	m_bEnableHover = bHover;
	m_crHover = crHover;

	return *this;
}

CXIcon & CXIcon::SetClick(COLORREF crClick, BOOL bRedraw)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	m_crClick = crClick;

	return *this;
}

CXIcon & CXIcon::SetMultiLineText(BOOL bMultiLine, BOOL bRedraw)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	m_bMultiLine = bMultiLine;

	return *this;
}

//=============================================================================
CXIcon& CXIcon::SetTextColor(COLORREF cr, BOOL bRedraw /*= TRUE*/) 
//=============================================================================
{ 
	m_crText = cr; 
	if (m_bAutoResize) 
		Resize();
	if (bRedraw) 
		RedrawWindow(); 
	return *this; 
}

//=============================================================================
CXIcon& CXIcon::SetAutoResize(BOOL bAutoResize, BOOL bRedraw /*= TRUE*/)
//=============================================================================
{ 
	m_bAutoResize = bAutoResize; 
	if (m_bAutoResize) 
		Resize(); 
	if (bRedraw) 
		RedrawWindow(); 
	return *this; 
}

//=============================================================================
void CXIcon::Resize()
//=============================================================================
{
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);

	CDC *pDC = GetDC();

	CFont *pOldFont = 0;

	CString strText = _T("");
	GetWindowText(strText);

	int cx = m_nXMargin;

	if (!strText.IsEmpty())
	{
		pOldFont = pDC->SelectObject(&m_font);
		// get size of text so we can update rect
		CSize size = pDC->GetTextExtent(strText);
		cx += size.cx + 2;
		if (m_hIcon)
			cx += m_nIconSpacing;
	}

	if (m_hIcon)
	{
		cx += m_nIconSize + 2;
	}

	cx += m_nXMargin;

	if (cx > rect.Width())
	{
		rect.right = rect.left + cx;
		MoveWindow(&rect);
	}
	else if (cx < (rect.Width() - 5))
	{
		rect.right = rect.left + cx;
		MoveWindow(&rect);
	}

	if (pOldFont)
		pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
}

//=============================================================================
void CXIcon::DrawItem(LPDRAWITEMSTRUCT lpDIS)
//=============================================================================
{
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);

	CRect rectItem = lpDIS->rcItem;
	//TRACERECT(rectItem);
	CRect rectDraw(rectItem);

	// set up for double buffering
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rectDraw.Width(), rectDraw.Height());
	CBitmap *pOldBitmap = memDC.SelectObject(&bmp);

	UINT state = lpDIS->itemState;
	if ((state & ODS_SELECTED)
		|| (m_bCheckBoxType && GetCheck() == TRUE))
	{
		memDC.FillSolidRect(&rectItem, m_crClick);//Here you can define the required color to appear on the Button.
	}
	else if ((state & ODS_DISABLED))
	{
		memDC.FillSolidRect(&rectItem, m_crDisable);//Here you can define the required color to appear on the Button.
	}
	else if (m_bHover && m_bEnableHover)
	{
		memDC.FillSolidRect(&rectItem, m_crHover);
	}
	else //normal
	{
		memDC.FillSolidRect(&rectItem, m_crBk);
	}

	rectItem.left += m_nXMargin;
	rectItem.top  += m_nYMargin;
		
	switch (m_iconLocation)
	{
	case CXIcon::iconLocation::top:
		DrawIcon(&memDC, rectItem);
		rectItem.top += m_nIconSize;
		DrawText(&memDC, rectItem);
		break;
	case CXIcon::iconLocation::left:
		rectItem.left = DrawIcon(&memDC, rectItem);
		rectItem.left += m_nIconSpacing;
		DrawText(&memDC, rectItem);
		break;
	case CXIcon::iconLocation::right:
		rectItem.left = DrawText(&memDC, rectItem);
		rectItem.left += m_nIconSpacing;
		DrawIcon(&memDC, rectItem);
		break;
	default:
		break;
	}

	// end double buffering
	pDC->BitBlt(0, 0, rectDraw.Width(), rectDraw.Height(),
		&memDC, 0, 0, SRCCOPY);			

	// swap back the original bitmap
	if (pOldBitmap)
		memDC.SelectObject(pOldBitmap);
	if (bmp.GetSafeHandle())
		bmp.DeleteObject();

	memDC.DeleteDC();
}

#if 0  // -----------------------------------------------------------
#define BS_LEFT             0x00000100L
#define BS_RIGHT            0x00000200L
#define BS_CENTER           0x00000300L
#define BS_TOP              0x00000400L
#define BS_BOTTOM           0x00000800L
#define BS_VCENTER          0x00000C00L
#endif // -----------------------------------------------------------

//=============================================================================
int CXIcon::DrawIcon(CDC *pDC, CRect& rect)
//=============================================================================
{
	int rc = 0;

	if (m_hIcon)
	{
		CRect rectIcon(rect);
		//rectIcon.top = rectIcon.top + (rectIcon.Height() - m_nIconSize) / 2;

		int xLeft = rectIcon.left;
		int yTop  = rectIcon.top + (rectIcon.Height() - m_nIconSize) / 2;
		if (m_iconLocation == iconLocation::top)
		{
			xLeft = rectIcon.top + (rectIcon.Width() - m_nIconSize) / 2;
			yTop  = rectIcon.top + (rectIcon.Height() - m_nIconSize*2) / 2;
		}
			

		VERIFY(::DrawIconEx(pDC->m_hDC, xLeft, yTop, m_hIcon,
			m_nIconSize, m_nIconSize, 0, 0, DI_IMAGE | DI_MASK));
		rc = rect.left + m_nIconSize;	// return new left drawing margin
	}

	return rc;
}

//=============================================================================
int CXIcon::DrawText(CDC *pDC, CRect& rect)
//=============================================================================
{
	int rc = 0;

	CString strText = _T("");
	GetWindowText(strText);
	TRACE(_T("strText=<%s>\n"), strText);

	if (!strText.IsEmpty())
	{
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);

		CFont *pOldFont = pDC->SelectObject(&m_font);

		DWORD dwStyle = GetStyle() & 0xC00;		// isolate vertical style bits
		//UINT nFormat = DT_LEFT;					// always left aligned in drawing rect
		UINT nFormat = DT_CENTER;

		// set vertical format from button style settings
		if (dwStyle == BS_TOP)
			nFormat |= DT_TOP;
		else if (dwStyle == BS_BOTTOM)
			nFormat |= DT_BOTTOM;
		else if (dwStyle == BS_VCENTER)				// vertical centering ==> single line only
			nFormat |= DT_VCENTER | DT_SINGLELINE;

		//nFormat |= DT_SINGLELINE;

		CRect rectText(rect);
		CSize size = pDC->GetTextExtent(strText);		// get size of text so we can update rect
		if (m_bMultiLine)
			rectText.top = rectText.top + (rectText.Height() - m_nIconSize) / 2;
		else {
			nFormat |= DT_VCENTER | DT_SINGLELINE;
			rectText.top += m_nYMargin;
		}
			
		//rectText.right = rectText.left + size.cx + 2;
		//TRACERECT(rectText);

		pDC->DrawText(strText, rectText, nFormat);

		rc = rectText.right;

		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}

	return rc;
}

//=============================================================================
BOOL CXIcon::OnEraseBkgnd(CDC* /*pDC*/) 
//=============================================================================
{
	return TRUE;	
	//return CButton::OnEraseBkgnd(pDC);
}

//=============================================================================
int CXIcon::GetFontHeight(int nPointSize)
//=============================================================================
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nHeight = -MulDiv(nPointSize, cyPixelsPerInch, 72);

	return nHeight;
}

//=============================================================================
int CXIcon::GetFontPointSize(int nHeight)
//=============================================================================
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nPointSize = MulDiv(nHeight, 72, cyPixelsPerInch);
	if (nPointSize < 0)
		nPointSize = -nPointSize;

	return nPointSize;
}

//=============================================================================
CFont * CXIcon::GetSafeFont()
//=============================================================================
{
	// get current font
	CFont *pFont = CWnd::GetFont();

	if (pFont == 0)
	{
		// try to get parent font
		CWnd *pParent = GetParent();
		if (pParent && IsWindow(pParent->m_hWnd))
			pFont = pParent->GetFont();

		if (pFont == 0)
		{
			// no font, so get a system font
			HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
			if (hFont == 0)
				hFont = (HFONT)::GetStockObject(SYSTEM_FONT);
			if (hFont == 0)
				hFont = (HFONT)::GetStockObject(ANSI_VAR_FONT);
			if (hFont)
				pFont = CFont::FromHandle(hFont);
		}
	}

	return pFont;
}

//=============================================================================
CXIcon& CXIcon::SetBold(BOOL bBold, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	m_font.GetLogFont(&lf);

	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

	return SetFont(&lf, bRedraw);
}


//=============================================================================
BOOL CXIcon::GetBold()
//=============================================================================
{
	BOOL rc = FALSE;

	CFont *pFont = GetFont(); 
	if (pFont)
	{
		LOGFONT lf; 
		pFont->GetLogFont(&lf);
		rc = lf.lfWeight == FW_BOLD;
	}
	return rc;
}

//=============================================================================
CString CXIcon::GetFontFaceName()
//=============================================================================
{
	CString strFaceName = _T("MS Sans Serif");

	CFont *pFont = GetFont(); 
	if (pFont)
	{
		LOGFONT lf; 
		pFont->GetLogFont(&lf);
		strFaceName = lf.lfFaceName;
	}

	return strFaceName;
}

//=============================================================================
int CXIcon::GetFontPointSize()
//=============================================================================
{
	int rc = 8;

	CFont *pFont = GetFont(); 
	if (pFont)
	{
		LOGFONT lf; 
		pFont->GetLogFont(&lf);
		rc = GetFontPointSize(lf.lfHeight);
	}
	return rc;
}

//=============================================================================
CXIcon& CXIcon::SetFont(LPCTSTR lpszFaceName, 
						int nPointSize, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	// null face name is ok - we will use current font

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	if ((lpszFaceName == NULL) || (lpszFaceName[0] == _T('\0')))
		m_font.GetLogFont(&lf);
	else
		_tcsncpy(lf.lfFaceName, lpszFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR)-1);

	lf.lfHeight = GetFontHeight(nPointSize);

	return SetFont(&lf, bRedraw);
}

//=============================================================================
CXIcon& CXIcon::SetFont(CFont *pFont, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	ASSERT(pFont);

	if (!pFont)
		pFont = GetSafeFont();

	if (pFont)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));

		pFont->GetLogFont(&lf);

		SetFont(&lf, bRedraw);
	}

	return *this;
}

//=============================================================================
BOOL CXIcon::GetFont(LOGFONT *pLF)
//=============================================================================
{
	BOOL rc = FALSE;

	ASSERT(pLF);

	if (pLF)
	{
		memset(pLF, 0, sizeof(LOGFONT));
		CFont *pFont = GetFont(); 
		if (pFont)
		{
			pFont->GetLogFont(pLF);
			rc = TRUE;
		}
	}
	return rc;
}

//=============================================================================
CXIcon& CXIcon::SetFont(LOGFONT *pLogFont, 
						BOOL bRedraw /*= TRUE*/)
//=============================================================================
{
	TRACE(_T("in CXIcon::SetFont =================\n"));
	ASSERT(pLogFont);

	if (pLogFont)
	{
		if (m_font.GetSafeHandle())
			m_font.DeleteObject();
		VERIFY(m_font.CreateFontIndirect(pLogFont));

		if (m_bAutoResize)
			Resize();

		if (bRedraw)
			RedrawWindow();
	}

	return *this;
}



void CXIcon::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bHover = true;
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
}


void CXIcon::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bHover == FALSE)
	{
		TRACKMOUSEEVENT tme;
		ZeroMemory(&tme, sizeof(TRACKMOUSEEVENT));
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bHover = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}


void CXIcon::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bHover = false;
	Invalidate();
	CButton::OnMouseLeave();
}

CXIcon& CXIcon::SetIconLocation(iconLocation loc)
{
	m_iconLocation = loc;

	return *this;
}

LRESULT CXIcon::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_bCheckBoxType)
	{
		if (message == BM_SETCHECK)
		{
			m_bCheckBoxFlag = (int)wParam;
			if (m_hNotifyParent) ::PostMessage(m_hNotifyParent, 1010, GetDlgCtrlID(), m_bCheckBoxFlag);
		}
		else if (message == BM_GETCHECK)
		{
			return m_bCheckBoxFlag;
		}
	}
	return CButton::WindowProc(message, wParam, lParam);
}

void CXIcon::SetCheck(int nCheck)
{
	m_bCheckBoxFlag = nCheck;
	if (m_hNotifyParent && m_bCheckBoxFlag) ::PostMessage(m_hNotifyParent, 1010, GetDlgCtrlID(), 1);
	Invalidate(TRUE);
}

void CXIcon::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bCheckBoxType)
	{
		CRect rect;
		GetClientRect(&rect);
		if (rect.left <= point.x && rect.right >= point.x
			&& rect.top <= point.y && rect.bottom >= point.y)
		{
			m_bCheckBoxFlag = !m_bCheckBoxFlag;
			if (m_hNotifyParent)
			{
				CIconRadio* pRadio = (CIconRadio*)CWnd::FromHandle(m_hNotifyParent);
				if (pRadio->IsCheckAllFalse() == TRUE)
				{
					m_bCheckBoxFlag = !m_bCheckBoxFlag;
				}
				::PostMessage(m_hNotifyParent, 1010, GetDlgCtrlID(), m_bCheckBoxFlag);
			}
			Invalidate(TRUE);
		}

	}


	CButton::OnLButtonUp(nFlags, point);
}