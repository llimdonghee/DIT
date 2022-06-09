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
	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���

	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //
	virtual void RenderTitle(Graphics* grfx, CDC* pDC);
	virtual void RenderEnd(Graphics* grfx, CDC* pDC);

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void MouseInput(CakMouseState mousestate);

	inline int GetWindowPosX(double xvalue); //��ġ ���� ���� ������ ��ǥ
	inline int GetWindowPosY(double yvalue); //��ġ ���� ���� ������ ��ǥ
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //������ ��ǥ�� ���� ������ 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //������ ��ǥ�� ���� ������ 

	virtual void SetAutoScale();
	virtual void SetDataNum(int datanum);

	virtual void ReDraw(bool bReDrawALL);

	void SetHorMode(bool bFlag = true); //���� ���

	void SetCalculatorAvr();//������ ����� ȭ�鿡 ǥ��
	CString m_strAVR;

	void SetAutoBarScale(int margin);
	int m_nBarGab; //���� �׷��� ������ ����
	
	void SetFullScreen(bool flag);
	void SetFullMode(bool flag, int x, int y, int width, int height); //Ǯ ��� Ű�� ��.. �׸��� Ǯ��� ������

	void GraphType(int nType);

	bool GetValuebyMousePos(int mousex, int mousey, int* index, double* val);

private:
	
	
protected:
	bool m_bGrid;
	CakColorSettingDlg* m_pColorSettingDlg;
public:
	bool m_bIntegerAxisX;
	bool m_bIntegerAxisY;

	bool m_bAVR;//ȭ�鿡 ��հ� ǥ�� �� �������� ���� �÷���
	std::vector<double> m_vecProbabilityData; //�̻�����Ȯ����굥����
	int m_nBarWidth;


	CakRect m_rectFullMode;
	CakRect m_rectNormalMode;
	
	int m_nGraphType; //0�� ��Ÿ�� �� Ÿ�� ��� ǥ��, 1�� ��Ÿ�Ը� ǥ��
	int m_nTextValueShowLevel; //�׷��� ���� �ؽ�Ʈ�� �� ǥ��(-1�� ǥ�� ���� 0���� 3���� �Ҽ� �ڸ��� ǥ��)

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
