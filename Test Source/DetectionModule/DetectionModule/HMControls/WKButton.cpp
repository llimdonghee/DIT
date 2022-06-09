// WKButton.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "WKButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWKButton

CWKButton::CWKButton()
{
	m_rgbColor1 = RGB(255,255,255);
	m_rgbColor2 = RGB(0,0,0);
	m_rgbText = RGB(0,0,0);
	m_dStyle = WK_BUTTON_COLOR_ONE;
	m_dShape = WK_BUTTON_SHPAE_RECTANGLE;
	m_dButtonStatus = WK_BUTTON_STATUS_MOUSELEAVE;
	m_bValueChange = FALSE;

/*	m_logFont.lfHeight = 20;
	m_logFont.lfWidth = 0; 
	m_logFont.lfEscapement = GM_ADVANCED; 
	m_logFont.lfOrientation = 0; 
	m_logFont.lfWeight = FW_DONTCARE; 
	m_logFont.lfItalic = 0; 
	m_logFont.lfUnderline = 0; 
	m_logFont.lfStrikeOut = 0; 
	m_logFont.lfCharSet = DEFAULT_CHARSET; 
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS; 
	m_logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS; 
	m_logFont.lfQuality = DEFAULT_QUALITY; 
	m_logFont.lfPitchAndFamily = FF_DONTCARE;
	strcpy(m_logFont.lfFaceName,"굴림"); */
	
	m_Font.CreatePointFont(100,"굴림");
	m_strCaption = "Button";
}

CWKButton::~CWKButton()
{
	
}


BEGIN_MESSAGE_MAP(CWKButton, CButton)
	//{{AFX_MSG_MAP(CWKButton)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWKButton message handlers
BOOL CWKButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_rtButton = rect;
	m_nWidth = rect.right - rect.left;
	m_nHeight = rect.bottom - rect.top;
	SetButtonText(lpszCaption);
	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}

BOOL CWKButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_rtButton = rect;
	m_nWidth = rect.right - rect.left;
	m_nHeight = rect.bottom - rect.top;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CWKButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	
}

void CWKButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CWKButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW | BS_AUTOCHECKBOX);
	
	CButton::PreSubclassWindow();
}


void CWKButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if(m_bValueChange)
	{
		CreateButtonImage();
		m_bValueChange = FALSE;
	}

	CDC MemDC;
	CRect rt;
	CBitmap* pOldBitmap = NULL;
	GetClientRect(&rt);
	MemDC.CreateCompatibleDC(((CWnd*)(dc.GetWindow()))->GetDC());

	switch(m_dButtonStatus)
	{
		case WK_BUTTON_STATUS_MOUSEOVER:
			pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_bmpImage2);
			break;
		case WK_BUTTON_STATUS_MOUSELEAVE:
			pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_bmpImage1);
			break;
		case WK_BUTTON_STATUS_MOUSECLICK:
			pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_bmpImage3);
			break;
	}
	dc.BitBlt(0,0,rt.right,rt.bottom,&MemDC,0,0,SRCCOPY);
	if (pOldBitmap)
		dc.SelectObject(pOldBitmap);
	// Do not call CButton::OnPaint() for painting messages
}

int CWKButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CPaintDC dc(this);

	// 버튼의 DC와 호환 되는 비트맵 생성
	m_bmpImage1.CreateCompatibleBitmap(((CWnd*)(dc.GetWindow()))->GetDC(),m_nWidth,m_nHeight);	// 평상시 이미지
	m_bmpImage2.CreateCompatibleBitmap(((CWnd*)(dc.GetWindow()))->GetDC(),m_nWidth,m_nHeight);	// 마우스 오버시 이미지
	m_bmpImage3.CreateCompatibleBitmap(((CWnd*)(dc.GetWindow()))->GetDC(),m_nWidth,m_nHeight);	// 클릭시 이미지

	// 생성된 비트맵에 이미지 작업
	CreateButtonImage();	

	return 0;
}

