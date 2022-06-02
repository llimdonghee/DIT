
// ImageInspectionDlg.h : 헤더 파일
//

#pragma once
// #include "afxwin.h"
// #include "afxcmn.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"


#pragma comment(lib, "cxcore_64.lib")
#pragma comment(lib, "cv_64.lib")
#pragma comment(lib, "highgui_64.lib")

#include "DDMAlgorithm.h"
#include "afxwin.h"

#include "CDlgAutoCalPeriod.h"

class CImageViewWnd;
// CImageInspectionDlg 대화 상자
class CImageInspectionDlg : public CDialog
{
//	DECLARE_DYNAMIC(CImageInspectionDlg)

// 생성입니다.
public:
	CImageInspectionDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CImageInspectionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMAGEINSPECTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
public:

	enum DIR_TYPE {
		ROI_LEFT = 0,
		ROI_RIGHT = 1,
		ROI_TOP = 2,
		ROI_BOTTOM = 3
	};

	BOOL					LoadImage(CString strPath);
	void					DisplayStatus(CString strText);
	static DWORD WINAPI		ThreadProc(__in  LPVOID lpParameter);
	void					SetDefect(CDefectFeature* pDefectFeature, int iCnt);
	void					Inspection();
	void					ApplyParam();
	void					DefectMoveToPoint(CPoint ptPos, CSize szSize);
	void					DrawImage(IplImage* pImage, float fZoom=0.5f, CPoint ptScrollPos=CPoint(0,0));
	void					LoadParam();
	void					InitListHeader();
	void					InitImageListHeader();

	void					AutoCalInspect(BOOL bVer, BOOL bHor, int iPeriod);
	void					SetAutoCalPeriod(BOOL bVer, BOOL bHor, int iPeriod);
	CString					FindRecipeFolderShell();
	CString					FindRecipeFileName(CString folderPath);
	void					LoadRecipeParameter(CString strFileName);
	CString					GetRecipeData(CString strRecipeFileName, CString strStdData, int &nSplitBuffer, int &nLastBuffer);
	void 					SetRectCoordinate(CRect rect);
	void					SaveResult(CString _strResultFilePath);
	CvPoint					AutoMatchingROIOffset(IplImage* iplOrg, CRect rtMatchingROI, int iVDist, int iHDist, int iDirType, double dMatchingRate);

	CDlgAutoCalPeriod*		m_pAutoCalDlg;
 	CImageViewWnd*			m_pImageWnd;
	CParam*					m_pParam;
	CDDMAlgorithm*			m_DDMAlgorithm;
	CString					m_strImagePath;
	IplImage*				m_iplLoadImage;
	IplImage*				m_iplProcImage;
	IplImage*				m_iplSubtractLowImage;
	IplImage*				m_iplSubtractHighImage;
	BOOL					m_bImageProcessing;
	BOOL					m_bImageCutting;
	CRect					m_rtROI, m_rtCuttingROI;
	int						m_iDefectCount, m_iPixelCount;
	double					m_dblProcessingTime;
	int						m_iDisplayImageType;
	CString					m_strLoadImageFilePath;

	CString m_TopPath; //최상위 작업 폴더 경로
	CString m_fileName;
	CString m_originfileName;
	IplImage * m_img_color;
	int m_iImageSizeX, m_iImageSizeY;


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//
public:
	CStatic m_ctrlImageView;
	CListCtrl m_ctrlDefectList;
	CButton m_ctrlRadioConnect;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedSet();
	int m_iROI_Left;
	int m_iROI_Right;
	int m_iROI_Top;
	int m_iROI_Bottom;
	bool m_bMultiThreshold;
	int m_iMultiTHGray;
	bool m_bHoriCompare;
	int m_iMergeDist;
	int m_iMinSize;
	int m_iMaxSize;
	int m_iHDist;
	int m_iVDist;
	int m_iThresholdLow;
	int m_iThresholdHigh;
	int m_iThresholdLow2;
	int m_iThresholdHigh2;
	BOOL m_bAdjacentSlopeFilter;
	int	m_iEdgeJudgment;
	int	m_iEdgeSlopeTh;
	BOOL m_bPeriodSlopeFilter;
	int m_iPeriodSlopeJudgment;
	int	m_iPeriodSlopeTh;
	BOOL m_bTrackingFilter;
	int m_iTrackingRange;
	int m_iTrackingTh;
	BOOL m_bNearbyCheckFilter;
	BOOL m_bASGInspection;
	double m_dFOV;
	double m_dPixelsize;
	float m_fZoomRate;
	BOOL m_bDiagonal;
	BOOL m_bMountainFilter;

	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedButtonInspection();
	afx_msg void OnBnClickedButtonGrayDataSave();
	afx_msg void OnBnClickedButtonSaveRoiImage();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnNMClickDefectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedDefectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadrecipe();
	afx_msg void OnBnClickedButtonSaveResult();
	CButton m_EnableMultiThresholdCheckBox;
	CButton m_EnableHorCompCheckBox;
	CButton m_EnableEdgeSlopeCheckBox;
	CButton m_EnablePeriodSlopeCheckBox;
	CButton m_EnableTrackingFilterCheckBox;
	CButton m_EnableNearbyCheckCheckBox;
	CButton m_EnableASGInspectCheckBox;

private:
	CRect m_rtDisplayRoiCoordinate;

	virtual void OnOK();
public:
	CListBox m_ImageList;
	afx_msg void OnBnClickedButtonList();
	void typeImageRead(CString inputType);
	afx_msg void OnDblclkImagelist();
	void ZoomRateValue(float fZoom);
	afx_msg void OnSelchangeImagelist();
	void CreateHorizontalScroll();
	afx_msg void OnBnClickedButtonDDMCalc();
	afx_msg void OnBnClickedImageSizeLoad();
	CButton m_EnableDiagonal;
	CButton m_ctlShow;
	afx_msg void OnBnClickedCheckShow();
	BOOL m_bShowCheck;
	afx_msg void OnBnClickedRoiOffsetFunc();
	afx_msg void OnBnClickedButtonAutoCalPeriod();
	afx_msg void OnBnClickedCheckHorizontalCompare();
	CButton m_EnableMountainFilterCheckBox;
};
