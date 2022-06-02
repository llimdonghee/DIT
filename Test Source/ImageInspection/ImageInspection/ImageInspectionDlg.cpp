
// ImageInspectionDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImageInspection.h"
#include "ImageInspectionDlg.h"
#include "DDMCalculator.h"

#include "ImageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CImageInspectionDlg 대화 상자




CImageInspectionDlg::CImageInspectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageInspectionDlg::IDD, pParent)
	, m_iROI_Left(0)
	, m_iROI_Right(12000)
	, m_iROI_Top(0)
	, m_iROI_Bottom(8192)
	, m_bMultiThreshold(FALSE)
	, m_iMultiTHGray(100)
	, m_bHoriCompare(FALSE)
	, m_iMergeDist(0)
	, m_iMinSize(0)
	, m_iMaxSize(9999)
	, m_iHDist(0)
	, m_iVDist(0)
	, m_iThresholdLow(12)
	, m_iThresholdHigh(15)
	, m_iThresholdLow2(12)
	, m_iThresholdHigh2(15)
	, m_bAdjacentSlopeFilter(FALSE)
	, m_iEdgeJudgment(15)
	, m_iEdgeSlopeTh(30)
	, m_bPeriodSlopeFilter(FALSE)
	, m_iPeriodSlopeJudgment(10)
	, m_iPeriodSlopeTh(255)
	, m_dFOV(3.5)
	, m_dPixelsize(100)
	, m_bTrackingFilter(FALSE)
	, m_iTrackingRange(0)
	, m_iTrackingTh(0)
	, m_bNearbyCheckFilter(FALSE)
	, m_bASGInspection(FALSE)
	, m_bShowCheck(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//	m_pDlgCam = NULL;
	m_pImageWnd = NULL;
	m_iplLoadImage = m_iplProcImage = m_iplSubtractLowImage = m_iplSubtractHighImage = NULL;
	m_bImageProcessing = m_bImageCutting = FALSE;
	m_iDefectCount = m_iPixelCount = 0;
	m_iDisplayImageType = 0;
	m_pParam = NULL;
	m_strLoadImageFilePath.Format("");
	m_fZoomRate = 6.0f;

	m_pAutoCalDlg = NULL;

}

CImageInspectionDlg::~CImageInspectionDlg()
{
}

void CImageInspectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_ctrlImageView);
	DDX_Control(pDX, IDC_DEFECT_LIST, m_ctrlDefectList);
	DDX_Text(pDX, IDC_EDIT_MULTI_TH_GRAY, m_iMultiTHGray);
	DDX_Text(pDX, IDC_EDIT_VERTICAL_DISTANCE, m_iVDist);
	DDX_Text(pDX, IDC_EDIT_HORIZONTAL_DISTANCE, m_iHDist);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_LOW, m_iThresholdLow);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_HIGH, m_iThresholdHigh);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_LOW2, m_iThresholdLow2);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_HIGH2, m_iThresholdHigh2);
	DDX_Text(pDX, IDC_EDIT_MIN_SIZE, m_iMinSize);
	DDX_Text(pDX, IDC_EDIT_MAX_SIZE, m_iMaxSize);
	DDX_Text(pDX, IDC_EDIT_MERGE_DISTANCE, m_iMergeDist);
	DDX_Text(pDX, IDC_EDIT_ROI_LEFT, m_iROI_Left);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT, m_iROI_Right);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP, m_iROI_Top);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM, m_iROI_Bottom);
	DDX_Text(pDX, IDC_EDIT_EDGE_JUDGMENT, m_iEdgeJudgment);
	DDX_Text(pDX, IDC_EDIT_EDGE_SLOPE_TH, m_iEdgeSlopeTh);
	DDX_Text(pDX, IDC_EDIT_PERIOD_SLOPE_JUDGMENT, m_iPeriodSlopeJudgment);
	DDX_Text(pDX, IDC_EDIT_PERIOD_SLOPE_TH, m_iPeriodSlopeTh);
	DDX_Text(pDX, IDC_EDIT_FOV, m_dFOV);
	DDX_Text(pDX, IDC_EDIT_CellSize, m_dPixelsize);
	DDX_Control(pDX, IDC_CHECK_MULTI_THRESHOLD, m_EnableMultiThresholdCheckBox);
	DDX_Control(pDX, IDC_CHECK_HORIZONTAL_COMPARE, m_EnableHorCompCheckBox);
	DDX_Control(pDX, IDC_CHECK_ADJACENT_SLOPE_FILTER, m_EnableEdgeSlopeCheckBox);
	DDX_Control(pDX, IDC_CHECK_PERIOD_SLOPE_FILTER, m_EnablePeriodSlopeCheckBox);
	DDX_Control(pDX, IDC_IMAGELIST, m_ImageList);
	DDX_Control(pDX, IDC_CHECK_ASG_INSPECT_ON, m_EnableASGInspectCheckBox);
	DDX_Text(pDX, IDC_EDIT_TRACKING_PIXEL_RANGE, m_iTrackingRange);
	DDX_Text(pDX, IDC_EDIT_TRACKING_THRESHOLD, m_iTrackingTh);
	DDX_Control(pDX, IDC_CHECK_TRACKING_FILTER, m_EnableTrackingFilterCheckBox);
	DDX_Control(pDX, IDC_CHECK_NEARBYCHECK_FILTER, m_EnableNearbyCheckCheckBox);
	DDX_Control(pDX, IDC_CHECK_DIAGONAL, m_EnableDiagonal);
	DDX_Control(pDX, IDC_CHECK_SHOW, m_ctlShow);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_bShowCheck);
	DDX_Control(pDX, IDC_CHECK_MOUNTAIN_FILTER, m_EnableMountainFilterCheckBox);
}

BEGIN_MESSAGE_MAP(CImageInspectionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO1, &CImageInspectionDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_SET, &CImageInspectionDlg::OnBnClickedSet)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CImageInspectionDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION, &CImageInspectionDlg::OnBnClickedButtonInspection)
	ON_BN_CLICKED(IDC_BUTTON_GRAY_DATA_SAVE, &CImageInspectionDlg::OnBnClickedButtonGrayDataSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ROI_IMAGE, &CImageInspectionDlg::OnBnClickedButtonSaveRoiImage)
	ON_BN_CLICKED(IDC_RADIO2, &CImageInspectionDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CImageInspectionDlg::OnBnClickedRadio3)
	ON_NOTIFY(NM_CLICK, IDC_DEFECT_LIST, &CImageInspectionDlg::OnNMClickDefectList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEFECT_LIST, &CImageInspectionDlg::OnLvnItemchangedDefectList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_LOADRECIPE, &CImageInspectionDlg::OnBnClickedButtonLoadrecipe)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RESULT, &CImageInspectionDlg::OnBnClickedButtonSaveResult)
	ON_BN_CLICKED(IDC_BUTTON_LIST, &CImageInspectionDlg::OnBnClickedButtonList)
	ON_LBN_DBLCLK(IDC_IMAGELIST, &CImageInspectionDlg::OnDblclkImagelist)
	ON_LBN_SELCHANGE(IDC_IMAGELIST, &CImageInspectionDlg::OnSelchangeImagelist)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DDMCALC_DLG, &CImageInspectionDlg::OnBnClickedButtonDDMCalc)
	ON_BN_CLICKED(IDC_IMAGE_SIZE_LOAD, &CImageInspectionDlg::OnBnClickedImageSizeLoad)
	ON_BN_CLICKED(IDC_CHECK_SHOW, &CImageInspectionDlg::OnBnClickedCheckShow)
	ON_BN_CLICKED(IDC_ROI_OFFSET_FUNC, &CImageInspectionDlg::OnBnClickedRoiOffsetFunc)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_CAL_PERIOD, &CImageInspectionDlg::OnBnClickedButtonAutoCalPeriod)
	ON_BN_CLICKED(IDC_CHECK_HORIZONTAL_COMPARE, &CImageInspectionDlg::OnBnClickedCheckHorizontalCompare)
END_MESSAGE_MAP()


