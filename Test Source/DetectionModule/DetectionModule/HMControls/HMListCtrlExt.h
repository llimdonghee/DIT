#pragma once

#include <afxtempl.h>
#include <vector>
#include "InPlaceEdit.h"
#include "InPlaceComboBox.h"
#define EDITDISABLE_NONE	0
#define EDITDISABLE_ROWS	1
#define EDITDISABLE_COLS	2
#define EDITDISABLE_ROWCOL	3

#define INPLACECTRL_EDIT	4
#define INPLACECTRL_COMBO	5
#define INPLACECTRL_EDIT_COMBO	6

#define EDITDISABLE_ROWS_ARRAY	7
#define EDITDISABLE_COLS_ARRAY	8

#define COMBODATASTATE_SINGLE 9
#define COMBODATASTATE_MULTI  10

/////////////////////////////////////////////////////////////////////////////
// CHMListCtrlExt window



class CColorHeader : public CHeaderCtrl
{
	// Construction
public:
	CColorHeader();

	// Attributes
public:
	COLORREF m_clrBk;
	COLORREF m_clrBorder;
	COLORREF m_clrText;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorHeader)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:
	CFont *m_pFont;
	virtual ~CColorHeader();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorHeader)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

typedef struct _LISTROWCOLOR
{
	int			s_nRow;
	COLORREF	s_Color;
} ListRowColor, *pListRowColor;
struct _ListRowColor 
{
	int	m_nRow;
	int	m_nCol;
	COLORREF m_clrText;
	COLORREF m_clrBack;
};



class AFX_EXT_CLASS CHMListCtrlExt : public CListCtrl
{
// Construction
public:
	CHMListCtrlExt();
	virtual ~CHMListCtrlExt();

// Attributes
public:

// Operations
public:
	void SetComboCtrlCols(CString strCols,int nLength);
	void SetEditCtrlCols(CString strCols,int nLength);
	void SetComboData(char *pData[],int nState,int nLength);
	void SetComboData(CStringList *pList, int nState);
	void EditDisableRows(CString strRows,int nLength);
	void EditDisableCols(CString strCols,int nLength);
	BOOL GetEnableSortList();
	void SetEnableSortList(BOOL nEnable);
	void SortListCtrl(int nCol);
	void InitListHeader(char *colList[],int *widthList,int nLength, int nHeight);
	void SetInPlaceCtrl(int nCtrl);
	void SetDefaultEditText(CString strInitText);
	void SetEditDisableState(int nState);
	int GetEditDisableCols();
	int GetEditDisableRows();
	int GetEditDisableState();
	void EditDisableRowCol(int nRow,int nCol);
	void EditDisableCols(int nCols);
	void EditDisableRows(int nRows);
	void SetColNum(int nColNum);
	void AddBlankItem();
	CImageList* SetImageList(CImageList* pImageList, int nImageListType);
	
	void InsertListItem(int nRow, int nCol, LPSTR lpString);
	void InsertListItem(int nRow, int nCol, int nImageIdx, LPSTR lpString);
	void InsertListItem(int nRow, int nCol, int nImageIdx);
	
	void InsertListItemEx(int nRow, int nCol, const CString& strString);
	void InsertListItemEx(int nRow, int nCol, int nValue);
	void InsertListItemEx(int nRow, int nCol, double dValue, int nRight = 3);

	void SetTextColor(int nRow, COLORREF clrText);
	void SetBKColor(int nRow, COLORREF clrBK);
	void ReleaseTextColor(int nRow);
	void ReleaseBKColor(int nRow);
	void SetCellColor(int nRow, int nCol, COLORREF clrText, COLORREF clrBK);

	void ResetRowColor();
	void ResetCellColor();

	COLORREF m_clrBaseColorText;
	COLORREF m_clrBaseColorBack;
protected:
	_ListRowColor* GetRowColor(int nRow);
	_ListRowColor* GetCellColor(int nRow, int nCol);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMListCtrlExt)
	//}}AFX_VIRTUAL

protected:
	CInPlaceEdit *m_pEdit;
	CInPlaceComboBox *m_pComboBox;
	int m_nInPlaceCtrl;
	int m_nColNum;
	int m_nEditDisableState;
	int m_nDisableCol;
	int m_nDisableRow;
	BOOL m_bSortOrder;
	CString m_strInitEditText;
	BOOL m_bEnableSortList;
	int *m_pEditDisableCols;
	int *m_pEditDisableRows;
	// Generated message map functions
protected:
	int m_nComboCtrlLength;
	int m_nEditCtrlLength;
	int *m_pComboCtrlCols;
	int *m_pEditCtrlCols;
	CStringList *m_pComboData;
	int m_nLength;
	int m_nComboDataState;
	BOOL m_bSetImageList;

// Implementation
public:
	//CList<pListRowColor, pListRowColor>	m_ListColorText;
	//CList<pListRowColor, pListRowColor>	m_ListColorBk;
	std::vector<_ListRowColor> m_vecColorRow;
	std::vector<_ListRowColor> m_vecColorCell;

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMListCtrlExt)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	CColorHeader m_HeaderCtrl;
};
