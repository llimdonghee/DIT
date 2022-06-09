#pragma once

#include "Graph/akGraphLinker.h"
#include "Graph/akResource.h"


#define RANGESETTINGTIMEUPDATE WM_USER+0x107

class CakRangeSettingTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CakRangeSettingTimeDlg)

public:
	CakRangeSettingTimeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CakRangeSettingTimeDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_AKRANGESETTINGDLG_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void SetUpdateData(bool bToVariable); //true --> 변수로 저장, false-->다이얼로그에 표시
	void SetTitle(char* strTitle);
	void SetReverse(bool bReverse);
public:
	int m_RangeMin;
	int m_RangeMax;

	CFont m_fontTitle, m_fontText;
	CBrush m_bruTitle, m_bruText;

private:
	CString m_strTitle;
	CWnd* m_pParent;
	bool m_bReverse;//최대, 최소 반대로 설정


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL DestroyWindow();

};
