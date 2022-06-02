#pragma once

#include "ImageInspection.h"
#include "CamInfo.h"
#include "Polygon.h"
#include "ROI.h"
using namespace std;

// DDMCalculator 대화 상자

typedef struct
{
	int		iScanLength;
	BOOL	bForward;
	long	lDDMOffsetX;
	long	lDDMOffsetY;
	long	lDDMBaseOffsetX;
	long	lDDMBaseOffsetY;
	double	fFOV[4];
}CamParam;

class DDMCalculator : public CDialogEx
{
	DECLARE_DYNAMIC(DDMCalculator)

public:
	DDMCalculator(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DDMCalculator();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DDMCALC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectRecipeFile();
	afx_msg void OnBnClickedButtonCoordinateToImageposition();
	virtual BOOL OnInitDialog();
	void	Clear();
	void	LoadRecipe(CString _strRecipeFileName);
	void	EncoderPositionCalc();
	int		ADDCam(int iCamNo, int iDDMOffset_x, int iDDMOffset_y, int	IsForwardDir, double *fFOV, int iScanLength = 0, int iDDMBaseOffsetX = 0, int iDDMBaseOffsetY = 0);
	int		MakeInspectionFrameInfo(int iCamNo, POINT* _ptVertex);

	int		m_iGlassOrigin;
	int		m_iScanCnt;
	int		m_iGlassSizeX;
	int		m_iGlassSizeY;
	int		m_iCamCnt;
	int		m_iFrameNo;
	double	m_fScanResolution;
	double	m_dSpeedRipple;
	double	m_fEncoderResolution;
	double	m_iCamStage;
	BOOL	m_bCenter;

	vector<CamParam>	m_vecCamParam; 
	vector<CCamInfo>	m_vCamInfo;
	POINT ptROI[4];
	int m_iEditCalcLeft;
	int m_iEditCalcRight;
	int m_iEditCalcTop;
	int m_iEditCalcBottom;
};
