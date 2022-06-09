#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataVector2.h"

#include "akGraph/akRangeSettingNormalDlg.h"
#include "akGraph/akRangeSettingTimeDlg.h"

//벡터 데이터를 쓰는 시리즈 한개뿐인 비등간격 데이터

class AFX_EXT_CLASS CakGraphType2V : public CakGraphBase , public CakDataVector2
{
public:

public:
	CakGraphType2V(void);
	virtual ~CakGraphType2V(void);


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);
	
	virtual void RenderGrid(Graphics* grfx, CDC* pDC);
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //데이터 선/바 그리기
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //축 영역 그리기
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //제목 영역 그리기
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //해당 축에 틱그리기, 데이터 영역 외각선 그리기
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //그리는 부분 마지막에 추가(Redraw false 할때도 그림)

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	//데이터 클래스 동기화 객체 추가
	virtual void Clear();
	virtual bool SetData(int index, double x, double y); //들어있는 데이터 값 변경
	virtual void AddData(double x, double y, int index = -1); //데이터 추가
	virtual void SetHidden(bool enable, int startIndex, int endIndex);


	inline int GetWindowPosX(double xvalue); //위치 값에 따른 윈도우 좌표
	inline int GetWindowPosY(double yvalue); //위치 값에 따른 윈도우 좌표
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //윈도우 좌표에 따른 데이터 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //윈도우 좌표에 따른 데이터 

	void SetReverseModeX(bool flag); //x축 역방향 표시
	bool GetReverseModeX(){return m_bReverseX;}; //x축 역방향 표시
	void SetReverseModeY(bool flag); //y축 역방향 표시
	bool GetReverseModeY(){return m_bReverseY;}; //y축 역방향 표시

	void SetAxisXMode(bool flag){m_nAxisXMode = flag;};

	virtual void SetAutoScale();

	double GetYvalueByXposition(double xpos){return 0;};//미구현
	
	LRESULT OnRangeSettingNormalUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT OnRangeSettingTimeUpdate(WPARAM wParam, LPARAM lParam);


private:
	bool m_bGrid;
	bool m_bReverseX;
	bool m_bReverseY;
	int m_nAxisXMode; //0:일반 모드, 1:초단위 시간표시, 2:ms단위 시간표시
public:
	CakRangeSettingNormalDlg* m_pRangeYSettingDlg;
	CakRangeSettingNormalDlg* m_pRangeXSettingDlg;
	afx_msg void OnDestroy();
};
