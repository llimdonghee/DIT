#include "StdAfx.h"
#include "akButtonColor.h"
#include "akRadio.h"

IMPLEMENT_DYNAMIC(CakButtonColor, CButton)
BEGIN_MESSAGE_MAP(CakButtonColor, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CakButtonColor::CakButtonColor(void)
{
	m_pButtonHover = 0;

	m_bHover = FALSE;
	m_bTracking = FALSE;

	m_bCheckBoxType = FALSE;
	m_bCheckBoxFlag = FALSE;

	m_hNotifyParent = NULL;
}

CakButtonColor::~CakButtonColor(void)
{
}

void CakButtonColor::addControl( CButton* pButton )
{
	UINT nBtnStyle = pButton->GetButtonStyle();
	if((nBtnStyle == BS_CHECKBOX)
		||(nBtnStyle == BS_AUTOCHECKBOX)
		||(nBtnStyle == (BS_CHECKBOX|BS_PUSHLIKE))
		||(nBtnStyle == (BS_AUTOCHECKBOX|BS_PUSHLIKE)))
	{
		m_bCheckBoxType = TRUE;
	}
	pButton->ModifyStyle(NULL, WS_CLIPSIBLINGS | BS_OWNERDRAW ,	0);
	m_mapControls[pButton->GetDlgCtrlID()] = pButton;
}

void CakButtonColor::clear()
{
	m_mapControls.clear();
}

bool CakButtonColor::processDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	int stTime = GetTickCount();
	std::map<int,CButton*>::iterator itor = m_mapControls.find(nIDCtl);
	if(itor == m_mapControls.end()) return FALSE;

	_ColorButtonOption* pOption = &m_ButtonColorOption;

	CButton* pButton = (CButton*)itor->second;

	if(pButton)
	{
		if(pOption->nStyle == 0)
		{
			drawButton3dNormal(pButton, lpDrawItemStruct, pOption);
		}
		if(pOption->nStyle == 1)
		{
			drawButton3dGradation(pButton, lpDrawItemStruct, pOption);
		}
	}


	return FALSE;
}

void CakButtonColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	_ColorButtonOption* pOption = &m_ButtonColorOption;


	{
		if(pOption->nStyle == 0)
		{
			drawButton3dNormal(this, lpDrawItemStruct, pOption);
		}
		if(pOption->nStyle == 1)
		{
			drawButton3dGradation(this, lpDrawItemStruct, pOption);
		}
	}
}


