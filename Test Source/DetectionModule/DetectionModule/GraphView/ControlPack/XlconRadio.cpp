#include "StdAfx.h"
#include "XlconRadio.h"
#include <atlstr.h>

CIconRadio::CIconRadio(void)
{
	m_nMode = 0;
	//Create(NULL,"", WS_OVERLAPPEDWINDOW , CRect(10,10,100,100),this,11);
	
}

CIconRadio::~CIconRadio(void)
{
	DestroyWindow();
}

void CIconRadio::AddCheckBox(CXIcon* checkbox)
{
	if(m_hWnd == NULL)
	{
		Create(_T("STATIC"), _T("DEMO"), 
			WS_CHILD | /*WS_VISIBLE | */WS_BORDER ,
			CRect(30, 30, 180, 180), checkbox->GetParent(), 1234);
	}	
	
	m_vecCheckBox.push_back(checkbox);
	checkbox->SetUpdateStateNotify(GetSafeHwnd());
}


int CIconRadio::GetCheckedIndex()
{
	int i=0, size = (int)m_vecCheckBox.size();

	for(int i=0; i<size; i++)
	{
		if(m_vecCheckBox[i]->GetCheck())
		{
			return i;
		}
		
	}

	return -1;
}

void CIconRadio::SetMode( int nMode )
{
	m_nMode = nMode;
}

BOOL CIconRadio::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == 1010)
	{
		int nDlgCtrlID = pMsg->wParam;
		int nCheck = pMsg->lParam;
		
		for(int i=0; i<m_vecCheckBox.size(); i++)
		{
			if(m_vecCheckBox[i]->GetDlgCtrlID() != nDlgCtrlID)
			{
				m_vecCheckBox[i]->SetCheck(FALSE);
			}
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CIconRadio::IsCheckAllFalse()
{
	BOOL bAllFalse = TRUE;
	for(int i=0; i<m_vecCheckBox.size(); i++)
	{
		if(m_vecCheckBox[i]->GetCheck() == TRUE)
		{
			bAllFalse = FALSE;
			break;
		}
	}
	return bAllFalse;
}
