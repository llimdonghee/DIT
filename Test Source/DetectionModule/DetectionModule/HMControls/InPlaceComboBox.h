#if !defined(AFX_INPLACECOMBOBOX_H__2909C5E5_6C2D_42DE_AB56_904A78606462__INCLUDED_)
#define AFX_INPLACECOMBOBOX_H__2909C5E5_6C2D_42DE_AB56_904A78606462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceComboBox window

class CInPlaceComboBox : public CComboBox
{
// Construction
public:
	CInPlaceComboBox(int iItem,int iSubItem);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	int m_iSubItem;
	int m_iItem;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACECOMBOBOX_H__2909C5E5_6C2D_42DE_AB56_904A78606462__INCLUDED_)
