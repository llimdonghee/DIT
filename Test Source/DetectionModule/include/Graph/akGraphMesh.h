#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataMesh.h"
#include "akGraph/akColorTable.h"
#include "akGraph/akColorTableOption.h"
#include "akGraph/akRangeSettingNormalDlg.h"

class AFX_EXT_CLASS CakGraphMesh : public CakGraphBase, public CakDataMesh
{
public:
	CakGraphMesh(void);
	virtual ~CakGraphMesh(void);

public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);

	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //
	virtual void RenderTitle(Graphics* grfx, CDC* pDC);
	virtual void RenderEnd(Graphics* grfx, CDC* pDC);
	virtual void RenderMouseHoverInfo(CDC* pDC);

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void CaculatorGraphSize();

	virtual void SetDataNum(int sizex, int sizey);

	void SetReverseModeX(bool flag); //x축 역방향 표시
	bool GetReverseModeX(){return m_bReverseX;}; //x축 역방향 표시
	void SetReverseModeY(bool flag); //x축 역방향 표시
	bool GetReverseModeY(){return m_bReverseY;}; //x축 역방향 표시

	inline int GetWindowPosX(double xvalue); //위치 값에 따른 윈도우 좌표
	inline int GetWindowPosY(double yvalue); //위치 값에 따른 윈도우 좌표
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //윈도우 좌표에 따른 데이터 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //윈도우 좌표에 따른 데이터 

	virtual void SetAutoScale();
	
	double GetDataValue(double posx, double posy);//
	void CaculatorDataRange(double *min, double *max);

	void SetColorTableFile(char* pFileName);
	virtual BOOL ShowColorTableOptionDlg(CPoint point);
protected:
	
protected:
	bool m_bReverseX;
	bool m_bReverseY;

public:
	double m_NoDataValue; //데이터가 없는 곳의 값 처리
	

	CakColorTableOptionDlg* m_pColorTableOption;
	CakRangeSettingNormalDlg* m_pRangeXSettingDlg;
	CakRangeSettingNormalDlg* m_pRangeYSettingDlg;

	bool m_bRangeYSetting;
	bool m_bRangeXSetting;

	bool m_bDataMinAnti;
	bool m_bDataMaxAnti;

	CString m_strComment;//데이터 영역 우측 상단에 코멘트 표시
protected:
	CakColorTable m_ColorTable;
	
	//컬러테이블 관련
public:
	bool m_bColorBar;//색상바 표시 설정
	bool m_bColorTableTickBar;
	CakRect m_rectColorTablebar;
	COLORREF m_ColorBarFontColor; //컬러바 폰트 컬러
	int m_colorMin;
	int m_colorMax;
	int m_colorLevel;
	int m_colorIndex;
	bool m_colorInverse;
	bool m_colorInterpolation;
	bool m_colorAuto;
	
	
public:
	DECLARE_MESSAGE_MAP()
	
	afx_msg LRESULT OnColorTableOptionUpdate(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRangeSettingUpdate(WPARAM wParam, LPARAM lParam);
	

	
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	
	
};
