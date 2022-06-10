#include "stdafx.h"
#include "framework.h"
#include "DetectionModule.h"
#include "DetectionModuleDlg.h"
#include "afxdialogex.h"

#include "ImageWnd.h"
#include "GraphView/DlgGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDetectionModuleDlg::CDetectionModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DETECTIONMODULE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Load
	m_pImageWnd = NULL;
	m_pParam = NULL;
	m_strLoadImageFilePath.Format("");
	m_fZoomRate = 6.0f;
	m_iplLoadImage = m_iplProcImage = NULL;
	m_iDefectCount = m_iPixelCount = 0;

	m_pDlgGraphView = NULL;
	m_ctrlGrid_DefectList.DeleteAllItems();
}

void CDetectionModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_ctrlImageView);
	DDX_Text(pDX, IDC_EDIT_ROI_LEFT, m_iROI_Left);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT, m_iROI_Right);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP, m_iROI_Top);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM, m_iROI_Bottom);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_LOW, m_iThresholdLow);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_HIGH, m_iThresholdHigh);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_LOW2, m_iThresholdLow2);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_HIGH2, m_iThresholdHigh2);
	DDX_Text(pDX, IDC_EDIT_MIN_SIZE, m_iMinSize);
	DDX_Text(pDX, IDC_EDIT_MERGE_DISTANCE, m_iMergeDist);
	DDX_Text(pDX, IDC_EDIT_VERTICAL_DISTANCE, m_iVDist);
	DDX_Text(pDX, IDC_EDIT_HORIZONTAL_DISTANCE, m_iHDist);
	DDX_Control(pDX, IDC_CHECK_HORIZONTAL_COMPARE, m_EnableHorizontalComp);
	DDX_Control(pDX, IDC_CHECK_DIAGONAL, m_EnableDiagonalComp);
	DDX_Control(pDX, IDC_GRID_DEFECTLIST, m_ctrlGrid_DefectList);
	DDX_Control(pDX, IDC_GRID_CONDITION, m_ctrlGrid_Condition);
}

BEGIN_MESSAGE_MAP(CDetectionModuleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CDetectionModuleDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION, &CDetectionModuleDlg::OnBnClickedButtonInspection)
	ON_BN_CLICKED(IDC_SET, &CDetectionModuleDlg::OnBnClickedSet)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GRID_DEFECTLIST, &CDetectionModuleDlg::OnLvnItemchangedDefectGrid)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL CDetectionModuleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// 버전 관리 규칙 : [주,부,수]
	// 주(Major) = 기존버전에서 API가 변경되어 호환이 되지 않는 경우
	// 부(Minor) = 기존버전과 호환되면서 기능이 추가될 경우
	// 수(Patch) = 기존버전과 호환되면서 버그를 수정할 경우
	
	CString strProgramName = "";
	CString strProgramVersion = "";
	CString strSetWindowText = "";

	strProgramName.Format("Detection Modular");
	strProgramVersion.Format("1.0.0");
	strSetWindowText.Format("%s [%s]", strProgramName, strProgramVersion);

	SetWindowText(strSetWindowText);

	CRect rect;
	m_ctrlImageView.GetClientRect(rect);

	m_DetectionAlgorithm = new CDetectionAlgorithm;
	m_pParam = (&m_DetectionAlgorithm->m_Param);
	LoadParam();

	m_pImageWnd = new CImageViewWnd;
	m_pImageWnd->m_pDetectionAlgorithm = m_DetectionAlgorithm;
	m_pImageWnd->m_pDlgDetectionModule = this;
	m_pImageWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, rect, &m_ctrlImageView, 0x77);
	m_pImageWnd->ShowWindow(SW_SHOW);
	m_pImageWnd->UpdateWindow();

	InitTabControl();
	InitDefectGridControl();
	InitConditionGridControl();
	GetGridParam();
	return TRUE;
}

