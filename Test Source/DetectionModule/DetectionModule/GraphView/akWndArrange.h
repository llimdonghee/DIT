#pragma once

//���̾�α� ũ�⺯ȭ�� ���� ũ�� ��ġ �ڵ� ����   ����[2016/12/23]
#include <vector>

enum _POSSIZETYPE
{
	WA_LEFTTOP = 0x1,
	WA_LEFTBOTTOM = 0x2,
	WA_RIGHTTOP = 0x4,
	WA_RIGHTBOTTOM = 0x8,
	WA_RESIZE_WIDTH = 0x10,
	WA_RESIZE_HEIGHT = 0x20,
};

class CakWndArrange
{
public:
	CakWndArrange(void);
	virtual ~CakWndArrange(void);

protected:
	struct _WndData 
	{
		HWND hWnd;
		CRect rectOrginal;
		int nStyle;
	};

public:
	void clear();
	void setParentWnd(CWnd* pWnd);
	void addChildWnd(CWnd* pWnd, int nStyle);
	void setChildStyle(CWnd* pWnd, int nStyle);
	
	void process(int nWidth, int nHeight);//void OnSize(UINT nType, int cx, int cy) �Լ����� ȣ��
protected:
	CWnd* m_pWndParent;
	CRect m_rectOriginal;
	std::vector<_WndData> m_vecWndChild;
};
