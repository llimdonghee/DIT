#include "stdafx.h"
#include "BGSetDlg.h"
#include "HMControls/GradientStatic.h"

BEGIN_MESSAGE_MAP(CBGSetDlg, CDialog)	
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

CBGSetDlg::CBGSetDlg(UINT nIDTemplate, CWnd* pParentWnd)
: CDialog(nIDTemplate, pParentWnd)
{
	m_clBGColor = RGB(0,0,0);//GetSysColor(COLOR_WINDOW);
	m_clCtrlColor = RGB(255,255,255);//GetSysColor(COLOR_WINDOWTEXT);
	SetBtnColor();
}

CBGSetDlg::CBGSetDlg(void)
{
	m_clBGColor = RGB(0,0,0);//GetSysColor(COLOR_WINDOW);
	m_clCtrlColor = RGB(255,255,255);//GetSysColor(COLOR_WINDOWTEXT);
}

CBGSetDlg::~CBGSetDlg(void)
{
}

void CBGSetDlg::SetTitleBar(CGradientStatic *pstaticbar, CString strTitle, COLORREF clTitle1, COLORREF clTitle2, COLORREF clText, COLORREF clOut, COLORREF clOut2)
{
	pstaticbar->SetControlFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,_T("Arial"));
	pstaticbar->SetTextAlign(1);
	pstaticbar->SetColor(clTitle1);
	pstaticbar->SetGradientColor(clTitle2);
	pstaticbar->SetTextColor(clText);	
	pstaticbar->SetOulineColor(clOut, clOut2);
	pstaticbar->SetWindowText(strTitle);
}

void CBGSetDlg::SetBtnColor(COLORREF clBtn, COLORREF clBtn2, COLORREF clText, COLORREF clSelected, COLORREF clFocus, COLORREF clOut1, COLORREF clOut2)
{
	m_clBtnColor = clBtn;
	m_clBtnColor2 = clBtn2;
	m_clBtnTextColor = clText;
	m_clBtnSelectedColor = clSelected;
	m_clBtnFocusColor = clFocus;
	m_clBtnOutColor1 = clOut1;
	m_clBtnOutColor2 = clOut2;
}

HBRUSH CBGSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	CRect rect;
	CString strClass = _T("");
	UINT nID = pWnd->GetDlgCtrlID();

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(m_clCtrlColor);
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CBGSetDlg::OnEraseBkgnd(CDC* pDC)
{

	CRect	rect;
	GetClientRect(&rect);
	// 	CBrush	*pBrush = new CBrush;
	// 	pBrush->CreateSolidBrush(m_clBGColor);
	// 	if(pBrush != NULL)
	// 	{
	// 		pDC->FillRect(&rect, pBrush);
	// 		pBrush->DeleteObject();
	// 	}
	// 
	// 	delete pBrush;

	pDC->FillSolidRect(rect, m_clBGColor);

	return TRUE;
}

void CBGSetDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DrawButton(nIDCtl, lpDrawItemStruct);
	__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CBGSetDlg::DrawButton(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//if(nIDCtl != IDC_BUTTON_MAP_EXTENTION)
	//	return;

	// DRAWITEMSTRUCT 구조체의 정보중에서 HDC 형식의 핸들값을 CDC 객체로 변환한다.
	CDC *p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	// RECT 형식의 구조체값을 이용하여 CRect 객체를 생성한다.
	CRect r(lpDrawItemStruct->rcItem);
	CString str;
	// 버튼의 캡션을 얻는다.
	GetDlgItemText(nIDCtl, str);

	// 글자를 출력할 때 적용할 배경을 투명으로 설정한다.
	int old_mode = p_dc->SetBkMode(TRANSPARENT);

	// 버튼을 사용할 수 없는 상태일 경우
	if(lpDrawItemStruct->itemState & ODS_DISABLED){
		// 버튼 영역을 회색으로 채운다.
		p_dc->FillSolidRect(r, RGB(192, 192, 192));

		// 진한 회색으로 글자색을 설정한다.
		p_dc->SetTextColor(RGB(128, 128, 128));
		// 버튼의 캡션을 출력한다.
		p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	} else {
		// 버튼 영역을 특정 Color로 채운다.
		//p_dc->FillSolidRect(r, m_clBtnColor);
		CGradientStatic::DrawGradRect(p_dc, r, m_clBtnColor, m_clBtnColor2, FALSE);
		// 버튼이 포커스를 얻은 경우
		if(lpDrawItemStruct->itemState & ODS_FOCUS){
			// 주황색으로 테두리를 그리고, 글자색은 흰색으로 설정한다.
			p_dc->Draw3dRect(r, m_clBtnFocusColor, m_clBtnFocusColor);
			p_dc->SetTextColor(m_clBtnTextColor);

		} else {
			// 회색으로 테두리를 그리고, 글자색을 흰색으로 설정한다.
			p_dc->Draw3dRect(r, m_clBtnOutColor2, m_clBtnOutColor2);
			p_dc->SetTextColor(m_clBtnTextColor);
		}
		// 영역을 한픽셀씩 내부 방향으로 줄인다.
		r.left++;
		r.top++;
		r.right--;
		r.bottom--;		

		// 버튼을 선택한 경우
		if(lpDrawItemStruct->itemState & ODS_SELECTED){
			// 테두리의 색상을 지정한다.
			p_dc->Draw3dRect(r, m_clBtnSelectedColor, m_clBtnSelectedColor);

			// 버튼의 캡션을 오른쪽 아래 방향으로 한픽셀씩 이동시켜 출력한다.
			p_dc->DrawText(str, r + CPoint(1, 1), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		} else {
			// 테두리의 색상을 지정한다.
			p_dc->Draw3dRect(r, m_clBtnOutColor1, m_clBtnOutColor1);
			// 버튼의 캡션을 출력한다.
			p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// 배경을 이전 모드로 설정한다.
	p_dc->SetBkMode(old_mode);
}

BOOL CBGSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_ESCAPE )
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;                    // DO NOT process further
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}