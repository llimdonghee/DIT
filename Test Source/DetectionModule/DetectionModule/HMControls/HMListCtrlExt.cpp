// HMListCtrlExt.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "HMListCtrlExt.h"

#include "SortClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMListCtrlExt

CHMListCtrlExt::CHMListCtrlExt()
{
	m_nColNum = 0;
	m_nDisableRow = -1;
	m_nDisableCol = -1;
	m_nEditDisableState = EDITDISABLE_NONE;

	m_pEdit = NULL;
	m_pComboBox = NULL;
	m_pEditDisableCols = NULL;
	m_pEditDisableRows = NULL;
	m_strInitEditText = _T("");
	m_nInPlaceCtrl = -1;
	m_bSortOrder = TRUE;
	m_bEnableSortList = TRUE;
	m_nLength = 0;

	m_pComboData = NULL;
	m_nComboDataState = COMBODATASTATE_SINGLE;

	m_pEditCtrlCols = NULL;
	m_pComboCtrlCols = NULL;
	m_nEditCtrlLength = 0;
	m_nComboCtrlLength = 0;

	m_bSetImageList = FALSE;
	m_clrBaseColorText = RGB(0,0,0);
	m_clrBaseColorBack = RGB(255,255,255);
}

void CHMListCtrlExt::SetTextColor(int nRow, COLORREF clrText)
{
	for (int i = 0; i < m_vecColorRow.size(); i++)
	{
		if (m_vecColorRow[i].m_nRow == nRow)
		{
			m_vecColorRow[i].m_clrText = clrText;
			//m_vecColorRow[i].m_clrBack = clrBK;

			return;
		}
	}

	_ListRowColor colornew;
	colornew.m_nRow = nRow;
	colornew.m_nCol = -1;
	colornew.m_clrText = clrText;
	colornew.m_clrBack = m_clrBaseColorBack;
	m_vecColorRow.push_back(colornew);
}

void CHMListCtrlExt::SetBKColor(int nRow, COLORREF clrBK)
{
	for (int i = 0; i < m_vecColorRow.size(); i++)
	{
		if (m_vecColorRow[i].m_nRow == nRow)
		{
			//m_vecColorRow[i].m_clrText = RGB(0, 0, 0);
			m_vecColorRow[i].m_clrBack = clrBK;

			return;
		}
	}

	_ListRowColor colornew;
	colornew.m_nRow = nRow;
	colornew.m_nCol = -1;
	colornew.m_clrText = m_clrBaseColorText;
	colornew.m_clrBack = clrBK;
	m_vecColorRow.push_back(colornew);
}

void CHMListCtrlExt::ReleaseTextColor(int nRow)
{
	for (int i = 0; i < m_vecColorRow.size(); i++)
	{
		if (m_vecColorRow[i].m_nRow == nRow)
		{
			m_vecColorRow[i].m_clrText = m_clrBaseColorText;
			//m_vecColorRow[i].m_clrBack = RGB(255,255,255);
		}
	}
}

void CHMListCtrlExt::ReleaseBKColor(int nRow)
{
	for (int i = 0; i < m_vecColorRow.size(); i++)
	{
		if (m_vecColorRow[i].m_nRow == nRow)
		{
			//m_vecColorRow[i].m_clrText = RGB(0, 0, 0);
			m_vecColorRow[i].m_clrBack = m_clrBaseColorBack;
		}
	}
}

void CHMListCtrlExt::SetCellColor(int nRow, int nCol, COLORREF clrText, COLORREF clrBK)
{
	for (int i = 0; i < m_vecColorCell.size(); i++)
	{
		if (m_vecColorCell[i].m_nRow == nRow
			&& m_vecColorCell[i].m_nCol == nCol)
		{
			m_vecColorCell[i].m_clrText = clrText;
			m_vecColorCell[i].m_clrBack = clrBK;

			return;
		}
	}

	_ListRowColor colornew;
	colornew.m_nRow = nRow;
	colornew.m_nCol = nCol;
	colornew.m_clrText = clrText;
	colornew.m_clrBack = clrBK;
	m_vecColorCell.push_back(colornew);
}

