// DlgGraphView.cpp : implementation file
//
#include "stdafx.h"
#include "DlgGraphView.h"
#include "../../DetectionModule/DetectionModuleDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphView dialog


CDlgGraphView::CDlgGraphView(CWnd* pParent /*=NULL*/)
	: CBGSetDlg(CDlgGraphView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//그래프 색 설정
	m_nGraphColor[0] = COLOR_4;		// TB
	m_nGraphColor[1] = COLOR_5;		// TW
	m_nGraphColor[2] = COLOR_2;		// RB
	m_nGraphColor[3] = COLOR_3;		// RW
	m_nGraphColor[4] = COLOR_6;		// MD
	m_nGraphColor[5] = COLOR_7;		// CD

	for (int i = 0; i < CHART_TYPEGRAPHCOUNT; i++ )
	{
		for (int j = 0; j < MAX_TRENDCOUNT; j++)
			m_dDefectCount[i][j] = 0;
	}

	m_pCurTrendType = NULL;
}

CDlgGraphView::~CDlgGraphView()
{
	
}

void CDlgGraphView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphView)
	//DDX_Control(pDX, IDC_STATIC_CHARTDEFECT, m_ChartTrend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphView, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphView)
	
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphView message handlers

void CDlgGraphView::OnOK()
{
//	CDialog::OnOK();
}

void CDlgGraphView::OnCancel()
{
//	CDialog::OnCancel();
}

void CDlgGraphView::SetGraphView(CDetectionModuleDlg* pView)
{
	m_pView = pView;
}

void CDlgGraphView::InitDefectView()
{
	createGraph();
}

void CDlgGraphView::ResetDefectCount()
{
	for (int j = 0; j < 50; j++)
	{
		for( int i = 0; i < CHART_TYPEGRAPHCOUNT; i++ )
			m_dDefectCount[i][j] = 0;
	}
}

#define DEFECT_GRAPH_W			1000
#define DEFECT_GRAPH_H			1000
#define GRAPH_BG_COLOR			RGB(146, 136, 86)
#define GRAPH_TITLE_COLOR		RGB(255, 255, 255)
#define GRAPH_ZONE_COLOR		RGB(0, 0, 0)
#define GRAPH_STEPLINE_COLOR	RGB(10, 250, 10)

HBRUSH CDlgGraphView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CDlgGraphView::OnEraseBkgnd(CDC* pDC)
{
	return __super::OnEraseBkgnd(pDC);
}

void CDlgGraphView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return __super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDlgGraphView::createGraph()
{
	CakGraphAOI* pGraph = &m_akgDefect;
	{
		CRect rectTemp;
		GetDlgItem(IDC_AOI_GRAPHAREA1)->GetWindowRect(rectTemp);
		ScreenToClient(rectTemp);
		//rectTemp.SetRect(0, 0, DEFECT_GRAPH_W , DEFECT_GRAPH_H);
		pGraph->CreateGraph(this, rectTemp);
		pGraph->SetTitle("Graph");
		pGraph->m_LegendOption.m_bEnable = true;

		pGraph->ShowWindow(SW_SHOW);

		//pGraph->setDataNum(6, MAX_TRENDCOUNT);
		pGraph->setAxisXStringNum(MAX_TRENDCOUNT);
	}
}

void CDlgGraphView::OnSize(UINT nType, int cx, int cy)
{
	CBGSetDlg::OnSize(nType, cx, cy);
	m_WndArrange.process(cx,cy);

	m_akgDefect.ReDraw(true);
}

BOOL CDlgGraphView::OnInitDialog()
{
	CBGSetDlg::OnInitDialog();

	InitDefectView();

	m_WndArrange.setParentWnd(this);
	m_WndArrange.addChildWnd(&m_akgDefect, WA_RESIZE_WIDTH | WA_RESIZE_HEIGHT);
	return TRUE;  // return TRUE unless you set the focus to a control
}