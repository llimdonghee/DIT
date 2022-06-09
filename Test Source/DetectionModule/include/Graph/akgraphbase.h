#pragma once

#include "Graph/akGraphLinker.h"

#include "Graph/akMemDC.h"
#include "Graph/akGraphStruct.h"
#include "Graph/akSyncObject.h"
#include "STL/akStruct.h"


// akGraphBase

#define AKGRAPH_BUTTONDOWN WM_USER+0x1001
#define AKGRAPH_MOUSEMOVE WM_USER+0x1002


#include <process.h>
class AFX_EXT_CLASS CakGraphThreadDraw;

class AFX_EXT_CLASS  CakGraphBase :  public CWnd, public CakSyncObject
{
	DECLARE_DYNAMIC(CakGraphBase)

public:
	CakGraphBase();
	virtual ~CakGraphBase();

protected:
	DECLARE_MESSAGE_MAP()


	CakMouseState m_MouseState;


public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);
	
	virtual void RenderBegin(CDC* pDC, CRect rectDC);
	virtual void RenderGrid(Graphics* grfx, CDC* pDC){};
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //데이터 영역 그리기
	virtual void RenderSeries(Graphics* grfx, CDC* pDC){}; //데이터 선/바 그리기
	virtual void RenderLegend(Graphics* grfx, CDC* pDC){}; //범례
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //축 영역 그리기
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //제목 영역 그리기
	virtual void RenderLabel(Graphics* grfx, CDC* pDC){}; //해당 축에 틱그리기, 데이터 영역 외각선 그리기
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //그리는 부분 마지막에 추가(Redraw false 할때도 그림)
	virtual void RenderClossLine(CDC* pDC); //마우스 크로스 라인 그리기 [김태현 2018/9/7]
	virtual void RenderMouseHoverInfo(CDC* pDC){}; //마우스 올려져 있을 경우 데이터표시 그리기 [김태현 2018/9/7]

	virtual void RenderPerformance(Graphics* grfx, CDC* pDC); 
	
	virtual void GetRenderFrame(CDC* pDC, CRect rect);

	virtual void ReDraw(bool bReDrawALL = true);
	
	virtual void SetAutoScale(){}; //이런건 기본적으로 있어야됨!!

	virtual void SetTitle(char* titlename);
	virtual char* GetTitle();
	virtual void SetAxisXTitle(char* titlename){m_AxisX.m_strTitle = titlename;};
	virtual void SetAxisYTitle(char* titlename){m_AxisY.m_strTitle = titlename;};

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown //키보드 입력 이벤트 처리
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point); //마우스 입력 이벤트처리
	virtual void MouseInput(CakMouseState mousestate){}; //마우스 상태처리
	virtual void OnSystemKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown //키보드 입력 이벤트 처리

	virtual void EnableThreadDrawMode(int nSleepTime = 30, int nSleepTimeMin = 1);//한번 키면 끝
	void SetDragStyle(akDragStyle style){m_DragStyle = style;} ; //0은 끄기, 1은 양쪽모두, 2은 세로선만, 3는 가로선만
	akDragStyle GetDragStyle(){return m_DragStyle;};
	CPoint GetMousePos(){return m_MouseState.Point;};

	void SetWindowSize(int x, int y, int width, int height, UINT flag = NULL, bool reDraw = true);
	void SetWindowSize(int width, int height, UINT flag = NULL, bool reDraw = true);
	void GetWindowSize(int* width, int* height);

	inline CakRect GetDataArea(){return m_rectData;};
	inline CakRect GetTitleArea(){return m_rectTitle;};
	inline CakRect GetAxisXArea(){return m_rectAxisX;};
	inline CakRect GetAxisYArea(){return m_rectAxisY;};

	void SetTitleSize(int height);//타이틀바 세로축 크기
	void SetAxisXSize(int height);//x축 세로 크기
	void SetAxisYSize(int width);//y축 가로 크기

	void SetSaveImage(char* strFilepathname); 

	void SetClossLine(bool flag = true){m_bClossLine = flag;};

	//마우스 제어 모드(0:우클릭 이동모드, 1:좌클릭 이동모드) [김태현 2018/8/22]
	void SetMouseControlMode(int nType){m_nMouseControlMode = nType;};
	inline void SetID(int nID);
	inline int GetID();

	virtual void FuncThreadDraw();
protected:
	virtual void CaculatorGraphSize();
	virtual void getLbuttonDragPoint(int* px1, int* py1, int* px2, int* py2);

	
private:
	

public:
	int m_nID;//메세지 보낼때 같이 보내주면서.. 인스턴스를 구분한다.

	CAxis m_AxisX;
	CAxis m_AxisY;
	CAxis m_AxisY2;//오른쪽 y공간

	CTitle m_Title;

	CMouseMoveInfo m_MouseMoveInfo;//마우스 움직임에 따라 화면에 표시되는 내용

	CakRectd m_AutoScaleMarginRate;//자동스케일 양옆 아래,위 여백 비율 설정
	CakRect m_rectLabelAdjust;

	bool m_bCheckDC; //올바른 DC인가 확인 절차를 생략할 것인지 여부

	bool m_bClossLine;

	int	 m_nMouseControlMode;

	CakGraphThreadDraw* m_pThreadDraw;
