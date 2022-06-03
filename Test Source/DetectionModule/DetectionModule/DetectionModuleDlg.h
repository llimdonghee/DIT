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
class CDetectionModuleDlg : public CDialog
{

public:
	CDetectionModuleDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETECTIONMODULE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	// 
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//AFX_MSG BUTTON
public:
	afx_msg void OnBnClickedButtonLoadImage();

//변수 선언
public:
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

//함수 선언
public:
	BOOL LoadFrameImage(CString strPath);
	void DisplayStatus(CString strText);

	void ZoomRateValue(float fZoom);
	void SetRectCoordinate(CRect rect);

};
