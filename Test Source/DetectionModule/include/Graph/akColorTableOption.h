#pragma once

//#define IDD_AKCOLORTABLEOPTIONDLG 11000

#include "akGraph/akResource.h"
#include "akGraph/akGraphBase.h"
#include "afxwin.h"

// CakColorTableOptionDlg 대화 상자입니다.
#define COLORTABLEOPTIONUPDATE WM_USER+0x100



class CakColorTableOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CakColorTableOptionDlg)

public:
	CakColorTableOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CakColorTableOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AKCOLORTABLEOPTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:
	void SetData(int index, int level, int min, int max, bool inverse, bool interpolation, bool colorauto);
	
public:
	CWnd* m_pParent;

	CComboBox	m_ctrlColorIndex;
	CComboBox	m_ctrlColorLevel;
	CButton		m_ctrlCheckInverse;
	CButton		m_ctrlCheckInterpolation;
	CEdit		m_ctrlEditMin;
	CEdit		m_ctrlEditMax;
	


	int m_index;
	int m_level;
	int m_colorMin;
	int m_colorMax;
	BOOL m_colorInverse;
	BOOL m_colorInterpolation;
	BOOL m_colorAuto;
	int m_colorMinAuto;
	int m_colorMaxAuto;

	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;
	
private:


public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CButton m_ctrlColorAuto;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
