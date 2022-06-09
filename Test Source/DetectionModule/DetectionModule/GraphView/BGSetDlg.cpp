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

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	CRect rect;
	CString strClass = _T("");
	UINT nID = pWnd->GetDlgCtrlID();

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(m_clCtrlColor);
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
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

	// DRAWITEMSTRUCT ����ü�� �����߿��� HDC ������ �ڵ鰪�� CDC ��ü�� ��ȯ�Ѵ�.
	CDC *p_dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	// RECT ������ ����ü���� �̿��Ͽ� CRect ��ü�� �����Ѵ�.
	CRect r(lpDrawItemStruct->rcItem);
	CString str;
	// ��ư�� ĸ���� ��´�.
	GetDlgItemText(nIDCtl, str);

	// ���ڸ� ����� �� ������ ����� �������� �����Ѵ�.
	int old_mode = p_dc->SetBkMode(TRANSPARENT);

	// ��ư�� ����� �� ���� ������ ���
	if(lpDrawItemStruct->itemState & ODS_DISABLED){
		// ��ư ������ ȸ������ ä���.
		p_dc->FillSolidRect(r, RGB(192, 192, 192));

		// ���� ȸ������ ���ڻ��� �����Ѵ�.
		p_dc->SetTextColor(RGB(128, 128, 128));
		// ��ư�� ĸ���� ����Ѵ�.
		p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	} else {
		// ��ư ������ Ư�� Color�� ä���.
		//p_dc->FillSolidRect(r, m_clBtnColor);
		CGradientStatic::DrawGradRect(p_dc, r, m_clBtnColor, m_clBtnColor2, FALSE);
		// ��ư�� ��Ŀ���� ���� ���
		if(lpDrawItemStruct->itemState & ODS_FOCUS){
			// ��Ȳ������ �׵θ��� �׸���, ���ڻ��� ������� �����Ѵ�.
			p_dc->Draw3dRect(r, m_clBtnFocusColor, m_clBtnFocusColor);
			p_dc->SetTextColor(m_clBtnTextColor);

		} else {
			// ȸ������ �׵θ��� �׸���, ���ڻ��� ������� �����Ѵ�.
			p_dc->Draw3dRect(r, m_clBtnOutColor2, m_clBtnOutColor2);
			p_dc->SetTextColor(m_clBtnTextColor);
		}
		// ������ ���ȼ��� ���� �������� ���δ�.
		r.left++;
		r.top++;
		r.right--;
		r.bottom--;		

		// ��ư�� ������ ���
		if(lpDrawItemStruct->itemState & ODS_SELECTED){
			// �׵θ��� ������ �����Ѵ�.
			p_dc->Draw3dRect(r, m_clBtnSelectedColor, m_clBtnSelectedColor);

			// ��ư�� ĸ���� ������ �Ʒ� �������� ���ȼ��� �̵����� ����Ѵ�.
			p_dc->DrawText(str, r + CPoint(1, 1), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		} else {
			// �׵θ��� ������ �����Ѵ�.
			p_dc->Draw3dRect(r, m_clBtnOutColor1, m_clBtnOutColor1);
			// ��ư�� ĸ���� ����Ѵ�.
			p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	// ����� ���� ���� �����Ѵ�.
	p_dc->SetBkMode(old_mode);
}

BOOL CBGSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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