// CImageInspectionDlg 메시지 처리기
BOOL CImageInspectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 1.0.0 : 02.24 QD Image Inspection Version 관리 시작
	// 1.0.1 : vs2008, vs2013 컴파일 지원
	// 1.0.2 : DDM Calculator 에 LEFT, RIGHT, TOP, BOTTOM 좌표 표시
	///////////////////////////////////////////////////////////////////////////
	// 1.0.0 : CSOT T10

	SetWindowText("ImageInspection [ CSOT T10 Ver. 1.0.0 ]");

	CRect rect;
	m_ctrlImageView.GetClientRect(rect);

	m_DDMAlgorithm = new CDDMAlgorithm;
	m_pParam = (&m_DDMAlgorithm->m_Param);
	LoadParam();

	m_pImageWnd = new CImageViewWnd;
	m_pImageWnd->m_pDDMAlgorithm = m_DDMAlgorithm;	
	m_pImageWnd->m_pDlgInspection = this;
	m_pImageWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL, rect, &m_ctrlImageView, 0x77);
	m_pImageWnd->ShowWindow(SW_SHOW);
	m_pImageWnd->UpdateWindow();


	InitListHeader();

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	DragAcceptFiles();
	m_ctrlImageView.DragAcceptFiles();

	if(m_bAdjacentSlopeFilter)
		CheckDlgButton(IDC_CHECK_ADJACENT_SLOPE_FILTER, TRUE);
	else
		CheckDlgButton(IDC_CHECK_ADJACENT_SLOPE_FILTER, FALSE);

	if(m_bPeriodSlopeFilter)
		CheckDlgButton(IDC_CHECK_PERIOD_SLOPE_FILTER, TRUE);
	else
		CheckDlgButton(IDC_CHECK_PERIOD_SLOPE_FILTER, FALSE);

	if (m_bTrackingFilter)
		CheckDlgButton(IDC_CHECK_TRACKING_FILTER, TRUE);
	else
		CheckDlgButton(IDC_CHECK_TRACKING_FILTER, FALSE);

	if (m_bASGInspection)
		CheckDlgButton(IDC_CHECK_ASG_INSPECT_ON, TRUE);
	else
		CheckDlgButton(IDC_CHECK_ASG_INSPECT_ON, FALSE);

	if (m_bNearbyCheckFilter)
		CheckDlgButton(IDC_CHECK_NEARBYCHECK_FILTER, TRUE);
	else
		CheckDlgButton(IDC_CHECK_NEARBYCHECK_FILTER, FALSE);
	
	if (m_bHoriCompare)
		CheckDlgButton(IDC_CHECK_HORIZONTAL_COMPARE, TRUE);
	else
		CheckDlgButton(IDC_CHECK_HORIZONTAL_COMPARE, FALSE);

	if (m_bDiagonal)
		CheckDlgButton(IDC_CHECK_DIAGONAL, TRUE);
	else
		CheckDlgButton(IDC_CHECK_DIAGONAL, FALSE);

	if (m_bMountainFilter)
		CheckDlgButton(IDC_CHECK_MOUNTAIN_FILTER, TRUE);
	else
		CheckDlgButton(IDC_CHECK_MOUNTAIN_FILTER, FALSE);


	m_pAutoCalDlg = new CDlgAutoCalPeriod(this);
	m_pAutoCalDlg->Create(IDD_DIALOG_AUTO_CAL);
	m_pAutoCalDlg->SetMainDlg((CDialogEx*)this);
	m_pAutoCalDlg->ShowWindow(SW_HIDE);
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImageInspectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageInspectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageInspectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageInspectionDlg::OnBnClickedRadio1()
{
	if(m_iDisplayImageType == 0)
		return;

	m_iDisplayImageType = 0;

	DrawImage(m_iplLoadImage, m_pImageWnd->GetZoomFactor(), m_pImageWnd->GetScrollPosition());	
}

void CImageInspectionDlg::OnBnClickedSet()
{
	ApplyParam();
}

