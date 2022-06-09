#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataBasic1.h"
#include "akGraph/akColorSettingDlg.h"
#include <vector>

class AFX_EXT_CLASS CakGraphCodeJ : public CakGraphBase, public CakDataBasic1
{
public:

public:
	CakGraphCodeJ(void);
	virtual ~CakGraphCodeJ(void);

public:
	virtual	void RenderBegin( CDC* pDC, CRect rectDC );
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기

	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //
	virtual void RenderTitle(Graphics* grfx, CDC* pDC);
	virtual void RenderEnd(Graphics* grfx, CDC* pDC);

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	inline int GetWindowPosX(double xvalue); //위치 값에 따른 윈도우 좌표
	inline int GetWindowPosY(double yvalue); //위치 값에 따른 윈도우 좌표
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //윈도우 좌표에 따른 데이터 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //윈도우 좌표에 따른 데이터 

	virtual void SetAutoScale();
	virtual void SetDataNum(int datanum);

	virtual void ReDraw(bool bReDrawALL);

	void SetHorMode(bool bFlag = true); //가로 모드

	void SetCalculatorAvr();//값들의 평균을 화면에 표시
	CString m_strAVR;

	void SetAutoBarScale(int margin);
	int m_nBarGab; //막대 그래프 사이의 간격
	
	void SetFullScreen(bool flag);
	void SetFullMode(bool flag, int x, int y, int width, int height); //풀 모드 키고 끔.. 그리고 풀모드 싸이즈

	void GraphType(int nType);

	bool GetValuebyMousePos(int mousex, int mousey, int* index, double* val);

private:
	
	
protected:
	bool m_bGrid;
	CakColorSettingDlg* m_pColorSettingDlg;
public:
	bool m_bIntegerAxisX;
	bool m_bIntegerAxisY;

	bool m_bAVR;//화면에 평균값 표시 할 것인지에 대한 플레그
	std::vector<double> m_vecProbabilityData; //이상이하확률계산데이터
	int m_nBarWidth;


	CakRect m_rectFullMode;
	CakRect m_rectNormalMode;
	
	int m_nGraphType; //0은 바타입 선 타입 모두 표현, 1은 바타입만 표현
	int m_nTextValueShowLevel; //그래프 위에 텍스트로 값 표시(-1은 표시 안함 0에서 3까지 소수 자리수 표현)

	CString*  m_strAxisXText;
	COLORREF* m_clrAxisXColor;
	double m_avr;
protected:
	double m_nTimeline;
	bool m_bFullScreen;
	bool m_bFullMode;

	bool m_bHorMode;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	LRESULT OnColorSettingUpdate(WPARAM wParam, LPARAM lParam);
};