void CDetectionModuleDlg::Clear()
{
	//DestroyWindow();
	m_DetectionAlgorithm->Clear();

	GV_ITEM Item;
	CString strValue;
	Item.mask = GVIF_TEXT;

	CGridCtrl* pGrid = &m_ctrlGrid_DefectList;

	int i = pGrid->GetFixedRowCount();
	for (i; i < pGrid->GetRowCount(); i++)
	{
		for (int j = pGrid->GetFixedColumnCount(); j < pGrid->GetColumnCount(); j++)
		{
			Item.row = i;
			Item.col = j;
			Item.strText = "";
			pGrid->SetItemText(i, j, strValue);
		}
	}
	pGrid->Invalidate(FALSE);
}
BOOL CDetectionModuleDlg ::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CDetectionModuleDlg::PreCreateWindow(cs);
}

void CDetectionModuleDlg::InitTabControl()
{
	m_pDlgGraphView = new CDlgGraphView;
	m_pDlgGraphView->Create(CDlgGraphView::IDD, this);
	m_pDlgGraphView->SetGraphView(this);

	CRect rtSubTab;
	GetDlgItem(IDC_STATIC_SUB_TAB)->GetWindowRect(rtSubTab);
	ScreenToClient(rtSubTab);
	m_pDlgGraphView->MoveWindow(rtSubTab.left, rtSubTab.top, rtSubTab.Width(), rtSubTab.Height());
	m_pDlgGraphView->ShowWindow(SW_SHOW);
}

void CDetectionModuleDlg::InitDefectGridControl()
{
	m_ctrlGrid_DefectList.GetDefaultCell(FALSE, FALSE)->SetBackClr(GRID_COLOR);
	m_ctrlGrid_DefectList.SetFixedBkColor(GRID_FIX_COLOR);
	m_ctrlGrid_DefectList.SetFixedTextColor(GRID_TEXT_COLOR);
	m_ctrlGrid_DefectList.SetGridLineColor(GRID_LINE_COLOR);
	m_ctrlGrid_DefectList.SetGridLines(GVL_BOTH);		//행, 열 확장시 스크롤 생성

	m_ctrlGrid_DefectList.SetColumnCount(8);			//행 갯수
	m_ctrlGrid_DefectList.SetRowCount(MAX_DEFECT_CNT);			//열 갯수
	m_ctrlGrid_DefectList.SetFixedRowCount(1);			//윗쪽부터 N열까지 고정 열 설정
	m_ctrlGrid_DefectList.SetFixedColumnCount(0);		//왼쪽부터 N행까지 고정 행 설정
	//m_ctrlGrid_DefectList.ExpandColumnsToFit();		//열방향 확장하지 않고 고정

	CFont *pFont = m_ctrlGrid_DefectList.GetFont();
	if (!pFont)
		return;

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfItalic = 0;
	lf.lfHeight = 15;
	lf.lfWeight = FW_NORMAL;
	strcpy_s(lf.lfFaceName, _T("Arial"));

	m_ctrlGrid_DefectList.GetDefaultCell(TRUE, FALSE)->SetFont(&lf);
	m_ctrlGrid_DefectList.GetDefaultCell(FALSE, TRUE)->SetFont(&lf);
	m_ctrlGrid_DefectList.GetDefaultCell(TRUE, TRUE)->SetFont(&lf);

	m_ctrlGrid_DefectList.SetEditable(TRUE);			//Grid 내용 수정 가능여부
	m_ctrlGrid_DefectList.EnableSelection(FALSE);

	FixDefectListGrid();
}

void CDetectionModuleDlg::FixDefectListGrid()
{
	CString str;
	int nRow, nCol;
	GV_ITEM Item;

	//////////////////////////////////////////////////////////////////////////
	// 행 방향 TEXT
	nRow = nCol = 0;

	Item.mask = GVIF_TEXT;
	Item.row = nRow;
	Item.col = nCol++;
	Item.strText = "Num";
	m_ctrlGrid_DefectList.SetItem(&Item);
	Item.col = nCol++;
	Item.strText = "x";
	m_ctrlGrid_DefectList.SetItem(&Item);
	Item.col = nCol++;
	Item.strText = "y";
	m_ctrlGrid_DefectList.SetItem(&Item);
	Item.col = nCol++;
	Item.strText = "Area";
	m_ctrlGrid_DefectList.SetItem(&Item);

	//m_ctrlGrid_DefectList.ExpandRowsToFit(); //쓰지않음
}

