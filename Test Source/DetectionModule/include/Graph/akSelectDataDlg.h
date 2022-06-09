#pragma once

//#define IDD_AKCOLORTABLEOPTIONDLG 11000

#include "akGraph/akResource.h"
#include "akGraph/akGraphBase.h"

// CakSelectDataDlg 대화 상자입니다.
#define SELECTDATAUPDATE WM_USER+0x103



class CakSelectDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CakSelectDataDlg)

public:
	CakSelectDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CakSelectDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AKSELECTDATADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:
	void SetUpdate();
public:
	CWnd* m_pParent;

	CComboBox	m_ctrlSelectData;

	COLORREF	m_SelectColor;
	int m_selectData;
	int m_selectedAxisY;
	
	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;

private:


public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSelectdataColor1();
	afx_msg void OnBnClickedSelectdataColor2();
	afx_msg void OnBnClickedSelectdataColor3();
	afx_msg void OnBnClickedSelectdataColor4();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
