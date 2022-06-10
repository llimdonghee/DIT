
#pragma once
// DlgGraphView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphView dialog
#include "BGSetDlg.h"
#include "akWndArrange.h"

#define CHART_TYPEGRAPHCOUNT		6
#define CHART_STACKGRAPHCOUNT		4
#define CHART_SIZEGRAPHCOUNT		4
#define CHART_JUDGEMENTGRAPHCOUNT	3
#define MAX_TRENDCOUNT				50


class	CDetectionModuleDlg;

#include "akGraphAOI.h"
#include "../Resource.h"

class CDlgGraphView : public CBGSetDlg
{
	// Construction
public:
	CDlgGraphView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGraphView();

	// Dialog Data
		//{{AFX_DATA(CDlgGraphView)
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GRAPH };
//#endif
	//CKTChartViewer	m_ChartTrend;
	//}}AFX_DATA

public:
	void createGraph();
public:
	CakGraphAOI		m_akgDefect;
	CakWndArrange	m_WndArrange;

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDlgGraphView)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphView)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDetectionModuleDlg*		m_pView;
	int*						m_pCurTrendType;

	int							m_nGraphColor[CHART_TYPEGRAPHCOUNT];
	double						m_dDefectCount[CHART_TYPEGRAPHCOUNT][MAX_TRENDCOUNT];
	char						m_strLabels[MAX_TRENDCOUNT][50];

public:
	void						ResetDefectCount();
	void						SetGraphView(CDetectionModuleDlg* pView);
	void						InitDefectView();

};