#pragma once

#include <emmintrin.h>
#include "../Fileini.h"
#include "../CommonData.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#if WIN32_VERSION
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "highgui.lib")
#else
#pragma comment(lib, "cxcore_64.lib")
#pragma comment(lib, "cv_64.lib")
#pragma comment(lib, "highgui_64.lib")
#endif

#define		MAX_PATTERNWIDTH			640
#define		MAX_PATTERNHEIGHT			640
#define		MAX_PATTERNSIZE				(MAX_PATTERNWIDTH * MAX_PATTERNHEIGHT)
#define		MAX_ZONE_IMAGE				MAX_PATTERNWIDTH*MAX_PATTERNHEIGHT
//#define		ZONE_MASK_WIDTH				((MAX_PATTERNWIDTH * 4) + CAMERA_CCD_SIZE)
//#define		ZONE_MASK_HEIGHT			MAX_PATTERNHEIGHT
#define		MAX_ZONE_CNT				20
#define		DEVIDE_NUMBER				8
// Pattern Matching 관련 Error
#define		ERR_PM_SKIP					0x2001
#define		ERR_PM_DET					0x2002
#define		ERR_PM_ZOOM					0x2004
#define		ERR_PM_SCANRESOLTION		0x2008
#define		ERR_PM_LOW_RATE				0x2016
#define		ERR_NONE					0
#define		ERR_REAL_ZONE_SKIP			0x3001
#define		ERR_ZONE_SORT_SKIP			0x3002

#define		MAX_1_MERGE_SIZE_BY_PIXEL	8192
#define		MAX_MERGE_PIXELCNT			MAX_1_MERGE_SIZE_BY_PIXEL
#define		MAX_DEFECT_PIXELCNT			8192	// 163840
#define		MAX_DEFECT_CNT				8192	// 18432

#define		DETECT_PARAM_PATH			"C:\\0_TEST\\DetectionModule\\Param.ini"

typedef struct tagEdgeSlopeFilterParam
{
	tagEdgeSlopeFilterParam() : left(0), right(0), x(0), y(0), iDistH(0), iDistV(0), iMAX_RPOCESS_WIDTH(16384),
		pInsp(NULL), pUp(NULL), pDown(NULL), xDifference(NULL), xDifferenceLow(NULL), xDifferenceHigh(NULL),
		bMultiTH(FALSE), bEnableEdgeSlope(FALSE), iEdgeJudgment(0), iEdgeSlopeTh(0),
		xResult(NULL), xResultLow(NULL), xResultHigh(NULL)
	{
		xThreshold_UseGray = _mm256_set1_epi8(static_cast<char>(0));
		xThreshold_UseGrayLow = _mm256_set1_epi8(static_cast<char>(0));
		xThreshold_UseGrayHigh = _mm256_set1_epi8(static_cast<char>(0));
		xImg = _mm256_set1_epi8(static_cast<char>(0));
		xZeros = _mm256_set1_epi8(static_cast<char>(0));
		xMask = _mm256_set1_epi8(0xFF);
	}

	void Set(int left, int right, int x, int y, int iDistH, int iDistV, int iMAX_RPOCESS_WIDTH,
		__m256i *pInsp, __m256i *pUp, __m256i *pDown, __m256i xImg,
		__m256i xThreshold_UseGray, __m256i xThreshold_UseGrayLow, __m256i xThreshold_UseGrayHigh,
		__m256i *xDifference, __m256i *xDifferenceLow, __m256i *xDifferenceHigh,
		BOOL bMultiTH, BOOL bEnableEdgeSlope, int iEdgeJudgment, int iEdgeSlopeTh,
		__m256i xZeros, __m256i xMask, __m256i *xResult, __m256i *xResultLow, __m256i *xResultHigh)
	{
		this->left = left;
		this->right = right;
		this->x = x;
		this->y = y;
		this->iDistH = iDistH;
		this->iDistV = iDistV;
		this->iMAX_RPOCESS_WIDTH = iMAX_RPOCESS_WIDTH;
		this->pInsp = pInsp;
		this->pUp = pUp;
		this->pDown = pDown;
		this->xImg = xImg;
		this->xThreshold_UseGray = xThreshold_UseGray;
		this->xThreshold_UseGrayLow = xThreshold_UseGrayLow;
		this->xThreshold_UseGrayHigh = xThreshold_UseGrayHigh;
		this->xDifference = xDifference;
		this->xDifferenceLow = xDifferenceLow;
		this->xDifferenceHigh = xDifferenceHigh;
		this->bMultiTH = bMultiTH;
		this->bEnableEdgeSlope = bEnableEdgeSlope;
		this->iEdgeJudgment = iEdgeJudgment;
		this->iEdgeSlopeTh = iEdgeSlopeTh;
		this->xZeros = xZeros;
		this->xMask = xMask;
		this->xResult = xResult;
		this->xResultLow = xResultLow;
		this->xResultHigh = xResultHigh;
	}

	void operator = (const tagEdgeSlopeFilterParam & rhs)
	{
		left = rhs.left;
		right = rhs.right;
		x = rhs.x;
		y = rhs.y;
		iDistH = rhs.iDistH;
		iDistV = rhs.iDistV;
		iMAX_RPOCESS_WIDTH = rhs.iMAX_RPOCESS_WIDTH;
		pInsp = rhs.pInsp;
		pUp = rhs.pUp;
		pDown = rhs.pDown;
		xImg = rhs.xImg;
		xThreshold_UseGray = rhs.xThreshold_UseGray;
		xThreshold_UseGrayLow = rhs.xThreshold_UseGrayLow;
		xThreshold_UseGrayHigh = rhs.xThreshold_UseGrayHigh;
		xDifference = rhs.xDifference;
		xDifferenceLow = rhs.xDifferenceLow;
		xDifferenceHigh = rhs.xDifferenceHigh;
		bMultiTH = rhs.bMultiTH;
		bEnableEdgeSlope = rhs.bEnableEdgeSlope;
		iEdgeJudgment = rhs.iEdgeJudgment;
		iEdgeSlopeTh = rhs.iEdgeSlopeTh;
		xZeros = rhs.xZeros;
		xMask = rhs.xMask;
		xResult = rhs.xResult;
		xResultLow = rhs.xResultLow;
		xResultHigh = rhs.xResultHigh;
	}

	int left, right, x, y, iDistH, iDistV, iMAX_RPOCESS_WIDTH;
	__m256i *pInsp, *pUp, *pDown, xImg;
	__m256i xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh;
	__m256i *xDifference, *xDifferenceLow, *xDifferenceHigh;
	BOOL bMultiTH;
	BOOL bEnableEdgeSlope;
	int iEdgeJudgment, iEdgeSlopeTh;
	__m256i xZeros, xMask;
	__m256i *xResult, *xResultLow, *xResultHigh;
} EdgeSlopeFilterParam;

