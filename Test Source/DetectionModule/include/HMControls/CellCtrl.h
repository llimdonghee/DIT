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
		CELLCOLOR oldClr;	//�������� ��
		CELLCOLOR newClr;	//�������� ��
		LPCSTR lpstrOld;	//�Է��� ���ڿ�
		LPCSTR lpstrNew;	//�Է��� ���ڿ�
	};

	struct CELLDATA
	{
		COLORREF txtclr;//���ڻ�
		COLORREF bkgclr;//����
		bool	 bUseCtl;//��Ʈ�� ���
		bool	 bTurn; //��¦��¦
		DWORD    dwData;
	};

	class CCellData : public CELLDATA
	{
	public:
		CCellData()
		{
			bTurn =false;
			bUseCtl=false;//��Ʈ�� ���
			txtclr=clrTEXT;
			bkgclr=clrBACK;
			dwData=0;
		}

		CCellData(COLORREF TextColor, COLORREF BackColor,
				  bool bTurning, DWORD dwUserData=0)
		{
			bTurn =bTurning;
			bUseCtl=false;//��Ʈ�� ���
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
	CBrush m_brush;		//����Ʈ�ڽ��� ���
	CEdit  m_edit;		//����Ʈ �ڽ�
	BOOL	m_bCanEdit;
	CFont* m_pFont;		//����Ʈ ��Ʈ

protected:
	void Printing(CDC* pDC);//����Ʈ�� ����Ʈ�� ����Ѵ�.
	CRect GetLogRect(CDC* pDC, int nItem, int nSubItem);//����Ʈ ��Rect�� ������ ���� ��ȯ�Ѵ�.
	void SetCrtToPrintRate(CDC* pDC);//ȭ��� ��ǥ�� ������ ���Ѵ�.
	int  GetCrtWidth();//ȭ��(����Ʈ)�� ������ �����Ѵ�.

protected:
	BOOL m_bSort;//Sort Flag
	BOOL m_bTime;//SetTimer(,,)�� �����ߴ��� ����
	BOOL m_bTurnDraw;//����Ʈ ���Ž� ���� ������ ���ؾ��ϴ��� ����
	UINT  m_nClipOptin;//Ŭ������ �ɼ�
	CHeadCtrl m_head;//HeaderCtrl
	LOCATION m_mcLct;//right mouse click
	LOCATION m_edLct;//edit location
	UINT m_nStartColEdit;
	UINT m_nEndColEdit;
	CMenu m_menu;//����Ʈ �޴� 
	CMenu* m_pUmenu;//����� ���� �޴�
	CELLINFO* m_pInfo;

	//������ Item Data�� �����Ѵ�.
	void DeleteItemData(int nItem);
	void DeleteAllItemData();

	//����Ʈ�ڽ��� Data�� �����ߴ��� ���θ� üũ�ϰ� 
	//�θ�������� �޽����� ������.
	BOOL IsModifyEdit(int nItem, int nSubItem);

	//�������� ������ ������ ��쿡 ����Ʈ�ڽ��� ũ�⵵ ���ؾ� �ϹǷ�
	//�����Լ��� �̿��Ͽ� ����Ʈ�ڽ��� �����Ѵ�.
	void UpdateEdit(int nItem, int nSubItem);

	//���� ������ �����Ѵ�.
	void UpdateCell(int nItem, int nSubItem);

	//���� �÷��� ���ϵ��� �����Ǿ� �ִ��� ����
	bool IsTurning(int nItem, int nSubItem);

	//����Ʈ�� ���� left,right,center ������ ���Ѵ�.
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

	//Rect�� ���ߴ��� �Ǵ��Ѵ�.
	BOOL IsSubRectChange();	

	//����Ʈ�� ����Ʈ�� ����Ѵ�.
	BOOL ListPrinting(BOOL bDialog, int Orientation=1/*����=1, ����=2*/);

	//keystate function 
	BOOL IsMultiKey();//shiftŰ, ctrlŰ���� ���������� check�Ѵ�

	//font function
	void SetLogFont(LOGFONT logfont);//��ǥ ��Ʈ
	void SetListFont(LOGFONT* logfont);//����Ʈ ��Ʈ

	//
	int GetSubItemCount();		//�÷����� ���Ѵ�.
	int GetLeftSubItem();		//�������κ��� Ȱ��ȭ�� �÷��� ã�´�.
	int GetRightSubItem();		//�������κ��� Ȱ��ȭ�� �÷��� ã�´�.
	
	//����ڰ� �ʿ��� ���� �����ϰ� �ҷ��ü� �ִ� �Լ��̴�.
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