void CHMListCtrlExt::ResetRowColor()
{
	m_vecColorRow.clear();
}

void CHMListCtrlExt::ResetCellColor()
{
	m_vecColorCell.clear();
}

_ListRowColor* CHMListCtrlExt::GetRowColor(int nRow)
{
	int nSize = m_vecColorRow.size();
	for (int i = 0; i < nSize; i++)
	{
		if (m_vecColorRow[i].m_nRow == nRow)
		{
			return &m_vecColorRow[i];
		}
	}
	return NULL;
}

_ListRowColor* CHMListCtrlExt::GetCellColor(int nRow, int nCol)
{
	int nSize = m_vecColorCell.size();
	for (int i = 0; i < nSize; i++)
	{
		if (m_vecColorCell[i].m_nRow == nRow
			&& m_vecColorCell[i].m_nCol == nCol)
		{
			return &m_vecColorCell[i];
		}
	}
	return NULL;
}

CHMListCtrlExt::~CHMListCtrlExt()
{
	if(m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	if(m_pComboBox)
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if(m_pEditDisableCols)
	{
		delete m_pEditDisableCols;
		m_pEditDisableCols = NULL;
	}
	
	if(m_pEditDisableRows)
	{
		delete m_pEditDisableRows;
		m_pEditDisableRows = NULL;
	}

	if(m_pComboData)
	{
		m_pComboData->RemoveAll();
		delete m_pComboData;
		m_pComboData = NULL;
	}

	if(m_pEditCtrlCols)
	{
		delete m_pEditCtrlCols;
		m_pEditCtrlCols = NULL;
	}

	if(m_pComboCtrlCols)
	{
		delete m_pComboCtrlCols;
		m_pComboCtrlCols = NULL;
	}

}


BEGIN_MESSAGE_MAP(CHMListCtrlExt, CListCtrl)
	//{{AFX_MSG_MAP(CHMListCtrlExt)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMListCtrlExt message handlers

void CHMListCtrlExt::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnLButtonDown(nFlags, point);

//	AddBlankItem();

	LVHITTESTINFO pItem;
	pItem.pt = point;
	int test = SubItemHitTest(&pItem);

	int nCol,nRow;
	nRow = pItem.iItem;
	nCol = pItem.iSubItem;

	if(nRow != -1)
	{
		CRect rect;
		if(nCol == 0)
		{
			GetItemRect(nRow,rect,LVIR_BOUNDS);
			rect.right = GetColumnWidth(nCol);
		}
		else
			GetSubItemRect(nRow,nCol,LVIR_BOUNDS,rect);

		// 제외된 행,열 체크 //////////////////////////////////////////////////////
		if(m_nEditDisableState == EDITDISABLE_ROWS || m_nEditDisableState == EDITDISABLE_COLS)
		{
			if(m_nDisableRow == nRow)
				return;
			if(m_nDisableCol == nCol)
				return;
		}
		if(m_nEditDisableState == EDITDISABLE_ROWCOL)
		{
			if(m_nDisableRow == nRow && m_nDisableCol == nCol)
				return;
		}

		if(m_nEditDisableState == EDITDISABLE_ROWS_ARRAY)
		{
			if(m_pEditDisableRows)
			{
				for(int i=0;i<m_nLength;i++)
				{
					if(m_pEditDisableRows[i] == nRow)
						return;
				}
			}
		}
		if(m_nEditDisableState == EDITDISABLE_COLS_ARRAY)
		{
			if(m_pEditDisableCols)
			{
				for(int i=0;i<m_nLength;i++)
				{
					if(m_pEditDisableCols[i] == nCol)
						return;
				}
			}
		}
		// End.. //////////////////////////////////////////////////////////////////

		if(m_nInPlaceCtrl == INPLACECTRL_EDIT)
		{
			if(m_pEdit)
			{
				delete m_pEdit;
				m_pEdit = NULL;
			}

			m_pEdit = new CInPlaceEdit(nRow,nCol,m_strInitEditText);
			m_pEdit->Create(WS_BORDER | WS_CHILD | WS_VISIBLE,rect,this,10000);
			m_pEdit->ShowWindow(SW_SHOW);
		}

		
		if(m_nInPlaceCtrl == INPLACECTRL_COMBO)
		{
			if(m_pComboBox)
			{
				delete m_pComboBox;
				m_pComboBox = NULL;
			}

			m_pComboBox = new CInPlaceComboBox(nRow,nCol);
			rect.bottom = rect.bottom + 200;
			m_pComboBox->Create(WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,rect,this,10001);
//						DWORD dwStyle = m_pComboBox->GetStyle();
//						dwStyle &= ~CBS_SORT;
//						SetWindowLong(m_pComboBox->m_hWnd, GWL_STYLE, dwStyle);
			POSITION pos = m_pComboData->GetHeadPosition();
			while(pos)
			{
				m_pComboBox->AddString(m_pComboData->GetNext(pos));
			}

			m_pComboBox->ShowDropDown();
			m_pComboBox->SetCurSel(0);
			
			m_pComboBox->ShowWindow(SW_SHOW);
		}

		if(m_nInPlaceCtrl == INPLACECTRL_EDIT_COMBO)
		{
			if(m_pEditCtrlCols)
			{
				for(int i=0;i<m_nEditCtrlLength;i++)
				{
					if(nCol == m_pEditCtrlCols[i])
					{
						if(m_pEdit)
						{
							delete m_pEdit;
							m_pEdit = NULL;
						}

						m_pEdit = new CInPlaceEdit(nRow,nCol,m_strInitEditText);
						m_pEdit->Create(WS_BORDER | WS_CHILD | WS_VISIBLE,rect,this,10000);
						m_pEdit->ShowWindow(SW_SHOW);
					}
				}
			}
			
			if(m_pComboCtrlCols)
			{
				for(int j=0;j<m_nComboCtrlLength;j++)
				{			
					if(nCol == m_pComboCtrlCols[j])
					{
						if(m_pComboBox)
						{
							delete m_pComboBox;
							m_pComboBox = NULL;
						}

						m_pComboBox = new CInPlaceComboBox(nRow,nCol);
						rect.bottom = rect.bottom + 200;
						m_pComboBox->Create(WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,rect,this,10001);
//						DWORD dwStyle = m_pComboBox->GetStyle();
//						dwStyle &= ~CBS_SORT;
//						SetWindowLong(m_pComboBox->m_hWnd, GWL_STYLE, dwStyle);

						if(!m_pComboData)
							return;

						if(m_nComboDataState == COMBODATASTATE_SINGLE)
						{
							POSITION pos = m_pComboData->GetHeadPosition();
							while(pos)
							{
								m_pComboBox->AddString(m_pComboData->GetNext(pos));
							}
						}
						
						if(m_nComboDataState == COMBODATASTATE_MULTI)
						{
							POSITION pos = m_pComboData->FindIndex(j);

							if(!pos)
								return;
							
							CString strData = m_pComboData->GetAt(pos);
							CString strToken;

							for(int k=0;k<strData.GetLength();k++)
							{
								char ch = strData.GetAt(k);

								if(ch == ',')
								{
									m_pComboBox->AddString((LPSTR)(LPCSTR)strToken);
									strToken.Empty();
								}
								else
									strToken += ch;
							}
							m_pComboBox->AddString((LPSTR)(LPCSTR)strToken);
						}

						m_pComboBox->ShowDropDown();
						m_pComboBox->SetCurSel(0);

						m_pComboBox->ShowWindow(SW_SHOW);					
					}
				}
			}
		}
	}
}