void CImageInspectionDlg::OnBnClickedButtonLoadImage()
{
	m_strLoadImageFilePath.Format("");
	CFileDialog dlg(TRUE, "", "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

	if(dlg.DoModal() == IDOK)
	{
		CString strPath;
		strPath.Format("%s", dlg.GetPathName());

		LoadImage(strPath);
		m_strLoadImageFilePath = strPath;
	}
}

void CImageInspectionDlg::SetDefect(CDefectFeature* pDefectFeature, int iCnt)
{
	LV_ITEM		lvItem;
	CString		str;
	CListCtrl&	listCtrl = m_ctrlDefectList;
	int			iSubIndex = 1;
	m_iPixelCount = 0;

	if(listCtrl.GetItemCount() > 0)
		listCtrl.DeleteAllItems();

	for(int i = 0; i < iCnt; i++)
	{
		CDefectFeature* pDefect = &(pDefectFeature[i]);

		if(pDefect == NULL)
			continue;

		lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lvItem.iSubItem = 0;

		str.Format("%d", i+1);
		listCtrl.InsertItem(i, str);

		iSubIndex = 1;
		//str.Format("%d", pDefect->iDefectIndex);
		//listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iCenterx);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iCentery);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iArea);
		m_iPixelCount += pDefect->iArea;
		listCtrl.SetItemText(i, iSubIndex++, str);

		/*
		str.Format("%.3lf", pDefect->fRealArea);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMinx);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMiny);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMaxx);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMaxy);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMaxx - pDefect->iMinx);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iMaxy - pDefect->iMiny);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%.3lf", (double)MAX(pDefect->iMaxx-pDefect->iMinx, pDefect->iMaxy-pDefect->iMiny)/(double)MIN(pDefect->iMaxx-pDefect->iMinx, pDefect->iMaxy-pDefect->iMiny));
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%.3lf", pDefect->fRealDx);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%.3lf", pDefect->fRealDy);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%.3lf", (double)MAX(pDefect->fRealDx, pDefect->fRealDy)/(double)MIN(pDefect->fRealDx, pDefect->fRealDy));
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iDefectMinGray);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iDefectMaxGray);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iReferenceMinGray);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iReferenceMaxGray);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d (%s)", pDefect->iDefectMaxGray - pDefect->iReferenceMaxGray, pDefect->iDefectMaxGray - pDefect->iReferenceMaxGray > 0 ? "Bright" : "Dark");
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iDefectGraySum);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iReferenceGraySum);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iDefectGraySquareSum);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d", pDefect->iReferenceGraySquareSum);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%d (%.2lf%%)", pDefect->bBubble, pDefect->dBubbleMatchingRate*100.0);
		listCtrl.SetItemText(i, iSubIndex++, str);

		str.Format("%s", pDefect->RTCCode);
		listCtrl.SetItemText(i, iSubIndex++, str);
		*/
	}

	m_iDefectCount = iCnt;
}

void CImageInspectionDlg::Inspection()
{	
	m_dblProcessingTime = 0.0;

	LARGE_INTEGER st, et, frq;
	::QueryPerformanceCounter(&st);

	if(m_bImageCutting)
	{
		m_rtCuttingROI = m_pImageWnd->GetCuttingROIRect();
		m_rtCuttingROI.right = m_rtCuttingROI.right - ((m_rtCuttingROI.right-m_rtCuttingROI.left)%16);

		m_rtROI = m_pParam->m_rtROI;
		m_pParam->m_rtROI = m_rtCuttingROI;
	}
	else
	{
		m_pParam->m_rtROI = m_rtROI;
	}

	m_DDMAlgorithm->Inspection(reinterpret_cast<BYTE*>(m_iplProcImage->imageData),
		reinterpret_cast<BYTE*>(m_iplSubtractLowImage->imageData),
		reinterpret_cast<BYTE*>(m_iplSubtractHighImage->imageData),
		m_iplProcImage->widthStep, m_iplProcImage->height);

	::QueryPerformanceCounter(&et);
	::QueryPerformanceFrequency(&frq);

	m_dblProcessingTime = (((double)et.QuadPart - (double)st.QuadPart)/(double)frq.QuadPart)*1000.0;

	SetDefect(m_DDMAlgorithm->m_DefectFeature, m_DDMAlgorithm->m_iDefectCnt);

	m_pImageWnd->Invalidate();
}


DWORD WINAPI CImageInspectionDlg::ThreadProc(__in  LPVOID lpParameter)
{
	CImageInspectionDlg* pDlg = (CImageInspectionDlg*)lpParameter;

	if(pDlg != NULL)
	{
		pDlg->m_bImageProcessing = TRUE;

		LARGE_INTEGER st, et, frq;
		::QueryPerformanceCounter(&st);

		pDlg->Inspection();

		::QueryPerformanceCounter(&et);
		::QueryPerformanceFrequency(&frq);

		double time = (((double)et.QuadPart - (double)st.QuadPart)/(double)frq.QuadPart)*1000.0;

		pDlg->m_bImageProcessing = FALSE;

		pDlg->DisplayStatus("Inspection End");
	}

	return 0;
}
void CImageInspectionDlg::OnBnClickedButtonInspection()
{

	if(m_iplProcImage == NULL)
	{
		CString str;
		str.Format("%s", "Selete file for inspection!");
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}

	if(m_bImageProcessing == TRUE)
	{
		CString str;
		str.Format("%s", "Already inspection processing!");
		DisplayStatus(str);
		return;
	}

	m_pImageWnd->ResetImage();
	m_DDMAlgorithm->Clear();
	m_ctrlDefectList.DeleteAllItems();
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

void CImageInspectionDlg::AutoCalInspect(BOOL bVer, BOOL bHor, int iPeriod)
{
	if (m_iplProcImage == NULL)
	{
		CString str;
		str.Format("%s", "Selete file for inspection!");
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}

	if (m_bImageProcessing == TRUE)
	{
		CString str;
		str.Format("%s", "Already inspection processing!");
		DisplayStatus(str);
		return;
	}

	m_pImageWnd->ResetImage();
	m_DDMAlgorithm->Clear();
	m_ctrlDefectList.DeleteAllItems();
	m_iDefectCount = 0;
	m_iPixelCount = 0;

	ApplyParam();

	UpdateData(FALSE);

	if (bVer)
	{
		m_pParam->m_bHoriCompare = FALSE;
		m_pParam->m_iVDist = iPeriod;
	}
	if (bHor)
	{
		m_pParam->m_bHoriCompare = TRUE;
		m_pParam->m_iHDist = iPeriod;
	}

	Inspection();

}
void CImageInspectionDlg::OnBnClickedButtonGrayDataSave()
{
	if( m_iplLoadImage == NULL)
		return;

	/*
	if(m_pImageWnd->m_bPanImage == TRUE)
	{
		AfxMessageBox("Pan Image 기능을 해제하세요.");
		return;
	}
	*/

	CString strPath;

	int iMessageBoxStatus;
	int iPosXY = 0;

	if(m_pImageWnd->m_bProfile == FALSE)
	{
		CRect rtPan;
		rtPan.SetRect(m_pImageWnd->m_ptSaveGrayStart.x, m_pImageWnd->m_ptSaveGrayStart.y, m_pImageWnd->m_ptSaveGrayEnd.x, m_pImageWnd->m_ptSaveGrayEnd.y);
		if (rtPan.Width() > 0 && rtPan.Height() > 0)
		{
			iMessageBoxStatus = MessageBox("선택된 영역이 있습니다. 선택된 영역의 데이터만 저장 할까요? \r\nYes = 선택 영역 저장 \r\nNo = 전체 영역 저장 \r\nCancel = 저장하지 않음", "Save", MB_YESNOCANCEL|MB_ICONQUESTION);

			if (iMessageBoxStatus == IDYES) 
			{
				CFileDialog dlg(FALSE, "csv", NULL, OFN_HIDEREADONLY, "CSV (*.CSV) | *.CSV;*.csv | All Files(*.*)|*.*||");

				if (IDOK == dlg.DoModal())
				{
					strPath = dlg.GetPathName();

					FILE* fp = NULL;

					fp = fopen((LPSTR)(LPCTSTR)strPath, "w+");

					if(fp == NULL)
						return;
					
					fprintf(fp, "Start X : %d, Start Y : %d, End X : %d, End Y : %d\n\n", rtPan.left, rtPan.top, rtPan.right, rtPan.bottom);

					for (int y = rtPan.top ; y < rtPan.bottom ; y++)
					{	
						iPosXY = m_iplLoadImage->widthStep * y;
						for (int x = rtPan.left ; x < rtPan.right ; x++)
						{				
							fprintf(fp, "%4d,", (unsigned char)(m_iplLoadImage->imageData[iPosXY + x]));
						}
						fprintf(fp, "%s", "\n");
					}
					fclose(fp);
				}
			}
			else if(iMessageBoxStatus == IDNO)
			{
				CFileDialog dlg(FALSE, "csv", NULL, OFN_HIDEREADONLY, "CSV (*.CSV) | *.CSV;*.csv | All Files(*.*)|*.*||");

				IplImage* iplGrayDataSave = NULL;

				if (IDOK == dlg.DoModal())
				{
					strPath = dlg.GetPathName();

					FILE* fp = NULL;

					fp = fopen((LPSTR)(LPCTSTR)strPath, "w+");

					if(fp == NULL)
						return;

					fprintf(fp, "Image Size - Width : %d, Height : %d\n\n", m_iplLoadImage->width, m_iplLoadImage->height);

					for (int y = 0 ; y < m_iplLoadImage->height ; y++)
					{			
						iPosXY = m_iplLoadImage->widthStep * y;
						for (int x = 0 ; x < m_iplLoadImage->width ; x++)
						{				
							fprintf(fp, "%4d,", (unsigned char)(m_iplLoadImage->imageData[iPosXY + x]));
						}
						fprintf(fp, "%s", "\n");
					}
					fclose(fp);
				}

				if (iplGrayDataSave != NULL)
				{
					cvReleaseImage(&iplGrayDataSave);
					iplGrayDataSave = NULL;
				}
			}
			else
				return;
		}
		else
		{
			MessageBox("저장할 영역이 없습니다.", "Warning", MB_ICONWARNING|MB_OK);
			return;
		}
	}
	else
	{
		CFileDialog dlg(FALSE, "csv", NULL, OFN_HIDEREADONLY, "CSV (*.CSV) | *.CSV;*.csv | All Files(*.*)|*.*||");
		if (IDOK == dlg.DoModal())
		{
			strPath = dlg.GetPathName();

			FILE* fp = NULL;

			fp = fopen((LPSTR)(LPCTSTR)strPath, "w+");

			if(fp == NULL)
				return;		

			BYTE *pImageBuf = m_pImageWnd->GetDrawImage()->GetMemStart();		
			CRect rtSave = m_pImageWnd->m_rtSaveGrayProfile;

			if(m_pImageWnd->m_bProfile == TRUE && rtSave.IsRectEmpty() == FALSE && abs(rtSave.right-rtSave.left) >= 0 || abs(rtSave.bottom-rtSave.top) >= 0)
			{
				if(pImageBuf != NULL)
				{
					int x = 0, y = 0, nVal = 0, min = 0, max = 0;
					double fMean = 0.0;
					int nWidthStep = m_pImageWnd->GetDrawImage()->GetPitch();

					if (rtSave.Width() > rtSave.Height())
					{
						fprintf(fp, "Start X : %d, End X : %d\n\n", rtSave.left, rtSave.right);

						for (x = rtSave.left; x < rtSave.right; x++)
						{
							y = (rtSave.top - rtSave.bottom) * (x - rtSave.left) / (rtSave.right - rtSave.left) + rtSave.bottom;
							nVal = pImageBuf[y * nWidthStep + x];
							if (nVal < min) min = nVal;
							if (nVal > max) max = nVal;
							fMean += (double)nVal;

							fprintf(fp, "%4d,", nVal);
						}
						fMean /= abs(rtSave.right - rtSave.left);
					}
					else
					{
						fprintf(fp, "Start Y : %d, End Y : %d\n\n", rtSave.top, rtSave.bottom);

						for (y = rtSave.top; y < rtSave.bottom; y++)
						{
							x = (rtSave.right - rtSave.left) * (y - rtSave.bottom) / (rtSave.top - rtSave.bottom) + rtSave.left;
							nVal = pImageBuf[y * nWidthStep + x];
							if (nVal < min) min = nVal;
							if (nVal > max) max = nVal;
							fMean += nVal;

							fprintf(fp, "%4d,", nVal);
						}
						fMean /= abs(rtSave.bottom - rtSave.top);
					}
				}
			}

			fclose(fp);
		}
	}

	if(strPath.IsEmpty() == FALSE && strPath != "")
	{
		CFileFind ff;
		if(ff.FindFile(strPath))
		{
			ShellExecute(NULL, "open", "notepad", (LPSTR)(LPCTSTR)strPath, NULL, SW_SHOW);
		}
		else
		{
			MessageBox("저장된 파일이 없습니다.", "Warning", MB_ICONWARNING|MB_OK);
		}
		ff.Close();
	}
	else
	{
		MessageBox("저장할 경로가 없습니다.", "Warning", MB_ICONWARNING|MB_OK);
	}
}

void CImageInspectionDlg::OnBnClickedButtonSaveRoiImage()
{
	UpdateData(TRUE);

	CRect rtSaveROI;
	rtSaveROI.left = m_iROI_Left;
	rtSaveROI.right = m_iROI_Right;
	rtSaveROI.top = m_iROI_Top;
	rtSaveROI.bottom = m_iROI_Bottom;	

	if(rtSaveROI.IsRectEmpty() == FALSE && rtSaveROI.Width() > 0 && rtSaveROI.Height() > 0)
	{
		if(m_iplLoadImage != NULL)
		{
			CFileDialog dlg(FALSE, "", "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");

			if(dlg.DoModal() == IDOK)
			{
				CString strPath;
				strPath.Format("%s", dlg.GetPathName());

				if(strPath.IsEmpty() == FALSE && strPath != "")
				{
					CvRect rtSave = cvRect(rtSaveROI.left, rtSaveROI.top, rtSaveROI.Width(), rtSaveROI.Height());
					cvSetImageROI(m_iplLoadImage, rtSave);
					cvSaveImage(strPath, m_iplLoadImage);
					cvResetImageROI(m_iplLoadImage);
				}
			}
		}
	}
}

void CImageInspectionDlg::OnBnClickedRadio2()
{
	if(m_iDisplayImageType == 1)
		return;

	m_iDisplayImageType = 1;

	DrawImage(m_iplSubtractLowImage, m_pImageWnd->GetZoomFactor(), m_pImageWnd->GetScrollPosition());
}

void CImageInspectionDlg::OnBnClickedRadio3()
{
	if(m_iDisplayImageType == 2)
		return;

	m_iDisplayImageType = 2;

	DrawImage(m_iplSubtractHighImage, m_pImageWnd->GetZoomFactor(), m_pImageWnd->GetScrollPosition());
}

BOOL CImageInspectionDlg::LoadImage(CString strPath)
{
	CFileFind ff;
	if(ff.FindFile(strPath) == FALSE)
		return FALSE;
	ff.Close();

	if(m_iplLoadImage != NULL)
	{
		cvReleaseImage(&m_iplLoadImage);
		m_iplLoadImage = NULL;
	}

	if(m_iplProcImage != NULL)
	{
		cvReleaseImage(&m_iplProcImage);
		m_iplProcImage = NULL;
	}

	if(m_iplSubtractLowImage != NULL)
	{
		cvReleaseImage(&m_iplSubtractLowImage);
		m_iplSubtractLowImage = NULL;
	}

	if(m_iplSubtractHighImage != NULL)
	{
		cvReleaseImage(&m_iplSubtractHighImage);
		m_iplSubtractHighImage = NULL;
	}

	m_strImagePath.Format("%s", strPath);

	IplImage* image = cvLoadImage((LPSTR)(LPCTSTR)strPath, CV_LOAD_IMAGE_GRAYSCALE);

	CvRect rt;
	rt.x = 0;
	rt.y = 0;
	rt.width = image->width / 16 * 16;
	rt.height = image->height;

	cvSetImageROI(image, rt);


	if(image == NULL)
	{
		DisplayStatus("Load Image Fail");
		return FALSE;
	}

	/*
	CvRect rect;
	rect.x = 0;
	rect.width = image->widthStep - (image->widthStep%16);
	rect.y = 0;
	rect.height = image->height;
	cvSetImageROI(image, rect);
	*/

	m_iplLoadImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	m_iplProcImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	m_iplSubtractLowImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	m_iplSubtractHighImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);

	cvCopyImage(image, m_iplLoadImage);
	cvCopyImage(image, m_iplProcImage);

	FCObjImage fcimage;
	fcimage.Create(image->widthStep, image->height, 8);
	cvFlip(image, image, 0);
	memcpy(fcimage.GetMemStart(), image->imageData, image->widthStep*image->height);

	//cvResetImageROI(image);

	cvResetImageROI(image);
	cvReleaseImage(&image);

	m_pImageWnd->DrawImage(&fcimage,m_fZoomRate);

	fcimage.Destroy();

	m_rtROI = CRect(0, 0, m_iplLoadImage->width, m_iplLoadImage->height);

	m_bImageProcessing = FALSE;
//	m_DDMAlgorithm->Clear();
	m_ctrlDefectList.DeleteAllItems();
	m_iDefectCount = 0;
	m_iPixelCount = 0;

	SetWindowText("Image Path : " + strPath);
	DisplayStatus("Load Image Complete");
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	return TRUE;
}

void CImageInspectionDlg::DisplayStatus(CString strText)
{
	SetDlgItemText(IDC_STATIC_TEXT, " " + strText);
}

void CImageInspectionDlg::ApplyParam()
{
	if(m_pParam != NULL)
	{
		UpdateData(TRUE);

		m_pParam->m_iVDist = m_iVDist, m_pParam->m_iHDist = m_iHDist;
		m_pParam->m_bMultiThreshold = m_bMultiThreshold = m_EnableMultiThresholdCheckBox.GetCheck();
		m_pParam->m_iMultiTHGray = m_iMultiTHGray;
		m_pParam->m_iThresholdLow = m_iThresholdLow, m_pParam->m_iThresholdHigh = m_iThresholdHigh;
		m_pParam->m_iThresholdLow2 = m_iThresholdLow2, m_pParam->m_iThresholdHigh2 = m_iThresholdHigh2;		
		m_pParam->m_iMinSize = m_iMinSize;
		m_pParam->m_iMaxSize = m_iMaxSize;
		m_pParam->m_iMergeDist = m_iMergeDist;
		//m_pParam->m_bHoriCompare = m_bHoriCompare;	
		m_pParam->m_bHoriCompare = IsDlgButtonChecked(IDC_CHECK_HORIZONTAL_COMPARE);
		m_pParam->m_bEnableDiagonal = IsDlgButtonChecked(IDC_CHECK_DIAGONAL);

		m_rtROI.left = m_iROI_Left;
		m_rtROI.right = m_iROI_Right;
		m_rtROI.top = m_iROI_Top;
		m_rtROI.bottom = m_iROI_Bottom;
		m_pParam->m_rtROI = m_rtROI;

		m_pParam->m_bAdjacentSlopeFilter = IsDlgButtonChecked(IDC_CHECK_ADJACENT_SLOPE_FILTER);
		m_pParam->m_iEdgeJudgment = m_iEdgeJudgment;
		m_pParam->m_iEdgeSlopeTh = m_iEdgeSlopeTh;

		m_pParam->m_iPeriodSlopeJudgment = IsDlgButtonChecked(IDC_CHECK_PERIOD_SLOPE_FILTER);
		m_pParam->m_iPeriodSlopeJudgment = m_iPeriodSlopeJudgment;

		m_pParam->m_bPeriodSlopeFilter = IsDlgButtonChecked(IDC_CHECK_PERIOD_SLOPE_FILTER);
		m_pParam->m_iPeriodSlopeTh = m_iPeriodSlopeTh;

		m_pParam->m_bTrackingFilter = IsDlgButtonChecked(IDC_CHECK_TRACKING_FILTER);
		m_pParam->m_iTrackingRange = m_iTrackingRange;
		m_pParam->m_iTrackingTh = m_iTrackingTh;

		m_pParam->m_bMountainFilter = IsDlgButtonChecked(IDC_CHECK_MOUNTAIN_FILTER);

		m_pParam->m_bNearbyCheckFilter = IsDlgButtonChecked(IDC_CHECK_NEARBYCHECK_FILTER);

		m_pParam->m_bASGInspection = IsDlgButtonChecked(IDC_CHECK_ASG_INSPECT_ON);

		m_pParam->m_fFOV = m_dFOV;

		m_pParam->m_fPixelsize = m_dPixelsize;

		m_pParam->SaveParam(DDM_PARAM_PATH);
	}
}

void CImageInspectionDlg::DefectMoveToPoint(CPoint ptPos, CSize szSize)
{
	if(m_pImageWnd != NULL)
	{
		int iMargin = 10;
		/*
		CRect rt;
		rt.left = (ptPos.x - szSize.cx/2) - 10;
		rt.top = (ptPos.y - szSize.cy/2) - 10;
		rt.right = (ptPos.x + szSize.cx/2) + 10;
		rt.bottom = (ptPos.y + szSize.cy/2) + 10;
		*/

		m_pImageWnd->MoveToPoint(ptPos, TRUE, TRUE);
	}
}

void CImageInspectionDlg::DrawImage(IplImage* pImage, float fZoom, CPoint ptScrollPos)
{
	if(pImage == NULL)
		return;

	IplImage* image = cvCreateImage(cvGetSize(pImage), pImage->depth, pImage->nChannels);
	FCObjImage fcimage;
	fcimage.Create(pImage->widthStep, pImage->height, 8);
	cvFlip(pImage, image, 0);
	memcpy(fcimage.GetMemStart(), image->imageData, image->widthStep*pImage->height);
	m_pImageWnd->DrawImage(&fcimage, fZoom, ptScrollPos);
	fcimage.Destroy();
	cvReleaseImage(&image);
	image = NULL;
}

void CImageInspectionDlg::LoadParam()
{
	if(m_pParam != NULL)
	{
		m_iVDist = m_pParam->m_iVDist, m_iHDist = m_pParam->m_iHDist;
		m_bMultiThreshold = m_pParam->m_bMultiThreshold;
		m_iMultiTHGray = m_pParam->m_iMultiTHGray;
		m_iThresholdLow = m_pParam->m_iThresholdLow, m_iThresholdHigh = m_pParam->m_iThresholdHigh;
		m_iThresholdLow2 = m_pParam->m_iThresholdLow2, m_iThresholdHigh2 = m_pParam->m_iThresholdHigh2;		
		m_iMinSize = m_pParam->m_iMinSize;
		m_iMaxSize = m_pParam->m_iMaxSize;
		m_iMergeDist = m_pParam->m_iMergeDist;
		m_bHoriCompare = m_pParam->m_bHoriCompare;
		m_rtROI = m_pParam->m_rtROI;

		m_iROI_Left = m_rtROI.left;
		m_iROI_Right = m_rtROI.right;
		m_iROI_Top = m_rtROI.top;
		m_iROI_Bottom = m_rtROI.bottom;
		m_dFOV = m_pParam->m_fFOV;
		m_dPixelsize = m_pParam->m_fPixelsize;

		m_bASGInspection = m_pParam->m_bASGInspection;
		m_bAdjacentSlopeFilter = m_pParam->m_bAdjacentSlopeFilter;
		m_iEdgeJudgment = m_pParam->m_iEdgeJudgment;
		m_iEdgeSlopeTh = m_pParam->m_iEdgeSlopeTh;
		m_bPeriodSlopeFilter = m_pParam->m_bPeriodSlopeFilter;
		m_iPeriodSlopeJudgment = m_pParam->m_iPeriodSlopeJudgment;
		m_iPeriodSlopeTh = m_pParam->m_iPeriodSlopeTh;
		m_bTrackingFilter = m_pParam->m_bTrackingFilter;
		m_iTrackingRange = m_pParam->m_iTrackingRange;
		m_iTrackingTh = m_pParam->m_iTrackingTh;
		m_bNearbyCheckFilter = m_pParam->m_bNearbyCheckFilter;
		m_bMountainFilter = m_pParam->m_bMountainFilter;
		m_bDiagonal = m_pParam->m_bEnableDiagonal;

		UpdateData(FALSE);
	}
}
void CImageInspectionDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.



	int nFiles;
	char szPathName[MAX_PATH];
	CString strFileName;

	nFiles = ::DragQueryFile(hDropInfo, 0, szPathName, MAX_PATH);

	CString strPath;
	strPath.Format("%s", szPathName);

	if (strPath.Right(3) == "bmp")
	{
		m_strLoadImageFilePath.Format("");
		LoadImage(strPath);
		m_strLoadImageFilePath = strPath;
	}
	else if (strPath.Right(3) == "txt")
	{
		LoadRecipeParameter(strPath);
		ApplyParam();
	}

	::DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CImageInspectionDlg::InitListHeader()
{
	m_ctrlDefectList.ModifyStyleEx(0, WS_EX_ACCEPTFILES);
	m_ctrlDefectList.SetExtendedStyle(LVS_REPORT | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//char*	cListHeader[] = {"No", "Index", "Center X", "Center Y", "Area", "Real-Area", "Min X", "Min Y", "Max X", "Max Y", "DX", "DY", "Aspect", "Real-DX", "Real-DY", "Real-Aspect", "D-Min Gray", "D-Max Gray", "R-Min Gray", "R-Max Gray", "Max-Diff", "GraySum", "RefGraySum", "GraySqareSum", "RefGraySqareSum", "Bubble", "RTC Code", NULL};
	char*	cListHeader[] = {"No", "Center X", "Center Y", "Area", NULL};
	const	int		nHeaderWidth[] = {52, 80, 80, 62};

	LV_COLUMN	lvColumn;
	CString		strHeader;
	CListCtrl&	listCtrl = m_ctrlDefectList;
	lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

	for(int i = 0 ; cListHeader[i] != NULL ; i++)
	{
		strHeader = cListHeader[i];
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.cx = nHeaderWidth[i];
		lvColumn.pszText = cListHeader[i];
		lvColumn.iSubItem = i;
		listCtrl.InsertColumn(i, &lvColumn);
	}

	CHeaderCtrl* pH = (CHeaderCtrl*)m_ctrlDefectList.GetDlgItem(0);
	pH->ModifyStyle(0, HDS_HOTTRACK);
}

void CImageInspectionDlg::InitImageListHeader()
{
}

BOOL CImageInspectionDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(m_iplLoadImage != NULL)
	{
		cvReleaseImage(&m_iplLoadImage);
		m_iplLoadImage = NULL;
	}

	if(m_iplProcImage != NULL)
	{
		cvReleaseImage(&m_iplProcImage);
		m_iplProcImage = NULL;
	}

	if(m_iplSubtractLowImage != NULL)
	{
		cvReleaseImage(&m_iplSubtractLowImage);
		m_iplSubtractLowImage = NULL;
	}

	if(m_iplSubtractHighImage != NULL)
	{
		cvReleaseImage(&m_iplSubtractHighImage);
		m_iplSubtractHighImage = NULL;
	}

	if(m_pImageWnd != NULL)
	{
		m_pImageWnd->DestroyWindow();
		delete m_pImageWnd;
		m_pImageWnd = NULL;
	}
	if (m_pAutoCalDlg != NULL)
	{
		m_pAutoCalDlg->DestroyWindow();
		delete m_pAutoCalDlg;
		m_pAutoCalDlg = NULL;
	}

	return CDialog::DestroyWindow();
}
void CImageInspectionDlg::OnNMClickDefectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nItem = pNMItemActivate->iItem;

	if(nItem > -1)
	{
		CListCtrl& listCtrl = m_ctrlDefectList;
		CPoint ptPos;
		CSize szSize;
		ptPos.x = atoi(listCtrl.GetItemText(nItem, 1));
		ptPos.y = atoi(listCtrl.GetItemText(nItem, 2));
		//szSize.cx = abs(atoi(listCtrl.GetItemText(nItem, 7)) - atoi(listCtrl.GetItemText(nItem, 5)));
		//szSize.cy = abs(atoi(listCtrl.GetItemText(nItem, 8)) - atoi(listCtrl.GetItemText(nItem, 6)));

		DefectMoveToPoint(ptPos, szSize);
	}

	*pResult = 0;
}

