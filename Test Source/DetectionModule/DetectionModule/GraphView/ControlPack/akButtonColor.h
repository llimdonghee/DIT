#pragma once
#include "afxcmn.h"

#include <map>

struct _ColorButtonOption
{
	_ColorButtonOption()
	{
		nStyle=1;
		nEdgeStyle=0;

		clrEdgeLeftTop;
		clrEdgeRightBottom;

		//clrButtonShow = RGB(240,240,240);
		clrButtonShow = RGB(37, 86, 210);
		clrButtonPush = RGB(37, 86, 190);
		clrButtonHorver = RGB(37, 86, 210);
		clrButtonDisable = RGB(150,150,150);

		clrFontNormal = RGB(0,0,0);
		clrFontPush = RGB(0,0,0);
		clrFontMouseOver = RGB(128, 128, 128);
		clrFontDisable = RGB(150,150,150);


		bTextEnable = TRUE;
		nFontBoxOffsetX = nFontBoxOffsetY = 0;
		nTextPushX = nTextPushY = 1;

		bTextShadow = FALSE;
		nTextShadowX = nTextShadowY=1;
		strFontName = "";
	}
	int nStyle;//0�Ϲ�, 1�׶��̼�
	int nEdgeStyle;//0:3d, 1:Flat

	COLORREF clrEdgeLeftTop;
	COLORREF clrEdgeRightBottom;

	COLORREF clrButtonShow;
	COLORREF clrButtonPush;
	COLORREF clrButtonHorver;
	COLORREF clrButtonDisable;


	//������°��� ����[2016/9/19]
	bool bTextEnable;
	int nFontBoxOffsetX, nFontBoxOffsetY; //����� �������� ����ǥ�� OFFSET
	int nTextPushX, nTextPushY;//������ ����̵������, ������ ��ġ �ɼ� ����

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//��Ʈ �׸��� ��� ����[2016/9/19]
	bool bTextShadow;
	int nTextShadowX, nTextShadowY;

	CString strFontName;
};

typedef struct {
	double r;       // percent
	double g;       // percent
	double b;       // percent
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // percent
	double v;       // percent

} hsv;

class CakButtonColor : public CButton
{
	DECLARE_DYNAMIC(CakButtonColor);

public:
	CakButtonColor(void);
	~CakButtonColor(void);

	void clear();
	void addControl(CButton* pButton);

	void setButtonColorOption(_ColorButtonOption ButtonColorOption){m_ButtonColorOption=ButtonColorOption;};
	_ColorButtonOption getButtonColorOption(){return m_ButtonColorOption;};

	//OnDrawItem���� ȣ���ϸ��
	bool processDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	//�Ʒ����� ��Ͼ��ص� �������� ���� ������ ����[2016/9/19]
	void processMouseHover(int nIDCtl);
	void processDoubleClick(int nIDCtl);

	int GetCheck() const {return m_bCheckBoxFlag;};
	void SetCheck(int nCheck) ;
	void setFont(char* fontName);

protected:
	COLORREF getAlphaColor(COLORREF colorA, COLORREF colorB, double dColorBAlpha);
	void getButtonColor3Point(COLORREF color, COLORREF* part1Top, COLORREF* part1Bottom, COLORREF* part2Bottom);

	void draw2PartGraditionRect(CDC* pDC, CRect rectDraw, COLORREF clrPart1T, COLORREF clrPart1B, COLORREF clrPart2T, COLORREF clrPart2B);
	bool drawButton3dNormal(CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption);//0�� ��� 3d ����[2016/9/19]
	bool drawButton3dGradation( CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption );
	hsv rgb2hsv(rgb in);
	rgb hsv2rgb(hsv in);
protected:
	std::map<int,CButton*> m_mapControls;
	CButton* m_pButtonHover;
	_ColorButtonOption m_ButtonColorOption;

	//subclassing��Ŀ��� ���Ǵ°͵� ����[2016/9/22]
protected:
	BOOL m_bHover;
	BOOL m_bTracking;
	BOOL m_bCheckBoxType;
	BOOL m_bCheckBoxFlag;
	CFont m_font;

public:
	void SetUpdateStateNotify(HWND hParent){m_hNotifyParent = hParent;};
protected:
	HWND m_hNotifyParent;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	COLORREF getBrightColor( COLORREF color, double dBright );
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


/*
//�Ʒ��� ��뿹��

void CButtonTestDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_akButtonColorNormal.processDrawItem(nIDCtl, lpDrawItemStruct);
	m_akButtonColorGradition.processDrawItem(nIDCtl, lpDrawItemStruct);
	//m_bcFlatGradition.processDrawItem(nIDCtl, lpDrawItemStruct);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CButtonTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message == WM_MOUSEMOVE)
	{
		m_akButtonColorNormal.processMouseHover(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());
		m_akButtonColorGradition.processMouseHover(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());
		//m_bcFlatGradition.processMouseHover(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());
	}
	else if(pMsg->message == WM_LBUTTONDBLCLK)
	{
		m_akButtonColorNormal.processDoubleClick(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());
		m_akButtonColorGradition.processDoubleClick(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());
		//m_bcFlatGradition.processDoubleClick(CWnd::FromHandle(pMsg->hwnd)->GetDlgCtrlID());

	}
	return CDialog::PreTranslateMessage(pMsg);
}

*/