typedef struct tagPeriodSlopeFilterParam
{
	tagPeriodSlopeFilterParam() : left(0), right(0), x(0), y(0), iDistH(0), iDistV(0),
		pInsp(NULL), xDifference(NULL), xDifferenceLow(NULL), xDifferenceHigh(NULL),
		bMultiTH(FALSE), bEnablePeriodSlope(FALSE), iPeriodSlopeJudgment(10), iPeriodSlopeTh(0),
		xResult(NULL), xResultLow(NULL), xResultHigh(NULL)
	{
		xThreshold_UseGray = _mm256_set1_epi8(static_cast<char>(0));
		xThreshold_UseGrayLow = _mm256_set1_epi8(static_cast<char>(0));
		xThreshold_UseGrayHigh = _mm256_set1_epi8(static_cast<char>(0));
		xImg = _mm256_set1_epi8(static_cast<char>(0));
		xZeros = _mm256_set1_epi8(static_cast<char>(0));
		xMask = _mm256_set1_epi8(0xFF);
	};

	void Set(int left, int right, int x, int y, int iDistH, int iDistV,
		__m256i *pInsp, __m256i xImg,
		__m256i xThreshold_UseGray, __m256i xThreshold_UseGrayLow, __m256i xThreshold_UseGrayHigh,
		__m256i *xDifference, __m256i *xDifferenceLow, __m256i *xDifferenceHigh,
		BOOL bMultiTH, BOOL bEnablePeriodSlope, int iPeriodSlopeJudgment, int iPeriodSlopeTh,
		__m256i xZeros, __m256i xMask, __m256i *xResult, __m256i *xResultLow, __m256i *xResultHigh)
	{
		this->left = left;
		this->right = right;
		this->x = x;
		this->y = y;
		this->iDistH = iDistH;
		this->iDistV = iDistV;
		this->pInsp = pInsp;
		this->xImg = xImg;
		this->xThreshold_UseGray = xThreshold_UseGray;
		this->xThreshold_UseGrayLow = xThreshold_UseGrayLow;
		this->xThreshold_UseGrayHigh = xThreshold_UseGrayHigh;
		this->xDifference = xDifference;
		this->xDifferenceLow = xDifferenceLow;
		this->xDifferenceHigh = xDifferenceHigh;
		this->bMultiTH = bMultiTH;
		this->iPeriodSlopeJudgment = iPeriodSlopeJudgment;
		this->bEnablePeriodSlope = bEnablePeriodSlope;
		this->iPeriodSlopeTh = iPeriodSlopeTh;
		this->xZeros = xZeros;
		this->xMask = xMask;
		this->xResult = xResult;
		this->xResultLow = xResultLow;
		this->xResultHigh = xResultHigh;
	}

	void operator = (const tagPeriodSlopeFilterParam & rhs)
	{
		left = rhs.left;
		right = rhs.right;
		x = rhs.x;
		y = rhs.y;
		iDistH = rhs.iDistH;
		iDistV = rhs.iDistV;
		pInsp = rhs.pInsp;
		xImg = rhs.xImg;
		xThreshold_UseGray = rhs.xThreshold_UseGray;
		xThreshold_UseGrayLow = rhs.xThreshold_UseGrayLow;
		xThreshold_UseGrayHigh = rhs.xThreshold_UseGrayHigh;
		xDifference = rhs.xDifference;
		xDifferenceLow = rhs.xDifferenceLow;
		xDifferenceHigh = rhs.xDifferenceHigh;
		bMultiTH = rhs.bMultiTH;
		bEnablePeriodSlope = rhs.bEnablePeriodSlope;
		iPeriodSlopeJudgment = rhs.iPeriodSlopeJudgment;
		iPeriodSlopeTh = rhs.iPeriodSlopeTh;
		xZeros = rhs.xZeros;
		xMask = rhs.xMask;
		xResult = rhs.xResult;
		xResultLow = rhs.xResultLow;
		xResultHigh = rhs.xResultHigh;
	}

	int left, right, x, y, iDistH, iDistV;
	__m256i *pInsp, xImg;
	__m256i xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh;
	__m256i *xDifference, *xDifferenceLow, *xDifferenceHigh;
	BOOL bMultiTH;
	BOOL bEnablePeriodSlope;
	int iPeriodSlopeJudgment, iPeriodSlopeTh;
	__m256i xZeros, xMask;
	__m256i *xResult, *xResultLow, *xResultHigh;
}PeriodSlopeFilterParam;