bool CakButtonColor::drawButton3dNormal( CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption )
{
	if(pButton)
	{
		CDC dc;
		CRect rect;
		dc.Attach(lpDrawItemStruct ->hDC);   // Get the Button DC to CDC

		// FONT 추가
		CFont* pOldFont = (CFont*)dc.SelectObject(m_font);
		
		rect = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.

		//rect.DeflateRect(1,1,1,1);
		//

		UINT state=lpDrawItemStruct->itemState;  //This defines the state of the Push button either pressed or not. 

		//horver 태현[2016/9/19]
		if((state & ODS_SELECTED)
			|| (m_bCheckBoxType && GetCheck() == TRUE) )//push
		{
			dc.FillSolidRect(&rect, pOption->clrButtonPush);//Here you can define the required color to appear on the Button.
		}
		else if(m_pButtonHover == pButton || m_bHover)
		{
			dc.FillSolidRect(&rect, pOption->clrButtonHorver);//Here you can define the required color to appear on the Button.
		}
		else //normal
		{
			dc.FillSolidRect(&rect, pOption->clrButtonShow);//Here you can define the required color to appear on the Button.
		}

		int nButtonStyle=pOption->nEdgeStyle;
		if(nButtonStyle == 0)//3d
		{
			if((state & ODS_SELECTED))
			{
				CRect rect3D = rect;

				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
				COLORREF color1, color2;
				color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				color2 = getAlphaColor(RGB(160,160,160), pOption->clrButtonShow, 0.35);
				dc.Draw3dRect(&rect3D,color1, color1); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D,color2, color2);
			}
			else
			{
				//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
				CRect rect3D = rect;
				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

				COLORREF color21 = getAlphaColor(RGB(255,255,255), pOption->clrButtonShow, 0.35);
				COLORREF color22 = getAlphaColor(RGB(105,105,105), pOption->clrButtonShow, 0.35);
				COLORREF color31 = getAlphaColor(RGB(227,227,227), pOption->clrButtonShow, 0.35);
				COLORREF color32 = getAlphaColor(RGB(160, 160, 160), pOption->clrButtonShow, 0.35);


				if(pButton->GetFocus() == pButton)
				{
					COLORREF color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.34);
					dc.Draw3dRect(&rect3D,color1,color1); 
					rect3D.DeflateRect(1,1,1,1);
				}
				dc.Draw3dRect(&rect3D, color21, color22); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D, color31, color32); 
			}
		}
		else //flat
		{
			if((state & ODS_SELECTED))
			{
				CRect rect3D = rect;

				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
				COLORREF color1, color2;
				color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				dc.Draw3dRect(&rect3D,color1, color1); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D,RGB(255,255,255), RGB(255,255,255));
			}
			else
			{
				//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
				CRect rect3D = rect;
				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

				COLORREF color21 = getAlphaColor(RGB(255,255,255), pOption->clrButtonShow, 0.35);
				COLORREF color22 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				COLORREF color31 = getAlphaColor(RGB(227,227,227), pOption->clrButtonShow, 0.35);
				COLORREF color32 = getAlphaColor(RGB(160, 160, 160), pOption->clrButtonShow, 0.35);


				if(pButton->GetFocus() == pButton)
				{
					COLORREF color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.34);
					dc.Draw3dRect(&rect3D,color1,color1); 
					rect3D.DeflateRect(1,1,1,1);
				}
				dc.Draw3dRect(&rect3D, color22, color22); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D, color21, color21); 
			}
		}



		//if((state & ODS_SELECTED))
		//{
		//	CRect rect3D = rect;

		//	//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		//	dc.Draw3dRect(&rect3D,RGB(100,100,100),RGB(100,100,100)); 
		//	rect3D.DeflateRect(1,1,1,1);
		//	dc.Draw3dRect(&rect3D,RGB(160,160,160),RGB(160, 160, 160));
		//}
		//else
		//{
		//	//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
		//	CRect rect3D = rect;
		//	//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		//	if(pButton->GetFocus() == pButton)
		//	{
		//		dc.Draw3dRect(&rect3D,RGB(100,100,100),RGB(100,100,100)); 
		//		rect3D.DeflateRect(1,1,1,1);
		//	}
		//	dc.Draw3dRect(&rect3D,RGB(255,255,255),RGB(105,105,105)); 
		//	rect3D.DeflateRect(1,1,1,1);
		//	dc.Draw3dRect(&rect3D,RGB(227,227,227),RGB(160, 160, 160)); 
		//}



		//텍스트 태현[2016/9/19]
		{
			TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
			ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero
			::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 

			dc.SetBkMode(TRANSPARENT);
			//dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the  Caption of Button Window 

			{

				//CFont* poldFont = dc->SelectObject(GetFont());

				// 문자열 표시 영역 설정
				CRect rectText;;
				//다중 문자열의 길이 연산
				CRect rc;
				dc.DrawText(buffer,rc, DT_CALCRECT);
				// 다중 문자열의 중앙점 연산
				int a= rc.Height()/2; 
				// 문자열이 중앙에 올수 있는 문자열의 시작 위치 연산
				int scalex, scaley;
				scalex = rect.Width();
				scaley = rect.Height();

				if(lpDrawItemStruct->itemState & ODS_SELECTED
					|| (m_bCheckBoxType && GetCheck() == TRUE))
				{
					if(pOption->bTextShadow) 
					{
						dc.SetTextColor(pOption->clrFontNormal);
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextPushX+pOption->nTextShadowX,
							0+pOption->nTextPushY+pOption->nTextShadowY);
					}
					else
					{
						dc.SetTextColor(pOption->clrFontPush);
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextPushX,
							0+pOption->nTextPushY);

					}
					rectText.top = (rectText.Height()/2) - a;
					rectText.left += pOption->nTextPushX;
					rectText.top += pOption->nTextPushY;

					//memDC.SetTextColor(m_textcolor);
					//memDC.DrawText(wintext,CRect(0+m_nTextPushX,0+m_nTextPushY,scalex+m_nTextPushX, scaley+m_nTextPushY),DT_SINGLELINE | DT_CENTER | DT_VCENTER);


				}
				else
				{
					if(pOption->bTextShadow) 
					{
						dc.SetTextColor(pOption->clrFontNormal);
						//memDC.DrawText(wintext,CRect(0+m_nFontShowOffsetX,0+m_nFontShowOffsetY,scalex+m_nFontShowOffsetX, scaley+m_nFontShowOffsetY),DT_SINGLELINE | DT_CENTER | DT_VCENTER);	
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextShadowX,
							0+pOption->nTextShadowY);
					}
					else
					{
						dc.SetTextColor(pOption->clrFontNormal);
						rectText.SetRect(0,0,scalex, scaley);
					}
					rectText.top = (rectText.Height()/2) - a;

					//memDC.DrawText(wintext,CRect(0,0,scalex, scaley),DT_SINGLELINE | DT_CENTER | DT_VCENTER);




				}

				rectText -= CRect(pOption->nFontBoxOffsetX, pOption->nFontBoxOffsetY,0,0);


				// 문자열 표시
				dc.DrawText(buffer,rectText, DT_CENTER);

				//dc->SelectObject(poldFont);

			}



		}

		dc.Detach();  // Detach the Button DC

		//TRACE("process time : %dms\n", GetTickCount()-stTime);
		return TRUE;
	}

	return FALSE;
}