void CHMListCtrlExt::AddBlankItem()
{
	int nRow;
	nRow = GetCountPerPage();

	if(GetItemCount()<nRow)
	{
		for(int i=0;i<nRow;i++)
		{
			for(int j=0;j<m_nColNum;j++)
			{
				if(GetItemText(i,j) == "")
					InsertListItem(i,j,"");
			}
		}
	}
}

void CHMListCtrlExt::SetColNum(int nColNum)
{
	m_nColNum = nColNum;
}

CImageList* CHMListCtrlExt::SetImageList(CImageList* pImageList, int nImageListType)
{
	if (pImageList)
	{
		m_bSetImageList = TRUE;
		return CListCtrl::SetImageList(pImageList, nImageListType);
	}

	return NULL;
}

void CHMListCtrlExt::InsertListItem(int nRow, int nCol,LPSTR lpString)
{
	LV_ITEM item;

	item.mask = LVIF_TEXT;
	item.iItem = nRow;
	item.iSubItem = nCol;
	item.pszText = lpString;
	InsertItem(&item);
		
	item.iSubItem = nCol;
	item.pszText = lpString;
	SetItem(&item);
}

void CHMListCtrlExt::InsertListItem(int nRow, int nCol, int nImageIdx, LPSTR lpString)
{
	if (m_bSetImageList)
	{
		LV_ITEM item;

		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.iItem = nRow;
		item.iSubItem = nCol;
		item.iImage = nImageIdx;
		item.pszText = lpString;
		InsertItem(&item);

		item.iSubItem = nCol;
		item.pszText = lpString;
		SetItem(&item);
	}
	else
		InsertListItem(nRow, nCol, lpString);
}

