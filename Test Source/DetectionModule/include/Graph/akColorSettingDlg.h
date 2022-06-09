#pragma once




#include "akGraph/akGraphLinker.h"
#include "akGraph/akResource.h"


#define COLORSETTINGUPDATE WM_USER+0x109


class CakColorSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CakColorSettingDlg)

public:
	CakColorSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CakColorSettingDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AKCOLORSETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

public:
	void SetUpdateData(bool bToVariable); //true --> ������ ����, false-->���̾�α׿� ǥ��
	void SetTitle(char* strTitle);
	void SetReverse(bool bReverse);
public:
	COLORREF m_Color;
	int nDataIndex;
public:
	double m_RangeMin;
	double m_RangeMax;
	
	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;
	
	int m_nID;
private:
	CString m_strTitle;
	CWnd* m_pParent;
	bool m_bReverse;//�ִ�, �ּ� �ݴ�� ����


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();
	
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