void CImageInspectionDlg::OnLvnItemchangedDefectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nItem = pNMLV->iItem;

	if(nItem > -1)
	{
		CListCtrl& listCtrl = m_ctrlDefectList;
		CPoint ptPos;
		CSize szSize;
		ptPos.x = atoi(listCtrl.GetItemText(nItem, 1));
		ptPos.y = atoi(listCtrl.GetItemText(nItem, 2));
		//szSize.cx = abs(atoi(listCtrl.GetItemText(nItem, 7)) - atoi(listCtrl.GetItemText(nItem, 5)));
		//szSize.cy = abs(atoi(listCtrl.GetItemText(nItem, 8)) - atoi(listCtrl.GetItemText(nItem, 6)));

		DefectMoveToPoint(ptPos, szSize);
	}

	*pResult = 0;
}

void CImageInspectionDlg::OnBnClickedButtonLoadrecipe()
{
	CString strRecipeFolderName = "", strRecipeFileName = "";
	strRecipeFolderName = FindRecipeFolderShell();
	strRecipeFileName = FindRecipeFileName(strRecipeFolderName);
	LoadRecipeParameter(strRecipeFileName);
	ApplyParam();
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lpData);
			break;
	}
	return 0;
}

CString CImageInspectionDlg::FindRecipeFolderShell()
{
	char dirName[MAX_PATH];
	char dirFolderPath[MAX_PATH];
	int len;
	CString str, strInitPath;
	BSTR buf;

	strInitPath = _T("D:\\Writable\\Recipe\\");
	BROWSEINFO   bi;
    LPITEMIDLIST  idl;
    ZeroMemory(&bi,sizeof(BROWSEINFO));
    bi.hwndOwner   =   this->m_hWnd;
    bi.pszDisplayName =   dirName;  // 선택된 디렉토리명 저장 
    bi.lpszTitle = "폴더를 선택해 주세요";
//      bi.ulFlags  =  BIF_EDITBOX    // 선택된 디렉토리명을 표시하고 편집할 수 있는 창
//                       | 0x0040             // 새로운 디렉토리를 만들수 있는 버튼 추가 
//                         ;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc;

	bi.lParam = (LPARAM)(LPCTSTR)strInitPath;

	idl = ::SHBrowseForFolder(&bi);

    if (idl) {
        SHGetPathFromIDList(idl, dirFolderPath); // 전체 PATH을 포함한 디렉토리명
		len = MultiByteToWideChar(CP_ACP, 0, dirFolderPath, (int)strlen(dirFolderPath), NULL, NULL);
		buf = SysAllocStringLen(NULL, len);
		MultiByteToWideChar(CP_ACP, 0, dirFolderPath, (int)strlen(dirFolderPath), buf, len);
		return dirFolderPath;
    }
	return false;
}