bool CakButtonColor::drawButton3dGradation( CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption )
{
	if(pButton)
	{
		CDC dc;
		CRect rect;
		dc.Attach(lpDrawItemStruct ->hDC);   // Get the Button DC to CDC

		// FONT 추가
		CFont* pOldFont = (CFont*)dc.SelectObject(m_font);

		rect = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.

		//rect.DeflateRect(1,1,1,1);
		//

		UINT state=lpDrawItemStruct->itemState;  //This defines the state of the Push button either pressed or not. 


		//horver 태현[2016/9/19]
		if((state & ODS_SELECTED)
			|| (m_bCheckBoxType && GetCheck() == TRUE))//push
		{
			dc.FillSolidRect(&rect, pOption->clrButtonPush);//Here you can define the required color to appear on the Button.
		}
		else if(m_pButtonHover == pButton || m_bHover)
		{
			dc.FillSolidRect(&rect, pOption->clrButtonHorver);//Here you can define the required color to appear on the Button.
		}
		else //normal
		{
			COLORREF part1Top, part1Bottom, part2Top, part2Bottom;
			part2Top = pOption->clrButtonShow;
			getButtonColor3Point(part2Top, &part1Top, &part1Bottom, &part2Bottom);
			draw2PartGraditionRect(&dc, rect, part1Top, part1Bottom, part2Top, part2Bottom);			

		}


		int nButtonStyle=pOption->nEdgeStyle;
		if(nButtonStyle == 0)//3d
		{
			if((state & ODS_SELECTED))
			{
				CRect rect3D = rect;

				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
				COLORREF color1, color2;
				color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				color2 = getAlphaColor(RGB(160,160,160), pOption->clrButtonShow, 0.35);
				dc.Draw3dRect(&rect3D,color1, color1); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D,color2, color2);
			}
			else
			{
				//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
				CRect rect3D = rect;
				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

				COLORREF color21 = getAlphaColor(RGB(255,255,255), pOption->clrButtonShow, 0.35);
				COLORREF color22 = getAlphaColor(RGB(105,105,105), pOption->clrButtonShow, 0.35);
				COLORREF color31 = getAlphaColor(RGB(227,227,227), pOption->clrButtonShow, 0.35);
				COLORREF color32 = getAlphaColor(RGB(160, 160, 160), pOption->clrButtonShow, 0.35);


				if(pButton->GetFocus() == pButton)
				{
					COLORREF color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.34);
					dc.Draw3dRect(&rect3D,color1,color1); 
					rect3D.DeflateRect(1,1,1,1);
				}
				dc.Draw3dRect(&rect3D, color21, color22); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D, color31, color32); 
			}
		}
		else //flat
		{
			if((state & ODS_SELECTED))
			{
				CRect rect3D = rect;

				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
				COLORREF color1, color2;
				color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				dc.Draw3dRect(&rect3D,color1, color1); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D,RGB(255,255,255), RGB(255,255,255));
			}
			else
			{
				//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
				CRect rect3D = rect;
				//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

				COLORREF color21 = getAlphaColor(RGB(255,255,255), pOption->clrButtonShow, 0.35);
				COLORREF color22 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.35);
				COLORREF color31 = getAlphaColor(RGB(227,227,227), pOption->clrButtonShow, 0.35);
				COLORREF color32 = getAlphaColor(RGB(160, 160, 160), pOption->clrButtonShow, 0.35);


				if(pButton->GetFocus() == pButton)
				{
					COLORREF color1 = getAlphaColor(RGB(100,100,100), pOption->clrButtonShow, 0.34);
					dc.Draw3dRect(&rect3D,color1,color1); 
					rect3D.DeflateRect(1,1,1,1);
				}
				dc.Draw3dRect(&rect3D, color22, color22); 
				rect3D.DeflateRect(1,1,1,1);
				dc.Draw3dRect(&rect3D, color21, color21); 
			}
		}

		//if((state & ODS_SELECTED))
		//{
		//	CRect rect3D = rect;

		//	//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		//	dc.Draw3dRect(&rect3D,RGB(100,100,100),RGB(100,100,100)); 
		//	rect3D.DeflateRect(1,1,1,1);
		//	dc.Draw3dRect(&rect3D,RGB(160,160,160),RGB(160, 160, 160));
		//}
		//else
		//{
		//	//dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
		//	CRect rect3D = rect;
		//	//dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		//	if(pButton->GetFocus() == pButton)
		//	{
		//		dc.Draw3dRect(&rect3D,RGB(100,100,100),RGB(100,100,100)); 
		//		rect3D.DeflateRect(1,1,1,1);
		//	}
		//	dc.Draw3dRect(&rect3D,RGB(255,255,255),RGB(105,105,105)); 
		//	rect3D.DeflateRect(1,1,1,1);
		//	dc.Draw3dRect(&rect3D,RGB(227,227,227),RGB(160, 160, 160)); 
		//}



		//텍스트 태현[2016/9/19]
		{
			TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
			ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero
			::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 

			dc.SetBkMode(TRANSPARENT);
			//dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the  Caption of Button Window 

			{

				//CFont* poldFont = dc->SelectObject(GetFont());

				// 문자열 표시 영역 설정
				CRect rectText;;
				//다중 문자열의 길이 연산
				CRect rc;
				dc.DrawText(buffer,rc, DT_CALCRECT);
				// 다중 문자열의 중앙점 연산
				int a= rc.Height()/2; 
				// 문자열이 중앙에 올수 있는 문자열의 시작 위치 연산
				int scalex, scaley;
				scalex = rect.Width();
				scaley = rect.Height();

				if(lpDrawItemStruct->itemState & ODS_SELECTED
					|| (m_bCheckBoxType && GetCheck() == TRUE))
				{
					if(pOption->bTextShadow) 
					{
						dc.SetTextColor(pOption->clrFontNormal);
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextPushX+pOption->nTextShadowX,
							0+pOption->nTextPushY+pOption->nTextShadowY);
					}
					else
					{
						dc.SetTextColor(pOption->clrFontPush);
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextPushX,
							0+pOption->nTextPushY);

					}
					rectText.top = (rectText.Height()/2) - a;
					rectText.left += pOption->nTextPushX;
					rectText.top += pOption->nTextPushY;

					//memDC.SetTextColor(m_textcolor);
					//memDC.DrawText(wintext,CRect(0+m_nTextPushX,0+m_nTextPushY,scalex+m_nTextPushX, scaley+m_nTextPushY),DT_SINGLELINE | DT_CENTER | DT_VCENTER);


				}
				else
				{
					if(pOption->bTextShadow) 
					{
						dc.SetTextColor(pOption->clrFontNormal);
						//memDC.DrawText(wintext,CRect(0+m_nFontShowOffsetX,0+m_nFontShowOffsetY,scalex+m_nFontShowOffsetX, scaley+m_nFontShowOffsetY),DT_SINGLELINE | DT_CENTER | DT_VCENTER);	
						rectText.SetRect(0,0,scalex, scaley);
						rectText.OffsetRect(0+pOption->nTextShadowX,
							0+pOption->nTextShadowY);
					}
					else
					{
						dc.SetTextColor(pOption->clrFontNormal);
						rectText.SetRect(0,0,scalex, scaley);
					}
					rectText.top = (rectText.Height()/2) - a;

					//memDC.DrawText(wintext,CRect(0,0,scalex, scaley),DT_SINGLELINE | DT_CENTER | DT_VCENTER);




				}

				rectText -= CRect(pOption->nFontBoxOffsetX, pOption->nFontBoxOffsetY,0,0);


				// 문자열 표시
				dc.DrawText(buffer,rectText, DT_CENTER);

				//dc->SelectObject(poldFont);

			}



		}

		dc.Detach();  // Detach the Button DC

		//TRACE("process time : %dms\n", GetTickCount()-stTime);
		return TRUE;
	}
	return FALSE;
}


