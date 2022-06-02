// CDlgAutoCalPeriod.cpp: 구현 파일
//
#include "stdafx.h"
#include "CDlgAutoCalPeriod.h"
#include "afxdialogex.h"
#include "ImageInspectionDlg.h"

// CDlgAutoCalPeriod 대화 상자

IMPLEMENT_DYNAMIC(CDlgAutoCalPeriod, CDialogEx)

CDlgAutoCalPeriod::CDlgAutoCalPeriod(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_AUTO_CAL, pParent)
{

}

CDlgAutoCalPeriod::~CDlgAutoCalPeriod()
{
}

void CDlgAutoCalPeriod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAutoCalPeriod, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SEARCH, &CDlgAutoCalPeriod::OnBnClickedButtonAutoSearch)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgAutoCalPeriod::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgAutoCalPeriod::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SEARCH_HORIZONTAL, &CDlgAutoCalPeriod::OnBnClickedButtonAutoSearchHorizontal)
END_MESSAGE_MAP()


// CDlgAutoCalPeriod 메시지 처리기

void CDlgAutoCalPeriod::SetMainDlg(CDialogEx* pDlg)
{
	m_pMainDlg = pDlg;
}


void CDlgAutoCalPeriod::OnBnClickedButtonAutoSearch()
{
	SetDlgItemText(IDC_EDIT_RESULT, " ");

	m_bVer = TRUE;
	m_bHor = FALSE;

	int iPeriod;
	int iStart, iEnd;
	int iDefectCnt;

	iStart = GetDlgItemInt(IDC_EDIT_PERIOD_START);
	iEnd = GetDlgItemInt(IDC_EDIT_PERIOD_END);

	CString str;
	int iMinDefectCnt = 0xfffff;

	SetDlgItemText(IDC_EDIT_RESULT, " ");

	for (iPeriod = iStart; iPeriod < iEnd; iPeriod++)
	{
		((CImageInspectionDlg*)m_pMainDlg)->AutoCalInspect(m_bVer, m_bHor, iPeriod);
		iDefectCnt = ((CImageInspectionDlg*)m_pMainDlg)->m_DDMAlgorithm->m_iPixelCnt;

		if (iDefectCnt < iMinDefectCnt)
		{
			iMinDefectCnt = iDefectCnt;
			m_iAutoCalPeriod = iPeriod;
		}		
		if (iPeriod % 2 == 0)
			str.Format("++");
		else str.Format("--");

		SetDlgItemText(IDC_EDIT_RESULT, str);
	}
	str.Format("Defect : %d / Period : %d", iMinDefectCnt, m_iAutoCalPeriod);
	SetDlgItemText(IDC_EDIT_RESULT,str);	

	AfxMessageBox("Cal Finish");
}

void CDlgAutoCalPeriod::OnBnClickedButtonAutoSearchHorizontal()
{
	SetDlgItemText(IDC_EDIT_RESULT, " ");

	m_bVer = FALSE;
	m_bHor = TRUE;

	int iPeriod;
	int iStart, iEnd;
	int iDefectCnt;

	iStart = GetDlgItemInt(IDC_EDIT_HORI_PERIOD_START);
	iEnd = GetDlgItemInt(IDC_EDIT_HORI_PERIOD_END);

	CString str;
	int iMinDefectCnt = 0xfffff;

	for (iPeriod = iStart; iPeriod < iEnd; iPeriod++)
	{
		((CImageInspectionDlg*)m_pMainDlg)->AutoCalInspect(m_bVer, m_bHor, iPeriod);
		iDefectCnt = ((CImageInspectionDlg*)m_pMainDlg)->m_DDMAlgorithm->m_iPixelCnt;

		if (iDefectCnt < iMinDefectCnt)
		{
			iMinDefectCnt = iDefectCnt;
			m_iAutoCalPeriod = iPeriod;
		}
		if(iPeriod % 2 == 0)
			str.Format("++");
		else str.Format("--");

		SetDlgItemText(IDC_EDIT_RESULT, str);
	}
	str.Format("Defect : %d / Period : %d", iMinDefectCnt, m_iAutoCalPeriod);
	SetDlgItemText(IDC_EDIT_RESULT, str);

	AfxMessageBox("Cal Finish");
}


void CDlgAutoCalPeriod::OnBnClickedButtonApply()
{
	if(m_bVer == TRUE)
		((CImageInspectionDlg*)m_pMainDlg)->SetAutoCalPeriod(m_bVer, m_bHor, m_iAutoCalPeriod);
	else if (m_bHor == TRUE)
		((CImageInspectionDlg*)m_pMainDlg)->SetAutoCalPeriod(m_bVer, m_bHor, m_iAutoCalPeriod);
}


void CDlgAutoCalPeriod::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->ShowWindow(SW_HIDE);
}





BOOL CDlgAutoCalPeriod::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemInt(IDC_EDIT_PERIOD_START, 100);
	SetDlgItemInt(IDC_EDIT_PERIOD_END, 150);
	
	SetDlgItemInt(IDC_EDIT_HORI_PERIOD_START, 100);
	SetDlgItemInt(IDC_EDIT_HORI_PERIOD_END, 150);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