CString CImageInspectionDlg::FindRecipeFileName(CString folderPath)
{
	
	CString strFileName, strFolderName;
	bool bWorking = false;
	CFileFind ff;
	strFolderName.Format("%s\\%s", folderPath, _T("*.*"));
	bWorking = ff.FindFile(strFolderName);
	
	while(bWorking)
	{
		bWorking = ff.FindNextFile();
		if(ff.IsArchived())
		{
			CString _fileName = ff.GetFileName();
			if(0 == stricmp(_fileName, "ddmcore_param.txt"))
			{
				strFileName.Format("%s\\%s", folderPath, _fileName);
				return strFileName;
			}
		}
	}
	return false;
}

CString CImageInspectionDlg::GetRecipeData(CString strRecipeFileName, CString strStdData, int &nSplitBuffer, int &nLastBuffer)
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

void CImageInspectionDlg::LoadRecipeParameter(CString strFileName)
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

	nMaxSize = GetPrivateProfileInt("CELL PARAMETERS", "MaxSize[0]", 9999, strFileName);
	SetDlgItemInt(IDC_EDIT_MAX_SIZE, nMaxSize);

	nMergeDist = GetPrivateProfileInt("CELL PARAMETERS", "MergeDist[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_MERGE_DISTANCE, nMergeDist);

	nEdgeJudgment = GetPrivateProfileInt("CELL PARAMETERS", "EdgeJudgment[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_EDGE_JUDGMENT, nEdgeJudgment);

	nEdgeSlopeTh = GetPrivateProfileInt("CELL PARAMETERS", "EdgeSlopeTh[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_EDGE_SLOPE_TH, nEdgeSlopeTh);

	nPeriodSlopeTh = GetPrivateProfileInt("CELL PARAMETERS", "PeriodSlopeTh[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_PERIOD_SLOPE_TH, nPeriodSlopeTh);

	nPeriodSlopeJudgment = GetPrivateProfileInt("CELL PARAMETERS", "PeriodSlopeJudgment[0]", 0, strFileName);
	SetDlgItemInt(IDC_EDIT_PERIOD_SLOPE_JUDGMENT, nPeriodSlopeJudgment);
	
	nMultiThresholdEnable = GetPrivateProfileInt("CELL PARAMETERS", "MultiThresholdEnable[0]", 0, strFileName);
	if(nMultiThresholdEnable)
		m_EnableMultiThresholdCheckBox.SetCheck(TRUE);
	else
		m_EnableMultiThresholdCheckBox.SetCheck(FALSE);
	
	nEnableHorComp = GetPrivateProfileInt("CELL PARAMETERS", "EnableHorComp[0]", 0, strFileName);
	if (nEnableHorComp)
		m_EnableHorCompCheckBox.SetCheck(TRUE);
	else
		m_EnableHorCompCheckBox.SetCheck(FALSE);

	nEnableEdgeSlope = GetPrivateProfileInt("CELL PARAMETERS", "EnableEdgeSlope[0]", 0, strFileName);
	if(nEnableEdgeSlope)
		m_EnableEdgeSlopeCheckBox.SetCheck(TRUE);
	else
		m_EnableEdgeSlopeCheckBox.SetCheck(FALSE);
	
	nEnablePeriodSlope = GetPrivateProfileInt("CELL PARAMETERS", "EnablePeriodSlope[0]", 0, strFileName);
	if(nEnablePeriodSlope)
		m_EnablePeriodSlopeCheckBox.SetCheck(TRUE);
	else
		m_EnablePeriodSlopeCheckBox.SetCheck(FALSE);
}	

void CImageInspectionDlg::SetRectCoordinate(CRect rect)
{
	m_rtDisplayRoiCoordinate.left = rect.left;
	m_rtDisplayRoiCoordinate.right = rect.right;
	m_rtDisplayRoiCoordinate.top = rect.top;
	m_rtDisplayRoiCoordinate.bottom = rect.bottom;

	SetDlgItemInt(IDC_EDIT_ROI_LEFT,m_rtDisplayRoiCoordinate.left);
	SetDlgItemInt(IDC_EDIT_ROI_RIGHT,m_rtDisplayRoiCoordinate.right);
	SetDlgItemInt(IDC_EDIT_ROI_TOP,m_rtDisplayRoiCoordinate.top);
	SetDlgItemInt(IDC_EDIT_ROI_BOTTOM,m_rtDisplayRoiCoordinate.bottom);

	m_iROI_Left = m_rtDisplayRoiCoordinate.left;
	m_iROI_Right = m_rtDisplayRoiCoordinate.right;
	m_iROI_Top = m_rtDisplayRoiCoordinate.top;
	m_iROI_Bottom = m_rtDisplayRoiCoordinate.bottom;
}

void CImageInspectionDlg::OnBnClickedButtonSaveResult()
{
	if (m_strLoadImageFilePath == "")
		return;
	int nDotPos = m_strLoadImageFilePath.ReverseFind('.');
	CString strResultFilePath;
	strResultFilePath.Format("%s.txt", m_strLoadImageFilePath.Left(nDotPos));
	SaveResult(strResultFilePath);
}

void CImageInspectionDlg::SaveResult(CString _strResultFilePath)
{	
	if (m_DDMAlgorithm->m_iDefectCnt == 0)
		return;
	CString strResult;
	CStdioFile FileResult_Cell;
	if (!FileResult_Cell.Open(_strResultFilePath, CFile::modeWrite | CFile::typeText | CFile::modeCreate)) {
		return;
	}

	for (int i = 0; i < m_DDMAlgorithm->m_iDefectCnt; i++)
	{
		CDefectFeature* pDefect = &(m_DDMAlgorithm->m_DefectFeature[i]);

		if (pDefect == NULL)
			continue;

		strResult.Format("def=%04d,%04d,%07d,%07d,%07d,%07d,%07d,%07d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			pDefect->iFrameCount,
			pDefect->iArea,
			(int)(pDefect->iCenterx * m_dFOV),// * FOV,
			(int)(pDefect->iCentery * m_dFOV),
			(int)(pDefect->iMinx * m_dFOV),
			(int)(pDefect->iMaxx * m_dFOV),
			(int)(pDefect->iMiny * m_dFOV),
			(int)(pDefect->iMaxy * m_dFOV),
			pDefect->iDefectGraySum,
			pDefect->iDefectGraySquareSum,
			pDefect->iDefectMinGray,
			pDefect->iDefectMaxGray,
			pDefect->iReferenceGraySum,
			pDefect->iReferenceGraySquareSum,
			pDefect->iReferenceMinGray,
			pDefect->iReferenceMaxGray,
			pDefect->iZoneArea[0],
			pDefect->iZoneArea[1],
			pDefect->iZoneArea[2],
			pDefect->iZoneArea[3],
			pDefect->iZoneArea[4],
			pDefect->iZoneArea[5],
			pDefect->iZoneArea[6],
			pDefect->iZoneArea[7],
			pDefect->iZoneArea[8],
			pDefect->iZoneArea[9],
			pDefect->iZoneArea[10],
			pDefect->iZoneArea[11],
			pDefect->iZoneArea[12],
			pDefect->iZoneArea[13],
			pDefect->iZoneArea[14],
			pDefect->iZoneArea[15],
			pDefect->iZoneArea[16],
			pDefect->iZoneArea[17],
			pDefect->iZoneArea[18],
			pDefect->iZoneArea[19],
			pDefect->iZoneGray[0],
			pDefect->iZoneGray[1],
			pDefect->iZoneGray[2],
			pDefect->iZoneGray[3],
			pDefect->iZoneGray[4],
			pDefect->iZoneGray[5],
			pDefect->iZoneGray[6],
			pDefect->iZoneGray[7],
			pDefect->iZoneGray[8],
			pDefect->iZoneGray[9],
			pDefect->iZoneGray[10],
			pDefect->iZoneGray[11],
			pDefect->iZoneGray[12],
			pDefect->iZoneGray[13],
			pDefect->iZoneGray[14],
			pDefect->iZoneGray[15],
			pDefect->iZoneGray[16],
			pDefect->iZoneGray[17],
			pDefect->iZoneGray[18],
			pDefect->iZoneGray[19]);

			FileResult_Cell.WriteString(strResult);
	}

	FileResult_Cell.Close();
}

void CImageInspectionDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//CDialog::OnOK();
}

void CImageInspectionDlg::OnBnClickedButtonList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_ImageList.ResetContent();
	ITEMIDLIST *pidlBrowse;
	TCHAR       pszPathname[MAX_PATH];
	BROWSEINFO  BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}

	m_TopPath = (LPCTSTR)pszPathname;
	CString ImageType = _T(".jpg");
	typeImageRead(ImageType);
	ImageType = _T(".png");
	typeImageRead(ImageType);
	ImageType = _T(".bmp");
	typeImageRead(ImageType);

	CreateHorizontalScroll();
}

