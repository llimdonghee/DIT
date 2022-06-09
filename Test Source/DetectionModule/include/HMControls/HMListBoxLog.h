#if !defined(AFX_HMLISTBOXLOG_H__60501A52_5401_435F_996E_F3EABB095333__INCLUDED_)
#define AFX_HMLISTBOXLOG_H__60501A52_5401_435F_996E_F3EABB095333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMListBoxLog.h : header file
//

#include "HMFolderScheduling.h"

/////////////////////////////////////////////////////////////////////////////
// CHMListBoxLog window

#define LOG_NORMAL		1
#define LOG_HISTORY		2
#define LOG_WARNING		3
#define LOG_ERROR		4
#define LOG_DEBUG		5

#define STRINGBUFFER_COUNT		200
class AFX_EXT_CLASS CHMListBoxLog : public CListBox
{
// Construction
public:
	CHMListBoxLog();
	virtual ~CHMListBoxLog();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMListBoxLog)
	//}}AFX_VIRTUAL

	//////////////////////////////////////////////////////////////////////////
	// Logging
	void	SetPath(CString& strPath, CString& strName, BOOL bDelete, int nRemainCount);
	void	DisplayMessages(BOOL bAddMsgBox, int nLevel, char *str, ...);
	void	DisplayMessages(BOOL bAddMsgBox, char *str, ...);
	void	DisplayMessage(BOOL bAddMsgBox, int nLevel, char *str);
	void	DisplayMessage(BOOL bAddMsgBox, char *str);
	void	SetTextColor(long clbg1 = RGB(255,255,255), long clbg2 = RGB(255,255,255), long clText = RGB(0,0,0));
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	// Generated message map functions
protected:
	//{{AFX_MSG(CHMListBoxLog)
	afx_msg LRESULT OnDisplayMessage(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	// Folder Management
	CHMFolderScheduling	m_FolderSchedule;
		
	//////////////////////////////////////////////////////////////////////////
	// Logging
	CString				m_strLogDir;
	CString				m_strFileName;

	CString				m_strLogPath;
	CString				m_strLogFile;
	CTime				m_TimeLogFile;
	CFile*				m_pFileLog;

	char				m_strBuffer[512];

	BOOL				MakeLogFile();
	BOOL				WriteToFile(CTime& time, CString& strContents);
	BOOL				WriteToFile(CTime& time, char* strContents);

	DWORD				m_dwThreadID;

	CString				m_strArray[STRINGBUFFER_COUNT];
	BOOL				m_bMustDisplay[STRINGBUFFER_COUNT];
	int					m_nAddIndex;
	int					m_nReadIndex;
	CRITICAL_SECTION	m_csLog;

	long				m_clBG1;
	long				m_clBG2;
	long				m_clText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMLISTBOXLOG_H__60501A52_5401_435F_996E_F3EABB095333__INCLUDED_)
