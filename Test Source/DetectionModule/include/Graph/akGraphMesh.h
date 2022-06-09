#pragma once

#include "akGraph/akGraphBase.h"
#include "akGraph/akDataMesh.h"
#include "akGraph/akColorTable.h"
#include "akGraph/akColorTableOption.h"
#include "akGraph/akRangeSettingNormalDlg.h"

class AFX_EXT_CLASS CakGraphMesh : public CakGraphBase, public CakDataMesh
{
public:
	CakGraphMesh(void);
	virtual ~CakGraphMesh(void);

public:
	virtual BOOL CreateGraph(CWnd* pParent, CRect rect);

	virtual void RenderDataArea(Graphics* grfx, CDC* pDC); //������ ���� �׸���
	virtual void RenderAxis(Graphics* grfx, CDC* pDC); //
	virtual void RenderLabel(Graphics* grfx, CDC* pDC); //
	virtual void RenderTitle(Graphics* grfx, CDC* pDC);
	virtual void RenderEnd(Graphics* grfx, CDC* pDC);
	virtual void RenderMouseHoverInfo(CDC* pDC);

	virtual void OnKeyInput(int key, int mode); //mode 0==keyup, mode 1==keydown
	virtual void OnMouseInput(akMouseEvent mouseevent, CPoint point);
	virtual void CaculatorGraphSize();

	virtual void SetDataNum(int sizex, int sizey);

	void SetReverseModeX(bool flag); //x�� ������ ǥ��
	bool GetReverseModeX(){return m_bReverseX;}; //x�� ������ ǥ��
	void SetReverseModeY(bool flag); //x�� ������ ǥ��
	bool GetReverseModeY(){return m_bReverseY;}; //x�� ������ ǥ��

	inline int GetWindowPosX(double xvalue); //��ġ ���� ���� ������ ��ǥ
	inline int GetWindowPosY(double yvalue); //��ġ ���� ���� ������ ��ǥ
	inline double GetValuePosDataX(int xpos, bool dataArea = false); //������ ��ǥ�� ���� ������ 
	inline double GetValuePosDataY(int ypos, bool dataArea = false); //������ ��ǥ�� ���� ������ 

	virtual void SetAutoScale();
	
	double GetDataValue(double posx, double posy);//
	void CaculatorDataRange(double *min, double *max);

	void SetColorTableFile(char* pFileName);
	virtual BOOL ShowColorTableOptionDlg(CPoint point);
protected:
	
protected:
	bool m_bReverseX;
	bool m_bReverseY;

public:
	double m_NoDataValue; //�����Ͱ� ���� ���� �� ó��
	

	CakColorTableOptionDlg* m_pColorTableOption;
	CakRangeSettingNormalDlg* m_pRangeXSettingDlg;
	CakRangeSettingNormalDlg* m_pRangeYSettingDlg;

	bool m_bRangeYSetting;
	bool m_bRangeXSetting;

	bool m_bDataMinAnti;
	bool m_bDataMaxAnti;

	CString m_strComment;//������ ���� ���� ��ܿ� �ڸ�Ʈ ǥ��
protected:
	CakColorTable m_ColorTable;
	
	//�÷����̺� ����
public:
	bool m_bColorBar;//����� ǥ�� ����
	bool m_bColorTableTickBar;
	CakRect m_rectColorTablebar;
	COLORREF m_ColorBarFontColor; //�÷��� ��Ʈ �÷�
	int m_colorMin;
	int m_colorMax;
	int m_colorLevel;
	int m_colorIndex;
	bool m_colorInverse;
	bool m_colorInterpolation;
	bool m_colorAuto;
	
	
public:
	DECLARE_MESSAGE_MAP()
	
	afx_msg LRESULT OnColorTableOptionUpdate(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRangeSettingUpdate(WPARAM wParam, LPARAM lParam);
	

	
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	
	
};