void CImageInspectionDlg::typeImageRead(CString inputType)
{
	CString tpath;
	tpath = m_TopPath + _T("\\*") + inputType;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(tpath);
	//CString fileName;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsArchived())
		{
			CString _fileName = finder.GetFileName();
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;
			m_originfileName = _fileName;
			m_fileName = finder.GetFileTitle();
			m_fileName += inputType;
			m_ImageList.AddString(m_fileName);
			m_fileName = m_TopPath + _T("\\") + m_fileName;
		}

	}
}

void CImageInspectionDlg::OnDblclkImagelist()		//Double Click시
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_DDMAlgorithm->Clear();
	//wchar_t temp_path[MAX_PATH];
	//CString CstrSelect = m_TopPath;
	//
	//int index = m_ImageList.GetCurSel();
	//CString Cstr;
	//m_ImageList.GetText(index, Cstr);
	//
	//CstrSelect = CstrSelect + _T("\\") + Cstr;
	//
	//LoadImage(CstrSelect);
}
 
void CImageInspectionDlg::ZoomRateValue(float fZoom)
{
	m_fZoomRate = fZoom;
}

void CImageInspectionDlg::OnSelchangeImagelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_DDMAlgorithm->Clear();
	wchar_t temp_path[MAX_PATH];
	CString CstrSelect = m_TopPath;

	int index = m_ImageList.GetCurSel();
	CString Cstr;
	m_ImageList.GetText(index, Cstr);

	CstrSelect = CstrSelect + _T("\\") + Cstr;

	LoadImage(CstrSelect);
}