void CakButtonColor::processMouseHover(int nIDCtl)
{
	std::map<int,CButton*>::iterator itor = m_mapControls.find(nIDCtl);
	if(itor == m_mapControls.end() || nIDCtl == -1)
	{
		if(m_pButtonHover) m_pButtonHover->Invalidate(FALSE);
		m_pButtonHover = NULL;
		return;
	}
	if(m_pButtonHover == (CButton*)itor->second) return;

	if(m_pButtonHover) m_pButtonHover->Invalidate(FALSE);

	m_pButtonHover = (CButton*)itor->second;
	m_pButtonHover->Invalidate(FALSE);
}

void CakButtonColor::processDoubleClick(int nIDCtl)
{
	std::map<int,CButton*>::iterator itor = m_mapControls.find(nIDCtl);
	if(itor == m_mapControls.end() || nIDCtl == -1)
	{
		return;
	}

	m_pButtonHover = (CButton*)itor->second;
	//m_pButtonHover->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
	m_pButtonHover->SendMessage(WM_LBUTTONDOWN, 1, MAKELPARAM(0, 0));
	//m_pButtonHover->Invalidate(FALSE);

}

COLORREF CakButtonColor::getAlphaColor( COLORREF colorA, COLORREF colorB, double dColorBAlpha )
{

	float sum_red = GetRValue(colorB) * dColorBAlpha + GetRValue(colorA) * (1.0 - dColorBAlpha);

	float sum_green = GetGValue(colorB) * dColorBAlpha + GetGValue(colorA) * (1.0 - dColorBAlpha);

	float sum_blue = GetBValue(colorB) * dColorBAlpha + GetBValue(colorA) * (1.0 - dColorBAlpha);

	return RGB(sum_red, sum_green, sum_blue);
}




