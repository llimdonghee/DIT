#pragma once

//#define IDD_AKCOLORTABLEOPTIONDLG 11000

#include "akGraph/akResource.h"
#include "akGraph/akGraphBase.h"

// CakRangeSettingDlg 대화 상자입니다.
#define RANGESETTINGUPDATE WM_USER+0x101



class CakRangeSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CakRangeSettingDlg)

public:
	CakRangeSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CakRangeSettingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AKRANGESETTINGDLG };

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

	CEdit		m_ctrlEditXMin;
	CEdit		m_ctrlEditXMax;
	CEdit		m_ctrlEditYMin;
	CEdit		m_ctrlEditYMax;
	
	double m_RangeXMin;
	double m_RangeXMax;
	double m_RangeYMin;
	double m_RangeYMax;

	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;

private:


public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
