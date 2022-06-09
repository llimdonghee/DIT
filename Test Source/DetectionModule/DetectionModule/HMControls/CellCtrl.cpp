// CellCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CellCtrl.h"
#include "LogCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellCtrl
#define CHECKRGB(c) (c > 255 ? 255 : c)
#define DIVCLR(c) RGB(CHECKRGB(GetRValue(c)*1.5),\
					  CHECKRGB(GetGValue(c)*1.5),\
					  CHECKRGB(GetBValue(c)*1.5))

class CListSort
{
public:
	// The sort depend on the EnumDataType. We can extent the type ...
	enum SortDataType { dtNULL = 0, dtINT, dtDEC, dtSTRING, dtSTRINGNOCASE, dtDATETIME };

	CListSort(CCellCtrl *pList, const int nCol)
	{
		m_pList = pList;
		ASSERT(m_pList);
		int totItems = m_pList->GetItemCount();

		// Replace All Item Data with pointer to CSortItem class
		for (int i = 0; i < totItems; i++)
		{
			DWORD dw = static_cast<DWORD>(m_pList->GetItemData(i));
			CString txt = m_pList->GetItemText(i, nCol);
			m_pList->SetItemData(i, (DWORD) new CSortItem(dw, txt));
		}
	}
	virtual ~CListSort()
	{
		ASSERT(m_pList);
		int totItems = m_pList->GetItemCount();
		for (int i = 0; i < totItems; i++)
		{
			CSortItem *pItem = (CSortItem*)(m_pList->GetItemData(i));
			ASSERT(pItem);
			m_pList->SetItemData(i, pItem->m_dw);
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
	}
	void Sort(BOOL bAsc, SortDataType dtType)
	{
		long lParamSort = bAsc ? dtType : -dtType;
		m_pList->SortItems(Compare, lParamSort);
	}

protected:
	CCellCtrl *m_pList;
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		CSortItem *pItem1 = (CSortItem*)lParam1;
		CSortItem *pItem2 = (CSortItem*)lParam2;
		ASSERT(pItem1 && pItem2);

		int nOrder = (lParamSort < 0) ? -1 : 1;
		SortDataType dtType = (SortDataType) (lParamSort * nOrder); // get rid of sign

		switch (dtType)
		{
		case /*SortDataType::*/dtINT:
			return (atol(pItem1->m_txt) - atol(pItem2->m_txt)) * nOrder;
		case /*SortDataType::*/dtDEC:
			return (int) ((atof(pItem1->m_txt) - atof(pItem2->m_txt)) * nOrder);
		case /*SortDataType::*/dtSTRING:
			return pItem1->m_txt.Compare(pItem2->m_txt) * nOrder;
		case /*SortDataType::*/dtSTRINGNOCASE:
			return pItem1->m_txt.CompareNoCase(pItem2->m_txt) * nOrder;
		case /*SortDataType::*/dtDATETIME:
			{
				COleDateTime time1, time2;
				if (time1.ParseDateTime(pItem1->m_txt) && time2.ParseDateTime(pItem2->m_txt))
					return ((time1 == time2) ? 0 : (time1 < time2 ? -1 : 1)) * nOrder;
			}
		default:
			ASSERT("Error: attempt to sort a column without type.");
			return 0;
		}
	}

	class CSortItem
	{
	public:
		CSortItem(const DWORD dw, const CString &txt) : m_txt(txt),m_dw(dw){;}
		virtual ~CSortItem() {;}
		DWORD m_dw;
		CString m_txt;
	};
};

CCellCtrl::CCellCtrl()
{
	m_bSort=TRUE;//Sort Flag
	m_bTime=FALSE;//SetTimer Flag
	m_bTurnDraw=FALSE;//Item Color Convert 
	m_pFont=NULL;//리스트 폰트
	m_pUmenu=NULL;//사용자 메뉴
	m_edLct.nItem=0;//에디트박스 위치
	m_edLct.nSubItem=0;
	m_mcLct.nItem=0;//마우스 클릭 위치
	m_mcLct.nSubItem=0;
	m_bCanEdit = FALSE;
	m_pInfo=new CELLINFO;//리스트 메시지 정보
	m_nClipOptin=CCellCtrl::clipNO;//클립보드 state
}

