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
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���
	virtual void RenderSeries(Graphics* grfx, CDC* pDC){}; //������ ��/�� �׸���
	virtual void RenderLegend(Graphics* grfx, CDC* pDC){}; //����
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //�� ���� �׸���
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //���� ���� �׸���
	virtual void RenderLabel(Graphics* grfx, CDC* pDC){}; //�ش� �࿡ ƽ�׸���, ������ ���� �ܰ��� �׸���
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //�׸��� �κ� �������� �߰�(Redraw false �Ҷ��� �׸�)
	virtual void RenderClossLine(CDC* pDC); //���콺 ũ�ν� ���� �׸��� [������ 2018/9/7]
	virtual void RenderMouseHoverInfo(CDC* pDC){}; //���콺 �÷��� ���� ��� ������ǥ�� �׸��� [������ 2018/9/7]

	virtual void RenderPerformance(Graphics* grfx, CDC* pDC); 
	
	virtual void GetRenderFrame(CDC* pDC, CRect rect);

	virtual void ReDraw(bool bReDrawALL = true);
	
	virtual void SetAutoScale(){}; //�̷��� �⺻������ �־�ߵ�!!

	virtual void SetTitle(char* titlename);
	virtual char* GetTitle();
	virtual void SetAxisXTitle(char* titlename){m_AxisX.m_strTitle = titlename;};
	virtual void SetAxisYTitle(char* titlename){m_AxisY.m_strTitle = titlename;};

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown //Ű���� �Է� �̺�Ʈ ó��
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point); //���콺 �Է� �̺�Ʈó��
	virtual void MouseInput(CakMouseState mousestate){}; //���콺 ����ó��
	virtual void OnSystemKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown //Ű���� �Է� �̺�Ʈ ó��

	virtual void EnableThreadDrawMode(int nSleepTime = 30, int nSleepTimeMin = 1);//�ѹ� Ű�� ��
	void SetDragStyle(akDragStyle style){m_DragStyle = style;} ; //0�� ����, 1�� ���ʸ��, 2�� ���μ���, 3�� ���μ���
	akDragStyle GetDragStyle(){return m_DragStyle;};
	CPoint GetMousePos(){return m_MouseState.Point;};

	void SetWindowSize(int x, int y, int width, int height, UINT flag = NULL, bool reDraw = true);
	void SetWindowSize(int width, int height, UINT flag = NULL, bool reDraw = true);
	void GetWindowSize(int* width, int* height);

	inline CakRect GetDataArea(){return m_rectData;};
	inline CakRect GetTitleArea(){return m_rectTitle;};
	inline CakRect GetAxisXArea(){return m_rectAxisX;};
	inline CakRect GetAxisYArea(){return m_rectAxisY;};

	void SetTitleSize(int height);//Ÿ��Ʋ�� ������ ũ��
	void SetAxisXSize(int height);//x�� ���� ũ��
	void SetAxisYSize(int width);//y�� ���� ũ��

	void SetSaveImage(char* strFilepathname); 

	void SetClossLine(bool flag = true){m_bClossLine = flag;};

	//���콺 ���� ���(0:��Ŭ�� �̵����, 1:��Ŭ�� �̵����) [������ 2018/8/22]
	void SetMouseControlMode(int nType){m_nMouseControlMode = nType;};
	inline void SetID(int nID);
	inline int GetID();

	virtual void FuncThreadDraw();
protected:
	virtual void CaculatorGraphSize();
	virtual void getLbuttonDragPoint(int* px1, int* py1, int* px2, int* py2);

	
private:
	

public:
	int m_nID;//�޼��� ������ ���� �����ָ鼭.. �ν��Ͻ��� �����Ѵ�.

	CAxis m_AxisX;
	CAxis m_AxisY;
	CAxis m_AxisY2;//������ y����

	CTitle m_Title;

	CMouseMoveInfo m_MouseMoveInfo;//���콺 �����ӿ� ���� ȭ�鿡 ǥ�õǴ� ����

	CakRectd m_AutoScaleMarginRate;//�ڵ������� �翷 �Ʒ�,�� ���� ���� ����
	CakRect m_rectLabelAdjust;

	bool m_bCheckDC; //�ùٸ� DC�ΰ� Ȯ�� ������ ������ ������ ����

	bool m_bClossLine;

	int	 m_nMouseControlMode;

	CakGraphThreadDraw* m_pThreadDraw;
protected:
	CWnd* m_pParent; //�θ� ������ Ŭ����

	

	//�޸� ��~
	CDC m_GraphDC; 
	CBitmap m_GraphBitmap, *m_pGraphOldBitmap;
	CRect m_rectGraphBitmap;

	//�׷��� �ٽ� �׷��� �Ҷ�
	bool m_bReRender; 
	
	//�׸��� �κ� ���� ũ��
	int m_nWindowSizeWidth, m_nWindowSizeHeight;

	//�׸��� ���� ������ ũ��(�Ź� CaculatorGraphSize() �Լ��� ���� ����� �ȴ�)
	CakRect m_rectData; //�����ϰ� �׷��� �׸��� �κ� ����
	CakRect m_rectChartMargin; //������ �׸��� �κ� ������ ����
	CakRect m_rectAxisX;
	CakRect m_rectAxisY;
	CakRect m_rectTitle;

	
	CFont m_fontData; //������ ��Ʈ 

	bool m_bRenderState;
	bool m_bHover;

public:
	COLORREF m_colorBack; //����
	COLORREF m_colorDataArea; //������ �׸��ºκ� ����
protected:
	akDragStyle m_DragStyle;
private:
	CFont m_fontWaterMake;
	ULONG_PTR m_gdiplusToken; //GDI+ ��ū
	CString m_strSaveFilePath;

	//���� üũ ����
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