class CDetectionModuleDlg;

typedef struct tagDefectPos
{
	int x;
	int y;
	__m256i	POS;
	__m256i DefectGray;
	__m256i ReferenceGray;
	__m256i DeleteGray;
} DefectPos;

class CParam
{
public:
	CParam(void);
	~CParam(void);

	// 특성입니다.
public:
	BOOL		m_bEnableInspection;
	int			m_iVDist, m_iHDist;
	BOOL		m_bMultiThreshold;
	int			m_iMultiTHGray;
	int			m_iThresholdLow, m_iThresholdHigh, m_iThresholdLow2, m_iThresholdHigh2;
	int			m_iThresholdOffsetLow, m_iThresholdOffsetHigh, m_iThresholdOffsetLow2, m_iThresholdOffsetHigh2;
	int			m_iMinSize, m_iMaxSize;
	int			m_iMergeDist;
	BOOL		m_bHoriCompare;
	BOOL		m_bCrossCompare;
	BOOL		m_bAVACompare;
	BOOL		m_bSlopeFilter;
	int			m_iSlopeFactor;
	BOOL		m_bGrayFilter;

	BOOL		m_bCellOffsetZero;
	BOOL		m_bBubbleInspection;
	BOOL		m_bASGInspection;
	BOOL		m_bPIInspection;
	BOOL		m_bNonPeriodicInspection;
	BOOL		m_bEnableMultiHorPitch;
	int			m_iMultiHorPitch;
	BOOL		m_bEnableMultiVerPitch;
	int			m_iMultiVerPitch;

	BOOL		m_bEnableZone;
	BOOL		m_bEnableDiagonal;
	int			m_iZoneThresholdLow[MAX_ZONE_CNT], m_iZoneThresholdHigh[MAX_ZONE_CNT], m_iZoneThresholdLow2[MAX_ZONE_CNT], m_iZoneThresholdHigh2[MAX_ZONE_CNT];
	int			m_iZoneMinSize[MAX_ZONE_CNT];

	CRect		m_rtROI;

	double		m_fScanResolution;
	double		m_fRealFOV;
	double		m_fFOV;
	double		m_fPixelsize;