void CDetectionModuleDlg::UpdateDefectDataGrid(CDefectFeature* pDefectFeature, int iCnt)
{
	CString str;
	int nRow, nCol;
	int iSubIndex = 0;
	//////////////////////////////////////////////////////////////////////////
	// 열 방향 TEXT
	// 고정 TEXT 기재
	nCol = 0;
	nRow = 0;	//Data Set하려는 시작지점
	for (int i = 0; i < iCnt; i++)
	{
		int iIdx = i + 1;
		CDefectFeature* pDefect = &(pDefectFeature[i]);
		
		if (pDefect == NULL)
			continue;

		str.Format("%d", iIdx);	//Data 읽어서 뿌리기
		m_ctrlGrid_DefectList.SetItemText(iIdx, nRow, str);

		iSubIndex = 1;

		str.Format("%d", pDefect->iCenterx);
		m_ctrlGrid_DefectList.SetItemText(iIdx, iSubIndex++, str);

		str.Format("%d", pDefect->iCentery);
		m_ctrlGrid_DefectList.SetItemText(iIdx, iSubIndex++, str);

		str.Format("%d", pDefect->iArea);
		m_iPixelCount += pDefect->iArea;
		m_ctrlGrid_DefectList.SetItemText(iIdx, iSubIndex++, str);
	}

	m_ctrlGrid_DefectList.Invalidate(FALSE);
}

void CDetectionModuleDlg::InitConditionGridControl()
{
	m_ctrlGrid_Condition.GetDefaultCell(FALSE, FALSE)->SetBackClr(GRID_COLOR);
	m_ctrlGrid_Condition.SetFixedBkColor(GRID_FIX_COLOR);
	m_ctrlGrid_Condition.SetFixedTextColor(GRID_TEXT_COLOR);
	m_ctrlGrid_Condition.SetGridLineColor(GRID_LINE_COLOR);
	m_ctrlGrid_Condition.SetGridLines(GVL_BOTH);		//행, 열 확장시 스크롤 생성

	m_ctrlGrid_Condition.SetColumnCount(3);				//행 갯수
	m_ctrlGrid_Condition.SetRowCount(8);				//열 갯수
	m_ctrlGrid_Condition.SetFixedRowCount(1);			//윗쪽부터 N열까지 고정 열 설정
	m_ctrlGrid_Condition.SetFixedColumnCount(1);		//왼쪽부터 N행까지 고정 행 설정
	m_ctrlGrid_DefectList.ExpandColumnsToFit();			//열방향 확장하지 않고 고정
	//m_ctrlGrid_DefectList.ExpandRowsToFit();			//행방향 확장하지 않고 고정

	CFont *pFont = m_ctrlGrid_Condition.GetFont();
	if (!pFont)
		return;

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfItalic = 0;
	lf.lfHeight = 15;
	lf.lfWeight = FW_NORMAL;
	strcpy_s(lf.lfFaceName, _T("Arial"));

	m_ctrlGrid_Condition.GetDefaultCell(TRUE, FALSE)->SetFont(&lf);
	m_ctrlGrid_Condition.GetDefaultCell(FALSE, TRUE)->SetFont(&lf);
	m_ctrlGrid_Condition.GetDefaultCell(TRUE, TRUE)->SetFont(&lf);

	m_ctrlGrid_Condition.SetEditable(TRUE);			//Grid 내용 수정 가능여부
	m_ctrlGrid_Condition.EnableSelection(TRUE);

	FixConditionGrid();
}