CCellCtrl::~CCellCtrl()
{
	m_menu.DestroyMenu();
	m_edit.DestroyWindow();
	if(m_pFont)	
	{
		m_pFont->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
	if(m_pInfo)
		delete m_pInfo;
	m_pInfo = NULL;
}

BEGIN_MESSAGE_MAP(CCellCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCellCtrl)
//	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertitem)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomdrawList )
	ON_COMMAND(CCMSG_OPEN_EXCEL, OnOpenExcel)
	ON_COMMAND(CCMSG_OPEN_LIST, OnOpenList)
	ON_COMMAND(CCMSG_SAVE_FILE, OnSaveFile)
	ON_COMMAND(CCMSG_USE_EDIT, OnUseEdit)
	ON_COMMAND(CCMSG_MOVE_EDIT, OnMoveEdit)
	ON_COMMAND(CCMSG_RECT_CHANGE, OnRectchange)
	ON_COMMAND(CCMSG_LIST_PRINT, OnListPrint)
	ON_COMMAND(CCMSG_FONT_CHANGE, OnFontChange)
	ON_COMMAND(CCMSG_COPY_CLIPBOARD, OnCopyClipboard)
	ON_COMMAND(CCMSG_PASTE_CLIPBOARD, OnPasteClipboard)
	ON_COMMAND(CCMSG_CUT_CLIPBOARD, OnCutClipboard)
	ON_COMMAND(CCMSG_SELITEM_DELETE, OnSelItemDelete)
	ON_COMMAND(CCMSG_BACK_COLOR, OnBackColor)
	ON_COMMAND(CCMSG_TEXT_COLOR, OnTextColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellCtrl message handlers

/*
void CCellCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(point.x==-1 && point.y==-1)
	{
//		ScreenToClient(&point);
		point=CPoint(0,0);
		ClientToScreen(&point);
	}
	if(m_pUmenu)
	{
		CWnd* pParWnd = GetParent();
 		m_pUmenu->TrackPopupMenu(
 			TPM_CENTERALIGN | TPM_RIGHTBUTTON,
 			point.x, point.y, pWnd, 0);
	}
	else if(m_menu.GetSafeHmenu())
	{
 		m_menu.TrackPopupMenu(
 			TPM_CENTERALIGN | TPM_RIGHTBUTTON,
 			point.x, point.y, pWnd, 0);
	}
}
*/
void CCellCtrl::UseMenu(UINT nOption)
{
	
	m_pUmenu=NULL;
	//메뉴를 사용하지 않을 경우
	if(nOption==CCellCtrl::menuNO)
	{
		if(m_menu.GetSafeHmenu())
			m_menu.DestroyMenu();
		return;
	}

	if(m_menu.GetSafeHmenu())
		m_menu.DestroyMenu();

	m_menu.CreatePopupMenu( ); 

	//clipboard Option
//	if(nOption & CCellCtrl::menuCOPY)
//		m_menu.AppendMenu(MF_STRING, CCMSG_COPY_CLIPBOARD, "복사	Ctrl+C"); 
//	if(nOption & CCellCtrl::menuPASTE)
//		m_menu.AppendMenu(MF_STRING, CCMSG_PASTE_CLIPBOARD,"붙여넣기	Ctrl+V"); 
//	if(nOption & CCellCtrl::menuCUT)
//		m_menu.AppendMenu(MF_STRING, CCMSG_CUT_CLIPBOARD,	 "잘라내기	Ctrl+X"); 
//	if(nOption & CCellCtrl::menuDELETE)
//		m_menu.AppendMenu(MF_STRING, CCMSG_SELITEM_DELETE, "삭제	Ctrl+Del"); 
//	if(nOption & CCellCtrl::menuBACKCOLOR)
//		m_menu.AppendMenu(MF_STRING, CCMSG_BACK_COLOR, "배경색 설정"); 
//	if(nOption & CCellCtrl::menuTEXTCOLOR)
//		m_menu.AppendMenu(MF_STRING, CCMSG_TEXT_COLOR, "글색 설정"); 

//	if(nOption & 0xFFFF0000)
//		m_menu.AppendMenu(MF_SEPARATOR); 

	//menu Option
//	if(nOption & CCellCtrl::menuINPUT)
//		m_menu.AppendMenu(MF_STRING, CCMSG_USE_EDIT,"입력"); 
	if(nOption & CCellCtrl::menuSAVE)
		m_menu.AppendMenu(MF_STRING, CCMSG_SAVE_FILE,"전환(Excel)"); 
	if(nOption & CCellCtrl::menuEXCEL)
		m_menu.AppendMenu(MF_STRING, CCMSG_OPEN_EXCEL,"파일열기(Excel)"); 
//	if(nOption & CCellCtrl::menuLIST)
//		m_menu.AppendMenu(MF_STRING, CCMSG_OPEN_LIST,"파일열기(List)"); 
//	if(nOption & CCellCtrl::menuPRINT)
//		m_menu.AppendMenu(MF_STRING, CCMSG_LIST_PRINT,"프린트"); 
//	if(nOption & CCellCtrl::menuFONT)
//		m_menu.AppendMenu(MF_STRING, CCMSG_FONT_CHANGE,"폰트 설정"); 

}
void CCellCtrl::SerializeAllInfo(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{
	}
}

void CCellCtrl::Serialize(CArchive& ar) 
{
	
	if (ar.IsStoring())
	{	// storing code
		CString strText="";
		char szText[256];
		int  i=0, j=0, nCol=0, nRow=0;
		LVCOLUMN lvcol;
		memset(&lvcol, NULL, sizeof(lvcol));
		lvcol.mask = LVCF_TEXT;
		lvcol.pszText=szText;
		lvcol.cchTextMax=256;

		while(GetColumn(nCol, &lvcol))
		{
			strText += lvcol.pszText;
			strText += ",";
			lvcol.iSubItem=nCol++;
		}
		strText.TrimRight(",");
		strText+="\n";

		ar.WriteString(strText);

		//리스트 아이템의 문자열을 가져온다.
		nRow=GetItemCount();
		for( i=0; i<nRow; i++)
		{
			strText="";
			for( j=0; j<nCol; j++)
				strText+=(GetItemText(i,j)+",");

			strText.TrimRight(",");
			strText+="\n";
			ar.WriteString(strText);
		}	
	}
	else
	{	
		int nStart=0, nEnd=0;
		int nRow=0, nCol=0;
		int nSubItem=0;
		CString strText="";
		CString str="";
		// HEADER
		ar.ReadString(strText);
//		if(!strText.IsEmpty())
//		{
//			while((nEnd=strText.Find(_T(","), nStart))>=0)
//			{
//				str=strText.Mid(nStart, nEnd-nStart);
//				InsertColumn(++nCol, str, LVCFMT_LEFT, 100);
//				nStart=nEnd+1;
//			}
//			str=strText.Mid(nStart, nStart+256);
//			InsertColumn(nCol, str, LVCFMT_LEFT, 100);
//		}
			
		// LIST
		while(ar.ReadString(strText))
		{
			if(strText.IsEmpty())
				break;

			nStart=0;
			nCol=0;
			InsertItem(nRow,"");
			while((nEnd=strText.Find(_T(","), nStart))>=0)
			{
				str=strText.Mid(nStart, nEnd-nStart);
				SetItemText(nRow, nCol++, str);
				nStart=nEnd+1;
			}
			str=strText.Mid(nStart, nStart+256);
			SetItemText(nRow, nCol, str);
			nRow++;
		}

	}

}

int CCellCtrl::GetSubItemCount()
{
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	int iColCnt=0;
	while(GetColumn( ++iColCnt, &lvcol) ){};
	return iColCnt;
}

void CCellCtrl::OnRectchange()
{
	CRect rectLC;
	CRect rectED;
	rectLC.SetRectEmpty();
	rectED.SetRectEmpty();

	m_edit.GetWindowRect(rectED);
	if(m_edLct.nSubItem)
		GetSubItemRect( m_edLct.nItem, m_edLct.nSubItem,LVIR_BOUNDS,rectLC);
	else
		GetSubItemRect( m_edLct.nItem, m_edLct.nSubItem,LVIR_LABEL,rectLC);

	ClientToScreen(rectLC);

	if(rectED.IsRectEmpty() || rectLC.IsRectEmpty())
		return;

	if(	rectED.left==rectLC.left && rectED.Width()==rectLC.Width())
		return;

	PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
}

void CCellCtrl::OnUseEdit()
{
	UseEdit( m_mcLct.nItem, m_mcLct.nSubItem);
}

void CCellCtrl::OnMoveEdit()
{
	CRect rect;
	CString strLCtxt=_T("");

	if(m_edLct.nSubItem)
		GetSubItemRect( m_edLct.nItem, m_edLct.nSubItem,LVIR_BOUNDS,rect);
	else
		GetSubItemRect( m_edLct.nItem, m_edLct.nSubItem,LVIR_LABEL,rect);
	
	if(GetStyle()&LVS_REPORT && rect.top<GetHeadHeight())
	{
		m_edit.ShowWindow(FALSE);
		return;
	}

	strLCtxt=GetItemText( m_edLct.nItem, m_edLct.nSubItem);
	m_edit.MoveWindow(rect);
	m_edit.SetWindowText(strLCtxt);
	m_edit.Invalidate();
	m_edit.ShowWindow(TRUE);
	m_edit.SetSel(0,-1);

	memset(m_pInfo, NULL, sizeof(CELLINFO));
	m_pInfo->nId	 =GetDlgCtrlID();
	m_pInfo->nItem   =m_edLct.nItem;
	m_pInfo->nSubItem=m_edLct.nSubItem;
	m_pInfo->lpstrNew=strLCtxt;
	m_pInfo->lpstrOld=strLCtxt;
	CWnd* pWnd=GetParent();
	::SendMessage(pWnd->m_hWnd, CCMSG_LIST, (WPARAM)CCMSG_MOVE_EDIT, (LPARAM)m_pInfo);
}

BOOL CCellCtrl::UseEdit( int nItem, int nSubItem)
{
	if(nItem<0 || nSubItem<0)
		return 0;

	m_edLct.nItem=nItem;
	m_edLct.nSubItem=nSubItem;
	CRect rect;
	if(!m_edit.GetSafeHwnd())
	{
		CFont* pFont=GetFont();
		m_edit.Create(ES_LEFT | ES_AUTOHSCROLL | WS_BORDER | WS_CHILD ,rect,this,0);
		m_edit.SetFont(pFont);
	}
	PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
	return 1;
}

void CCellCtrl::UpdateEdit(int nItem, int nSubItem)
{
	if(m_mcLct.nItem==nItem && m_mcLct.nSubItem==nSubItem)
	{
		if(m_edit.GetSafeHwnd() && m_edit.IsWindowVisible())
			PostMessage(WM_COMMAND,CCMSG_RECT_CHANGE);
	}
}

BOOL CCellCtrl::IsModifyEdit(int nItem, int nSubItem)
{
	CString strEDtxt="";
	CString strLCtxt="";

	m_edit.GetWindowText(strEDtxt);
	strLCtxt = GetItemText(nItem, nSubItem);
	SetItemText(nItem,nSubItem,strEDtxt);


	if(strEDtxt==strLCtxt)
		return 0;

	memset(m_pInfo, NULL, sizeof(CELLINFO));
	m_pInfo->nId	 = GetDlgCtrlID();
	m_pInfo->nItem	 = nItem;
	m_pInfo->nSubItem= nSubItem;
	m_pInfo->lpstrNew= strEDtxt;
	m_pInfo->lpstrOld= strLCtxt;
	CWnd* pWnd=GetParent();
	::SendMessage(pWnd->m_hWnd, CCMSG_LIST, (WPARAM)CCMSG_MODIFY_EDIT, (LPARAM)m_pInfo);
	int nn=CCMSG_LIST;
	return 1;
}

void CCellCtrl::EditCtrl(MSG msg)
{
	char szKey[128]="";
	int nSChar=0,nEChar=0;
	CString string="";
	int nICnt=GetItemCount()-1;
	int nSCnt=GetSubItemCount()-1;
	int nItem=m_edLct.nItem;
	int nSubItem=m_edLct.nSubItem;

	if((CWnd*)&m_edit != GetFocus())
	{
		m_edit.SetFocus();
		GetKeyNameText( static_cast<LONG>(msg.lParam), szKey, sizeof(szKey));
		if(strlen(szKey)==1)
			m_edit.SetWindowText(szKey);
		m_edit.SetSel(1,1);
	}
	switch(msg.wParam)
	{
	case VK_UP:
		if(m_edLct.nItem<1)	return;
		m_edLct.nItem--;
		SetItemState(m_edLct.nItem,LVIS_FOCUSED ,LVIS_FOCUSED );
		EnsureVisible(m_edLct.nItem, TRUE);
		break;
	case VK_DOWN:	
		if(nICnt<=m_edLct.nItem)return;
		m_edLct.nItem++;
		SetItemState(m_edLct.nItem,LVIS_FOCUSED ,LVIS_FOCUSED );
		EnsureVisible(m_edLct.nItem, TRUE);
		break;
	case VK_LEFT:	
		if(m_edLct.nSubItem<1)return;
//		m_edit.GetSel(nSChar, nEChar);
//		if(nSChar!=nEChar)
//		{
//			m_edit.SetSel(nSChar, nSChar);
//			return;
//		}
		m_edLct.nSubItem--;
		break;
	case VK_RIGHT:	
		if(nSCnt<=m_edLct.nSubItem)return;
//		m_edit.GetSel(nSChar, nEChar);
//		if(nSChar!=nEChar)
//		{
//			m_edit.SetSel(nEChar, nEChar);
//			return;
//		}
		m_edLct.nSubItem++;
		break;
	case VK_HOME:	
		m_edLct.nSubItem=0;
		break;
	case VK_END:	
		m_edLct.nSubItem=nSCnt;
		break;
	case VK_PRIOR:	return;
	case VK_NEXT:	return;
	case VK_RETURN:
		msg.wParam=VK_DOWN;
		EditCtrl(msg);
		return;
	case VK_ESCAPE: 
		m_edit.ShowWindow(FALSE);
		return;
	default:		
		return;
	}

	//에디트 박스의 문자열과 리스트에 있는 문자열과 비교하여 
	//다를 경우 에디트 박스의 문자열을 리스트에 입력한다.
	IsModifyEdit(nItem, nSubItem);
	UpdateCell(m_edLct.nItem, m_edLct.nSubItem);
	PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);//에디트박스 이동
}

void CCellCtrl::UpdateCell(int nItem, int nSubItem)
{
	CRect rect;
	if(nSubItem==0)
		GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rect);
	else
		GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	InvalidateRect(&rect);
}

void CCellCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_mcLct.nItem=pNMListView->iItem;
	m_mcLct.nSubItem=pNMListView->iSubItem;
	*pResult = 0;
}

void CCellCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_edit.GetSafeHwnd() && m_edit.IsWindowVisible())
		PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
		
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCellCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_edit.GetSafeHwnd() && m_edit.IsWindowVisible())
		PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCellCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!m_bSort)
		return;

	static BOOL bAsc=TRUE;
	CString strText=GetItemText(0,pNMListView->iSubItem);

	COleDateTime oleTime;
	CListSort listSort(this,pNMListView->iSubItem);
	if(bAsc)bAsc=FALSE;
	else	bAsc=TRUE;

	if(strText.IsEmpty())
		listSort.Sort(bAsc,CListSort/*::SortDataType*/::dtSTRING);
	else if(oleTime.ParseDateTime(strText))
		listSort.Sort(bAsc,CListSort/*::SortDataType*/::dtDATETIME);
	else if(atoi(strText) || strText.GetAt(0)=='0')
		listSort.Sort(bAsc,CListSort/*::SortDataType*/::dtINT);
	else
		listSort.Sort(bAsc,CListSort/*::SortDataType*/::dtSTRING);

	*pResult = 0;
}

BOOL CCellCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_edit.GetSafeHwnd() && m_edit.IsWindowVisible())
		PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

