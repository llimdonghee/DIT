#include "StdAfx.h"
#include "akMemDC.h"
#include "akGroupDraw.h"


BEGIN_MESSAGE_MAP(CakGroupDraw, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

typedef struct {
	double r;       // percent
	double g;       // percent
	double b;       // percent
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // percent
	double v;       // percent

} hsv;

hsv rgb2hsv(rgb in)
{
	hsv        out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min  : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max  : in.b;

	out.v = max;                                // v
	delta = max - min;
	if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	} else {
		// if max is 0, then r = g = b = 0
		// s = 0, v is undefined
		out.s = 0.0;
		out.h = 0.0;                            // its now undefined
		return out;
	}
	if( in.r >= max )                           // > is bogus, just keeps compilor happy
		if(delta == 0){
			out.h = 0.0;
		}
		else{
			out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
		}
	else
		if( in.g >= max )
			out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if( out.h < 0.0 )
		out.h += 360.0;

	return out;
}
rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb        out;

	if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) {
		 case 0:
			 out.r = in.v;
			 out.g = t;
			 out.b = p;
			 break;
		 case 1:
			 out.r = q;
			 out.g = in.v;
			 out.b = p;
			 break;
		 case 2:
			 out.r = p;
			 out.g = in.v;
			 out.b = t;
			 break;

		 case 3:
			 out.r = p;
			 out.g = q;
			 out.b = in.v;
			 break;
		 case 4:
			 out.r = t;
			 out.g = p;
			 out.b = in.v;
			 break;
		 case 5:
		 default:
			 out.r = in.v;
			 out.g = p;
			 out.b = q;
			 break;
	}
	return out;    

}

CakGroupDraw::CakGroupDraw(void)
{
	m_Option.nDrawStyle = -1;

}

CakGroupDraw::~CakGroupDraw(void)
{
	clear();
}


void CakGroupDraw::clear()
{

}



BOOL CakGroupDraw::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//if(m_bImageLoad == true) return true;
	//return CStatic::OnEraseBkgnd(pDC);
}




void CakGroupDraw::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}

void CakGroupDraw::OnPaint()
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

	CRect rectClient;
	GetClientRect(&rectClient);
	
	memDC->FillSolidRect(rectClient, RGB(255,255,255));

	if(m_Option.nDrawStyle == 0)
	{
		drawGroup(pMemDC, rectClient, m_Option);
	}
	else
	{
		memDC->FillSolidRect(rectClient, RGB(255,255,255));
	}
	drawText(pMemDC, rectClient, m_Option);
	
}

BOOL CakGroupDraw::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID))
	{
		
		return true;

	}

	return false;
}

void CakGroupDraw::setFont( char* fontName, int fontSize )
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

BOOL CakGroupDraw::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_LBUTTONDOWN
		||pMsg->message == WM_LBUTTONDBLCLK)
	{
		return TRUE;
	}
	return CStatic::PreTranslateMessage(pMsg);
}

void CakGroupDraw::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_Option.nDrawStyle >= 0)
	{
		ModifyStyle(NULL, WS_CLIPSIBLINGS | SS_OWNERDRAW ,	0);
		SetWindowPos(&wndBottom, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	}
	
	CRect rect;
	GetClientRect(&rect);
	HRGN  hRgn = CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), 15, 15);
	SetWindowRgn( hRgn, FALSE);

	CRgn rgnRect1;
	CRgn rgnHole;
	CRgn rgnTotal;
	rgnRect1.CreateRectRgn(0,0,rect.Width(),rect.Height());
	rgnHole.CreateRectRgn(1,m_Option.nTitleHeight+1,rect.Width()-1,rect.Height()-1);
	rgnTotal.CreateRectRgn(0,0,rect.Width(),rect.Height());
	rgnTotal.CombineRgn(&rgnRect1,&rgnHole,RGN_XOR);
	SetWindowRgn( rgnTotal, FALSE);


	CStatic::PreSubclassWindow();
}

