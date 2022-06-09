#include "StdAfx.h"
#include "akStaticText.h"
#include "akMemDC.h"

BEGIN_MESSAGE_MAP(CakStaticText, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


CakStaticText::CakStaticText(void)
{
	m_nFormat = DT_SINGLELINE | DT_LEFT | DT_TOP;
	m_backcolor = RGB(236,233,216);
	m_textcolor = RGB(0,0,0);

	m_brush.CreateSolidBrush(m_backcolor);

	m_bColorUpdate = false;
	m_bFastMode = false;
	m_nDrawOutline = false;
}

CakStaticText::~CakStaticText(void)
{
 
}

void CakStaticText::SetFastMode()
{
	//setFont("Microsoft Sans Serif");
	//setFontSize(14);
	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf); 
	
	ModifyStyle(NULL, WS_CLIPSIBLINGS | SS_OWNERDRAW ,		0);
	Invalidate(false);

	m_bFastMode = true;
}

void CakStaticText::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_bColorUpdate == true)
	{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(m_backcolor);
		m_bColorUpdate = false;
	}

	CRect rect;
	GetClientRect(&rect);

	CakMemDC memdc(CDC::FromHandle(lpDrawItemStruct->hDC)); 
	
	CFont* pOldFont = (CFont*)memdc.SelectObject(m_font);
	memdc.SetTextColor(m_textcolor);
	memdc.SetBkColor(m_backcolor);

	CBrush brushback(m_backcolor);

	memdc.SelectObject(brushback);
	memdc.FillRect(rect, &brushback);

	
	
	if(m_nDrawOutline == 0)//draw outline
	{
		CRect rectDraw = rect;
		memdc.Draw3dRect(&rectDraw, m_clrOutline, m_clrOutline); 
		rectDraw.DeflateRect(1,1,1,1);
		memdc.Draw3dRect(&rectDraw,m_clrOutline2, m_clrOutline2); 
	}

	

	CString wintext;
	GetWindowText(wintext);

	
	memdc.SetBkMode(TRANSPARENT);
	memdc.DrawText(wintext,CRect(0,0,rect.right, rect.bottom), m_nFormat);
	
	
	//memdc.SelectObject(pOldFont);
}


BOOL CakStaticText::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return true;
	return CStatic::OnEraseBkgnd(pDC);
}


HBRUSH CakStaticText::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if(m_bFastMode == true)
	{	
		return NULL;
	}
	pDC->SetTextColor(m_textcolor);
	pDC->SetBkColor(m_backcolor);


	if(m_bColorUpdate == true)
	{
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(m_backcolor);
		

		m_bColorUpdate = false;
	}


	return (HBRUSH)m_brush;
}



void CakStaticText::setColor(COLORREF textcolor,COLORREF backcolor)
{
	m_backcolor = backcolor;
	m_textcolor = textcolor;

	m_bColorUpdate = true;
	//m_brush.CreateSolidBrush(m_backcolor);
	Invalidate(FALSE);
}

void CakStaticText::setTextColor(COLORREF textcolor)
{
	if(m_textcolor != textcolor)
	{
		m_bColorUpdate = true;
	}
	m_textcolor = textcolor;

	Invalidate(FALSE);
}

void CakStaticText::setBkColor(COLORREF backcolor)
{
	if(m_backcolor != backcolor)
	{
        m_bColorUpdate = true;
	}

	m_backcolor = backcolor;


	Invalidate(FALSE);
	
}

void CakStaticText::setOutLine( BOOL bFlag, COLORREF color )
{
	if(m_nDrawOutline != bFlag)
	{
		m_nDrawOutline = bFlag;
	}

	m_clrOutline = color;
	m_clrOutline2 = RGB(GetRValue(color)/2, GetGValue(color)/2, GetBValue(color)/2);

	Invalidate(FALSE);
}

void CakStaticText::setFont(char* fontName)
{
	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);

	strcpy(lf.lfFaceName, fontName); // font setting 

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf); 

	SetFont(&m_font);
}
void CakStaticText::setFontSize(int size)
{
	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);

	lf.lfHeight = -size;

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf); 

	SetFont(&m_font);
}

void CakStaticText::SetAlign(UINT format)
{
	m_nFormat = format;
	
}