HBRUSH CCellCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here
	CELLCOLOR cellcolor;
	if(GetItemColor(m_edLct.nItem, m_edLct.nSubItem, &cellcolor))
	{
		if(m_edit.IsWindowVisible())
		{
			m_brush.DeleteObject();
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cellcolor.txtclr);
			m_brush.CreateSolidBrush(cellcolor.bkgclr);
			return (HBRUSH)m_brush;
		}
	}
	return hbr;
}

BOOL CCellCtrl::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		HitTest(CPoint(static_cast<int>(pMsg->wParam), static_cast<int>(pMsg->lParam)));
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_KEYDOWN:
		if(m_edit.GetSafeHwnd() && m_edit.IsWindowVisible())
		{
			if(!IsMultiKey())
				EditCtrl(*pMsg);
		}

		if(GetKeyState(VK_CONTROL) & 128)
		{
			if((CWnd*)&m_edit == GetFocus())
				break;

			switch(pMsg->wParam)
			{
			case 'C':	CopyClipboard();	break;
			case 'V':	PasteClipboard();	break;
			case 'X':	CutClipboard();		break;
			case VK_DELETE:	SelectItemDelete();	break;
			}
		}
	}
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CCellCtrl::OnSaveFile() 
{

	
	CFileDialog fileDlg( FALSE, NULL, NULL,
						OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT,
						"Excel Data Files (*.csv)|*.csv||", this );

	if(fileDlg.DoModal()!=IDOK)
		return;

	//파일 이름을 가져온다.
	CString filename = fileDlg.GetFileName();
	CString extname = fileDlg.GetFileExt();
	extname.MakeLower();

	//파일의 확장명이 있는지 체크
	if(extname.IsEmpty() || extname!="csv")
		filename+=".csv";

	CFile wfile(filename, 
				CFile::modeCreate | 
//				CFile::modeNoTruncate | 
				CFile::modeWrite | 
				CFile::typeBinary);

	CArchive ar(&wfile, CArchive::store);
	Serialize(ar);
	
}

void CCellCtrl::OnOpenExcel() 
{
	CFileDialog fileDlg( TRUE, NULL, NULL,
						OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT,
						"Data Files (*.xlc;*.xls;*.csv)|*.xlc; *.xls ; *.csv|All Files (*.*)|*.*||");
 
	if(fileDlg.DoModal()!=IDOK)
		return;

	//파일 이름을 가져온다.
	CString filename = fileDlg.GetPathName();
	ShellExecute(m_hWnd, "open", filename,NULL, NULL, SW_SHOW);
}

void CCellCtrl::OnOpenList() 
{
	CFileDialog fileDlg( TRUE, NULL, NULL,
						OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT,
						"Excel Data Files (*.csv)|*.csv||", this );

	if(fileDlg.DoModal()!=IDOK)
		return;

	//파일 이름을 가져온다.
	CString filename = fileDlg.GetFileName();
	CString extname = fileDlg.GetFileExt();
	extname.MakeLower();

	//파일의 확장명이 있는지 체크
	if(extname.IsEmpty() || extname!="csv")
		filename+=".csv";

	CFile rfile(filename, 
				CFile::modeCreate | 
				CFile::modeNoTruncate | 
				CFile::modeRead | 
				CFile::typeBinary);

	DeleteAllItems();
	CArchive ar(&rfile, CArchive::load);
	Serialize(ar);
}

void CCellCtrl::OnListPrint() 
{
	ListPrinting(TRUE);
}

