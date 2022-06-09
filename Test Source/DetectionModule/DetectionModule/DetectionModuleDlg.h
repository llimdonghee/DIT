#pragma once

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#pragma comment(lib, "cxcore_64.lib")
#pragma comment(lib, "cv_64.lib")
#pragma comment(lib, "highgui_64.lib")

#include "afxwin.h"

#include "DetectionAlgorithm/DetectionAlgorithm.h"
#include "CommonData.h"

class CImageViewWnd;
class CDlgGraphView;

class CDetectionModuleDlg : public CDialog
{

public:
	CDetectionModuleDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETECTIONMODULE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//virtual void OnInitialUpdate();

protected:
	HICON m_hIcon;

	// 
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//AFX_MSG BUTTON
public:
	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedButtonInspection();
	afx_msg void OnBnClickedSet();

	CButton m_EnableHorizontalComp;
	CButton m_EnableDiagonalComp;
//변수 선언
public:
	CDlgGraphView*			m_pDlgGraphView;

	CImageViewWnd*			m_pImageWnd;
	CStatic					m_ctrlImageView;
	CParam*					m_pParam;
	CDetectionAlgorithm*	m_DetectionAlgorithm;

	// Load Image (+ Info View)
	CString					m_strLoadImageFilePath;
	CString					m_strImagePath;
	float					m_fZoomRate;
	int						m_iImageSizeX, m_iImageSizeY;
	IplImage*				m_iplLoadImage;
	IplImage*				m_iplProcImage;

	// 검사 Param
	int m_iROI_Left;
	int m_iROI_Right;
	int m_iROI_Top;
	int m_iROI_Bottom;
	bool m_bMultiThreshold;
	int m_iMultiTHGray;
	BOOL m_bHoriCompare;
	BOOL m_bDiagonalCompare;
	int m_iMergeDist;
	int m_iMinSize;
	int m_iMaxSize;
	int m_iHDist;
	int m_iVDist;
	int m_iThresholdLow;
	int m_iThresholdHigh;
	int m_iThresholdLow2;
	int m_iThresholdHigh2;
	double m_dFOV;
	double m_dPixelsize;
	CRect m_rtROI, m_rtCuttingROI;
	int m_iDefectCount, m_iPixelCount;

//함수 선언
public:
	CRect m_rtDisplayRoiCoordinate;
	BOOL LoadFrameImage(CString strPath);
	void LoadRecipeParameter(CString strFileName);
	CString GetRecipeData(CString strRecipeFileName, CString strStdData, int &nSplitBuffer, int &nLastBuffer);
	void DisplayStatus(CString strText);

	void InitTabControl();
	void SetDialogColor();

	void ZoomRateValue(float fZoom);
	void SetRectCoordinate(CRect rect);

	afx_msg void OnSize(UINT nType, int cx, int cy);

//검사 관련 함수
public:
	void LoadParam();
	void ApplyParam();
	void Inspection(int iScanNo, int iFrameNo);
	static DWORD WINAPI ThreadProc(__in  LPVOID lpParameter);

	afx_msg void OnDropFiles(HDROP hDropInfo);
};
