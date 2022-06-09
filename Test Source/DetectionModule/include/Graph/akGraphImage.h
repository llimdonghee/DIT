#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataImage.h"

#include "akGraph/akRangeSettingNormalDlg.h"
#include "akGraph/akRangeSettingTimeDlg.h"


class AFX_EXT_CLASS CakGraphImage : public CakGraphBase , public CakDataImage
{
public:
	CakGraphImage(void);
	virtual ~CakGraphImage(void);


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);
	
	virtual void RenderGrid(Graphics* grfx, CDC* pDC);
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기
	
	
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //축 영역 그리기
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //제목 영역 그리기
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //해당 축에 틱그리기, 데이터 영역 외각선 그리기
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//마우스 움직일때 위치값 표시하는부분
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //그리는 부분 마지막에 추가(Redraw false 할때도 그림)
	
	virtual void RenderImage(Graphics* grfx, CDC* pDC);
	

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);


	virtual void SetColorType(int nIndex);

	inline int GetWindowPosX(double xvalue); //위치 값에 따른 윈도우 좌표
	inline int GetWindowPosY(double yvalue); //위치 값에 따른 윈도우 좌표
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //윈도우 좌표에 따른 데이터 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //윈도우 좌표에 따른 데이터 

	double GetValuePosRangeX(int nValue){return abs(GetValuePosDataX(nValue)-GetValuePosDataX(0));};
	double GetValuePosRangeY(int nValue){return abs(GetValuePosDataY(nValue)-GetValuePosDataY(0));};
	int GetWindowPosRangeX(double dValue){return abs(GetWindowPosX(dValue)-GetWindowPosX(0));};
	int GetWindowPosRangeY(double dValue){return abs(GetWindowPosY(dValue)-GetWindowPosY(0));};

	void SetReverseModeX(bool flag); //x축 역방향 표시
	bool GetReverseModeX(){return m_bReverseX;}; //x축 역방향 표시
	void SetReverseModeY(bool flag); //y축 역방향 표시
	bool GetReverseModeY(){return m_bReverseY;}; //y축 역방향 표시

	
	void SetClossLinePos(double x, double y);

	virtual void SetAutoScale();

	double getValueY(int nSeriesIndex, double xpos);
	int getDataIndex(int nSeriesIndex, double xpos);
	
	LRESULT OnRangeSettingNormalUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT OnRangeSettingTimeUpdate(WPARAM wParam, LPARAM lParam);

	
protected:
	
	bool m_bGrid;
	bool m_bReverseX;
	bool m_bReverseY;
	
public:
	CakRangeSettingNormalDlg* m_pRangeYSettingDlg;
	CakRangeSettingNormalDlg* m_pRangeXSettingDlg;
	bool m_bRangeYSetting;
	bool m_bRangeXSetting;
	bool m_bFastDrawAlg;
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