BOOL CWKButton::SetButtonStyle(DWORD dShape, DWORD dStyle, COLORREF rgbColor1, COLORREF rgbColor2)
{
	m_dShape = dShape;			// 모양 저장
	m_dStyle = dStyle;			// 컬러 스타일 저장
	m_rgbColor1 = rgbColor1;	// 첫번째 색 저장
	m_rgbColor2 = rgbColor2;	// 두번째 색 저장

	m_bValueChange = TRUE;

	return TRUE;
}

BOOL CWKButton::SetButtonStyle(DWORD dShape, COLORREF rgbColor1)
{
	return SetButtonStyle(dShape, WK_BUTTON_COLOR_ONE, rgbColor1, rgbColor1);
}

BOOL CWKButton::SetButtonText(CString strCaption, CFont* font, COLORREF rgbText)
{
	// 버튼 캡션 저장
	m_strCaption = strCaption;

	// 폰트 저장
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);

	// 글자색 저장
	m_rgbText = rgbText;

	m_bValueChange = TRUE;

	if(this->m_hWnd)
		Invalidate(FALSE);
	return TRUE;
}

BOOL CWKButton::SetButtonText(CString strCaption)
{
	SetButtonText(strCaption, &m_Font, m_rgbText);
	return TRUE;
}

BOOL CWKButton::SetButtonText(CString strCaption, COLORREF rgbText)
{
	SetButtonText(strCaption, &m_Font, rgbText);
	return TRUE;
}

BOOL CWKButton::CreateButtonImage()
{
	switch(m_dShape)	// 버튼 모양
	{
		case WK_BUTTON_SHPAE_RECTANGLE:	// 사각형
			DrawFormRectangle();
			break;
		case WK_BUTTON_SHAPE_CIRCLE:	// 원
			DrawFormRectangle();
			break;
		case WK_BUTTON_SHAPE_ROUNDRECT:	// 둥근 사각형
			DrawFormRectangle();
			break;
	}
	return TRUE;
}

BOOL CWKButton::DrawFormRectangle()
{
	switch(m_dStyle)
	{
		case WK_BUTTON_COLOR_ONE:
		case WK_BUTTON_COLOR_TWO:
		default:
			// 평상시 이미지 생성
			DrawGradation(&m_bmpImage1, m_rgbColor1, m_rgbColor2);
			DrawBorder(&m_bmpImage1, m_rgbColor2, m_rgbColor1);
			DrawText(&m_bmpImage1,WK_BUTTON_STATUS_MOUSELEAVE);
			// 마우스 오버
			DrawGradation(&m_bmpImage2, LightValueControl(m_rgbColor1,+50), LightValueControl(m_rgbColor2,+50));
			DrawBorder(&m_bmpImage2, LightValueControl(m_rgbColor2,+50), LightValueControl(m_rgbColor1,+50));
			DrawText(&m_bmpImage2,WK_BUTTON_STATUS_MOUSEOVER);
			// 클릭시
			DrawGradation(&m_bmpImage3, LightValueControl(m_rgbColor1,+20), LightValueControl(m_rgbColor2,+20));
			DrawBorder(&m_bmpImage3, LightValueControl(m_rgbColor1,-50), LightValueControl(m_rgbColor2,-50), 1);
			DrawText(&m_bmpImage3,WK_BUTTON_STATUS_MOUSECLICK);
			break;
	}
	return TRUE;
}

BOOL CWKButton::DrawFormCircle()
{
	return TRUE;
}

BOOL CWKButton::DrawFormRoundRect()
{
	return TRUE;
}


