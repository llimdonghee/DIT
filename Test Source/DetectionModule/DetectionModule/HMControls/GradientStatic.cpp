// GradientStatic.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GradientStatic.h"

// CGradientStatic
#define ID_TIMER_ANIMATION_CONTROL 1
IMPLEMENT_DYNAMIC(CGradientStatic, CStatic)

CGradientStatic::CGradientStatic()
{
	m_bVertical = FALSE;
	m_iLeftSpacing = 10;
	clLeft = GetSysColor(COLOR_ACTIVECAPTION);
	clRight = GetSysColor(COLOR_BTNFACE);
	clText = GetSysColor(COLOR_CAPTIONTEXT);
	clOut1 = RGB(160, 160, 160);
	clOut2 = RGB(0, 255, 0);
	
	m_iAlign = 0;

	hinst_msimg32 = LoadLibrary( _T("msimg32.dll") );

	m_bCanDoGradientFill = FALSE;
		
	if(hinst_msimg32)
	{
		m_bCanDoGradientFill = TRUE;		
		dllfunc_GradientFill = ((LPFNDLLFUNC1) GetProcAddress( hinst_msimg32, "GradientFill" ));
	}
	
	m_sTEXT = _T("");
	m_bDrawOutLine = FALSE;
	m_nAniColorCount = 1;
	m_enAnimation = EN_ANI_NONE;
	SetDrawColor(clLeft, clRight);
}

