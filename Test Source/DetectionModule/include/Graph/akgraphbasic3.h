#pragma once

#include "Graph/akGraphBase.h"
#include "Graph/akDataBasic3.h"

#include "Graph/akRangeSettingNormalDlg.h"
#include "Graph/akRangeSettingTimeDlg.h"


//class AFX_EXT_CLASS CakGraphType3 : public CakGraphULS
class AFX_EXT_CLASS CakGraphBasic3 : public CakGraphBase , public CakDataBasic3
{
public:
	CakGraphBasic3(void);
	virtual ~CakGraphBasic3(void);


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);
	
	virtual void RenderGrid(Graphics* grfx, CDC* pDC);
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //데이터 선/바 그리기
	virtual void RenderLegend(Graphics* grfx, CDC* pDC);
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //축 영역 그리기
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //제목 영역 그리기
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //해당 축에 틱그리기, 데이터 영역 외각선 그리기
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//마우스 움직일때 위치값 표시하는부분
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //그리는 부분 마지막에 추가(Redraw false 할때도 그림)
	
	

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	//데이터 클래스 동기화 객체 추가
	virtual void SetSeriesNum(int subsetNum); //기존 데이터 초기화
	virtual void SetSeriesAdd(int index = -1); 
	virtual bool SetSeriesDelete(int index);
	virtual void SetDataNum(int nDataNum);
	//virtual bool SetData(int index, double x, double y); <--요녀석은 알아서 동기화 시키도록.. 성능!!
	virtual void SetHidden(bool enable, int startIndex, int endIndex);

	virtual void SetColorType(int nIndex);

	inline int GetWindowPosX(double xvalue); //위치 값에 따른 윈도우 좌표
	inline int GetWindowPosY(double yvalue); //위치 값에 따른 윈도우 좌표
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //윈도우 좌표에 따른 데이터 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //윈도우 좌표에 따른 데이터 

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


//X축 값을 h/m/s/ms 표현이 가능하게
class AFX_EXT_CLASS CakGraphType3Time : public CakGraphBasic3
{
public:

public:
	CakGraphType3Time(void){};
	virtual ~CakGraphType3Time(void){};

public:
	virtual void RenderGrid(Graphics* grfx, CDC* pDC);
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //해당 축에 틱그리기, 데이터 영역 외각선 그리기
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);
	
	void getTime(double timeval, int *hour, int *min, int *sec, float *msec);
};


