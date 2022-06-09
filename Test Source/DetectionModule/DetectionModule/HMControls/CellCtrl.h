#ifndef _CELLCTRL_H_
#define _CELLCTRL_H_
#define _CELLCTRL_LIB_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CellCtrl.h : header file
//
#include "HeadCtrl.h"

#ifndef  __AFXTEMPL_H
#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellCtrl window
#define CCMSG_USE_EDIT			WM_USER+1
#define CCMSG_MOVE_EDIT			WM_USER+2
#define CCMSG_RECT_CHANGE		WM_USER+3
//#define CCMSG_SEL_PRINT		WM_USER+4
#define CCMSG_LIST_PRINT		WM_USER+5
#define CCMSG_FONT_CHANGE		WM_USER+6
#define CCMSG_SAVE_FILE			WM_USER+7
#define CCMSG_OPEN_EXCEL		WM_USER+8
#define CCMSG_OPEN_LIST			WM_USER+9
#define CCMSG_MODIFY_EDIT		WM_USER+10
#define CCMSG_COPY_CLIPBOARD	WM_USER+11
#define CCMSG_PASTE_CLIPBOARD	WM_USER+12
#define CCMSG_CUT_CLIPBOARD		WM_USER+13
#define CCMSG_SELITEM_DELETE	WM_USER+14
#define CCMSG_BACK_COLOR		WM_USER+15
#define CCMSG_TEXT_COLOR		WM_USER+16
#define CCMSG_LIST				WM_USER-1

const COLORREF clrTEXT=0x00000000;
const COLORREF clrBACK=0x00FFFFFF;

class AFX_EXT_CLASS CCellCtrl : public CListCtrl
{
public:
	struct LOCATION
	{
		int nItem;
		int nSubItem;
	};

	struct CELLCOLOR
	{
		COLORREF txtclr;
		COLORREF bkgclr;
	};

	struct CELLINFO
	{
		int nId;			//CONTROL ID
		int nItem;			//LIST ITEM
		int nSubItem;		//LIST SUBITEM
		CELLCOLOR oldClr;	//설정전의 색
		CELLCOLOR newClr;	//설정후의 색
		LPCSTR lpstrOld;	//입력전 문자열
		LPCSTR lpstrNew;	//입력한 문자열
	};

	struct CELLDATA
	{
		COLORREF txtclr;//글자색
		COLORREF bkgclr;//배경색
		bool	 bUseCtl;//컨트롤 사용
		bool	 bTurn; //반짝반짝
		DWORD    dwData;
	};

	class CCellData : public CELLDATA
	{
	public:
		CCellData()
		{
			bTurn =false;
			bUseCtl=false;//컨트롤 사용
			txtclr=clrTEXT;
			bkgclr=clrBACK;
			dwData=0;
		}

		CCellData(COLORREF TextColor, COLORREF BackColor,
				  bool bTurning, DWORD dwUserData=0)
		{
			bTurn =bTurning;
			bUseCtl=false;//컨트롤 사용
			txtclr=TextColor;
			bkgclr=BackColor;
			dwData=dwUserData;
		}
		~CCellData(){;}
		void operator=(const CELLDATA& cc)
		{
			(CELLDATA)*this=cc;	
		}
	};
	typedef CArray<CCellData, CCellData&> CCSArray;
	virtual void SerializeAllInfo(CArchive& ar);

protected:
	CBrush m_brush;		//에디트박스의 배경
	CEdit  m_edit;		//에디트 박스
	BOOL	m_bCanEdit;
	CFont* m_pFont;		//리스트 폰트

protected:
	void Printing(CDC* pDC);//리스트를 프린트로 출력한다.
	CRect GetLogRect(CDC* pDC, int nItem, int nSubItem);//리스트 셀Rect을 비율에 따라 변환한다.
	void SetCrtToPrintRate(CDC* pDC);//화면과 장표의 비율을 구한다.
	int  GetCrtWidth();//화면(리스트)의 폭값을 리턴한다.

protected:
	BOOL m_bSort;//Sort Flag
	BOOL m_bTime;//SetTimer(,,)를 설정했는지 여부
	BOOL m_bTurnDraw;//리스트 갱신시 셀의 색깔이 변해야하는지 여부
	UINT  m_nClipOptin;//클립보드 옵션
	CHeadCtrl m_head;//HeaderCtrl
	LOCATION m_mcLct;//right mouse click
	LOCATION m_edLct;//edit location
	UINT m_nStartColEdit;
	UINT m_nEndColEdit;
	CMenu m_menu;//리스트 메뉴 
	CMenu* m_pUmenu;//사용자 정의 메뉴
	CELLINFO* m_pInfo;

	//설정된 Item Data를 삭제한다.
	void DeleteItemData(int nItem);
	void DeleteAllItemData();

	//에디트박스의 Data를 수정했는지 여부를 체크하고 
	//부모윈도우로 메시지를 보낸다.
	BOOL IsModifyEdit(int nItem, int nSubItem);

	//아이템의 영역이 변했을 경우에 에디트박스의 크기도 변해야 하므로
	//다음함수를 이용하여 에디트박스를 갱신한다.
	void UpdateEdit(int nItem, int nSubItem);

	//셀의 영역을 갱신한다.
	void UpdateCell(int nItem, int nSubItem);

