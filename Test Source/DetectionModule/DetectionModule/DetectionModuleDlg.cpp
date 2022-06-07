#include "stdafx.h"
#include "framework.h"
#include "DetectionModule.h"
#include "DetectionModuleDlg.h"
#include "afxdialogex.h"

#include "ImageWnd.h"

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
}

void CDetectionModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_ctrlImageView);
	DDX_Text(pDX, IDC_EDIT_ROI_LEFT, m_iROI_Left);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT, m_iROI_Right);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP, m_iROI_Top);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM, m_iROI_Bottom);
}

BEGIN_MESSAGE_MAP(CDetectionModuleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CDetectionModuleDlg::OnBnClickedButtonLoadImage)
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION, &CDetectionModuleDlg::OnBnClickedButtonInspection)
	ON_BN_CLICKED(IDC_SET, &CDetectionModuleDlg::OnBnClickedSet)
	ON_WM_SIZE()
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

	return TRUE;
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
		m_rtROI = m_pParam->m_rtROI;

		m_iROI_Left = m_rtROI.left;
		m_iROI_Right = m_rtROI.right;
		m_iROI_Top = m_rtROI.top;
		m_iROI_Bottom = m_rtROI.bottom;
		m_dFOV = m_pParam->m_fFOV;
		m_dPixelsize = m_pParam->m_fPixelsize;

		//m_bDiagonal = m_pParam->m_bEnableDiagonal;

		UpdateData(FALSE);
	}
}

void CDetectionModuleDlg::ApplyParam()
{
	if (m_pParam != NULL)
	{
		UpdateData(TRUE);

		m_pParam->m_iVDist = m_iVDist, m_pParam->m_iHDist = m_iHDist;
		//m_pParam->m_bMultiThreshold = m_bMultiThreshold = m_EnableMultiThresholdCheckBox.GetCheck();
		m_pParam->m_iMultiTHGray = m_iMultiTHGray;
		m_pParam->m_iThresholdLow = m_iThresholdLow, m_pParam->m_iThresholdHigh = m_iThresholdHigh;
		m_pParam->m_iThresholdLow2 = m_iThresholdLow2, m_pParam->m_iThresholdHigh2 = m_iThresholdHigh2;
		m_pParam->m_iMinSize = m_iMinSize;
		m_pParam->m_iMaxSize = m_iMaxSize;
		m_pParam->m_iMergeDist = m_iMergeDist;
		//m_pParam->m_bHoriCompare = m_bHoriCompare;	
		//m_pParam->m_bHoriCompare = IsDlgButtonChecked(IDC_CHECK_HORIZONTAL_COMPARE);
		//m_pParam->m_bEnableDiagonal = IsDlgButtonChecked(IDC_CHECK_DIAGONAL);

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

	//SetDefect(m_DDMAlgorithm->m_DefectFeature, m_DDMAlgorithm->m_iDefectCnt);

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