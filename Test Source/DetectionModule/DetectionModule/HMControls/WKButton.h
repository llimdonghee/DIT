#if !defined(AFX_WKBUTTON_H__99FB32BE_6D54_4845_BD51_1BC64E774CC6__INCLUDED_)
#define AFX_WKBUTTON_H__99FB32BE_6D54_4845_BD51_1BC64E774CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WKButton.h : header file
/******************************< 프로그램 정보 >****************************/
/*   클 래 스 명 : WKButton                                                */ 
/*   버       전 :                                                         */
/*   최종 수정일 : 2006. 05. 28                                            */
/***************************************************************************/

/////////////////////////////////< 정  의 >//////////////////////////////////

// 버튼 형태
#define		WK_BUTTON_SHPAE_RECTANGLE				0x0001
#define		WK_BUTTON_SHAPE_CIRCLE					0x0002
#define		WK_BUTTON_SHAPE_ROUNDRECT				0x0004

// 버튼 색상
#define		WK_BUTTON_COLOR_ONE						0x0001
#define		WK_BUTTON_COLOR_TWO						0x0002
#define		WK_BUTTON_COLOR_GRADATION_UP			0x0011
#define		WK_BUTTON_COLOR_GRADATION_RIGHT			0x0012
#define		WK_BUTTON_COLOR_GRADATION_DOWN			0x0014
#define		WK_BUTTON_COLOR_GRADATION_LEFT			0x0018

// 버튼 상태
#define		WK_BUTTON_STATUS_MOUSEOVER				0x0001
#define		WK_BUTTON_STATUS_MOUSELEAVE				0x0002
#define		WK_BUTTON_STATUS_MOUSECLICK				0x0004


///////////////////////////////< 클래스 구조 >///////////////////////////////
class AFX_EXT_CLASS CWKButton : public CButton
{
public://<<<<<<<<<<<<<<<<<<<<<< 외부 접근 함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	CWKButton();
	virtual ~CWKButton();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWKButton)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:	
/*****************************< 버튼 설정 함수 >****************************/
	/* 텍스트 모양 및 내용 지정 */
	BOOL SetButtonText(CString strCaption);
	BOOL SetButtonText(CString strCaption, COLORREF rgbText);
	BOOL SetButtonText(CString strCaption, CFont *font, COLORREF rgbText);
	
	/* 버튼 형태 및 색상 지정 */
	BOOL SetButtonStyle(DWORD dShape, COLORREF rgbColor1);
	BOOL SetButtonStyle(DWORD dShape, DWORD dStyle,	COLORREF rgbColor1,	COLORREF rgbColor2);
	/***************************************************************************/

protected://<<<<<<<<<<<<<<<< 내부 처리 함수 및 변수 >>>>>>>>>>>>>>>>>>>>>>>>>
	/* 상태별 버튼 이미지 */
	CBitmap m_bmpImage1;	//평상시
	CBitmap m_bmpImage2;	//마우스 오버
	CBitmap m_bmpImage3;	//클릭
	
	/* 상태별 버튼 이미지 생성 */
	BOOL CreateButtonImage();

	/* 버튼 형태 생성 */
	BOOL DrawFormRectangle();	// 사각
	BOOL DrawFormCircle();		// 원
	BOOL DrawFormRoundRect();	// 둥근사각

	/* 이미지 처리 */
	COLORREF LightValueControl(COLORREF rgb, int nValue);						// 색상 밝기 조절
	void DrawGradation(CBitmap* bmpImage, COLORREF color1, COLORREF color2);	// 그라데이션 이미지 생성
	BOOL DrawBorder(CBitmap* pbmpImage, COLORREF color1, COLORREF color2, int nWidth = 2);		// 버튼 테두리 그리기
	BOOL DrawText(CBitmap* pBitmap, int nStatus);								// 버튼에 글씨 넣기

	/* 버튼 이미지 정보 */
	DWORD m_dShape;			// 버튼 형태
	DWORD m_dStyle;			// 컬러 스타일
	COLORREF m_rgbColor1;	// 첫번째 컬러
	COLORREF m_rgbColor2;	// 두번째 컬러
	CRect m_rtButton;		// 버튼크기
	int m_nWidth;			// 버튼 너비
	int m_nHeight;			// 버튼 높이

	/* 텍스트 정보 */
	CString m_strCaption;	// 캡션 내용
	CFont m_Font;			// 캡션 글꼴
	COLORREF m_rgbText;		// 캡션 색상
	BOOL m_bValueChange;	// 캡션 변경
	
	/* 버튼 상태 정보 */
	DWORD m_dButtonStatus;
	
	//{{AFX_MSG(CWKButton)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WKBUTTON_H__99FB32BE_6D54_4845_BD51_1BC64E774CC6__INCLUDED_)