CGradientStatic::~CGradientStatic()
{
	FreeLibrary( hinst_msimg32 );
	if(m_font.GetSafeHandle() != NULL)
		m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CGradientStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGradientStatic 메시지 처리기입니다
//this function will be used only if msimg32.dll library is not available
void CGradientStatic::DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight, BOOL a_bVertical)
{
	CRect stepR;					// rectangle for color's band
	COLORREF color;				// color for the bands
	float fStep;

	if(a_bVertical)
		fStep = ((float)r.Height())/255.0f;	
	else
		fStep = ((float)r.Width())/255.0f;	// width of color's band

	for (int iOnBand = 0; iOnBand < 255; iOnBand++) 
	{
		// set current band
		if(a_bVertical)
		{
			SetRect(&stepR,
				r.left, 
				r.top+(int)(iOnBand * fStep),
				r.right, 
				r.top+(int)((iOnBand+1)* fStep));	
		}
		else
		{
			SetRect(&stepR,
				r.left+(int)(iOnBand * fStep), 
				r.top,
				r.left+(int)((iOnBand+1)* fStep), 
				r.bottom);	
		}

		// set current color
		color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/255.0f+GetRValue(cLeft),
			(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/255.0f+GetGValue(cLeft),
			(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/255.0f+GetBValue(cLeft));
		// fill current band
		pDC->FillSolidRect(stepR,color);
	}
}

void CGradientStatic::DrawGradRect(CDC *pDC, CRect r, BOOL a_bVertical, COLORREF *cl)
{
	CRect stepR;					// rectangle for color's band
	COLORREF color;				// color for the bands
	float fStep;

	if(a_bVertical)
		fStep = ((float)r.Height())/255.0f;	
	else
		fStep = ((float)r.Width())/255.0f;	// width of color's band

	if(m_enAnimation == EN_ANI_LAMP)
	{
		// set current color
		color = cl[0];

		// fill current band
		pDC->FillSolidRect(r,color);
	}
	else
	{
		for (int iOnBand = 0; iOnBand < 256; iOnBand++) 
		{
			// set current band
			if(a_bVertical)
			{
				SetRect(&stepR,
					r.left, 
					r.top+(int)(iOnBand * fStep),
					r.right, 
					r.top+(int)((iOnBand+1)* fStep));	
			}
			else
			{
				SetRect(&stepR,
					r.left+(int)(iOnBand * fStep), 
					r.top,
					r.left+(int)((iOnBand+1)* fStep), 
					r.bottom);	
			}

			// set current color
			color = cl[iOnBand];

			// fill current band
			pDC->FillSolidRect(stepR,color);
		}
	}
	
}

void CGradientStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
	CDoubleBuff memdc(&dc, &rect);
	
	CRect rtOutline(rect);
	
	if(m_bDrawOutLine)
	{
		CPen OutLinePen, *pOldPen;
		OutLinePen.CreatePen(PS_SOLID, 1, clOut1);
		pOldPen = (CPen*)memdc.SelectObject(&OutLinePen);
		memdc.Rectangle(rtOutline);
		memdc.SelectObject(pOldPen);
		OutLinePen.DeleteObject();

		rtOutline.DeflateRect(1, 1);
		OutLinePen.CreatePen(PS_SOLID, 1, clOut2);
		pOldPen = (CPen*)memdc.SelectObject(&OutLinePen);
		memdc.Rectangle(rtOutline);
		memdc.SelectObject(pOldPen);
		OutLinePen.DeleteObject();	
		rtOutline.DeflateRect(1, 1);
	}	

	if(m_enAnimation == EN_ANI_NONE && m_bCanDoGradientFill) //msimg32.dll library is loaded
	{	
		TRIVERTEX rcVertex[2];
		rcVertex[0].x=rtOutline.left;
		rcVertex[0].y=rtOutline.top;
		rcVertex[0].Red=GetRValue(clLeft)<<8;	// color values from 0x0000 to 0xff00 !!!!
		rcVertex[0].Green=GetGValue(clLeft)<<8;
		rcVertex[0].Blue=GetBValue(clLeft)<<8;
		rcVertex[0].Alpha=0x0000;
		rcVertex[1].x=rtOutline.right; 
		rcVertex[1].y=rtOutline.bottom;
		rcVertex[1].Red=GetRValue(clRight)<<8;
		rcVertex[1].Green=GetGValue(clRight)<<8;
		rcVertex[1].Blue=GetBValue(clRight)<<8;
		rcVertex[1].Alpha=0;
		
		GRADIENT_RECT rect;
		rect.UpperLeft=0;
		rect.LowerRight=1;
		
		// fill the area 
		dllfunc_GradientFill( memdc,rcVertex,2,&rect,1, m_bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
		
	}
	else
	{
		//msimg32.dll is not available. Let's use our own code to display gradient background.
		//This code is very simple and produces worse gradient that function from the library - but works!
		DrawGradRect(&memdc,rtOutline,m_bVertical, clDrawColor);
	}
	
	//let's set color defined by user
	::SetTextColor(memdc,clText);

	HFONT hfontOld;

	/*
	VERIFY(font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BLACK,					// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("MS Sans Serif")));                 // lpszFacename

		*/

	hfontOld = (HFONT)SelectObject(memdc.m_hDC, m_font.m_hObject);

	::SetBkMode(memdc, TRANSPARENT);
	GetClientRect(&rect);

	if(m_iAlign == 1) // center
		::DrawText(memdc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	else if(m_iAlign == 0) // left
	{
		rect.left+=m_iLeftSpacing;
		::DrawText(memdc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}
	else //right
	{
		rect.right-=m_iLeftSpacing;
		::DrawText(memdc, m_sTEXT, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	}

	::SelectObject(memdc.m_hDC, hfontOld);
}

void CGradientStatic::SetReverseGradient()
{
	COLORREF cTemp = clLeft;
	clLeft = clRight;
	clRight = cTemp;
}

void CGradientStatic::SetWindowText(LPCTSTR a_lpstr)
{
//	CStatic::SetWindowText(a_lpstr);

	m_sTEXT = a_lpstr;

	Invalidate(FALSE);
}

void CGradientStatic::SetDrawColor(long cLeft, long cRight)
{
	for (int iOnBand = 0; iOnBand < 256; iOnBand++)
	{
		// set current color
		clDrawColor[iOnBand] = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/255.0f+GetRValue(cLeft),
			(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/255.0f+GetGValue(cLeft),
			(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/255.0f+GetBValue(cLeft));		
	}
	for (int iOnBand = 256; iOnBand < 510; iOnBand++)
	{
		clDrawColor[iOnBand] = clDrawColor[510 - iOnBand];
	}
}

void CGradientStatic::MoveDrawColor()
{
	COLORREF temp[510];
	memcpy(&temp[m_nAniColorCount], clDrawColor, sizeof(COLORREF) * (510 - m_nAniColorCount));
	memcpy(temp, &clDrawColor[510 - m_nAniColorCount], sizeof(COLORREF) * m_nAniColorCount);
	memcpy(clDrawColor, temp, sizeof(COLORREF) * 510);	
}

void CGradientStatic::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_ANIMATION_CONTROL:
		{
			if(m_enAnimation == EN_ANI_NONE)
			{
				KillTimer(ID_TIMER_ANIMATION_CONTROL);
				SetDrawColor(clLeft, clRight);
				return;
			}

			MoveDrawColor();
			Invalidate(FALSE);
		}
		break;
	}

	__super::OnTimer(nIDEvent);
}

void CGradientStatic::SetAnimationMode(int nMode, int nTime, int nIndex)
{
	m_enAnimation = (EN_ANI_MODE)nMode;
	m_nAniColorCount = nIndex;
	SetDrawColor(clLeft, clRight);
	if(m_enAnimation == EN_ANI_NONE)
	{
		KillTimer(ID_TIMER_ANIMATION_CONTROL);
	}
	else
	{
		SetTimer(ID_TIMER_ANIMATION_CONTROL, nTime, NULL);
	}
}

BOOL CGradientStatic::SetControlFont(int nHeight, int nWidth, int nEscapement,
									 int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
									 BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
									 BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
									 LPCTSTR lpszFacename)
{
	if(m_font.GetSafeHandle() != NULL)
		m_font.DeleteObject();

	return m_font.CreateFont(nHeight, nWidth, nEscapement,nOrientation, nWeight, bItalic, bUnderline, cStrikeOut, nCharSet, nOutPrecision, nClipPrecision, nQuality, nPitchAndFamily, lpszFacename);
}