	float		m_fParamZoomValue;

	int			m_iImgSizeX, m_iImgSizeY;
	int			m_iZonePixelSizeX_um, m_iZonePixelSizeY_um;
	int			m_iPMSearchWidth, m_iPMSearchHeight;
	CString		m_strPatImgName, m_strZoneImgName;
	BYTE*		m_pPatternImage;
	BYTE*		m_pZoneImage;

	BOOL		m_bAdjacentSlopeFilter;
	int			m_iEdgeJudgment;
	int			m_iEdgeSlopeTh;
	BOOL		m_bPeriodSlopeFilter;
	int			m_iPeriodSlopeJudgment;
	int			m_iPeriodSlopeTh;
	BOOL		m_bTrackingFilter;
	int			m_iTrackingRange;
	int			m_iTrackingTh;
	BOOL		m_bNearbyCheckFilter;
	BOOL		m_bMountainFilter;

	// 작업입니다.
public:
	BOOL		LoadParam(CString strPath);
	BOOL		SaveParam(CString strPath);
	void		ZoomRate(float fZoom);
};

typedef struct
{
	int iX, iY;
	int	iGray;
	int	iReference;
	//	int iZone ;
	//	int iZoneX ;
	//	int iZoneY ;
} CDefectPixel;

typedef struct tagPatternRes
{
	int iMatchx[32] ;
	int iMatchy[32] ;

	int iMatchx_TEMP[200] ;
	int iMatchy_TEMP[200] ;
	int iTempCount ;

	int iOrgPosX ;
	int iOrgPosY ;

	double dReliabilityx[32] ;
	double dReliabilityy[32] ;

	double dVectorxx[32] ;
	double dVectorxy[32] ;
	double dVectoryx ;
	double dVectoryy ;

	double dSlopex ;
	double dSlopey ;
	double dDeterminant[32] ;

	double dDeterminant_AVRG ;

	int	iStatus ;
} PMResult ;

class CMergedDefect
{
public:
	CMergedDefect();
	~CMergedDefect();

	void Release();

	BOOL Set(int x, int y, int Gray, int Reference, int iMergeDist);
	BOOL Add(int x, int y, int Gray, int Reference);
	void Merge(CMergedDefect* pDefect);

	BOOL bDelete ;

	int m_iRealLeft;
	int m_iRealRight;
	int m_iRealTop;
	int m_iRealBottom;

	int m_iBoundaryLeft;
	int m_iBoundaryRight;
	int m_iBoundaryTop;
	int m_iBoundaryBottom;

	int m_iMergeDist;

	int	iPixelCnt ;

	CDefectPixel PixelList[MAX_1_MERGE_SIZE_BY_PIXEL] ;
};

class CMerge
{
public:
	CMerge() ;
	~CMerge() ;

	void ResetContiner() ;

	int Merge(int iCnt, DefectPos *pPixelResult, int iMergeDist, int iMinSize = 0, int iMaxSize = 9999, int iMaxCnt = 10, int side = -1) ;

public:
	int		iMergeDistance;
	BOOL	AddPixel(int x, int y, char Gray, char Reference);
	int GetExponent(int num, int under);

	CMergedDefect DefectList[MAX_1_MERGE_SIZE_BY_PIXEL];

public :
	int	iDefectCnt ;
};

class CDefectFeature
{
public :
	CDefectFeature()
	{
		Init() ;
	}

	~CDefectFeature()
	{
	}

	void Init()
	{
		int i;
		iFrameCount= 0;
		iArea= 0;
		iCenterx= 0;
		iCentery= 0;
		iMinx= 0;
		iMaxx= 0;
		iMiny= 0;
		iMaxy= 0;
		iDefectGraySum= 0;
		iDefectGraySquareSum= 0;
		iDefectMinGray= 0;
		iDefectMaxGray= 0;
		iReferenceGraySum= 0;
		iReferenceGraySquareSum= 0;
		iReferenceMinGray= 0;
		iReferenceMaxGray = 0 ;
		iMatchingReferenceMinGray= 0;   // eto 20130108
		iMatchingReferenceMaxGray = 0 ; // eto 20130108

		for (i = 0; i < MAX_ZONE_CNT; i++) {
			iZoneArea[i] = 0 ;
			iZoneGray[i] = 0 ;
			iZoneReferenceGray[i] = 0 ;
		}
		for(i = 0; i < 26; i++)
			iGrayCount[i] = 0;

		iIsPAD= 0;
		iDefectIndex= 0;
		width = 0 ;
		height = 0 ;
		ImgData = NULL ;

		memset(fileName, 0, 20) ;
		memset(RTCCode, 0, 20) ;

		fRealArea = 0.0;
		fRealDx = fRealDy = 0.0;
		fPerimeter = 0.0;
		fFiberWidth = 0.0;
		fFiberLength = 0.0;
		fCurl = 0.0;
		bBubble = FALSE;
		dBubbleMatchingRate = 0.0;
	}

