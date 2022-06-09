#if !defined(AFX_WKBUTTON_H__99FB32BE_6D54_4845_BD51_1BC64E774CC6__INCLUDED_)
#define AFX_WKBUTTON_H__99FB32BE_6D54_4845_BD51_1BC64E774CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WKButton.h : header file
/******************************< ���α׷� ���� >****************************/
/*   Ŭ �� �� �� : WKButton                                                */ 
/*   ��       �� :                                                         */
/*   ���� ������ : 2006. 05. 28                                            */
/***************************************************************************/

/////////////////////////////////< ��  �� >//////////////////////////////////

// ��ư ����
#define		WK_BUTTON_SHPAE_RECTANGLE				0x0001
#define		WK_BUTTON_SHAPE_CIRCLE					0x0002
#define		WK_BUTTON_SHAPE_ROUNDRECT				0x0004

// ��ư ����
#define		WK_BUTTON_COLOR_ONE						0x0001
#define		WK_BUTTON_COLOR_TWO						0x0002
#define		WK_BUTTON_COLOR_GRADATION_UP			0x0011
#define		WK_BUTTON_COLOR_GRADATION_RIGHT			0x0012
#define		WK_BUTTON_COLOR_GRADATION_DOWN			0x0014
#define		WK_BUTTON_COLOR_GRADATION_LEFT			0x0018

// ��ư ����
#define		WK_BUTTON_STATUS_MOUSEOVER				0x0001
#define		WK_BUTTON_STATUS_MOUSELEAVE				0x0002
#define		WK_BUTTON_STATUS_MOUSECLICK				0x0004


///////////////////////////////< Ŭ���� ���� >///////////////////////////////
class AFX_EXT_CLASS CWKButton : public CButton
{
public://<<<<<<<<<<<<<<<<<<<<<< �ܺ� ���� �Լ� >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
/*****************************< ��ư ���� �Լ� >****************************/
	/* �ؽ�Ʈ ��� �� ���� ���� */
	BOOL SetButtonText(CString strCaption);
	BOOL SetButtonText(CString strCaption, COLORREF rgbText);
	BOOL SetButtonText(CString strCaption, CFont *font, COLORREF rgbText);
	
	/* ��ư ���� �� ���� ���� */
	BOOL SetButtonStyle(DWORD dShape, COLORREF rgbColor1);
	BOOL SetButtonStyle(DWORD dShape, DWORD dStyle,	COLORREF rgbColor1,	COLORREF rgbColor2);
	/***************************************************************************/

protected://<<<<<<<<<<<<<<<< ���� ó�� �Լ� �� ���� >>>>>>>>>>>>>>>>>>>>>>>>>
	/* ���º� ��ư �̹��� */
	CBitmap m_bmpImage1;	//����
	CBitmap m_bmpImage2;	//���콺 ����
	CBitmap m_bmpImage3;	//Ŭ��
	
	/* ���º� ��ư �̹��� ���� */
	BOOL CreateButtonImage();

	/* ��ư ���� ���� */
	BOOL DrawFormRectangle();	// �簢
	BOOL DrawFormCircle();		// ��
	BOOL DrawFormRoundRect();	// �ձٻ簢

	/* �̹��� ó�� */
	COLORREF LightValueControl(COLORREF rgb, int nValue);						// ���� ��� ����
	void DrawGradation(CBitmap* bmpImage, COLORREF color1, COLORREF color2);	// �׶��̼� �̹��� ����
	BOOL DrawBorder(CBitmap* pbmpImage, COLORREF color1, COLORREF color2, int nWidth = 2);		// ��ư �׵θ� �׸���
	BOOL DrawText(CBitmap* pBitmap, int nStatus);								// ��ư�� �۾� �ֱ�

	/* ��ư �̹��� ���� */
	DWORD m_dShape;			// ��ư ����
	DWORD m_dStyle;			// �÷� ��Ÿ��
	COLORREF m_rgbColor1;	// ù��° �÷�
	COLORREF m_rgbColor2;	// �ι�° �÷�
	CRect m_rtButton;		// ��ưũ��
	int m_nWidth;			// ��ư �ʺ�
	int m_nHeight;			// ��ư ����

	/* �ؽ�Ʈ ���� */
	CString m_strCaption;	// ĸ�� ����
	CFont m_Font;			// ĸ�� �۲�
	COLORREF m_rgbText;		// ĸ�� ����
	BOOL m_bValueChange;	// ĸ�� ����
	
	/* ��ư ���� ���� */
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