void CDetectionModuleDlg::FixConditionGrid()
{
	CString str;
	int nRow, nCol;
	GV_ITEM Item;

	//////////////////////////////////////////////////////////////////////////
	// 행 방향 TEXT
	nRow = nCol = 0;
	Item.mask = GVIF_TEXT;
	Item.row = nRow++;
	Item.col = nCol;
	Item.strText = "";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Horizontal";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Vertical";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Distance";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Threshold";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Threshold2";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Min Size";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText = "Merge Dist";
	m_ctrlGrid_Condition.SetItem(&Item);
	//m_ctrlGrid_DefectList.ExpandRowsToFit(); //쓰지않음

	//////////////////////////////////////////////////////////////////////////
	// 열 방향 TEXT
	nRow = 0;
	Item.row = nRow;
	Item.col = nCol++;
	Item.strText = "";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText = "1";
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText = "2";
	m_ctrlGrid_Condition.SetItem(&Item);
}

void CDetectionModuleDlg::SetGridParam()
{
	CString str;
	int nRow, nCol;
	GV_ITEM Item;
	CGridCtrl* gridCtrl = &m_ctrlGrid_Condition;
	//////////////////////////////////////////////////////////////////////////
	nRow = nCol = 1;
	Item.mask = GVIF_TEXT;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iROI_Left = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.col = nCol++;
	m_iROI_Right = atoi(gridCtrl->GetItemText(Item.row, Item.col));

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iROI_Top = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.col = nCol++;
	m_iROI_Bottom = atoi(gridCtrl->GetItemText(Item.row, Item.col));

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iVDist = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.col = nCol++;
	m_iHDist = atoi(gridCtrl->GetItemText(Item.row, Item.col));

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iThresholdLow = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.col = nCol++;
	m_iThresholdHigh = atoi(gridCtrl->GetItemText(Item.row, Item.col));

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iThresholdLow2 = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.col = nCol++;
	m_iThresholdHigh2 = atoi(gridCtrl->GetItemText(Item.row, Item.col));

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	m_iMinSize = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	Item.row = nRow++;
	m_iMergeDist = atoi(gridCtrl->GetItemText(Item.row, Item.col));
	//////////////////////////////////////////////////////////////////////////
}

void CDetectionModuleDlg::GetGridParam()
{
	CString str;
	int nRow, nCol;
	GV_ITEM Item;

	//////////////////////////////////////////////////////////////////////////
	nRow = nCol = 1;
	Item.mask = GVIF_TEXT;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iROI_Left);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText.Format("%d", m_iROI_Right);
	m_ctrlGrid_Condition.SetItem(&Item);
	
	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iROI_Top);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText.Format("%d", m_iROI_Bottom);
	m_ctrlGrid_Condition.SetItem(&Item);

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iVDist);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText.Format("%d", m_iHDist);
	m_ctrlGrid_Condition.SetItem(&Item);
	
	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iThresholdLow);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText.Format("%d", m_iThresholdHigh);
	m_ctrlGrid_Condition.SetItem(&Item);

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iThresholdLow2);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.col = nCol++;
	Item.strText.Format("%d", m_iThresholdHigh2);
	m_ctrlGrid_Condition.SetItem(&Item);

	nCol = 1;
	Item.row = nRow++;
	Item.col = nCol++;
	Item.strText.Format("%d", m_iMinSize);
	m_ctrlGrid_Condition.SetItem(&Item);
	Item.row = nRow++;
	Item.strText.Format("%d", m_iMergeDist);
	m_ctrlGrid_Condition.SetItem(&Item);
	//////////////////////////////////////////////////////////////////////////
}

void CDetectionModuleDlg::SetDialogColor()
{
	m_pDlgGraphView->SetBGColor(BG_COLOR);
	m_pDlgGraphView->SetCtrlColor(FONT_COLOR);
	m_pDlgGraphView->SetBtnColor(TAB_BTN_COLOR1, TAB_BTN_COLOR2);
}