	void Clear()
	{
		if (NULL != ImgData) delete[] ImgData;
		ImgData = NULL;

		Init();
	}

	int iFrameCount;
	int iArea;
	int iCenterx;
	int iCentery;
	int iMinx;
	int iMaxx;
	int iMiny;
	int iMaxy;
	int iDefectGraySum;
	int iDefectGraySquareSum;
	int iDefectMinGray;
	int iDefectMaxGray;
	int iReferenceGraySum;
	int iReferenceGraySquareSum;
	int iReferenceMinGray;
	int iReferenceMaxGray;
	int iMatchingReferenceMinGray; // eto 20130108
	int iMatchingReferenceMaxGray; // eto 20130108

	int iGrayCount[26];

	int iZoneArea[MAX_ZONE_CNT] ;
	int iZoneGray[MAX_ZONE_CNT] ;
	int iZoneReferenceGray[MAX_ZONE_CNT] ;
	int	iIsPAD;
	int iDefectIndex;
	int width ;
	int height ;

	BYTE *ImgData ;

	char fileName[20] ;

	double fRealArea;
	double fRealDx, fRealDy;
	double fPerimeter;
	double fFiberWidth;
	double fFiberLength;
	double fCurl;

	BOOL	bBubble;
	double	dBubbleMatchingRate;
	char	RTCCode[20];
};

class CDetectionAlgorithm
{
public:
	CDetectionAlgorithm(void);
	~CDetectionAlgorithm(void);

	// 특성입니다.
public:
	CDetectionModuleDlg* m_pDlgInspection;

	BYTE*				m_pImage;
	BYTE*				m_pSubtractLowImage;
	BYTE*				m_pSubtractHighImage;
	int					m_iImageWidth, m_iImageHeight;

	CParam				m_Param;
	DefectPos			m_pPixelResult[MAX_DEFECT_PIXELCNT];
	CMerge				m_Merge;
	CDefectFeature		m_DefectFeature[MAX_DEFECT_CNT];
	PMResult			m_PMResult;
	
	int					m_iDefectCnt;
	int					m_iPixelCnt;

	int					m_iVerticalCnt;
	int					m_iHorizontalCnt;

	// 작업입니다.
public:
	void				Clear() { m_Merge.ResetContiner(); m_DefectFeature->Clear(); }

	void				Inspection(BYTE* pImage, BYTE* pSubtractLowImage, BYTE* pSubtractHighImage, int iWidth, int iHeight);

	int					VerticalInspection(__m256i* pInsp, __m256i* pUp, __m256i* pDown, __m256i xImg, __m256i xUp, __m256i xDown, BOOL bMultiTH,
											__m256i xZeros, __m256i xOne, __m256i xMask,
											__m256i xThreshold, __m256i xThresholdH, __m256i xThreshold2, __m256i xThresholdH2,
											__m256i xSlope, __m256i xSlopeLow, __m256i xSlopeHigh,
											__m256i &xThreshold_UseGray, __m256i &xThreshold_UseGrayLow, __m256i &xThreshold_UseGrayHigh,
											__m256i &xDifference, __m256i &xDifferenceLow, __m256i &xDifferenceHigh,
											__m256i &xResult, __m256i &xResultLow, __m256i &xResultHigh);

	int					HorizontalInspection(__m256i* pInsp, __m256i* pLeft, __m256i* pRight, __m256i xImg, __m256i xLeft, __m256i xRight, BOOL bMultiTH,
											__m256i xZeros, __m256i xOne, __m256i xMask,
											__m256i xThreshold, __m256i xThresholdH, __m256i xThreshold2, __m256i xThresholdH2,
											__m256i xSlope, __m256i xSlopeLow, __m256i xSlopeHigh,
											__m256i &xThreshold_UseGray, __m256i &xThreshold_UseGrayLow, __m256i &xThreshold_UseGrayHigh,
											__m256i &xDifference, __m256i &xDifferenceLow, __m256i &xDifferenceHigh,
											__m256i &xResult, __m256i &xResultLow, __m256i &xResultHigh);