void CImageInspectionDlg::CreateHorizontalScroll()
{
	CString str; CSize sz; int dx = 0;
	CDC *pDC = m_ImageList.GetDC();

	for (int i = 0; i < m_ImageList.GetCount(); i++)
	{
		m_ImageList.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_ImageList.ReleaseDC(pDC);

	if (m_ImageList.GetHorizontalExtent() < dx)
	{
		m_ImageList.SetHorizontalExtent(dx);
		ASSERT(m_ImageList.GetHorizontalExtent() == dx);
	}
}

void CImageInspectionDlg::OnBnClickedButtonDDMCalc()
{
	DDMCalculator dlg;
	dlg.DoModal();
}

void CImageInspectionDlg::OnBnClickedImageSizeLoad()
{
	SetDlgItemInt(IDC_EDIT_ROI_LEFT, 0);
	SetDlgItemInt(IDC_EDIT_ROI_TOP, 0);
	SetDlgItemInt(IDC_EDIT_ROI_RIGHT, m_iImageSizeX);
	SetDlgItemInt(IDC_EDIT_ROI_BOTTOM, m_iImageSizeY);
}

void CImageInspectionDlg::OnBnClickedCheckShow()
{
	BOOL chk = FALSE;
	chk = m_ctlShow.GetCheck();

	if(chk == 1)
		m_bShowCheck = TRUE;
	else
		m_bShowCheck = FALSE;

}

void CImageInspectionDlg::OnBnClickedRoiOffsetFunc()
{
	UpdateData(TRUE);

	CRect rtSerchingROI, rtMatchingROI;
	rtSerchingROI.left = m_iROI_Left;
	rtSerchingROI.right = m_iROI_Right;
	rtSerchingROI.top = m_iROI_Top;
	rtSerchingROI.bottom = m_iROI_Bottom;

	rtMatchingROI.left = m_iROI_Left;
	rtMatchingROI.right = m_iROI_Right;
	rtMatchingROI.top = m_iROI_Top;
	rtMatchingROI.bottom = m_iROI_Bottom;

	CvPoint ptFindPos;
	double bRate = 0.4;
	BOOL bDir[4]; // 1:left, 2:right, 3:top, 4:bottom

	for(int i=0; i<4; i++)
	{
		bDir[i] = FALSE;
	}

	// 영역 판단
	if (rtSerchingROI.left <= 1 && rtSerchingROI.right < 16384 - 1)
		bDir[ROI_RIGHT] = TRUE;
	if (rtSerchingROI.right >= 16384 - 1 && rtSerchingROI.left > 1)
		bDir[ROI_LEFT] = TRUE;
	if (rtSerchingROI.top <= 1 && rtSerchingROI.bottom < 8192 - 1)
		bDir[ROI_BOTTOM] = TRUE;
	if (rtSerchingROI.bottom >= 8192 - 1 && rtSerchingROI.top > 1)
		bDir[ROI_TOP] = TRUE;

	for(int iDirIdx = 0; iDirIdx < 4; iDirIdx++)
	{
		if(bDir[iDirIdx] == TRUE)
		{
			switch (iDirIdx)
			{
			case ROI_LEFT:
				ptFindPos = AutoMatchingROIOffset(m_iplLoadImage, rtSerchingROI, m_iVDist, m_iHDist, ROI_LEFT, bRate);	//Frame Image, Inspect ROI, Vertical Period, Horizontal Period, Direction, MatchingRate
				rtMatchingROI.left = ptFindPos.x;
				break;		
			case ROI_RIGHT:
				ptFindPos = AutoMatchingROIOffset(m_iplLoadImage, rtSerchingROI, m_iVDist, m_iHDist, ROI_RIGHT, bRate);
				rtMatchingROI.right = ptFindPos.x;
				break;		
			case ROI_TOP:
				ptFindPos = AutoMatchingROIOffset(m_iplLoadImage, rtSerchingROI, m_iVDist, m_iHDist, ROI_TOP, bRate);
				rtMatchingROI.top = ptFindPos.y;
				break;		
			case ROI_BOTTOM:
				ptFindPos = AutoMatchingROIOffset(m_iplLoadImage, rtSerchingROI, m_iVDist, m_iHDist, ROI_BOTTOM, bRate);
				rtMatchingROI.bottom = ptFindPos.y;
				break;		
			default:
				break;
			}
		}
	}

SKIP:
	;
}
CvPoint CImageInspectionDlg::AutoMatchingROIOffset(IplImage* iplOrg, CRect rtMatchingROI, int iVDist, int iHDist, int iDirType, double dMatchingRate)
{
	CString strDebug;
	CvPoint ptFindPos, ptFinalPos;
	CRect rtFirstROI;
	int iSearchX, iSearchY;
	int iFinalX, iFinalY;
	double min, max;

	IplImage* pTemplate = cvLoadImage("D:\\Writable\\Recipe\\P_VAO_6500F0R-Q1_\\pattern1.bmp", CV_LOAD_IMAGE_GRAYSCALE);						// Matching Image
	IplImage* pImg = cvCreateImage(cvSize(pTemplate->width*2, pTemplate->height*2), 8, 1);														// Search Area Image
	IplImage* pResultImg = cvCreateImage(cvSize(pImg->width - pTemplate->width + 1, pImg->height - pTemplate->height + 1), IPL_DEPTH_32F, 1);	//Matching ccoeff Image

	ptFindPos.x = ptFindPos.y = 0;
	ptFinalPos.x = ptFinalPos.y = 0;

	BOOL bCreateRef = FALSE;

	switch(iDirType)					// 처음 Search Area
	{
	case ROI_LEFT:
		rtFirstROI.left = rtMatchingROI.left - (int)(iHDist / 2);
			break;
	case ROI_RIGHT:
		rtFirstROI.right = rtMatchingROI.right - (int)(iHDist / 2);
			break;
	case ROI_TOP:
		rtFirstROI.top = rtMatchingROI.top - (int)(iVDist / 2);
			break;
	case ROI_BOTTOM:
		rtFirstROI.bottom = rtMatchingROI.bottom - (int)(iVDist / 2);
			break;
	}

	if (iplOrg != NULL)
	{
		switch(iDirType)
		{
		case ROI_LEFT:		// left Search
			iSearchX = rtFirstROI.left;
			iSearchY = (int)((rtMatchingROI.top + rtMatchingROI.bottom) / 2);
			
			for(int ipatternStep = 0; ipatternStep < (iHDist/2)*5; ipatternStep+=(iHDist/2))	//최대 ipatternStep주기 좌우 Search
			{
				bCreateRef = FALSE;
				iFinalX = iSearchX;			// 최종 Find X
				iFinalY = iSearchY;			// 최종 Find Y
				iSearchX -= ipatternStep;	// Search Step

				if (iSearchX < 0 || iSearchX > 16383 || iSearchY < 0 || iSearchY > 8191)
					break;

				for (int v = 0; v < pImg->height; v++)
				{
					CopyMemory(&(pImg->imageData[v * pImg->widthStep]), &(iplOrg->imageData[((iSearchY+v) * iplOrg->widthStep) + iSearchX]), pImg->width) ;
				}
				ptFinalPos.x = ptFindPos.x;
				ptFinalPos.y = ptFindPos.y;

				cvMatchTemplate(pImg, pTemplate, pResultImg, CV_TM_CCOEFF_NORMED);
				cvMinMaxLoc(pResultImg, &min, &max, NULL, &ptFindPos);

				strDebug.Format("D:\\Left_%.3f.bmp", max);
				cvSaveImage(strDebug, pImg);

				if (dMatchingRate > max)	{	// Matching Rate 보다 작을 경우 Search Stop
					if (ptFindPos.x == 0 && ptFindPos.y == 0)
						ptFinalPos.x = rtFirstROI.left;
					else
						ptFinalPos.x = ptFinalPos.x + iFinalX;	//Left ROI 재설정
					break;
				}
				else
				{
					bCreateRef = TRUE;
					dMatchingRate = 0.6;
				}

				if (bCreateRef)
				{
					for (int v = 0; v < pTemplate->height; v++)
					{
						CopyMemory(&(pTemplate->imageData[v * pTemplate->widthStep]), &(iplOrg->imageData[((iSearchY + ptFindPos.y + v) * iplOrg->widthStep) + iSearchX + ptFindPos.x]), pTemplate->width);
					}
				}
			}
			break;
		case ROI_RIGHT:		// Right Search
			iSearchX = rtFirstROI.right;
			iSearchY = (int)((rtMatchingROI.top + rtMatchingROI.bottom) / 2);
			
			for(int ipatternStep = 0; ipatternStep < (iHDist/2)*5; ipatternStep+=(iHDist/2))	//최대 3주기 좌우 Search
			{
				bCreateRef = FALSE;
				iFinalX = iSearchX;			// 최종 Find X
				iFinalY = iSearchY;			// 최종 Find Y
				iSearchX += ipatternStep;	// Search Step

				if (iSearchX < 0 || iSearchX > 16383 || iSearchY < 0 || iSearchY > 8191)
					break;

				for (int v = 0; v < pImg->height; v++)
				{
					CopyMemory(&(pImg->imageData[v * pImg->widthStep]), &(iplOrg->imageData[((iSearchY + v) * iplOrg->widthStep) + iSearchX]), pImg->width);
				}

				ptFinalPos.x = ptFindPos.x;
				ptFinalPos.y = ptFindPos.y;

				cvMatchTemplate(pImg, pTemplate, pResultImg, CV_TM_CCOEFF_NORMED);
				cvMinMaxLoc(pResultImg, &min, &max, NULL, &ptFindPos);

				strDebug.Format("D:\\Right_%.3f.bmp", max);
				cvSaveImage(strDebug, pImg);

				if (dMatchingRate > max)		// Matching Rate 보다 작을 경우 Search Stop
				{
					if (ptFindPos.x == 0 && ptFindPos.y == 0)
						ptFinalPos.x = rtFirstROI.right;
					else
						ptFinalPos.x = ptFinalPos.x + iFinalX + pTemplate->width;	//Right ROI 재설정
					break;
				}
				else
				{
					bCreateRef = TRUE;
					dMatchingRate = 0.6;
				}

				if (bCreateRef)
				{
					for (int v = 0; v < pTemplate->height; v++)
					{
						CopyMemory(&(pTemplate->imageData[v * pTemplate->widthStep]), &(iplOrg->imageData[((iSearchY + ptFindPos.y + v) * iplOrg->widthStep) + iSearchX + ptFindPos.x]), pTemplate->width);
					}
				}
			}
			break;
		case ROI_TOP:		// Top Search
			iSearchX = (int)((rtMatchingROI.right + rtMatchingROI.left) / 2);
			iSearchY = rtFirstROI.top;

			for(int ipatternStep = 0; ipatternStep < (iVDist/2)*5; ipatternStep+=(iVDist/2))	//최대 3주기 상하 Search
			{
				bCreateRef = FALSE;
				iFinalX = iSearchX;			// 최종 Find X
				iFinalY = iSearchY;			// 최종 Find Y
				iSearchY -= ipatternStep;	// Search Step

				if (iSearchX < 0 || iSearchX > 16383 || iSearchY < 0 || iSearchY > 8191)
					break;

				for (int v = 0; v < pImg->height; v++)
				{
					CopyMemory(&(pImg->imageData[v * pImg->widthStep]), &(iplOrg->imageData[((iSearchY+v) * iplOrg->widthStep) + iSearchX]), pImg->width) ;
				}

				ptFinalPos.x = ptFindPos.x;
				ptFinalPos.y = ptFindPos.y;

				cvMatchTemplate(pImg, pTemplate, pResultImg, CV_TM_CCOEFF_NORMED);
				cvMinMaxLoc(pResultImg, &min, &max, NULL, &ptFindPos);

				strDebug.Format("D:\\Top_%.3f.bmp", max);
				cvSaveImage(strDebug, pImg);

				if (dMatchingRate > max) {		// Matching Rate 보다 작을 경우 Search Stop
					if (ptFindPos.x == 0 && ptFindPos.y == 0)
						ptFinalPos.y = rtFirstROI.top;
					else
						ptFinalPos.y = ptFinalPos.y + iFinalY;	//Top ROI 재설정
					break;
				}
				else
				{
					bCreateRef = TRUE;
					dMatchingRate = 0.6;
				}

				if (bCreateRef)
				{
					for (int v = 0; v < pTemplate->height; v++)
					{
						CopyMemory(&(pTemplate->imageData[v * pTemplate->widthStep]), &(iplOrg->imageData[((iSearchY + ptFindPos.y + v) * iplOrg->widthStep) + iSearchX + ptFindPos.x]), pTemplate->width);
					}
				}
			}	
			break;
		case ROI_BOTTOM:		// Bottom Search
			iSearchX = (int)((rtMatchingROI.right + rtMatchingROI.left) / 2);
			iSearchY = rtFirstROI.bottom;

			for(int ipatternStep = 0; ipatternStep < (iVDist/2)*5; ipatternStep+=(iVDist/2/2))	//최대 3주기 상하 Search
			{
				bCreateRef = FALSE;			
				iFinalX = iSearchX;			// 최종 Find X
				iFinalY = iSearchY;			// 최종 Find Y
				iSearchY += ipatternStep;	// Search Step

				if (iSearchX < 0 || iSearchX > 16383 || iSearchY < 0 || iSearchY > 8191)
					break;

				for (int v = 0; v < pImg->height; v++)
				{
					CopyMemory(&(pImg->imageData[v * pImg->widthStep]), &(iplOrg->imageData[((iSearchY+v) * iplOrg->widthStep) + iSearchX]), pImg->width) ;
				}
				ptFinalPos.x = ptFindPos.x;
				ptFinalPos.y = ptFindPos.y;

				cvMatchTemplate(pImg, pTemplate, pResultImg, CV_TM_CCOEFF_NORMED);
				cvMinMaxLoc(pResultImg, &min, &max, NULL, &ptFindPos);

				strDebug.Format("D:\\Bottom_%.3f.bmp", max);
				cvSaveImage(strDebug, pImg);

				if (dMatchingRate > max) {		// Matching Rate 보다 작을 경우 Search Stop
					if (ptFindPos.x == 0 && ptFindPos.y == 0)
						ptFinalPos.y = rtFirstROI.bottom;
					else
						ptFinalPos.y = ptFinalPos.y + iFinalY + pTemplate->height;	//Top ROI 재설정
					break;
				}
				else
				{
					bCreateRef = TRUE;
					dMatchingRate = 0.6;
				}

				if (bCreateRef)
				{
					for (int v = 0; v < pTemplate->height; v++)
					{
						CopyMemory(&(pTemplate->imageData[v * pTemplate->widthStep]), &(iplOrg->imageData[((iSearchY + ptFindPos.y + v) * iplOrg->widthStep) + iSearchX + ptFindPos.x]), pTemplate->width);
					}
				}
			}
			break;
		}
	}

	cvReleaseImage(&pTemplate);
	cvReleaseImage(&pImg);
	cvReleaseImage(&pResultImg);

	return ptFinalPos;
}


void CImageInspectionDlg::OnBnClickedButtonAutoCalPeriod()
{
	m_pAutoCalDlg->ShowWindow(SW_SHOW);
}

void CImageInspectionDlg::SetAutoCalPeriod(BOOL bVer, BOOL bHor, int iPeriod)
{
	if(bVer)
		SetDlgItemInt(IDC_EDIT_VERTICAL_DISTANCE, iPeriod);	
	if(bHor)
		SetDlgItemInt(IDC_EDIT_HORIZONTAL_DISTANCE, iPeriod);
}

void CImageInspectionDlg::OnBnClickedCheckHorizontalCompare()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