void CCellCtrl::OnFontChange()
{
	LOGFONT logfont;
	CFont *pFont=GetFont();
	pFont->GetLogFont(&logfont);
	CFontDialog fontDlg( &logfont, CF_EFFECTS | CF_BOTH, NULL, this );
	if(fontDlg.DoModal()!=IDOK)
		return;
	fontDlg.GetCurrentFont( &logfont);
	SetListFont(&logfont);
}

void CCellCtrl::OnBackColor()
{
	CELLCOLOR cellClr;
	COLORREF  newBkgclr;
	CColorDialog clrDlg;
	CWnd* pWnd=GetParent();
	int nID = GetDlgCtrlID();
	int nSubItemCnt=GetSubItemCount();
	if(clrDlg.DoModal()!=IDOK)
		return;
	newBkgclr = clrDlg.GetColor();
	POSITION pos=GetFirstSelectedItemPosition();
	while(1)
	{
		if(!pos) break;
		int nItem=GetNextSelectedItem(pos);
		for( int nSubItem=0; nSubItem<nSubItemCnt; nSubItem++)
		{
			GetItemColor(nItem,nSubItem, &cellClr);
			memset(m_pInfo, 0, sizeof(CELLINFO));
			m_pInfo->nId = nID;
			m_pInfo->nItem = nItem;
			m_pInfo->nSubItem=nSubItem;
			m_pInfo->oldClr.bkgclr = cellClr.bkgclr;
			m_pInfo->newClr.bkgclr = newBkgclr;

			SetItemBkColor(nItem, nSubItem, newBkgclr, IsTurning(nItem, nSubItem));
			UpdateCell(nItem,nSubItem);

			::SendMessage(pWnd->m_hWnd, CCMSG_LIST, 
				(WPARAM)CCMSG_BACK_COLOR, 
				(LPARAM)m_pInfo);
		}
	}
}

void CCellCtrl::OnTextColor()
{
	CELLCOLOR cellClr;
	COLORREF  newTxtclr;
	CColorDialog clrDlg;
	CWnd* pWnd=GetParent();
	int nID = GetDlgCtrlID();
	int nSubItemCnt=GetSubItemCount();
	if(clrDlg.DoModal()!=IDOK)
		return;
	newTxtclr = clrDlg.GetColor();

	POSITION pos=GetFirstSelectedItemPosition();
	while(1)
	{
		if(!pos) break;
		int nItem=GetNextSelectedItem(pos);
		for( int nSubItem=0; nSubItem<nSubItemCnt; nSubItem++)
		{
			GetItemColor(nItem,nSubItem, &cellClr);
			memset(m_pInfo, 0, sizeof(CELLINFO));

			m_pInfo->nId = nID;
			m_pInfo->nItem=nItem;
			m_pInfo->nItem=nSubItem;
			m_pInfo->oldClr.txtclr=cellClr.txtclr;
			m_pInfo->newClr.txtclr=newTxtclr;

			SetItemTextColor( nItem, nSubItem, newTxtclr);
			::SendMessage(pWnd->m_hWnd, CCMSG_LIST, 
				(WPARAM)CCMSG_TEXT_COLOR, 
				(LPARAM)m_pInfo);
		}
		Update(nItem);
	}
}

BOOL CCellCtrl::ListPrinting(BOOL bDialog, int Orientation)
{
	CPrintDC printDC;
	CDC* pDC;
	pDC= printDC.GetPrintDC(Orientation, bDialog);
	if(!pDC)return 0;
	Printing(pDC);
	return 1;
}

void CCellCtrl::SetListFont(LOGFONT* plogfont)
{
	if(m_pFont)
	{
		delete m_pFont; 
		m_pFont=NULL;
	}

	EnsureVisible( 0, TRUE);
	m_pFont=new CFont;
	m_pFont->CreateFontIndirect(plogfont);
	SetFont(m_pFont);

	if(m_edit.GetSafeHwnd())
	{
		m_edit.SetFont(m_pFont);
		PostMessage(WM_COMMAND,CCMSG_MOVE_EDIT);
	}

	if(GetSafeHead())
	{
		m_head.FontChange(*plogfont);
		Update(0);
	}
}