	int					InspectionProcess();

	BOOL				CellSubMultiInspection(int iFrameNo, int x, int y, __m256i *pInsp, __m256i *xImg, __m256i *pUp, __m256i *xUp, __m256i *pDown, __m256i *xDown, int iDistH, int iDistV,
						__m256i *xThreshold_UseGrayLow, __m256i *xThreshold_UseGrayHigh, __m256i *xThreshold_UseGray,
						__m256i *xDifferenceLow, __m256i *xDifferenceHigh, __m256i *xDifference, __m256i *xResultLow, __m256i *xResultHigh, __m256i *xResult, int MAX_RPOCESS_WIDTH_32,
						int iSubMultiCnt, int *iSubMultiDir, int *iSubMultiFilter, CRect &rtInsp, CRect &rtPreInsp, CRect &rtNextInsp, BOOL bMultiTH,
						BOOL bEnableDMR, BYTE *pCellMaskROI, BYTE* pPreCellMaskROI, BYTE *pNextCellMaskROI);

	int					ZoneInspection();
	int					ASGInspection();
	int					QDASGInspection();
	int					NonPeriodicInspection();
	int					EdgeInspection(BYTE *CurrentFrame, int left, int right, int top, int bottom, int offset, int th, int thH, DefectPos *pPixelResult, int iMAXCnt, int side, BOOL bSaveEdgeCrackImage);
	static int			CompareDefectSize(const void* arg1, const void* arg2);
	void				MakeVector(__m256i *img, int left, int right, int top, int bottom, PMResult *pResult);
	BOOL				MakeRealZoneMask(BYTE* pRealZoneMaskL, BYTE* pRealZoneMaskH, BYTE* pRealZoneMaskL2, BYTE* pRealZoneMaskH2, PMResult *pmResult, int left, int iGenerateWidth);
	BOOL				CellSlopeFilter(BYTE* pImg, int PosX, int PosY, int left, int right, int top, int bottom, int iDistV, int iDistH, int ThL, int ThH);
	BOOL				CellSlopeFilter2(BYTE* pImg, int PosX, int PosY, int left, int right, int top, int bottom, int iDistV, int iDistH, int ThL, int ThH, int iSlopeFactor=0);
	BOOL				ASGSlopeFilter(BYTE* pImg, int PosX, int PosY, int iDistH, int iDistV, int left, int right, int top, int bottom);
	int					MakeFeature(int iPixelCnt);
	BOOL				MakeMacroImage(__m256i *pImg, BYTE *pImage, int iWidth, int iHeight, int iMacroRatio);
	BOOL				SaveAsBMP(CString strFileName, BYTE *pImg, int iWidth, int iHeight, BOOL bFLR, BOOL bFUD);
	BOOL				SobelEdge( unsigned char *pSour, unsigned char *pDest, int iWidth, int iHeight, int nPitch);

	int					SubInspection(BYTE* pImage1, BYTE* pImage2, int iWidth, int iHeight, int iThL, int iThH, int iThL2, int iThH2, BOOL bMultiTH);

	BOOL				TrackingHoriFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap);
	BOOL				TrackingVertFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap);
	BOOL				TrackingLT2RBFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap);
	BOOL				TrackingRT2LBFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap);
	void				EdgeSlopeFilter(EdgeSlopeFilterParam stEdgeSlopeFilter);
	void				PeriodSlopeFilter(PeriodSlopeFilterParam stPeriodSlopeFilter);
	BOOL				PADFilter(BYTE* _pImg, int PosX, int PosY, POINT *pt, int ThL, int ThH);
	BOOL				GetGray(BYTE* pImg, int x, int y, int *gray, BOOL bIsHor);
//	void				DotFilter(EdgeSlopeFilterParam stEdgeSlopeFilter);
	
	//Cell Filter
	BOOL Cell2PixelMergeFilter(int _iCurFrameNo, int _iRef1FrameNo, int _iRef2FrameNo, int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray, BYTE* Tar, BYTE* Ref1, BYTE* Ref2);
	BOOL Cell2PixelVertMergeFilter(int _iCurFrameNo, int _iRef1FrameNo, int _iRef2FrameNo, int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray);
	BOOL Convolution2x2Filter(int _iCurFrameNo, int _iRef1FrameNo, int _iRef2FrameNo, int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray, BYTE* Tar, BYTE* Ref1, BYTE* Ref2);
	int Subtract(int _iGray1, int _iGray2);
};