void CWKButton::DrawGradation(CBitmap* bmpImage, COLORREF color1, COLORREF color2)
{
	CPaintDC dc(this);
	CDC BufferDC;
	CBitmap* pOldBitmap;
	BufferDC.CreateCompatibleDC(((CWnd*)(dc.GetWindow()))->GetDC());
	
	pOldBitmap = (CBitmap*)BufferDC.SelectObject(bmpImage);

	int C1Red, C1Green, C1Blue, C2Red, C2Green, C2Blue, x, y;
	
	// color1의 색상별 값을 분류
	C1Red	= GetRValue(color1);
	C1Green	= GetGValue(color1);
	C1Blue	= GetBValue(color1);

	// color2의 색상별 값을 분류
	C2Red	= GetRValue(color2);
	C2Green	= GetGValue(color2);
	C2Blue	= GetBValue(color2);

	float StepRed, StepGreen, StepBlue;
	COLORREF StepRGB;
	
	switch(m_dStyle)
	{
		case WK_BUTTON_COLOR_GRADATION_RIGHT:	// 오른쪽 방향으로
			StepRed		= ((float)C1Red		- (float)C2Red)		/(float)m_nWidth;	// RED의 단계별 증가값 구하기
			StepGreen	= ((float)C1Green	- (float)C2Green)	/(float)m_nWidth;	// GREEN의 단계별 증가값 구하기
			StepBlue	= ((float)C1Blue	- (float)C2Blue)	/(float)m_nWidth;	// BLUE의 단계별 증가값 구하기
			for(x = 0; x <= m_nWidth; x++)
			{
				StepRGB = RGB(C1Red - (int)(StepRed * x),
							C1Green - (int)(StepGreen * x),
							C1Blue - (int)(StepBlue * x));		// 단계별 RGB 값 만들기
				for(y = 0; y <= m_nHeight; y++)
					BufferDC.SetPixel(x, y, StepRGB);			// 단계별 라인으로 출력
			}
			break;
			
		case WK_BUTTON_COLOR_GRADATION_LEFT:	// 왼쪽 방향으로
			StepRed		= ((float)C1Red		- (float)C2Red)		/(float)m_nWidth;
			StepGreen	= ((float)C1Green	- (float)C2Green)	/(float)m_nWidth;
			StepBlue	= ((float)C1Blue	- (float)C2Blue)	/(float)m_nWidth;
			for(x = m_nWidth; x >= 0; x--)
			{
				StepRGB = RGB(C1Red - (int)(StepRed * x),
							C1Green - (int)(StepGreen * x),
							C1Blue - (int)(StepBlue * x));
				for(y = 0; y <= m_nHeight; y++)
					BufferDC.SetPixel(x, y, StepRGB);
			}
			break;
			
		case WK_BUTTON_COLOR_GRADATION_UP:		// 위쪽 방향으로
			StepRed		= ((float)C1Red		- (float)C2Red)		/(float)m_nHeight;
			StepGreen	= ((float)C1Green	- (float)C2Green)	/(float)m_nHeight;
			StepBlue	= ((float)C1Blue	- (float)C2Blue)	/(float)m_nHeight;
			for(y = 0; y <= m_nHeight; y++)
			{
				StepRGB = RGB(C1Red - (int)(StepRed * y),
							C1Green - (int)(StepGreen * y),
							C1Blue - (int)(StepBlue * y));
				for(x = 0; x <= m_nWidth; x++)
					BufferDC.SetPixel(x, y, StepRGB);
			}
			break;
			
		case WK_BUTTON_COLOR_GRADATION_DOWN:	// 아래쪽 방향으로
			StepRed		= ((float)C1Red		- (float)C2Red)		/(float)m_nHeight;
			StepGreen	= ((float)C1Green	- (float)C2Green)	/(float)m_nHeight;
			StepBlue	= ((float)C1Blue	- (float)C2Blue)	/(float)m_nHeight;
			for(y = m_nHeight; y >= 0; y--)
			{
				StepRGB = RGB(C1Red - (int)(StepRed * y),
							C1Green - (int)(StepGreen * y),
							C1Blue - (int)(StepBlue * y));
				for(x = 0; x <= m_nWidth; x++)
					BufferDC.SetPixel(x, y, StepRGB);
			}
			break;
	}
	BufferDC.SelectObject(pOldBitmap);
}

