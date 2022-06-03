#pragma once

#include "ImageStone.h"
#include "memdc.h"

//#include "afxext.h"
#include "RcTracker.h"
#include "resource.h"
#include "PCL_interface_zoom.h"
#include "CommonData.h"

class CDetectionAlgorithm;
class CDetectionModuleDlg;

class CImageViewWnd : public CWnd
				  , public PCL_Interface_ZoomScale
{
	DECLARE_DYNAMIC(CImageViewWnd)
public:
	CImageViewWnd(void);
	virtual ~CImageViewWnd(void);
	DECLARE_MESSAGE_MAP()

public:
	FCObjImage	m_DrawImage, m_OriginImage, m_CuttedImage;

	CSize		m_szImageSize;
	BOOL		m_bBoxDraw;
	BOOL		m_bProfile, m_bProfileStart;
	BOOL		m_bBoundingBoxView;
	BOOL		m_bCutting, m_bCutStart;
	BOOL		m_bCutted;
	BOOL		m_bCalcRect;
	BOOL		m_bPanImage;
	BOOL		m_bPanStart;
	float		m_fZoom;
	CPoint		m_ptBoxDraw;
	CRect		m_rtProfile;
	CRect		m_rtCalcRect;
	CRect		m_rtCutting;
	CRect		m_rtDrawView;
	CRect		m_rtEdge;
	int			m_iEdgeType;
	int			m_iEdgeThreshold;
	int			m_iEdgeSubCutting;
	int			m_iInspectionPitch;
	BOOL		m_bPaintDefect;
	BOOL		m_bCalcRealFOV;
	CPoint		m_ptPixel;
	CPoint		m_ptPanStart;
	CPoint		m_ptSaveGrayStart, m_ptSaveGrayEnd;
	CRect		m_rtSaveGrayProfile;
	BOOL		m_bNormalizeGray;
	CString		m_CSZoomRate;
	CRcTracker	m_rtTracker;

	CDetectionAlgorithm* m_pDetectionAlgorithm;
	CDetectionModuleDlg* m_pDlgDetectionModule;

	float		m_fZoomRate;
public:
	void		ResetImage();
	void		DrawImage(FCObjImage* pDrawImage, float fZoom=6.0f, CPoint ptScrollPos=CPoint(0,0));
	double		Calc_MeanGray(int startX, int startY, int width, int height);
	double		Calc_StdGray(double fMean, int startX, int startY, int width, int height);
	BOOL		Histogram(int startX, int startY, int width, int height, int *pBuffer, int *nMinGray, int *nMaxGray, int *nMaxCount, int *MaxCountGray, double* dblContrast);
	void		GetGrayStatistics(int left, int right, int top, int bottom, double &fMaxGray, double &fMinGray, double &fMeanGray, double &fStdev);
	void		MoveToPoint(CPoint point, BOOL bBoxDraw, BOOL bMovePosition);
	void		ShowFindedEdge(int nType, CPoint ptRef, int nThreshold, int nSubCutting);
	void		ZoomImage(float fZoomFactor, CPoint pt);
	void		DrawProfile(BOOL bProfile) { m_bProfile = bProfile; }
	void		ImageCutting(BOOL bCutting);
	BOOL		SaveCuttingImage();
	CRect		GetCuttingROIRect();
	void		PaintDefect(CDC *pDC);
	void		PaintROI(CDC *pDC);
	void		PaintPixel(CDC* pDC);
	void		PaintGrid(CDC* pDC);
	void		SetPaintDefect(BOOL bPaint) { m_bPaintDefect = bPaint; }
	void		SetCalcRealFOV(BOOL bCalc) { m_bCalcRealFOV = bCalc; }
	void		SetPanImage(BOOL bPan) { m_bPanImage = bPan; }
	double		CalcRealFOV(CDC *pDC);

	void		ModifyIndicator_ImageSize(CSize szImage, CSize szView);
	void		ModifyIndicator_Zoom(float fZoom);
	void		ModifyIndicator_Position(CPoint ptPos, CPoint ptPixel, BYTE btUp, BYTE btImage, BYTE btDown, BYTE btLeft, BYTE btRight, BYTE btDiff, BYTE btThreshold);
	void		ModifyIndicator_Image_Position(CPoint ptPos, BYTE btImage, BYTE btImageUp, BYTE btImageDown, BYTE btImageLeft, BYTE btImageRight,
												BYTE btUp, BYTE btDown, BYTE btLeft, BYTE btRight, BYTE btVertDiff, BYTE btHoriDiff,
												BYTE btMultiUp, BYTE btMultiDown, BYTE btMultiLeft, BYTE btMultiRight, BYTE btMultiVertDiff, BYTE btMultiHoriDiff,
												BYTE btThirdUp, BYTE btThirdDown, BYTE btThirdLeft, BYTE btThirdRight, BYTE btThirdVertDiff, BYTE btThirdHoriDiff,
												BYTE btThreshold);


	FCObjImage*	GetDrawImage() { return &m_DrawImage; }
	float		GetZoomFactor() { return m_fZoom; }
	CPoint		GetScrollPosition();

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const struct tagRECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnProfile();
	afx_msg void OnDrawDefect();
	afx_msg void OnUpdateProfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawDefect(CCmdUI *pCmdUI);
	afx_msg void OnPanImage();
	afx_msg void OnUpdatePanImage(CCmdUI *pCmdUI);
	afx_msg void OnNormalizeGray();
	afx_msg void OnUpdateNormalizeGray(CCmdUI *pCmdUI);
	virtual BOOL DestroyWindow();

private:
	bool m_bIsDisplayRoiCoordinate;
};