BOOL CCellCtrl::GetSafeHead()
{
	if((GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
	{
		if(!m_head.GetSafeHwnd())	
			m_head.SubclassWindow(GetHeaderCtrl()->m_hWnd);
		return 1;
	}
	return 0;
}

BOOL CCellCtrl::IsMultiKey()
{
	int vKey=0;
	UCHAR kBuf[256];
	GetKeyboardState(kBuf);
	if(kBuf[VK_CONTROL] & 128)	return 1;
	if (kBuf[VK_SHIFT] & 128)	return 1;

	return 0;
}

void CCellCtrl::Printing(CDC* pDC)
{
	DOCINFO di;
	CLogCtrl logctrl;

	int nHeadCnt   =m_head.GetCount();
	int nItemCnt   =GetItemCount();
	int nSubItemCnt=GetSubItemCount();
	int nMode = pDC->SetMapMode(MM_ISOTROPIC);
	int nVertres=pDC->GetDeviceCaps(VERTRES);
	int nVertSize=pDC->GetDeviceCaps(VERTSIZE);
	int nHorzres=pDC->GetDeviceCaps(HORZRES);
	int nHorzSize=pDC->GetDeviceCaps(HORZSIZE);
	SetCrtToPrintRate(pDC);

	int nHeight = GetLogRect(pDC, 0, 0).Height();
	if (nHeight==0)	return ;
	int nPageItemCnt = (nVertres-(nVertSize*3))/nHeight;

	memset(&di,NULL,sizeof(di));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "리스트 출력";

	int i=0,j=0;
	int nTopIndex=0;
	while(1)
	{
		CRect rect;
		CString strText;
		CLogPage logpage;
		logpage.RemoveRow(10);
		logpage.SetColCount(nSubItemCnt);
		logpage.SetRowCount(nPageItemCnt);

		for( i=0; i<nSubItemCnt; i++)
			logpage.SetColWidth(i,GetLogRect(pDC, 0, i).Width());

		for( i=0; i<nPageItemCnt; i++)
			logpage.SetRowHeight(i,nHeight);

		//헤더부분 편집
		for( i=0; i<nHeadCnt; i++)
		{
			for( j=0; j<m_head.GetItemCount(); j++)
			{
				int down=0, right=0;
				int nMode = m_head.GetItemState(i,j,down,right);
				if(nMode==LCS_UNUSE) continue;

				if(nMode==LCS_MERGE)
					logpage.CellMerge( i, j, i+down, j+right);

				strText=m_head.GetItemText(i,j);
				logpage.SetItemText(i,j,strText);
				logpage.SetCellBkgColor(i,j, RGB(200,200,200));
				logpage.SetCellTxtColor(i,j, RGB(0,0,0));
				logpage.SetCellFormat(i,j, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			}
		}

		//리스트 편집
		for( i=0; i<nPageItemCnt; i++)
		{
			if(i+nTopIndex>=GetItemCount())	break;

			for( j=0; j<nSubItemCnt; j++)
			{
				CELLCOLOR color;
				strText = GetItemText(i+nTopIndex,j);
				if(GetItemColor(i+nTopIndex,j, &color))
				{
					logpage.SetCellBkgColor(i+nHeadCnt,j, color.bkgclr);
					logpage.SetCellTxtColor(i+nHeadCnt,j, color.txtclr);
				}

				logpage.SetItemText( i+nHeadCnt, j, strText);
				logpage.SetCellFormat(i+nHeadCnt, j, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}
		}

		logctrl.AddLogPage(logpage);
		logpage.RemoveAll();

		if(i+nTopIndex>=GetItemCount())	break;
		nTopIndex+=(nPageItemCnt-nHeadCnt);
	}

	pDC->SetMapMode(MM_TEXT);
	logctrl.Draw(pDC,&di);
	pDC->SetMapMode(nMode);

	return;
}

CRect CCellCtrl::GetLogRect(CDC* pDC, int nItem, int nSubItem)
{
	CRect rect;
	if(nSubItem)
	{
		GetSubItemRect(nItem,nSubItem,LVIR_BOUNDS,rect);
	}
	else
	{
		GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rect);
		rect.left=0;
	}

	pDC->LPtoDP(&rect);

	return rect;
}

int  CCellCtrl::GetColumnFmt(int nSubItem)
{
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nSubItem, &lvcol);
	lvcol.fmt &=0x0f;
	switch(lvcol.fmt)
	{
	case LVCFMT_LEFT:	lvcol.fmt=DT_LEFT;	break;
	case LVCFMT_RIGHT:	lvcol.fmt=DT_RIGHT;	break;
	case LVCFMT_CENTER:	lvcol.fmt=DT_CENTER;break;
	default:			lvcol.fmt=DT_CENTER;break;
	}
	return lvcol.fmt;
}

void CCellCtrl::SetCrtToPrintRate(CDC* pPntDC)
{
	int iWidth=GetCrtWidth();
	float fPntWidth=(float)pPntDC->GetDeviceCaps(HORZRES);
	float fPntHeight=(float)pPntDC->GetDeviceCaps(VERTRES);
	int iHeight= (int)((float)iWidth * (fPntHeight/fPntWidth));

	pPntDC->SetWindowExt(iWidth, iHeight);
	pPntDC->SetViewportOrg( pPntDC->GetDeviceCaps(HORZSIZE),
							pPntDC->GetDeviceCaps(VERTSIZE)*3);
	pPntDC->SetViewportExt( (int)fPntWidth-(pPntDC->GetDeviceCaps(HORZSIZE)),
							(int)fPntHeight-(pPntDC->GetDeviceCaps(VERTSIZE)*2));
}

//리스트의 SubItem과 윈도우의 크기중 큰값을 리턴한다.
int CCellCtrl::GetCrtWidth()
{
	CRect rect;
	CRect rectClient;
	GetSubItemRect(0,0,LVIR_BOUNDS,rect);
	GetClientRect(rectClient);
	rect|=rectClient;
	return rect.Width();
}

void CCellCtrl::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
}

void CCellCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	DeleteItemData(pNMListView->iItem);
	*pResult = 0;
}

void CCellCtrl::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	DeleteAllItemData();
	*pResult = 0;
}

void CCellCtrl::DeleteItemData(int nItem)
{
	CCSArray* pcsAry= (CCSArray*)GetItemData(nItem);
	if(!pcsAry)	return;
	pcsAry->RemoveAll();
	if (pcsAry)
	{
		delete pcsAry;
		pcsAry = NULL;
	}
	
	SetItemData(nItem, 0);
}

void CCellCtrl::DeleteAllItemData()
{
	int count = GetItemCount();
	for( int i=0; i<count; i++)
		DeleteItemData(i);
}

BOOL CCellCtrl::GetItemColor(int nItem, int nSubItem, CELLCOLOR* pCellClr)
{
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry)	return 0;
	if(pcsAry->GetSize()<=nSubItem)	return 0;
	pCellClr->txtclr=(*pcsAry)[nSubItem].txtclr;
	pCellClr->bkgclr=(*pcsAry)[nSubItem].bkgclr;
	return 1;
}

BOOL CCellCtrl::SetItemBkColor(int nItem, int nSubItem, COLORREF color, bool bTurn)
{
	if(!m_bTime)
	{
		m_bTime=true;
		SetTimer(GetDlgCtrlID(),500, NULL);
	}

	int nSubItemCount = GetSubItemCount();
	int nItemCount = GetItemCount();
	if(nItemCount<=nItem || nSubItemCount<=nSubItem)
		return 0;

	//아이템에 데이터가 존재하지 않을 경우 생성한다.
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry)
	{
		pcsAry = new CCSArray;
		pcsAry->SetSize(nSubItemCount);
		SetItemData(nItem, (DWORD)pcsAry);
	}
	if(pcsAry->GetSize()<=nSubItem)
	{
		pcsAry->SetAtGrow(nSubItem, CCellData(clrTEXT, color, bTurn));
	}
	else
	{
		(*pcsAry)[nSubItem].bkgclr=color;
		(*pcsAry)[nSubItem].bTurn=bTurn;
	}
	return 1;
}

