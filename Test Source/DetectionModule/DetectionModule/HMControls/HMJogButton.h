#if !defined(AFX_HMJOGBUTTON_H__51BB375B_98EA_4921_B964_90AC2D952D70__INCLUDED_)
#define AFX_HMJOGBUTTON_H__51BB375B_98EA_4921_B964_90AC2D952D70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMJogButton.h : header file
//

#include "HMJogBtn2Parent.h"

/////////////////////////////////////////////////////////////////////////////
// CHMJogButton window

class AFX_EXT_CLASS CHMJogButton : public CButton
{
// Construction
public:
	CHMJogButton();

// Attributes
public:

// Operations
public:
	void SetJ2M(pHMJogBtn2Parent pJ2M)			{ m_pJ2M = pJ2M; }
	void SetStringDown(CString strMsg)			{ m_strMessageDown = strMsg; }
	void SetStringUp(CString strMsg)			{ m_strMessageUp = strMsg; }
	void SetIntDown(int nMsg)					{ m_nMessageDown = nMsg; }
	void SetIntUp(int nMsg)						{ m_nMessageUp = nMsg; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMJogButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHMJogButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMJogButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	pHMJogBtn2Parent	m_pJ2M;
	CString				m_strMessageDown;
	CString				m_strMessageUp;
	int					m_nMessageDown;
	int					m_nMessageUp;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOGBUTTON_H__51BB375B_98EA_4921_B964_90AC2D952D70__INCLUDED_)
