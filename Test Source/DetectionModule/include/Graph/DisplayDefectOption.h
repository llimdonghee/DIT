#pragma once

#include "AOIDefinitionType.h"
#include "BaseServer/Glass/Defect.h"

#define	COLOR_0			RGB(60,128,50)		// PadB
#define	COLOR_1			RGB(5,64,128)		// PadW
#define	COLOR_2			RGB(0,200,200)//(33,216,184)		// RB
#define	COLOR_3			RGB(88,224,88)//(148,138,84)		// RW
#define	COLOR_4			RGB(255,0,0)//(136,30,30)		// TB
#define	COLOR_5			RGB(0,0,255)//(105,161,191)	// TW
#define	COLOR_6			RGB(100,0,100)//(255,100,255)	// MD
#define	COLOR_7			RGB(128,128,128)	// CD
#define	COLOR_8			RGB(0,100,0)//(82,55,56)		// EG
#define	COLOR_9			RGB(255,255,0)		// NO
#define	COLOR_15		RGB(255,255,0)		// NO

#define		INDEX_PB			0
#define		INDEX_PW			1
#define		INDEX_RB			2
#define		INDEX_RW			3
#define		INDEX_TB			4
#define		INDEX_TW			5
#define		INDEX_MD			6
#define		INDEX_CD			7
#define		INDEX_EG			8
#define		INDEX_NO			9


// Stack Flag 별 구분
#define		INDEX_TD			0
#define		INDEX_SD			1
#define		INDEX_PD			2
#define		INDEX_SP			3

// Stack Flag 별 구분
#define		INDEX_SMALL			0
#define		INDEX_MID			1
#define		INDEX_LARGE			2
#define		INDEX_HUGE			3
#define		INDEX_UHUGE			4
#define		INDEX_UNKNOWN		5

// Judgement 별 구분
#define		INDEX_OK			0
#define		INDEX_TR			1
#define		INDEX_PR			2
#define		INDEX_UN			3

// Cam 별 구분
#define		INDEX_1D			0
#define		INDEX_2D			1
#define		INDEX_ALL			2

class CDisplayDefectOption
{
public:
	CDisplayDefectOption(void);
	virtual ~CDisplayDefectOption(void);

	
	BOOL				IsShowDefect(CDefect* pDefect);
	BOOL				IsFilteringDefect(CDefect* pDefect);

public:

	BOOL				m_bApplyCutoff;
	BOOL				m_bApplyNG;
	BOOL				m_bApplyRP;

	BOOL				m_bShowType[CFDEFECT_TYPECOUNT];
	BOOL				m_bShowCode[CFDEFECT_MAXLISTCOUNT];
	BOOL				m_bShowStackFlag[4];
	BOOL				m_bShowSize[CFDEFECT_SIZECOUNT];
	BOOL				m_bShowStackStep[CFDEFECT_STACKCOUNT];
	BOOL				m_bShowJudgement[CFDEFECT_JUDGEMENTCOUNT];
	BOOL				m_bShowCam[CFDEFECT_CAMCOUNT];

	//글라스맵에서 사용하는 옵션 [김태현 2018/8/10]
	BOOL				m_bShowTCam;
	BOOL				m_bShowRCam;
	BOOL				m_bShowInspFail;
	BOOL				m_bShowMasking;
	BOOL				m_bShowCellOrigin; 
	BOOL				m_bShowApplyCutoff;
	BOOL				m_bShowRecipeName;
	int					m_nScaningIndex;
	int					m_nShowCodeMax;

	// 보기옵션.
	DefectShowOption	m_ShowOption;
};