void CDetectionModuleDlg::OnBnClickedButtonLoadImage()
{
	m_strLoadImageFilePath.Format("");
	CFileDialog dlg(TRUE, "", "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		CString strPath;
		strPath.Format("%s", dlg.GetPathName());

		LoadFrameImage(strPath);
		m_strLoadImageFilePath = strPath;
	}
}

void CDetectionModuleDlg::OnBnClickedSet()
{
	SetGridParam();
	ApplyParam();
}

void CDetectionModuleDlg::OnBnClickedButtonInspection()
{
	if (m_iplProcImage == NULL)
	{
		CString str;
		str.Format("%s", "Selete file for inspection!");
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}

	/*if (m_bImageProcessing == TRUE)
	{
		CString str;
		str.Format("%s", "Already inspection processing!");
		DisplayStatus(str);
		return;
	}*/

	m_pImageWnd->ResetImage();
	m_DetectionAlgorithm->Clear();
	//m_ctrlDefectList.DeleteAllItems();
	m_iDefectCount = 0;
	m_iPixelCount = 0;

	ApplyParam();

	UpdateData(FALSE);

	DisplayStatus("Inspection Start");

	DWORD dwThreadId = 0;

	CloseHandle(CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		ThreadProc,				// thread function name
		(PVOID)this,            // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId));			// returns the thread identifier
}

BOOL CDetectionModuleDlg::LoadFrameImage(CString strPath)
{
	Clear();

	CFileFind ff;
	if (ff.FindFile(strPath) == FALSE)
		return FALSE;
	ff.Close();

	if (m_iplLoadImage != NULL)
	{
		cvReleaseImage(&m_iplLoadImage);
		m_iplLoadImage = NULL;
	}

	if (m_iplProcImage != NULL)
	{
		cvReleaseImage(&m_iplProcImage);
		m_iplProcImage = NULL;
	}

	m_strImagePath.Format("%s", strPath);

	IplImage* image = cvLoadImage((LPSTR)(LPCTSTR)strPath, CV_LOAD_IMAGE_GRAYSCALE);

	CvRect rt;
	rt.x = 0;
	rt.y = 0;
	rt.width = image->width / SSE_PROCESS_PIXELS * SSE_PROCESS_PIXELS;
	rt.height = image->height;

	cvSetImageROI(image, rt);

	if (image == NULL)
	{
		DisplayStatus("Load Image Fail");
		return FALSE;
	}

	m_iplLoadImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopyImage(image, m_iplLoadImage);
	m_iplProcImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopyImage(image, m_iplProcImage);

	FCObjImage fcimage;
	fcimage.Create(image->widthStep, image->height, 8);
	cvFlip(image, image, 0);
	memcpy(fcimage.GetMemStart(), image->imageData, image->widthStep * image->height);
	
	m_pImageWnd->DrawImage(&fcimage, m_fZoomRate);
	
	fcimage.Destroy();

	cvResetImageROI(image);
	cvReleaseImage(&image);
}

void CDetectionModuleDlg::DisplayStatus(CString strText)
{
	SetDlgItemText(IDC_STATIC_TEXT, " " + strText);
}

void CDetectionModuleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDetectionModuleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CDetectionModuleDlg::DestroyWindow()
{
	if (m_pDlgGraphView)
	{
		m_pDlgGraphView->DestroyWindow();
		delete m_pDlgGraphView;
		m_pDlgGraphView = NULL;
	}

	if (m_pImageWnd)
	{
		m_pImageWnd->DestroyWindow();
		delete m_pImageWnd;
		m_pImageWnd = NULL;
	}

	return CDialog::DestroyWindow();
}

void CDetectionModuleDlg::ZoomRateValue(float fZoom)
{
	m_fZoomRate = fZoom;
}

void CDetectionModuleDlg::SetRectCoordinate(CRect rect)
{
	m_rtDisplayRoiCoordinate.left = rect.left;
	m_rtDisplayRoiCoordinate.right = rect.right;
	m_rtDisplayRoiCoordinate.top = rect.top;
	m_rtDisplayRoiCoordinate.bottom = rect.bottom;

	SetDlgItemInt(IDC_EDIT_ROI_LEFT, m_rtDisplayRoiCoordinate.left);
	SetDlgItemInt(IDC_EDIT_ROI_RIGHT, m_rtDisplayRoiCoordinate.right);
	SetDlgItemInt(IDC_EDIT_ROI_TOP, m_rtDisplayRoiCoordinate.top);
	SetDlgItemInt(IDC_EDIT_ROI_BOTTOM, m_rtDisplayRoiCoordinate.bottom);

	m_iROI_Left = m_rtDisplayRoiCoordinate.left;
	m_iROI_Right = m_rtDisplayRoiCoordinate.right;
	m_iROI_Top = m_rtDisplayRoiCoordinate.top;
	m_iROI_Bottom = m_rtDisplayRoiCoordinate.bottom;
}

void CDetectionModuleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/*CWnd* pCtl = GetDlgItem(IDC_STATIC_IMAGE);

	if (!pCtl) { return; }

	CRect rectCtl;
	pCtl->GetWindowRect(&rectCtl);
	ScreenToClient(&rectCtl);

	pCtl->MoveWindow(rectCtl.left, rectCtl.top, cx - 2 * rectCtl.left, cy - rectCtl.top - rectCtl.left, TRUE);
	m_ctrlImageView.GetClientRect(rectCtl);
	
	m_pImageWnd = NULL;
	m_pImageWnd = new CImageViewWnd;
	m_pImageWnd->m_pDetectionAlgorithm = m_DetectionAlgorithm;
	m_pImageWnd->m_pDlgDetectionModule = this;
	m_pImageWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, rectCtl, &m_ctrlImageView, 0x77);
	m_pImageWnd->ShowWindow(SW_SHOW);
	m_pImageWnd->UpdateWindow();
	*/
	return;
}

