#include "StdAfx.h"
#include "akRadio.h"



CakRadio::CakRadio(void)
{
	m_nMode = 0;
	//Create(NULL,"", WS_OVERLAPPEDWINDOW , CRect(10,10,100,100),this,11);
	
}

CakRadio::~CakRadio(void)
{
	DestroyWindow();
}

void CakRadio::AddCheckBox(CakButtonColor* checkbox)
{
	if(m_hWnd == NULL)
	{
		Create(_T("STATIC"), _T("DEMO"), 
			WS_CHILD | /*WS_VISIBLE | */WS_BORDER ,
			CRect(30, 30, 180, 180), checkbox->GetParent(), 1234);
	}
	if(0)
	{
		if(m_vecCheckBox.empty()) 
			checkbox->SetCheck(true);
		else
			checkbox->SetCheck(false);
	}
	

	m_vecCheckBox.push_back(checkbox);
	checkbox->SetUpdateStateNotify(GetSafeHwnd());
}


int CakRadio::GetCheckedIndex()
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

void CakRadio::SetMode( int nMode )
{
	m_nMode = nMode;
}
BOOL CakRadio::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

BOOL CakRadio::IsCheckAllFalse()
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