void CHMListCtrlExt::InsertListItemEx(int nRow, int nCol, const CString& strString)
{
	LV_ITEM item;
	
	item.mask = LVIF_TEXT;
	item.iItem = nRow;
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strString;
	InsertItem(&item);
	
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strString;
	SetItem(&item);
}

void CHMListCtrlExt::InsertListItemEx(int nRow, int nCol, int nValue)
{
	CString strValue = _T("");
	strValue.Format("%d", nValue);
	
	LV_ITEM item;
	
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.iItem = nRow;
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strValue;
	InsertItem(&item);
	
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strValue;
	SetItem(&item);
}

void CHMListCtrlExt::InsertListItemEx(int nRow, int nCol, double dValue, int nRight)
{
	CString strValue = _T("");
	strValue.Format("%f", dValue);
	int nPoint = strValue.ReverseFind('.');
	if (nPoint > 0 && nPoint + 1 < strValue.GetLength() - nRight)
		strValue = strValue.Left(nPoint + 4);
	
	LV_ITEM item;
	
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.iItem = nRow;
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strValue;
	InsertItem(&item);
	
	item.iSubItem = nCol;
	item.pszText = (LPSTR)(LPCTSTR)strValue;
	SetItem(&item);
}

void CHMListCtrlExt::InsertListItem(int nRow, int nCol, int nImageIdx)
{
	if (m_bSetImageList)
	{
		LV_ITEM item;

		item.mask = LVIF_TEXT | LVIF_IMAGE;
		item.iItem = nRow;
		item.iImage = nImageIdx;

		item.iSubItem = nCol;
		item.pszText = "";
		SetItem(&item);
	}
	else
		InsertListItem(nRow, nCol, "");
}

void CHMListCtrlExt::EditDisableRows(int nRows)
{
	SetEditDisableState(EDITDISABLE_NONE);
	m_nDisableRow = nRows;
	m_nEditDisableState = EDITDISABLE_ROWS;
}

void CHMListCtrlExt::EditDisableCols(int nCols)
{
	SetEditDisableState(EDITDISABLE_NONE);
	m_nDisableCol = nCols;
	m_nEditDisableState = EDITDISABLE_COLS;
}