void CDetectionModuleDlg::LoadParam()
{
	if (m_pParam != NULL)
	{
		m_iVDist = m_pParam->m_iVDist;
		m_iHDist = m_pParam->m_iHDist;
		m_bMultiThreshold = m_pParam->m_bMultiThreshold;
		m_iMultiTHGray = m_pParam->m_iMultiTHGray;
		m_iThresholdLow = m_pParam->m_iThresholdLow, m_iThresholdHigh = m_pParam->m_iThresholdHigh;
		m_iThresholdLow2 = m_pParam->m_iThresholdLow2, m_iThresholdHigh2 = m_pParam->m_iThresholdHigh2;
		m_iMinSize = m_pParam->m_iMinSize;
		m_iMaxSize = m_pParam->m_iMaxSize;
		m_iMergeDist = m_pParam->m_iMergeDist;
		m_bHoriCompare = m_pParam->m_bHoriCompare;
		m_bDiagonalCompare = m_pParam->m_bDiagonalCompare;
		m_rtROI = m_pParam->m_rtROI;

		m_iROI_Left = m_rtROI.left;
		m_iROI_Right = m_rtROI.right;
		m_iROI_Top = m_rtROI.top;
		m_iROI_Bottom = m_rtROI.bottom;
		m_dFOV = m_pParam->m_fFOV;
		m_dPixelsize = m_pParam->m_fPixelsize;

		UpdateData(FALSE);
	}
}