hsv CakButtonColor::rgb2hsv(rgb in)
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


rgb CakButtonColor::hsv2rgb(hsv in)
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

void CakButtonColor::getButtonColor3Point( COLORREF color, COLORREF* part1Top, COLORREF* part1Bottom, COLORREF* part2Bottom )
{


	*part2Bottom = getBrightColor(color, 0.1);
	*part1Bottom = getBrightColor(color, 0.2);
	*part1Top = getBrightColor(color, 0.3);
	return;

	rgb colorRGB;
	hsv colorHSVOrg;
	hsv colorHSV;
	colorRGB.r = GetRValue(color)/255.0;
	colorRGB.g = GetGValue(color)/255.0;
	colorRGB.b = GetBValue(color)/255.0;

	colorHSVOrg = rgb2hsv(colorRGB);
	colorHSV = colorHSVOrg;
	colorHSV.v = colorHSVOrg.v+colorHSVOrg.v*0.1;
	if(colorHSV.v>1) colorHSV.v = 1;

	rgb colorTemp = hsv2rgb(colorHSV);
	*part2Bottom = RGB(colorTemp.r*255, colorTemp.g*255, colorTemp.b*255);

	colorHSV.v = colorHSVOrg.v+colorHSVOrg.v*0.2;
	if(colorHSV.v>1) colorHSV.v = 1;
	colorTemp = hsv2rgb(colorHSV);
	*part1Bottom = RGB(colorTemp.r*255, colorTemp.g*255, colorTemp.b*255);

	colorHSV.v = colorHSVOrg.v+colorHSVOrg.v*0.3;
	if(colorHSV.v>1) colorHSV.v = 1;
	colorTemp = hsv2rgb(colorHSV);
	*part1Top = RGB(colorTemp.r*255, colorTemp.g*255, colorTemp.b*255);

}