void CHMListCtrlExt::EditDisableRowCol(int nRow, int nCol)
{
	SetEditDisableState(EDITDISABLE_NONE);
	m_nDisableRow = nRow;
	m_nDisableCol = nCol;
	m_nEditDisableState = EDITDISABLE_ROWCOL;
}

void CHMListCtrlExt::EditDisableCols(CString strCols,int nLength)
{
	if(m_pEditDisableCols)
	{
		delete m_pEditDisableCols;
		m_pEditDisableCols = NULL;
	}
	
	m_pEditDisableCols = new int[nLength];
	m_nLength = nLength;
	int nCol = 0;

	CString strToken;
	for(int i=0;i<strCols.GetLength();i++)
	{
		char ch = strCols.GetAt(i);

		if(ch == ',')
		{
			m_pEditDisableCols[nCol++] = atoi(strToken);
			strToken.Empty();
		}
		else
			strToken += ch;
	}
	m_pEditDisableCols[nLength-1] = atoi(strToken);
	
	m_nEditDisableState = EDITDISABLE_COLS_ARRAY;
}

void CHMListCtrlExt::EditDisableRows(CString strRows,int nLength)
{
	if(m_pEditDisableRows)
	{
		delete m_pEditDisableRows;
		m_pEditDisableRows = NULL;
	}
	
	m_pEditDisableRows = new int[nLength];
	m_nLength = nLength;
	int nRow = 0;

	CString strToken;
	for(int i=0;i<strRows.GetLength();i++)
	{
		char ch = strRows.GetAt(i);

		if(ch == ',')
		{
			m_pEditDisableRows[nRow++] = atoi(strToken);
			strToken.Empty();
		}
		else
			strToken += ch;
	}
	m_pEditDisableRows[nLength-1] = atoi(strToken);
	
	m_nEditDisableState = EDITDISABLE_ROWS_ARRAY;
}

int CHMListCtrlExt::GetEditDisableState()
{
	return m_nEditDisableState;  
}

int CHMListCtrlExt::GetEditDisableRows()
{
	return m_nDisableRow;
}

int CHMListCtrlExt::GetEditDisableCols()
{
	return m_nDisableCol;
}

void CHMListCtrlExt::SetEditDisableState(int nState)
{
	if(EDITDISABLE_NONE)
	{
		m_nDisableRow = -1;
		m_nDisableCol = -1;
		m_nEditDisableState = EDITDISABLE_NONE;
	}
}

void CHMListCtrlExt::SetDefaultEditText(CString strInitText)
{
	m_strInitEditText = strInitText;
}

void CHMListCtrlExt::SetInPlaceCtrl(int nCtrl)
{
	if(m_pEditCtrlCols)
	{
		delete m_pEditCtrlCols;
		m_pEditCtrlCols = NULL;
	}

	if(m_pComboCtrlCols)
	{
		delete m_pComboCtrlCols;
		m_pComboCtrlCols = NULL;
	}

	m_nInPlaceCtrl = nCtrl;
}

void CHMListCtrlExt::InitListHeader(char *colList[], int *widthList, int nLength, int nHeight)
{
	LV_COLUMN lvcolumn;

	for(int i=0; i < nLength; i++) 
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText= colList[i];
		lvcolumn.iSubItem= i;
		lvcolumn.cx= widthList[i];
		InsertColumn(i, &lvcolumn);
	}
	
	if (!m_bSetImageList)
	{
		// 리스트의 row 높이 조절
		CImageList m_image;
		m_image.Create(1, nHeight, ILC_COLORDDB, 1, 0); //2번째 파라미터로 높이조절.....
		SetImageList(&m_image, LVSIL_SMALL);
	}
 
	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}

void CHMListCtrlExt::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) 
		SetFocus();
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CHMListCtrlExt::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus() != this) 
		SetFocus();
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHMListCtrlExt::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(m_bEnableSortList)
		SortListCtrl(pNMListView->iSubItem);
	
	*pResult = 0;
}

