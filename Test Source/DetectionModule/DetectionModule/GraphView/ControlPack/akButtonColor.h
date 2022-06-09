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
	int nStyle;//0일반, 1그라데이션
	int nEdgeStyle;//0:3d, 1:Flat

	COLORREF clrEdgeLeftTop;
	COLORREF clrEdgeRightBottom;

	COLORREF clrButtonShow;
	COLORREF clrButtonPush;
	COLORREF clrButtonHorver;
	COLORREF clrButtonDisable;


	//글자출력관련 태현[2016/9/19]
	bool bTextEnable;
	int nFontBoxOffsetX, nFontBoxOffsetY; //정가운데 기준으로 글자표시 OFFSET
	int nTextPushX, nTextPushY;//눌려진 모양이됐을경우, 글자의 위치 옵셋 설정

	COLORREF clrFontNormal;
	COLORREF clrFontPush;
	COLORREF clrFontMouseOver;
	COLORREF clrFontDisable;

	//폰트 그림자 출력 태현[2016/9/19]
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

	//OnDrawItem에서 호출하면됨
	bool processDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	//아래꺼는 등록안해도 괜찮지만 퀄이 낮아짐 태현[2016/9/19]
	void processMouseHover(int nIDCtl);
	void processDoubleClick(int nIDCtl);

	int GetCheck() const {return m_bCheckBoxFlag;};
	void SetCheck(int nCheck) ;
	void setFont(char* fontName);

protected:
	COLORREF getAlphaColor(COLORREF colorA, COLORREF colorB, double dColorBAlpha);
	void getButtonColor3Point(COLORREF color, COLORREF* part1Top, COLORREF* part1Bottom, COLORREF* part2Bottom);

	void draw2PartGraditionRect(CDC* pDC, CRect rectDraw, COLORREF clrPart1T, COLORREF clrPart1B, COLORREF clrPart2T, COLORREF clrPart2B);
	bool drawButton3dNormal(CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption);//0번 노멀 3d 태현[2016/9/19]
	bool drawButton3dGradation( CButton* pButton, LPDRAWITEMSTRUCT lpDrawItemStruct, _ColorButtonOption* pOption );
	hsv rgb2hsv(rgb in);
	rgb hsv2rgb(hsv in);
protected:
	std::map<int,CButton*> m_mapControls;
	CButton* m_pButtonHover;
	_ColorButtonOption m_ButtonColorOption;

	//subclassing방식에서 사용되는것들 태현[2016/9/22]
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
//아래는 사용예시

void CButtonTestDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_akButtonColorNormal.processDrawItem(nIDCtl, lpDrawItemStruct);
	m_akButtonColorGradition.processDrawItem(nIDCtl, lpDrawItemStruct);
	//m_bcFlatGradition.processDrawItem(nIDCtl, lpDrawItemStruct);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CButtonTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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