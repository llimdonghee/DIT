#pragma once

//#define IDD_AKCOLORTABLEOPTIONDLG 11000

#include "akGraph/akResource.h"
#include "akGraph/akGraphBase.h"

// CakRaySettingDlg ��ȭ �����Դϴ�.
#define RAYSETTINGUPDATE WM_USER+0x102



class CakRaySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CakRaySettingDlg)

public:
	CakRaySettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CakRaySettingDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AKRAYESETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void setFAIMS3Mode();
public:
	
public:
	CWnd* m_pParent;

	void SetUpdate(); //���� �����͸� ȭ�鿡 ����

	CEdit		m_ctrlRayNum;
	CEdit		m_ctrlRayMin;
	CEdit		m_ctrlRayMax;
	
	int m_RayNum;
	int m_RayMin;
	int m_RayMax;

	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;
	
private:


public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
