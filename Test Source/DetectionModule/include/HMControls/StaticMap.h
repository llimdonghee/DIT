#if !defined(AFX_STATICMAP_H__5A9433C1_E8A9_4393_9E0D_B7A996B9AF3F__INCLUDED_)
#define AFX_STATICMAP_H__5A9433C1_E8A9_4393_9E0D_B7A996B9AF3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticMap.h : header file
//

#define ORIGIN_LEFTTOP		1
#define ORIGIN_LEFTBOTTOM	0
#define ORIGIN_RIGHTTOP		2
#define ORIGIN_RIGHTBOTTOM	3

#define UM_MAP_CHANGED			5573
/////////////////////////////////////////////////////////////////////////////
// CStaticMap window
enum OriginDirection		{ OD_LeftTop = 0, OD_RightTop = 1, OD_LeftBottom = 10, OD_RightBottom = 11 };

class AFX_EXT_CLASS CStaticMap : public CStatic
{
// Construction
public:
	CStaticMap();

// Attributes
public:

// Operations
public:
	void				InitGlass(CWnd* pParent, int nMapWidth, int nMapHeight, int nMapMargin);	// Pixel 단위 마진.
	void				DeinitGlass();
	int					GetMapTotalWidth()		{ return m_nMapTotalWidth; }
	int					GetMapTotalHeight()		{ return m_nMapTotalHeight; }
	int					GetMapWidth()			{ return m_nMapWidth; }
	int					GetMapHeight()			{ return m_nMapHeight; }

	void				SetMiniMap(BOOL bIsMiniMap, int nUnitSize)	{ m_bIsMiniMap = bIsMiniMap; m_nUnitSize = nUnitSize; }
	int					GetUnitSize()			{ return m_nUnitSize; }
	
	virtual void		DrawGlass(CDC* pDC)						{ }
	virtual void		MouseLBtnDwn(CPoint& point)				{ }
	virtual void		MouseLBtnUp(CPoint& point)				{ }
	virtual void		MouseLBtnMove(CPoint& point)			{ }
	virtual void		MouseLBtnDblClk(CPoint& point)			{ }
	virtual void		MouseRBtnDwn(CPoint& point)				{ }
	virtual void		MouseRBtnUp(CPoint& point)				{ }
	virtual void		MouseRBtnDblClk(CPoint& point)			{ }
	virtual void		RegistNoDefect()						{ }
	virtual void		Timer(UINT nIDEvent)					{ }

	// 확대/축소
	void				IncreaseRatio();
	void				DecreaseRatio();
	void				ResetRatio();

	CPoint&				GetDisplayStart()		{ return m_PointGlassStart; }
	CPoint&				GetDisplayEnd()			{ return m_PointGlassEnd; }

	void				SetDisplayRect(CPoint& PointStart, CPoint& PointEnd);
	void				SetShowShot(BOOL bShow)	{ m_bShowShot = bShow; }

	// 저장
	void				SaveMapToFile(CString strFileName);
	void				SetBGColor(COLORREF color) { m_clBGColor = color; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticMap();

	// Generated message map functions
protected:
	void				SetMappingMode(CDC *pDC);
	CPoint				SetWindowToGlass(CPoint pt);
	CPoint				SetGlassToWindow(CPoint pt);
	void				ControlZoom(BOOL bZoomIn, CPoint ptMouse);

	void				PopupMenu(int nBaseID, CPoint point);

	CWnd*				m_pParentWnd;
	int					m_nMapTotalWidth;
	int					m_nMapTotalHeight;
	int					m_nMapWidth;
	int					m_nMapHeight;
	int					m_nMapMarginWidth;
	int					m_nMapMarginHeight;
	CRect				m_RectWnd;
	CRect				m_RectMap;
	long				m_clBGColor;
	long				m_clPatColor;
	//////////////////////////////////////////////////////////////////////////
	// 그리기 정보
	HBITMAP				m_hDrawingSurface;
	BITMAPINFOHEADER	m_BMIH;
	CRect				m_RectDrawingSurface;
	BYTE*				m_pDrawingSurfaceBits;

	OriginDirection		m_OriginDir;
	int					m_nUnitSize;

	// Cell 그리기
	BOOL				m_bShowShot;

	// 확대/축소
	CRect				m_RectShowWnd;

	double				m_dRatio;
	CPoint				m_PointWindowStart;
	BOOL				m_bDragMove;
	CPoint				m_PointDragStart;
	BOOL				m_bIsMiniMap;			// Minimap은 확대/축소를 안한다.
	CPoint				m_PointGlassStart;		// Glass 중 현재 보여지는 영역.
	CPoint				m_PointGlassEnd;

	POINT				m_PointCCut[3];			//코너컷 그리기 좌표
	int					m_nCCutSize;
	int					m_nCoordGabX;			// 좌표를 뿌릴 거리.
	int					m_nCoordGabY;			// 좌표를 뿌릴 거리.

	//{{AFX_MSG(CStaticGlass)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg long OnUpdateMap(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuChangeViewMode(UINT nID);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICMAP_H__5A9433C1_E8A9_4393_9E0D_B7A996B9AF3F__INCLUDED_)