void CDetectionModuleDlg::ApplyParam()
{
	if (m_pParam != NULL)
	{
		UpdateData(FALSE);

		m_pParam->m_iVDist = m_iVDist, m_pParam->m_iHDist = m_iHDist;
		//m_pParam->m_bMultiThreshold = m_bMultiThreshold = m_EnableMultiThresholdCheckBox.GetCheck();
		m_pParam->m_iMultiTHGray = m_iMultiTHGray;
		m_pParam->m_iThresholdLow = m_iThresholdLow, m_pParam->m_iThresholdHigh = m_iThresholdHigh;
		m_pParam->m_iThresholdLow2 = m_iThresholdLow2, m_pParam->m_iThresholdHigh2 = m_iThresholdHigh2;
		m_pParam->m_iMinSize = m_iMinSize;
		m_pParam->m_iMaxSize = m_iMaxSize;
		m_pParam->m_iMergeDist = m_iMergeDist;
		m_pParam->m_bHoriCompare = m_bHoriCompare;
		m_pParam->m_bDiagonalCompare = m_bDiagonalCompare;
		m_pParam->m_bHoriCompare = IsDlgButtonChecked(IDC_CHECK_HORIZONTAL_COMPARE);
		m_pParam->m_bDiagonalCompare = IsDlgButtonChecked(IDC_CHECK_DIAGONAL);

		m_rtROI.left = m_iROI_Left;
		m_rtROI.right = m_iROI_Right;
		m_rtROI.top = m_iROI_Top;
		m_rtROI.bottom = m_iROI_Bottom;
		m_pParam->m_rtROI = m_rtROI;

		m_pParam->m_fFOV = m_dFOV;
		m_pParam->m_fPixelsize = m_dPixelsize;

		m_pParam->SaveParam(DETECT_PARAM_PATH);
	}
}

void CDetectionModuleDlg::Inspection(int iScanNo, int iFrameNo)
{
	m_pParam->m_rtROI = m_rtROI;

	m_DetectionAlgorithm->Inspection(reinterpret_cast<BYTE*>(m_iplProcImage->imageData),
		reinterpret_cast<BYTE*>(m_iplProcImage->imageData),
		reinterpret_cast<BYTE*>(m_iplProcImage->imageData),
		m_iplProcImage->widthStep, m_iplProcImage->height);

	UpdateDefectDataGrid(m_DetectionAlgorithm->m_DefectFeature, m_DetectionAlgorithm->m_iDefectCnt);
	//SetDefect(m_DetectionAlgorithm->m_DefectFeature, m_DetectionAlgorithm->m_iDefectCnt);

	m_pImageWnd->Invalidate();
}

DWORD WINAPI CDetectionModuleDlg::ThreadProc(__in  LPVOID lpParameter)
{
	CDetectionModuleDlg* pDlg = (CDetectionModuleDlg*)lpParameter;

	if (pDlg != NULL)
	{
		int iScaneNo = 0;
		int iFrameNo = 0;
		pDlg->DisplayStatus("Inspection Start");

		pDlg->Inspection(iScaneNo, iFrameNo);

		pDlg->DisplayStatus("Inspection End");
	}

	return 0;
}

void CDetectionModuleDlg::DefectMoveToPoint(CPoint ptPos, CSize szSize)
{
	if (m_pImageWnd != NULL)
	{
		int iMargin = 10;

		m_pImageWnd->MoveToPoint(ptPos, TRUE, TRUE);
	}
}

void CDetectionModuleDlg::OnDropFiles(HDROP hDropInfo)
{
	Clear();

	int nFiles;
	char szPathName[MAX_PATH];
	CString strFileName;

	nFiles = ::DragQueryFile(hDropInfo, 0, szPathName, MAX_PATH);

	CString strPath;
	strPath.Format("%s", szPathName);

	if (strPath.Right(3) == "bmp")
	{
		m_strLoadImageFilePath.Format("");
		LoadFrameImage(strPath);
		m_strLoadImageFilePath = strPath;
	}
	else if (strPath.Right(3) == "txt")
	{
		LoadRecipeParameter(strPath);
		ApplyParam();
	}
	else
	{
		m_DetectionAlgorithm->Clear();
	}

	::DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CDetectionModuleDlg::OnLvnItemchangedDefectGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nY = pNMLV->iItem;			// 열 수
	int nX = pNMLV->iSubItem;		// 행 수

	if (nY > -1 && m_DetectionAlgorithm->m_iDefectCnt > 0)
	{
		BOOL bOk = FALSE;
		CGridCtrl& gridCtrl = m_ctrlGrid_DefectList;
		CPoint ptPos;
		CSize szSize;

		ptPos.x = atoi(gridCtrl.GetItemText(nY, 1));
		ptPos.y = atoi(gridCtrl.GetItemText(nY, 2));
		
		DefectMoveToPoint(ptPos, szSize);
	}

	*pResult = 0;
}