BOOL CWKButton::DrawBorder(CBitmap *pbmpImage, COLORREF color1, COLORREF color2, int nWidth)
{
	CPaintDC dc(this);
	CDC BufferDC;
	CBitmap* pOldBitmap;
	BufferDC.CreateCompatibleDC(((CWnd*)(dc.GetWindow()))->GetDC());
	
	pOldBitmap = (CBitmap*)BufferDC.SelectObject(pbmpImage);

	int i, j;
	for(i = 0; i < nWidth; i++)
		for(j = i; j <= (m_nWidth - i); j++)
		{
			BufferDC.SetPixel(j, i, color1);
			BufferDC.SetPixel(j, m_nHeight - i - 1, color2);
		}

	for(i = 0; i < nWidth; i++)
	{
		for(j = i; j <= (m_nHeight - i - 1); j++)
		{
			BufferDC.SetPixel(i, j, color1);
			BufferDC.SetPixel(m_nWidth - i - 1, j, color2);
		}
	}
	BufferDC.SelectObject(pOldBitmap);

	return TRUE;
}

BOOL CWKButton::DrawText(CBitmap* pBitmap, int nStatus)
{
	CPaintDC dc(this);
	CDC BufferDC;
	CBitmap* pOldBitmap;
	CRect rt;
	GetClientRect(&rt);
	BufferDC.CreateCompatibleDC(((CWnd*)(dc.GetWindow()))->GetDC());
	
	pOldBitmap = (CBitmap*)BufferDC.SelectObject(pBitmap);

	CFont *pOldFont;
	pOldFont = (CFont*)BufferDC.SelectObject(&m_Font);
	BufferDC.SetBkMode(TRANSPARENT);

	if(nStatus == WK_BUTTON_STATUS_MOUSECLICK)
	{	// 클릭시 평상시보다 한픽셀 밑에 글씨를 출력한다.
		rt.left++;
		rt.top++;
		BufferDC.DrawText(m_strCaption, &rt, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		BufferDC.DrawText(m_strCaption, &rt, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	BufferDC.SelectObject(pOldFont);
	BufferDC.SelectObject(pOldBitmap);
	
	return TRUE;
}

COLORREF CWKButton::LightValueControl(COLORREF rgb, int nValue)
{
	int r,g,b;
	// R,G,B 값을 분리후 nValue만큼 증가시킨다.
	b = GetBValue(rgb)+nValue;
	g = GetGValue(rgb)+nValue;
	r = GetRValue(rgb)+nValue;

	if(b<0)
		b = 0;
	if(b>255)
		b = 255;
	
	if(g<0)
		g = 0;
	if(g>255)
		g = 255;
	
	if(r<0)
		r = 0;
	if(r>255)
		r = 255;

	return RGB(r,g,b);
}

void CWKButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_dButtonStatus = WK_BUTTON_STATUS_MOUSECLICK;
	Invalidate(FALSE);
	CButton::OnLButtonDown(nFlags, point);
}

void CWKButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags != MK_LBUTTON &&							
	   nFlags != MK_RBUTTON &&
	   m_dButtonStatus != WK_BUTTON_STATUS_MOUSEOVER)	
	{// 마우스 버튼 입력이 없는 상태에서 마우스 커서가 버튼위를 지나거나 사라질 경우 이벤트를 발생시킨다.
		TRACKMOUSEEVENT track = {sizeof(TRACKMOUSEEVENT),TME_HOVER | TME_LEAVE,m_hWnd,10};
		_TrackMouseEvent(&track);
	}
	
	CButton::OnMouseMove(nFlags, point);
}

BOOL CWKButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch(pMsg->message)
	{
	case WM_MOUSELEAVE:	m_dButtonStatus = WK_BUTTON_STATUS_MOUSELEAVE;
						Invalidate(FALSE);
						break;

	case WM_MOUSEHOVER:	m_dButtonStatus = WK_BUTTON_STATUS_MOUSEOVER;
						Invalidate(FALSE);
						break;
	}

	return CButton::PreTranslateMessage(pMsg);
}


BOOL CWKButton::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CPaintDC dc(this);
	m_bmpImage1.DeleteTempMap();
	m_bmpImage2.DeleteTempMap();
	m_bmpImage3.DeleteTempMap();

	m_Font.DeleteObject();

	return CButton::DestroyWindow();
}