COLORREF CakButtonColor::getBrightColor( COLORREF color, double dBright )
{
	rgb colorRGB;
	hsv colorHSV;
	colorRGB.r = GetRValue(color)/255.0;
	colorRGB.g = GetGValue(color)/255.0;
	colorRGB.b = GetBValue(color)/255.0;


	colorHSV = rgb2hsv(colorRGB);
	colorHSV.v = colorHSV.v+colorHSV.v*dBright;
	if(colorHSV.v>1) colorHSV.v = 1;
	if(colorHSV.v<0) colorHSV.v = 0;

	// 	colorHSV.s = colorHSV.s-(colorHSV.s*dBright);
	// 	if(colorHSV.s>1) colorHSV.s = 1;
	// 	if(colorHSV.s<0) colorHSV.s = 0;

	rgb colorTemp = hsv2rgb(colorHSV);

	return RGB(colorTemp.r*255, colorTemp.g*255, colorTemp.b*255);
}

void CakButtonColor::draw2PartGraditionRect( CDC* pDC, CRect rectDraw, COLORREF clrPart1T, COLORREF clrPart1B, COLORREF clrPart2T, COLORREF clrPart2B )
{
	//bottom
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectGradient;

		// 그라데이션의 시작좌표를 명시한다.
		vert[0].x      = rectDraw.left;
		vert[0].y      = rectDraw.top+rectDraw.bottom/2;

		// 그라데이션의 시작색상을 명시한다.
		vert[0].Red    = GetRValue(clrPart2T)<<8;
		vert[0].Green  = GetGValue(clrPart2T)<<8;
		vert[0].Blue   = GetBValue(clrPart2T)<<8;
		vert[0].Alpha  = 0x0000;

		// 그라데이션의 끝좌표를 명시한다.
		vert[1].x      = rectDraw.right;
		vert[1].y      = rectDraw.bottom;

		// 그라데이션의 끝색상를 명시한다.
		vert[1].Red    = GetRValue(clrPart2B)<<8;
		vert[1].Green  = GetGValue(clrPart2B)<<8;
		vert[1].Blue   = GetBValue(clrPart2B)<<8;
		vert[1].Alpha  = 0x0000;

		rectGradient.UpperLeft  = 0;
		rectGradient.LowerRight = 1;

		// 가로 방향의 시작지점부터 중앙지점까지 그라데이션 효과를 준다.
		pDC->GradientFill(vert, 2, &rectGradient, 1, GRADIENT_FILL_RECT_V);
	}

	//top
	{
		TRIVERTEX vert[2];
		GRADIENT_RECT rectGradient;

		// 그라데이션의 시작좌표를 명시한다.
		vert[0].x      = rectDraw.left;
		vert[0].y      = rectDraw.top;

		// 그라데이션의 시작색상을 명시한다.
		vert[0].Red    = GetRValue(clrPart1T)<<8;
		vert[0].Green  = GetGValue(clrPart1T)<<8;
		vert[0].Blue   = GetBValue(clrPart1T)<<8;
		vert[0].Alpha  = 0x0000;

		// 그라데이션의 끝좌표를 명시한다.
		vert[1].x      = rectDraw.right;
		vert[1].y      = rectDraw.top+rectDraw.bottom/2;

		// 그라데이션의 끝색상를 명시한다.
		vert[1].Red    = GetRValue(clrPart1B)<<8;
		vert[1].Green  = GetGValue(clrPart1B)<<8;
		vert[1].Blue   = GetBValue(clrPart1B)<<8;
		vert[1].Alpha  = 0x0000;

		rectGradient.UpperLeft  = 0;
		rectGradient.LowerRight = 1;



		// 가로 방향의 시작지점부터 중앙지점까지 그라데이션 효과를 준다.
		pDC->GradientFill(vert, 2, &rectGradient, 1, GRADIENT_FILL_RECT_V);
	}
}


