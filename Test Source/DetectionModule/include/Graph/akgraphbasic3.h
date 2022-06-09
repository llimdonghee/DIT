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
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //������ ��/�� �׸���
	virtual void RenderLegend(Graphics* grfx, CDC* pDC);
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //�� ���� �׸���
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //���� ���� �׸���
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //�ش� �࿡ ƽ�׸���, ������ ���� �ܰ��� �׸���
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//���콺 �����϶� ��ġ�� ǥ���ϴºκ�
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //�׸��� �κ� �������� �߰�(Redraw false �Ҷ��� �׸�)
	
	

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	//������ Ŭ���� ����ȭ ��ü �߰�
	virtual void SetSeriesNum(int subsetNum); //���� ������ �ʱ�ȭ
	virtual void SetSeriesAdd(int index = -1); 
	virtual bool SetSeriesDelete(int index);
	virtual void SetDataNum(int nDataNum);
	//virtual bool SetData(int index, double x, double y); <--��༮�� �˾Ƽ� ����ȭ ��Ű����.. ����!!
	virtual void SetHidden(bool enable, int startIndex, int endIndex);

	virtual void SetColorType(int nIndex);

	inline int GetWindowPosX(double xvalue); //��ġ ���� ���� ������ ��ǥ
	inline int GetWindowPosY(double yvalue); //��ġ ���� ���� ������ ��ǥ
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //������ ��ǥ�� ���� ������ 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //������ ��ǥ�� ���� ������ 

	void SetReverseModeX(bool flag); //x�� ������ ǥ��
	bool GetReverseModeX(){return m_bReverseX;}; //x�� ������ ǥ��
	void SetReverseModeY(bool flag); //y�� ������ ǥ��
	bool GetReverseModeY(){return m_bReverseY;}; //y�� ������ ǥ��

	
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


//X�� ���� h/m/s/ms ǥ���� �����ϰ�
class AFX_EXT_CLASS CakGraphType3Time : public CakGraphBasic3
{
public:

public:
	CakGraphType3Time(void){};
	virtual ~CakGraphType3Time(void){};

public:
	virtual void RenderGrid(Graphics* grfx, CDC* pDC);
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //�ش� �࿡ ƽ�׸���, ������ ���� �ܰ��� �׸���
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);
	
	void getTime(double timeval, int *hour, int *min, int *sec, float *msec);
};