void CHMListCtrlExt::SortListCtrl(int nCol)
{
	CSortClass csc(this, nCol , 0);

	if(m_bSortOrder)
		m_bSortOrder = FALSE;
	else
		m_bSortOrder = TRUE;

	    
	csc.Sort(m_bSortOrder?1:0);
}

void CHMListCtrlExt::SetEnableSortList(BOOL nEnable)
{
	m_bEnableSortList = nEnable;
}


BOOL CHMListCtrlExt::GetEnableSortList()
{
	return m_bEnableSortList;
}


void CHMListCtrlExt::SetComboData(char *pData[], int nState , int nLength)
{
	if(m_nInPlaceCtrl == INPLACECTRL_EDIT)
		return;

	if(m_pComboData)
	{
		m_pComboData->RemoveAll();
		delete m_pComboData;
		m_pComboData = NULL;
	}

	m_pComboData = new CStringList;

	for(int i=0;i<nLength;i++)
	{
		m_pComboData->AddTail((LPSTR)(LPCSTR)pData[i]);
	}
	m_nComboDataState = nState;
}

void CHMListCtrlExt::SetComboData(CStringList* pList, int nState)
{
	if(m_nInPlaceCtrl == INPLACECTRL_EDIT)
		return;

	if(m_pComboData)
	{
		m_pComboData->RemoveAll();
		delete m_pComboData;
		m_pComboData = NULL;
	}

	m_pComboData = new CStringList;

	POSITION pos = pList->GetHeadPosition();

	while(pos)
	{
		m_pComboData->AddTail(pList->GetNext(pos));
	}

	m_nComboDataState = nState;
}

void CHMListCtrlExt::SetEditCtrlCols(CString strCols, int nLength)
{
	if(m_pEditCtrlCols)
	{
		delete m_pEditCtrlCols;
		m_pEditCtrlCols = NULL;
	}
	
	m_pEditCtrlCols = new int[nLength];
	m_nEditCtrlLength = nLength;
	int nCol = 0;

	CString strToken;
	for(int i=0;i<strCols.GetLength();i++)
	{
		char ch = strCols.GetAt(i);

		if(ch == ',')
		{
			m_pEditCtrlCols[nCol++] = atoi(strToken);
			strToken.Empty();
		}
		else
			strToken += ch;
	}
	m_pEditCtrlCols[nLength-1] = atoi(strToken);

	m_nInPlaceCtrl = INPLACECTRL_EDIT_COMBO;
}

void CHMListCtrlExt::SetComboCtrlCols(CString strCols, int nLength)
{
	if(m_pComboCtrlCols)
	{
		delete m_pComboCtrlCols;
		m_pComboCtrlCols = NULL;
	}
	
	m_pComboCtrlCols = new int[nLength];
	m_nComboCtrlLength = nLength;
	int nCol = 0;

	CString strToken;
	for(int i=0;i<strCols.GetLength();i++)
	{
		char ch = strCols.GetAt(i);

		if(ch == ',')
		{
			m_pComboCtrlCols[nCol++] = atoi(strToken);
			strToken.Empty();
		}
		else
			strToken += ch;
	}
	m_pComboCtrlCols[nLength-1] = atoi(strToken);

	m_nInPlaceCtrl = INPLACECTRL_EDIT_COMBO;
}