	//셀의 컬러가 변하도록 설정되어 있는지 여부
	bool IsTurning(int nItem, int nSubItem);

	//리스트의 정렬 left,right,center 형식을 구한다.
	int  GetColumnFmt(int nSubItem);	

public:
	enum ListOption
	{
		//MENU OPTION
		menuNO		=	0x00000000,
		menuEXCEL	=	0x00000001,
		menuLIST	=	0x00000002,
		menuPRINT	=	0x00000004,
		menuINPUT	=	0x00000010,
		menuSAVE	=	0x00000020,
		menuFONT	=	0x00000040,
		menuBACKCOLOR=	0x00000100,
		menuTEXTCOLOR=	0x00000200,
		menuCOPY	=	0x00010000,
		menuPASTE	=	0x00020000,
		menuCUT		=	0x00040000,
		menuDELETE	=	0x00080000,
		menuDitCore	=	0x000000FF,
		menuCLIP	=	0x000F0000,
		menuALL		=	0x000FFFFF,
		menuAceept  =   0x00010000,

		//CLIPBOARD OPTION
		clipNO		=	0x00000000,
		clipCOPY	=	0x00000001,
		clipPASTE	=	0x00000002,
		clipCUT		=	0x00000004,
		clipDELETE	=	0x00000008,
		clipALL		=	0x0000FFFF,
	};
	CCellCtrl();

	//Sort function
	void EnableSort(BOOL bSort=TRUE){m_bSort=bSort;}
	void EnableEdit(BOOL bCanEdit)	{ m_bCanEdit = bCanEdit; }

	//menu function
	BOOL CCellCtrl::SetMenu(CMenu* pMenu=NULL){m_pUmenu=pMenu; return TRUE;}
	void UseMenu( UINT nOption=menuDitCore/*default=menuDitCore*/);

	//editbox function
	BOOL SetEditMovable( int nStartCol, int nEndCol);
	BOOL UseEdit( int nItem, int nSubItem);
	void EditCtrl( MSG pMsg );

	//Rect이 변했는지 판단한다.
	BOOL IsSubRectChange();	

	//리스트를 프린트로 출력한다.
	BOOL ListPrinting(BOOL bDialog, int Orientation=1/*가로=1, 세로=2*/);

	//keystate function 
	BOOL IsMultiKey();//shift키, ctrl키등이 눌러졌는지 check한다

	//font function
	void SetLogFont(LOGFONT logfont);//장표 폰트
	void SetListFont(LOGFONT* logfont);//리스트 폰트

	//
	int GetSubItemCount();		//컬럼수를 구한다.
	int GetLeftSubItem();		//좌측으로부터 활성화된 컬럼을 찾는다.
	int GetRightSubItem();		//우측으로부터 활성화된 컬럼을 찾는다.
	
	//사용자가 필요한 값을 저장하고 불러올수 있는 함수이다.
	BOOL SetCellData(int nItem, int nSubItem, DWORD dwData);
	DWORD GetCellData(int nItem, int nSubItem);

	//color function
	BOOL SetItemTextColor(int nItem, COLORREF color);
	BOOL SetItemTextColor(int nItem, int nSubItem, COLORREF color);
	BOOL SetItemBkColor(int nItem, COLORREF color, bool bTurn=false);
	BOOL SetItemBkColor(int nItem, int nSubItem, COLORREF color, bool bTurn=false);
	BOOL GetItemColor(int nItem, int nSubItem, CELLCOLOR* pCellClr);

	//Clipboard function
	void UseClipboard( UINT nOption=clipALL/*default=clipALL*/){m_nClipOptin=nOption;}
	void CopyClipboard();// Ctrl C (select item copy)
	void PasteClipboard();//Ctrl V (paste)
	void CutClipboard();//Ctrl V (paste)
	void SelectItemDelete();

	BOOL DeleteColumn( int nCol );
	int  InsertColumn(int nCol, LPCTSTR lpstTxt,int nFormat=LVCFMT_LEFT, 
						int nWidth = -1, int nSubItem = -1);

	//HeaderCtrl function
	BOOL GetSafeHead();
	int  GetHeadHeight();
	int  GetHeadItemCount();
	void SetHeadItemCount(int nCount);
	void SetHeadItemText(int nItem, int nSubItem, LPCTSTR lpstTxt);
	CString GetHeadItemText(int nItem, int nSubItem);
	void SetHeadItemMerge(int nsItem, int nsSubItem, 
						  int neItem, int neSubItem);

protected:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellCtrl)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveFile();
	virtual ~CCellCtrl();

	// Generated message map functions
protected:

	//{{AFX_MSG(CCellCtrl)
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnSaveFile();
	afx_msg void OnOpenExcel();
	afx_msg void OnOpenList();
	afx_msg void OnUseEdit();
	afx_msg void OnMoveEdit();
	afx_msg void OnRectchange();
	afx_msg void OnListPrint();
	afx_msg void OnFontChange();
	afx_msg void OnBackColor();
	afx_msg void OnTextColor();

	afx_msg void OnCopyClipboard()	{	CopyClipboard();	}
	afx_msg void OnPasteClipboard()	{	PasteClipboard();	}
	afx_msg void OnCutClipboard()	{	CutClipboard();		}
	afx_msg void OnSelItemDelete()	{	SelectItemDelete();	}

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CELLCTRL_H_)
