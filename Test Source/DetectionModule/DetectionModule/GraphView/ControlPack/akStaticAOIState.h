#pragma once

#include "afxcmn.h"
#include <vector>

enum _AOIStep
{
	AS_None = 0,
	AS_GlassLoading,
	AS_GlassLoadingOK,
	AS_ScanReady,
	AS_ScanReadyOK,
	AS_ScanStart,
	AS_ScanEnd,//==AS_ScanStartOK
	AS_InspectionEnd,
	AS_InspectionEndOK,
	AS_DisplayMessage=100,
	AS_DisplayMessageOK
};

#define MAX_CAMNUM 64
#define MAX_SCANNUM 64

class CakStaticAOIState : public CStatic
{
public:
	CakStaticAOIState(void);
	virtual ~CakStaticAOIState(void);

	void setGlassLoading(int nScanNum, int nCamNum=0); //글라스 로딩
	void setGlassLoadingOK(); //글라스 로딩 완료

	void setScanReady();  //스캔 레디
	void setScanReadyOK(); //스캔 레디 완료

	void setScanStart(int nScanIndex); //스캔 시작
	void setScanEnd(int nCamIndex); //스캔 완료

	void setInspectionEnd(); //검사 후처리 시작
	void setInspectionEndOK(); //검사 후처리 완료

	void setDisplayMessage(char* pMessage, BOOL bBlink = TRUE);//메세지 표시할때 사용
	void setDisplayMessageOK();//메세지 표시할때 사용


protected:
	int m_nCamNum;
	int m_nScanNum;
	int m_nCurScanIndex;
	bool  m_bScanEnd[MAX_CAMNUM];//
	int m_nScanEndCamNum[MAX_SCANNUM];//
	DWORD m_nScanStartTime[MAX_SCANNUM];//
	DWORD m_nScanEndTime[MAX_SCANNUM];//
	int getScanEndCount();

	void drawGlassLoading(CDC* pDC, CRect rect, _AOIStep AOIStep);
	void drawScanReady(CDC* pDC, CRect rect, _AOIStep AOIStep);
	void drawScanStart(CDC* pDC, CRect rect, _AOIStep AOIStep);
	void drawScanStartVert(CDC* pDC, CRect rect, _AOIStep AOIStep);
	void drawInspectionEnd(CDC* pDC, CRect rect, _AOIStep AOIStep);

	void drawDisplayMessage(CDC* pDC, CRect rect, _AOIStep AOIStep);

	void makeFont(CFont* pFont, char*fontName, int nfontSize);
protected:
	_AOIStep m_nAOIStateStep;
	BOOL m_bBlinkSwitch;
	int m_nBlinkCounter;
	CString m_strDisplayMessage;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	COLORREF m_clrStateNormal;
	COLORREF m_clrStateRun1;
	COLORREF m_clrStateRun2;
	COLORREF m_clrStateOK;
	COLORREF m_clrStateOutLine;

	COLORREF m_clrTextColor1;
	COLORREF m_clrTextColor2;

public:
	CFont m_fntGlassLoading;
	CFont m_fntScanReady;
	CFont m_fntScanStart;
	CFont m_fntScanStartSub;
	CFont m_fntInspectionEnd;
	CFont m_fntDisplayMessage;

	BOOL m_nDrawMode;//0:가로, 1:세로
	int m_nMaxDisplayScanNum;//기본 3

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);






protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