bool CCellCtrl::IsTurning(int nItem, int nSubItem)
{
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry) return 0;
	return (*pcsAry)[nSubItem].bTurn;
}

BOOL CCellCtrl::SetCellData(int nItem, int nSubItem, DWORD dwUserData)
{
	int nSubItemCount = GetSubItemCount();
	int nItemCount = GetItemCount();
	if(nItemCount<=nItem || nSubItemCount<=nSubItem)
		return 0;

	//아이템에 데이터가 존재하지 않을 경우 생성한다.
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry)
	{
		pcsAry = new CCSArray;
		pcsAry->SetSize(nSubItemCount);
		SetItemData(nItem, (DWORD)pcsAry);
	}
	if(pcsAry->GetSize()<=nSubItem)
		pcsAry->SetAtGrow(nSubItem, CCellData(clrTEXT, clrBACK, false, dwUserData));
	else
		(*pcsAry)[nSubItem].dwData=dwUserData;
	return 1;
}

DWORD CCellCtrl::GetCellData(int nItem, int nSubItem)
{
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry)	return 0;
	if(pcsAry->GetSize()<=nSubItem)	return 0;
	return (*pcsAry)[nSubItem].dwData;
}

BOOL CCellCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF color)
{
	int nSubItemCount = GetSubItemCount();
	int nItemCount = GetItemCount();
	if(nItemCount<=nItem || nSubItemCount<=nSubItem)
		return 0;

	//아이템에 데이터가 존재하지 않을 경우 생성한다.
	CCSArray* pcsAry = (CCSArray*)GetItemData(nItem);
	if(!pcsAry)
	{
		pcsAry = new CCSArray;
		pcsAry->SetSize(nSubItemCount);
		SetItemData(nItem, (DWORD)pcsAry);
	}
	if(pcsAry->GetSize()<=nSubItem)
		pcsAry->SetAtGrow(nSubItem, CCellData(color, clrBACK, false));
	else
		(*pcsAry)[nSubItem].txtclr=color;
	return 1;
}

BOOL CCellCtrl::SetItemBkColor(int nItem, COLORREF color, bool bTurn)
{
	int nSubItemCount=GetSubItemCount();
	for(int i=0; i<nSubItemCount; i++)
		SetItemBkColor(nItem, i, color,bTurn);
	return 1;
}

BOOL CCellCtrl::SetItemTextColor(int nItem, COLORREF color)
{
	int nSubItemCount=GetSubItemCount();
	for(int i=0; i<nSubItemCount; i++)
		SetItemTextColor(nItem, i, color);
	return 1;
}

void CCellCtrl::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		CELLCOLOR cellclr;
		int  nItem= static_cast<int>( pLVCD->nmcd.dwItemSpec );
		BOOL bRtn = GetItemColor(nItem, pLVCD->iSubItem, &cellclr);

		pLVCD->clrTextBk= clrBACK;
		if(bRtn)
		{
			pLVCD->clrText	= cellclr.txtclr;
			pLVCD->clrTextBk= cellclr.bkgclr;
		}
		else
		{
			pLVCD->clrText	= GetTextColor();
			pLVCD->clrTextBk= GetTextBkColor();
		}
		if(	m_bTurnDraw && IsTurning(nItem, pLVCD->iSubItem))
		{
			pLVCD->clrText	= DIVCLR(cellclr.txtclr);
			pLVCD->clrTextBk= DIVCLR(cellclr.bkgclr);
		}
		UpdateEdit(nItem, pLVCD->iSubItem);
	}
}

void CCellCtrl::CopyClipboard()
{
	if(!(m_nClipOptin & clipCOPY))
		return;

	if(!OpenClipboard())
		return;

	EmptyClipboard();

	int nSubItemCount=GetSubItemCount();
	char* pBuff;
	char* szDivide="	";//TAB KEY
	CString strCopyData="";
	HGLOBAL hGlobal;
	
	POSITION pos=GetFirstSelectedItemPosition();
	while(1)
	{
		if(!pos) break;
		int nItem=GetNextSelectedItem(pos);

		CString strText="";
		for( int i=0; i<nSubItemCount; i++)
			strText+=(GetItemText(nItem,i)+szDivide);

		strText.TrimRight(szDivide);
		strText+="\r\n";
		strCopyData+=strText;
	}	

	hGlobal = GlobalAlloc(GMEM_DDESHARE, strCopyData.GetLength()+1);
	pBuff = (char*)GlobalLock(hGlobal);

	strcpy_s(pBuff, sizeof(*pBuff), LPCSTR(strCopyData));

	GlobalUnlock(hGlobal);

	SetClipboardData(CF_TEXT, hGlobal);
	CloseClipboard();
}

void CCellCtrl::PasteClipboard()
{
	if(!(m_nClipOptin & clipPASTE))
		return;

	if ( !OpenClipboard() ) 
		return;

	int i=0,j=0;
	int nRow=GetItemCount();
	int nCol=GetSubItemCount();
	char* szTab="	";	//TAB KEY

	CString strText="";
	CString strPasteData="";

	HANDLE hData = GetClipboardData( CF_TEXT );
	strPasteData = (LPCSTR)GlobalLock( hData );

	POSITION pos=GetFirstSelectedItemPosition();
	if(!pos)	return;
	int nItem=GetNextSelectedItem(pos);

	while(1)
	{
		int nStart=0, nEnd=0;
		int nFind = strPasteData.Find("\r\n");
		if(nFind<0)	break;

		strText = strPasteData.Left(nFind);
		strPasteData.Delete(0, nFind+2);

		nCol=0;
		InsertItem(nItem,"");

		while((nEnd=strText.Find(_T(szTab), nStart))>=0)
		{
			SetItemText(nItem, nCol++, strText.Mid(nStart, nEnd-nStart));
			nStart=nEnd+1;
		}

		SetItemText(nItem, nCol, strText.Mid(nStart, nStart+256));
		nItem++;
	}

	GlobalUnlock(hData);
	CloseClipboard();
}