void CHMListCtrlExt::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);	// 각 라인 설정

		_ListRowColor* pLRC = NULL;
		pLVCD->clrText = m_clrBaseColorText;
		pLVCD->clrTextBk = m_clrBaseColorBack;
		// 글자색 바꾸기
		pLRC = GetRowColor(nItem);
		if (pLRC)
		{
			pLVCD->clrText = pLRC->m_clrText;
			pLVCD->clrTextBk = pLRC->m_clrBack;
		}
			
		// Tell Windows to paint the control itself;
		//*pResult = CDRF_DODEFAULT;
		*pResult = (LRESULT)CDRF_NOTIFYSUBITEMDRAW;//sub-item 을 변경하기 위해서. 
	}	
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT)) 
	{
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		_ListRowColor* pLRC = NULL;

		// 글자색 바꾸기
		pLRC = GetCellColor(nItem, pLVCD->iSubItem);
		if(pLRC == NULL) pLRC = GetRowColor(nItem);

		if (pLRC)
		{
			pLVCD->clrText = pLRC->m_clrText;
			pLVCD->clrTextBk = pLRC->m_clrBack;
		}
		else
		{
			pLVCD->clrText = m_clrBaseColorText;
			pLVCD->clrTextBk = m_clrBaseColorBack;
		}

		*pResult = (LRESULT)CDRF_NEWFONT; // 이렇게 해야 설정한 대로 그려진다.
		return;
	}
}


void CHMListCtrlExt::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CHeaderCtrl* pHeader = NULL;
	pHeader = GetHeaderCtrl();
	if (pHeader != NULL)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd)); // m_HeaderCtrl is the new wrapper object
	}

	CListCtrl::PreSubclassWindow();
}


CColorHeader::CColorHeader()
{
	m_clrBk = RGB(255, 255, 204);
	m_clrBorder = RGB(204,204,204);
	m_clrText = RGB(0,0,0);

	m_clrBk = RGB(89,89,89);
	m_clrBorder = RGB(130,130,130);
	m_clrText = RGB(255,255,255);
}

CColorHeader::~CColorHeader()
{
}


BEGIN_MESSAGE_MAP(CColorHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(CColorHeader)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorHeader message handlers

BOOL CColorHeader::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}

void CColorHeader::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//for this notification, the structure is actually a
	// NMLVCUSTOMDRAW that tells you what's going on with the custom
	// draw action. So, we'll need to cast the generic pNMHDR pointer.
	LPNMCUSTOMDRAW lplvcd = (LPNMCUSTOMDRAW)pNMHDR;

	switch(lplvcd->dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			CRect rcItem(lplvcd->rc);
			CDC *pDC= CDC::FromHandle(lplvcd->hdc);

			pDC->FillSolidRect(&rcItem, m_clrBk);
		}
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		{
			CRect rcItem(lplvcd->rc);
			CDC *pDC= CDC::FromHandle(lplvcd->hdc);
			int nItemID = lplvcd->dwItemSpec;
			TCHAR buf[256];
			HD_ITEM hdi;
			CPen penBorder;

			hdi.mask = HDI_TEXT | HDI_FORMAT;
			hdi.pszText = buf;
			hdi.cchTextMax = 255;

			GetItem( nItemID, &hdi );

			UINT uFormat = DT_SINGLELINE | DT_VCENTER |
				((hdi.fmt & HDF_LEFT) ? DT_LEFT :
				(hdi.fmt & HDF_RIGHT) ? DT_RIGHT : DT_CENTER);

			pDC->FillSolidRect(rcItem, m_clrBk);

			// 테두리 그리기
			penBorder.CreatePen(PS_SOLID, 1, m_clrBorder);
			CPen *pOldPen = pDC->SelectObject(&penBorder);
			pDC->MoveTo(rcItem.left, rcItem.bottom);
			pDC->LineTo(rcItem.left, rcItem.top);
			pDC->LineTo(rcItem.right, rcItem.top);

			// 텍스트 그리기
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_clrText);
			pDC->SelectObject(m_pFont);
			pDC->DrawText(hdi.pszText, &rcItem, uFormat);

			pDC->SelectObject(pOldPen);
			penBorder.DeleteObject();
			*pResult= CDRF_SKIPDEFAULT;
		}
		break;
	default:// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
}

void CColorHeader::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CHeaderCtrl::PreSubclassWindow();

 	m_pFont = GetFont();
// 	CFont NewFont;
// 	NewFont.CreatePointFont(120,"굴림");
// 	SetFont(&NewFont);
}