CString CDetectionModuleDlg::GetRecipeData(CString strRecipeFileName, CString strStdData, int &nSplitBuffer, int &nLastBuffer)
{
	CString line, strFullData;
	CStdioFile file;
	int nTemp = 0;

	if (!file.Open((LPCTSTR)(strRecipeFileName), CFile::modeRead | CFile::typeText))
	{
		int ErrorNum = ::GetLastError();

		return false;
	}

	while (NULL != file.ReadString(line))
	{
		if (line.GetLength() <= 0)
			continue;
		AfxExtractSubString(strFullData, line, 0, '=');
		strFullData.TrimRight();
		if (0 == strStdData.Compare(strFullData))
		{
			AfxExtractSubString(strFullData, line, 1, '=');
			nSplitBuffer = strFullData.Find(',', 0);
			nTemp = strFullData.GetLength();
			nLastBuffer = nTemp - nSplitBuffer - 1;
			return strFullData;
		}
	}
	return false;
}

void CDetectionModuleDlg::LoadRecipeParameter(CString strFileName)
{
	CString strTemp;
	int nSplitBuffer = 0, nLastBuffer = 0;
	int nThresholdLow = 0, nThresholdHigh = 0, nThreshold2Low = 0, nThreshold2High = 0, nDistV = 0, nDistH = 0, nMinSize = 0, nMaxSize = 9999, nMergeDist = 0;
	int nEdgeJudgment = 0, nEdgeSlopeTh = 0, nPeriodSlopeTh = 0, nPeriodSlopeJudgment = 0;
	int nMultiThresholdEnable = 0, nEnableHorComp = 0, nEnableDiagonal = 0;
	int nEnableEdgeSlope = 0, nEnablePeriodSlope = 0;

	TCHAR buf[1024] = { 0, };
	GetPrivateProfileString("EQP PARAMETERS", "ScanResolution", "3.5", buf, 1024, strFileName);
	m_dFOV = (double)_tstof(buf);

	strTemp = GetRecipeData(strFileName, _T("Threshold[0]"), nSplitBuffer, nLastBuffer);
	nThresholdLow = _ttoi(strTemp.Left(nSplitBuffer));
	nThresholdHigh = _ttoi(strTemp.Right(nLastBuffer));

	SetDlgItemInt(IDC_EDIT_THRESHOLD_LOW, nThresholdLow);
	SetDlgItemInt(IDC_EDIT_THRESHOLD_HIGH, nThresholdHigh);

	strTemp = GetRecipeData(strFileName, _T("Threshold2[0]"), nSplitBuffer, nLastBuffer);
	nThreshold2Low = _ttoi(strTemp.Left(nSplitBuffer));
	nThreshold2High = _ttoi(strTemp.Right(nLastBuffer));

	SetDlgItemInt(IDC_EDIT_THRESHOLD_LOW2, nThreshold2Low);
	SetDlgItemInt(IDC_EDIT_THRESHOLD_HIGH2, nThreshold2High);

	strTemp = GetRecipeData(strFileName, _T("Dist[0]"), nSplitBuffer, nLastBuffer);
	nDistV = _ttoi(strTemp.Left(nSplitBuffer));
	nDistH = _ttoi(strTemp.Right(nLastBuffer));

	SetDlgItemInt(IDC_EDIT_VERTICAL_DISTANCE, nDistV);
	SetDlgItemInt(IDC_EDIT_HORIZONTAL_DISTANCE, nDistH);

	nMinSize = GetPrivateProfileInt("CELL PARAMETERS", "MinSize[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_MIN_SIZE, nMinSize);
}