void CCellCtrl::CutClipboard()
{
	CopyClipboard();
	SelectItemDelete();
}

void CCellCtrl::SelectItemDelete()
{
	if(!(m_nClipOptin & clipDELETE))
		return;

	ShowWindow(FALSE);
	int nItem=GetItemCount()-1;
	while(1)
	{
		if(nItem==-1) break;
		nItem = GetNextItem(nItem, LVNI_ABOVE|LVNI_SELECTED);
		DeleteItem(nItem);
	}
	ShowWindow(TRUE);
}

int CCellCtrl::GetHeadHeight()
{
	if(GetSafeHead())
		return m_head.GetHeadHeight();
	return 0;
}

int CCellCtrl::GetHeadItemCount()
{
	if(GetSafeHead())
		return m_head.GetCount();
	return 0;
}

void CCellCtrl::SetHeadItemCount(int nCount)
{
	if(GetSafeHead())
	{
		m_head.SetCount(nCount,GetFont());
		Update(0);
		Invalidate();
	}
}

BOOL CCellCtrl::DeleteColumn( int nCol )
{
	if(GetSafeHead()) m_head.DeleteColumn(nCol);
	return CListCtrl::DeleteColumn( nCol );
}


int CCellCtrl::InsertColumn(int nCol, LPCTSTR lpstTxt ,int nFormat, int nWidth, int nSubItem)
{
	int nRtn=CListCtrl::InsertColumn(nCol,lpstTxt,nFormat,nWidth,nSubItem);
	if(GetSafeHead())
	{
		m_head.InsertColumn(nCol);
		m_head.SetItemText(0, nCol, lpstTxt);
	}
	return nRtn;
}

void CCellCtrl::SetHeadItemText(int nItem, int nSubItem, LPCTSTR lpstTxt)
{
	if(!GetSafeHead()) return;
	m_head.SetItemText(nItem, nSubItem, lpstTxt);
}

CString CCellCtrl::GetHeadItemText(int nItem, int nSubItem)
{
	if(!GetSafeHead()) return "";
	return m_head.GetItemText(nItem, nSubItem);
}

void CCellCtrl::SetHeadItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem)
{
	if(!GetSafeHead()) return;
	m_head.SetItemMerge(nsItem, nsSubItem, neItem, neSubItem);
}

int CCellCtrl::GetLeftSubItem()
{
	CRect rect;
	int nSubItemCount = GetSubItemCount();
	for( int i=0; i<nSubItemCount; i++)
	{
		if(i) GetSubItemRect(0,i,LVIR_BOUNDS,rect);
		else  GetSubItemRect(0,i,LVIR_LABEL,rect);
		if(rect.right>0)
			return i;
	}
	return -1;
}

int CCellCtrl::GetRightSubItem()
{
	CRect rect,vRect;
	GetClientRect(vRect);
	int nSubItemCount = GetSubItemCount();
	for( int i=GetLeftSubItem(); i<nSubItemCount; i++)
	{
		if(i) GetSubItemRect(0,i,LVIR_BOUNDS,rect);
		else  GetSubItemRect(0,i,LVIR_LABEL,rect);

		if(rect.left>vRect.right)
			return i;
	}
	return nSubItemCount;
}

void CCellCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	if(GetDlgCtrlID()!=(int)nIDEvent)
		return;

	if(m_bTurnDraw)	m_bTurnDraw=FALSE;
	else			m_bTurnDraw=TRUE;

	int nItemCnt = GetItemCount();

	int nsItem=GetTopIndex();
	int neItem=GetCountPerPage()+nsItem+1;

	int nsSubI=GetLeftSubItem();
	int neSubI=GetRightSubItem();

	neItem= neItem < nItemCnt ? neItem:nItemCnt;

	for( int i=nsItem; i<neItem; i++)
	{
		for( int j=nsSubI; j<neSubI; j++)
		{
			if(IsTurning(i,j))
				UpdateCell(i, j);
		}
	}
//	CListCtrl::OnTimer(nIDEvent);
}
void CCellCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();
	KillTimer(GetDlgCtrlID());
}

//OnTimer
//SetTimer를 사용하지 않아도 nIDEvent==45인 Timer가 기본으로 돌고 있다.(1초주기)
//nIDEvent를 Check하지 않으면 ...?

void CCellCtrl::SaveFile()
{
	CFileDialog fileDlg( FALSE, NULL, NULL,
						OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT,
						"Excel Data Files (*.csv)|*.csv||", this );

	if(fileDlg.DoModal()!=IDOK)
		return;

	//파일 이름을 가져온다.
	CString filename = fileDlg.GetFileName();
	CString extname = fileDlg.GetFileExt();
	extname.MakeLower();

	//파일의 확장명이 있는지 체크
	if(extname.IsEmpty() || extname!="csv")
		filename+=".csv";

	CFile wfile(filename, 
				CFile::modeCreate | 
//				CFile::modeNoTruncate | 
				CFile::modeWrite | 
				CFile::typeBinary);

	CArchive ar(&wfile, CArchive::store);
	Serialize(ar);
}

void CCellCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bCanEdit)
	{
		if (m_edLct.nItem > 0 || m_edLct.nSubItem > 0)
		{
			IsModifyEdit(m_edLct.nItem, m_edLct.nSubItem);
			UpdateCell(m_edLct.nItem, m_edLct.nSubItem);
		}

		LVHITTESTINFO pItem;
		pItem.pt = point;
		int test = SubItemHitTest(&pItem);

		int nCol,nRow;
		nRow = pItem.iItem;
		nCol = pItem.iSubItem;

		if(nRow != -1 && nCol != -1)
			UseEdit(nRow, nCol);
		else
			m_edit.ShowWindow(FALSE);
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}