BOOL CakButtonColor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return true;
	return CButton::OnEraseBkgnd(pDC);
}

void CakButtonColor::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UINT nBtnStyle = GetButtonStyle();
	if((nBtnStyle == BS_CHECKBOX)
		||(nBtnStyle == BS_AUTOCHECKBOX)
		||(nBtnStyle == (BS_CHECKBOX|BS_PUSHLIKE))
		||(nBtnStyle == (BS_AUTOCHECKBOX|BS_PUSHLIKE)))
	{
		m_bCheckBoxType = TRUE;
	}


	ModifyStyle(0, BS_OWNERDRAW|BS_PUSHBUTTON);
	CButton::PreSubclassWindow();
}


void CakButtonColor::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

void CakButtonColor::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	CButton::OnLButtonDblClk(nFlags, point);
}

void CakButtonColor::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover=TRUE;
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
}

void CakButtonColor::OnMouseLeave()
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}


LRESULT CakButtonColor::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_bCheckBoxType)
	{
		if(message == BM_SETCHECK)
		{
			m_bCheckBoxFlag = (int)wParam;
			if(m_hNotifyParent) ::PostMessage(m_hNotifyParent, 1010, GetDlgCtrlID(), m_bCheckBoxFlag);
		}
		else if(message == BM_GETCHECK)
		{
			return m_bCheckBoxFlag;
		}
	}
	return CButton::WindowProc(message, wParam, lParam);
}


void CakButtonColor::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bCheckBoxType)
	{
		CRect rect;
		GetClientRect(&rect);
		if(rect.left <= point.x && rect.right >= point.x
			&& rect.top <= point.y && rect.bottom >= point.y)
		{
			m_bCheckBoxFlag = !m_bCheckBoxFlag;
			if(m_hNotifyParent) 
			{
				CakRadio* pRadio = (CakRadio*)CWnd::FromHandle( m_hNotifyParent );
				if(pRadio->IsCheckAllFalse() == TRUE)
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

void CakButtonColor::SetCheck( int nCheck )
{
	m_bCheckBoxFlag = nCheck;
	if(m_hNotifyParent && m_bCheckBoxFlag) ::PostMessage(m_hNotifyParent, 1010, GetDlgCtrlID(), 1);
	Invalidate(TRUE);
}

void CakButtonColor::setFont(char* fontName)
{
	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);

	strcpy(lf.lfFaceName, fontName); // font setting 

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);

	SetFont(&m_font);
}