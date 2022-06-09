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
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���
	
	
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //�� ���� �׸���
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //���� ���� �׸���
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //�ش� �࿡ ƽ�׸���, ������ ���� �ܰ��� �׸���
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//���콺 �����϶� ��ġ�� ǥ���ϴºκ�
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //�׸��� �κ� �������� �߰�(Redraw false �Ҷ��� �׸�)
	
	virtual void RenderImage(Graphics* grfx, CDC* pDC);
	

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);


	virtual void SetColorType(int nIndex);

	inline int GetWindowPosX(double xvalue); //��ġ ���� ���� ������ ��ǥ
	inline int GetWindowPosY(double yvalue); //��ġ ���� ���� ������ ��ǥ
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //������ ��ǥ�� ���� ������ 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //������ ��ǥ�� ���� ������ 

	double GetValuePosRangeX(int nValue){return abs(GetValuePosDataX(nValue)-GetValuePosDataX(0));};
	double GetValuePosRangeY(int nValue){return abs(GetValuePosDataY(nValue)-GetValuePosDataY(0));};
	int GetWindowPosRangeX(double dValue){return abs(GetWindowPosX(dValue)-GetWindowPosX(0));};
	int GetWindowPosRangeY(double dValue){return abs(GetWindowPosY(dValue)-GetWindowPosY(0));};

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


