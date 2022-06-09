#include "StdAfx.h"
#include "akSliderCtrl.h"



CakSliderCtrl::CakSliderCtrl(void)
{
	m_testa = 0 ;
}

CakSliderCtrl::~CakSliderCtrl(void)
{

}
BEGIN_MESSAGE_MAP(CakSliderCtrl, CSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

HBRUSH CakSliderCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;// = CSliderCtrl::CtlColor(pDC, pWnd, nCtlColor);
	
	if(m_testa == 0)
	{
		m_nBrushSlider.CreateSolidBrush(RGB(255,0,0));
		m_testa = 1;
	}
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkColor(RGB(0,255,0));
	// TODO: Return a different brush if the default is not desired
	hbr = m_nBrushSlider;
	return hbr;
}
BOOL CakSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	return CSliderCtrl::OnEraseBkgnd(pDC);
}
