#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataVector2.h"

#include "akGraph/akRangeSettingNormalDlg.h"
#include "akGraph/akRangeSettingTimeDlg.h"

//���� �����͸� ���� �ø��� �Ѱ����� ���� ������

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
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //������ ��/�� �׸���
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //�� ���� �׸���
	virtual void RenderTitle(Graphics* grfx, CDC* pDC); //���� ���� �׸���
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //�ش� �࿡ ƽ�׸���, ������ ���� �ܰ��� �׸���
	virtual void RenderEnd(Graphics* grfx, CDC* pDC); //�׸��� �κ� �������� �߰�(Redraw false �Ҷ��� �׸�)

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	//������ Ŭ���� ����ȭ ��ü �߰�
	virtual void Clear();
	virtual bool SetData(int index, double x, double y); //����ִ� ������ �� ����
	virtual void AddData(double x, double y, int index = -1); //������ �߰�
	virtual void SetHidden(bool enable, int startIndex, int endIndex);


	inline int GetWindowPosX(double xvalue); //��ġ ���� ���� ������ ��ǥ
	inline int GetWindowPosY(double yvalue); //��ġ ���� ���� ������ ��ǥ
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //������ ��ǥ�� ���� ������ 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //������ ��ǥ�� ���� ������ 

	void SetReverseModeX(bool flag); //x�� ������ ǥ��
	bool GetReverseModeX(){return m_bReverseX;}; //x�� ������ ǥ��
	void SetReverseModeY(bool flag); //y�� ������ ǥ��
	bool GetReverseModeY(){return m_bReverseY;}; //y�� ������ ǥ��

	void SetAxisXMode(bool flag){m_nAxisXMode = flag;};

	virtual void SetAutoScale();

	double GetYvalueByXposition(double xpos){return 0;};//�̱���
	
	LRESULT OnRangeSettingNormalUpdate(WPARAM wParam, LPARAM lParam);
	LRESULT OnRangeSettingTimeUpdate(WPARAM wParam, LPARAM lParam);


private:
	bool m_bGrid;
	bool m_bReverseX;
	bool m_bReverseY;
	int m_nAxisXMode; //0:�Ϲ� ���, 1:�ʴ��� �ð�ǥ��, 2:ms���� �ð�ǥ��
public:
	CakRangeSettingNormalDlg* m_pRangeYSettingDlg;
	CakRangeSettingNormalDlg* m_pRangeXSettingDlg;
	afx_msg void OnDestroy();
};