void CakGroupDraw::setGroupDrawOption( _GroupDrawOption Option )
{
	if(m_Option.nDrawStyle == -1 && Option.nDrawStyle >= 0)
	{
		ModifyStyle(NULL, WS_CLIPSIBLINGS | SS_OWNERDRAW ,	0);
		SetWindowPos(&wndBottom, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	}

	m_Option = Option;
}

void CakGroupDraw::drawGroup( CDC* pDC, CRect rect, _GroupDrawOption option )
{
	CRect rectdraw;
	int nTitleHeight = option.nTitleHeight;

	pDC->FillSolidRect(rect, option.clrTransparency);
	
	pDC->FillSolidRect(rect, option.clrBack);

	if(nTitleHeight>0)//title
	{
		
		rectdraw = rect;
		rectdraw.bottom = nTitleHeight;
		pDC->FillSolidRect(rectdraw, option.clrTitle);

		rectdraw = rect;
		rectdraw.bottom = rectdraw.top+nTitleHeight/2;
		fillRectGradientT2B(pDC, rectdraw, getBrightColor(option.clrTitle, 0.25), getBrightColor(option.clrTitle, 0.2));
		
		rectdraw = rect;
		rectdraw.bottom = rectdraw.top+nTitleHeight+1;
		rectdraw.top = rectdraw.top+nTitleHeight/2;
		
		fillRectGradientT2B(pDC, rectdraw, option.clrTitle, getBrightColor(option.clrTitle, 0.1));

	}
	if(nTitleHeight>0)//마크
	{
		COLORREF clrMark = option.clrMark;
		CPen pen, *pOldpen;
		pen.CreatePen(PS_SOLID, 2, clrMark);
		pOldpen = pDC->SelectObject(&pen);
		pDC->MoveTo(7, rect.top+5);
		pDC->LineTo(7, rect.top+nTitleHeight-5);
		pDC->MoveTo(10, rect.top+5);
		pDC->LineTo(10, rect.top+nTitleHeight-5);
		pDC->MoveTo(13, rect.top+5);
		pDC->LineTo(13, rect.top+nTitleHeight-5);
	}

	pDC->Draw3dRect(rect, option.clrOutline, option.clrOutline );
	
}

void CakGroupDraw::drawText( CDC* pDC, CRect rect, _GroupDrawOption option )
{
	CString strTitle;	GetWindowText(strTitle);
	if(strTitle.IsEmpty() == FALSE)
	{
		UINT oldAlign = pDC->SetTextAlign(TA_TOP | TA_LEFT);
		CFont *pOldFont;

		pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(option.clrFontNormal);

		pDC->SetBkMode(TRANSPARENT);

		pDC->TextOut(option.nTextPosX, option.nTextPosY, strTitle);


		pDC->SetTextAlign(oldAlign);
		pDC->SelectObject(pOldFont);
	}
}

COLORREF CakGroupDraw::getBrightColor( COLORREF color, double dBright )
{
	rgb colorRGB;
	hsv colorHSV;
	colorRGB.r = GetRValue(color)/255.0;
	colorRGB.g = GetGValue(color)/255.0;
	colorRGB.b = GetBValue(color)/255.0;
	unsigned char a = GetBValue(color);


 	colorHSV = rgb2hsv(colorRGB);
 	colorHSV.v = colorHSV.v+colorHSV.v*dBright;
 	if(colorHSV.v>1) colorHSV.v = 1;
 	if(colorHSV.v<0) colorHSV.v = 0;

	colorHSV.s = colorHSV.s-(colorHSV.s*dBright);
	if(colorHSV.s>1) colorHSV.s = 1;
	if(colorHSV.s<0) colorHSV.s = 0;

	rgb colorTemp = hsv2rgb(colorHSV);

	return RGB(colorTemp.r*255, colorTemp.g*255, colorTemp.b*255);
}

void CakGroupDraw::fillRectGradientT2B( CDC* pDC, CRect rect, COLORREF colorTop, COLORREF colorBottom )
{
	TRIVERTEX vert[2];
	GRADIENT_RECT rectGradient;

	// 그라데이션의 시작좌표를 명시한다.
	vert[0].x      = rect.left;
	vert[0].y      = rect.top;

	// 그라데이션의 시작색상을 명시한다.
	vert[0].Red    = GetRValue(colorTop)<<8;
	vert[0].Green  = GetGValue(colorTop)<<8;
	vert[0].Blue   = GetBValue(colorTop)<<8;
	vert[0].Alpha  = 0x0000;

	// 그라데이션의 끝좌표를 명시한다.
	vert[1].x      = rect.right;
	vert[1].y      = rect.bottom;

	// 그라데이션의 끝색상를 명시한다.
	vert[1].Red    = GetRValue(colorBottom)<<8;
	vert[1].Green  = GetGValue(colorBottom)<<8;
	vert[1].Blue   = GetBValue(colorBottom)<<8;
	vert[1].Alpha  = 0x0000;

	rectGradient.UpperLeft  = 0;
	rectGradient.LowerRight = 1;

	// 가로 방향의 시작지점부터 중앙지점까지 그라데이션 효과를 준다.
	pDC->GradientFill(vert, 2, &rectGradient, 1, GRADIENT_FILL_RECT_V);
}

void CakGroupDraw::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	//if(this->IsWindow())
	{
		CRect rect;
		//GetClientRect(&rect);
		rect.SetRect(0,0,cx,cy);
		HRGN  hRgn = CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), 15, 15);
		SetWindowRgn( hRgn, FALSE);

		CRgn rgnRect1;
		CRgn rgnHole;
		CRgn rgnTotal;
		rgnRect1.CreateRectRgn(0,0,rect.Width(),rect.Height());
		rgnHole.CreateRectRgn(1,m_Option.nTitleHeight+1,rect.Width()-1,rect.Height()-1);
		rgnTotal.CreateRectRgn(0,0,rect.Width(),rect.Height());
		rgnTotal.CombineRgn(&rgnRect1,&rgnHole,RGN_XOR);
		SetWindowRgn( rgnTotal, FALSE);
	}
}