protected:
	CWnd* m_pParent; //부모 윈도우 클래스

	

	//메모리 디씨~
	CDC m_GraphDC; 
	CBitmap m_GraphBitmap, *m_pGraphOldBitmap;
	CRect m_rectGraphBitmap;

	//그래프 다시 그려야 할때
	bool m_bReRender; 
	
	//그리는 부분 영역 크기
	int m_nWindowSizeWidth, m_nWindowSizeHeight;

	//그리는 영역 각각의 크기(매번 CaculatorGraphSize() 함수에 의해 계산이 된다)
	CakRect m_rectData; //순수하게 그래프 그리는 부분 영역
	CakRect m_rectChartMargin; //데이터 그리는 부분 오른쪽 여백
	CakRect m_rectAxisX;
	CakRect m_rectAxisY;
	CakRect m_rectTitle;

	
	CFont m_fontData; //데이터 폰트 

	bool m_bRenderState;
	bool m_bHover;

public:
	COLORREF m_colorBack; //배경색
	COLORREF m_colorDataArea; //데이터 그리는부분 배경색
protected:
	akDragStyle m_DragStyle;
private:
	CFont m_fontWaterMake;
	ULONG_PTR m_gdiplusToken; //GDI+ 토큰
	CString m_strSaveFilePath;

	//성능 체크 관련
private:
	int m_nShowPerformance;
	int m_nTimeRenderTotal;
	int m_nTimeRenderDraw1stBuffer;
	int m_nTimeRenderDraw2ndBuffer;

	int m_nTimeTempTotal;
	int m_nTimeTempDraw1stBuffer;
	int m_nTimeTempDraw2ndBuffer;
	int m_nTimeRenderCount;

	int m_nTimerRenderTotalMax;
	int m_nTimerRenderTotalMin;
	
	
	
public:
	static bool m_bAK;
	static void setAK(){CakGraphBase::m_bAK = false;};
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//int m_DataState;
	//afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


class AFX_EXT_CLASS CakGraphThreadDraw
{
public:
	CakGraphThreadDraw()
	{
		m_bThreadDrawMode = FALSE;
		m_bDrawByThread = FALSE;
		m_nThreadDrawFlag = 0;
		m_nThreadDrawSleepTimeMin = 1;
		m_nThreadDrawSleepTime = 30;

		InitializeCriticalSection(&m_csThreadDraw);
	};
	virtual ~CakGraphThreadDraw()
	{
		if(GetThreadDrawMode())
		{
			m_nThreadDrawFlag = 2;
			int nReTryCount = 10;
			while (m_nThreadDrawFlag != 0 && nReTryCount--)
			{
				Sleep(1);
			}
		}

	};

public:
	virtual BOOL CreateThreadDraw(CakGraphBase* pBase)
	{
		if(pBase == NULL) return FALSE;

		//if(GetThreadDrawMode())
		{
			m_nThreadDrawFlag = 1;
			//_beginthread(threadThreadDraw, NULL, this);
			AfxBeginThread(threadThreadDraw, this, NULL);
			m_pGraphBase = pBase;
			m_bDrawByThread = TRUE;
		}
		return TRUE;
	}
	void SetDrawByThread(BOOL bFlag){m_bDrawByThread = bFlag;};
	void SetThreadDrawMode(BOOL bFlag){m_bThreadDrawMode = bFlag;};
	BOOL GetThreadDrawMode(){return m_bThreadDrawMode;};
	CRITICAL_SECTION* GetCriticalSection(){return &m_csThreadDraw;};
	void SleepMsg(IN DWORD dwMilliseconds)  
	{  
		DWORD dwStart   = 0;  
		MSG   msg       = {0,};  

		dwStart  = GetTickCount();  
		while (GetTickCount() - dwStart < dwMilliseconds)  
		{  
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  
			{  
				TranslateMessage(&msg);  
				DispatchMessage(&msg);  
			}  

			// Lower CPU Usage  
			::Sleep(1);  
		}  
	}  

public:
	int m_nThreadDrawSleepTime;
	int m_nThreadDrawSleepTimeMin;
protected:
	BOOL m_bThreadDrawMode;

	//CDC m_ThreadDrawDC; 
	//CBitmap m_ThreadDrawBitmap;
	//CRect m_rectThreadDrawBitmap;
	int m_nThreadDrawFlag;
	BOOL m_bDrawByThread;

	CRITICAL_SECTION m_csThreadDraw;
	CakGraphBase* m_pGraphBase;

	
protected:
	//static void threadThreadDraw(void* arg)
	static UINT threadThreadDraw(LPVOID arg)
	{
		CakGraphThreadDraw* pThis = (CakGraphThreadDraw*)arg;
		while (pThis->m_nThreadDrawFlag == 1)
		{
			DWORD	dwTimeStart = GetTickCount();
			if(pThis->m_bDrawByThread)
			{
				pThis->m_bDrawByThread = FALSE;
				pThis->funcThreadDraw();
			}
			else
			{
				
			}

			int nSleepTimeDynamic = pThis->m_nThreadDrawSleepTime-(GetTickCount()-dwTimeStart);
			nSleepTimeDynamic = max(nSleepTimeDynamic, pThis->m_nThreadDrawSleepTimeMin);

			nSleepTimeDynamic = pThis->m_nThreadDrawSleepTime;
			Sleep(nSleepTimeDynamic);
			//pThis->SleepMsg(nSleepTimeDynamic);
		}
		pThis->m_nThreadDrawFlag = 0;

		return TRUE;
	};

	virtual void funcThreadDraw()
	{
		m_pGraphBase->FuncThreadDraw();
	};
};
