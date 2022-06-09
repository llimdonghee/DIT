#pragma once

//#define IDD_AKCOLORTABLEOPTIONDLG 11000

#include "akGraph/akResource.h"
#include "akGraph/akGraphBase.h"
#include "afxwin.h"

// CakDDSettingDlg ��ȭ �����Դϴ�.
#define DDSETTINGUPDATE WM_USER+0x105



class CakDDSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CakDDSettingDlg)

public:
	CakDDSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CakDDSettingDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AKDDSETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	CEdit		m_ctrlEditGraphDepth1;
	CEdit		m_ctrlEditGraphDepth2;
	CEdit		m_ctrlEditGraphDepth3;
	CEdit		m_ctrlEditGraphDepth4;
	CEdit		m_ctrlEditStd;
	
	double m_RangeXMin;
	double m_RangeXMax;
	int m_GraphDepth1;
	int m_GraphDepth2;
	int m_GraphDepth3;
	int m_GraphDepth4;
	int m_Standard;
	double m_DetectPoint[4][4];
	
	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;

private:


public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_ctrlDetectPoint[4][4];
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
