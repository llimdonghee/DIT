#pragma once
#include "ImageInspection.h"

// CDlgAutoCalPeriod 대화 상자

class CDlgAutoCalPeriod : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAutoCalPeriod)

public:
	CDlgAutoCalPeriod(CWnd* pParent /*= nullptr*/);   // 표준 생성자입니다.
	virtual ~CDlgAutoCalPeriod();

	void SetMainDlg(CDialogEx* pDlg);
	int m_iAutoCalPeriod;
	CDialogEx* m_pMainDlg;

	BOOL m_bVer;
	BOOL m_bHor;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AUTO_CAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAutoSearch();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonAutoSearchHorizontal();
	virtual BOOL OnInitDialog();
};
