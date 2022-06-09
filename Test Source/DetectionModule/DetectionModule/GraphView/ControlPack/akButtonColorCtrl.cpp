#include "StdAfx.h"
#include "akButtonColorCtrl.h"

CakButtonColorCtrl::CakButtonColorCtrl(void)
{

}

CakButtonColorCtrl::~CakButtonColorCtrl(void)
{
	clear();
}


void CakButtonColorCtrl::clear()
{
	std::map<int,void*>::iterator itor;
	for(itor = m_mapControls.begin(); itor!=m_mapControls.end(); itor++)
	{
		CakButtonColor* pControl = (CakButtonColor*)itor->second;
		pControl->DestroyWindow();
		delete pControl;
	}

	m_mapControls.clear();
}

void CakButtonColorCtrl::addControl( int nIDCtl, CWnd* pParent )
{
	_ColorButtonOption* pOption = &m_CtrlOption;
	
	if(1)
	{
		CakButtonColor* pControl;

		std::map<int,void*>::iterator itor = m_mapControls.find(nIDCtl);
		if(itor != m_mapControls.end())
		{
			pControl = (CakButtonColor*)m_mapControls[nIDCtl];
		}
		else
		{
			pControl = new CakButtonColor;
			pControl->SubclassDlgItem(nIDCtl, pParent);

			
		}
		
		char *pFontName = NULL;
		if (pOption->strFontName.IsEmpty() == FALSE) pFontName = pOption->strFontName.GetBuffer();
		if (pFontName) pControl->setFont(pFontName);

		pControl->setButtonColorOption(m_CtrlOption);

		
		m_mapControls[nIDCtl] = (void*)pControl;
		//m_mapControls.push_back((void*)pControl);
	}
// 	else
// 	{
// 
// 	}
	
}

void CakButtonColorCtrl::setCtrlOption( _ColorButtonOption GroupCtrlOption )
{
	
	_ColorButtonOption* pOption = &m_CtrlOption;

	std::map<int,void*>::iterator itor;
	for(itor = m_mapControls.begin(); itor!=m_mapControls.end(); itor++)
	{
		CakButtonColor* pControl = (CakButtonColor*)itor->second;
		pControl->setButtonColorOption(GroupCtrlOption);
		delete pControl;
	}


	m_CtrlOption=GroupCtrlOption;
}

CButton* CakButtonColorCtrl::GetDlgItem( int nIDCtl )
{
	return (CButton*)m_mapControls[nIDCtl];
}
