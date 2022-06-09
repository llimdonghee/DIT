#include "StdAfx.h"
#include "akStaticTextCtrl.h"
#include "akStaticText.h"

CakStaticTextCtrl::CakStaticTextCtrl(void)
{

}

CakStaticTextCtrl::~CakStaticTextCtrl(void)
{
	clear();
}


void CakStaticTextCtrl::clear()
{
	std::map<int,void*>::iterator itor;
	for(itor = m_mapControls.begin(); itor!=m_mapControls.end(); itor++)
	{
		CakStaticText* pControl = (CakStaticText*)itor->second;
		pControl->DestroyWindow();
		delete pControl;
	}

	m_mapControls.clear();
}

void CakStaticTextCtrl::addControl( int nIDCtl, CWnd* pParent )
{
	_StaticTextCtrlOption* pOption = &m_CtrlOption;
	
	if(pOption->nStyle == 0)
	{
		
	}
	else if(pOption->nStyle == 1)
	{
		CakStaticText* pControl;

		std::map<int,void*>::iterator itor = m_mapControls.find(nIDCtl);
		if(itor != m_mapControls.end())
		{
			pControl = (CakStaticText*)m_mapControls[nIDCtl];
		}
		else
		{
			pControl = new CakStaticText;
			pControl->SubclassDlgItem(nIDCtl, pParent);
		}


		char *pFontName = NULL;
		if(pOption->strFontName.IsEmpty() == FALSE) pFontName = pOption->strFontName.GetBuffer();
		if(pFontName) pControl->setFont(pFontName);
		pControl->setFontSize(pOption->nFontSize);
		
		//pControl->SetFastMode();
		pControl->setColor(pOption->clrTitle,pOption->clrBack);

		pControl->SetAlign(pOption->nTextAlign);

		pControl->setOutLine(pOption->nDrawOutlineType, pOption->clrOutline);
		m_mapControls[nIDCtl] = (void*)pControl;
		//m_mapControls.push_back((void*)pControl);
	}
	
}

void CakStaticTextCtrl::setCtrlOption( _StaticTextCtrlOption GroupCtrlOption )
{
	//if (m_CtrlOption.strFontName != GroupCtrlOption.strFontName
	//	|| m_CtrlOption.nFontSize != GroupCtrlOption.nFontSize)
	{
		_StaticTextCtrlOption* pOption = &GroupCtrlOption;

		if (pOption->nStyle == 1)
		{
			std::map<int, void*>::iterator itor;
			for (itor = m_mapControls.begin(); itor != m_mapControls.end(); itor++)
			{
				CakStaticText* pControl = (CakStaticText*)itor->second;

				char *pFontName = NULL;
				if (pOption->strFontName.IsEmpty() == FALSE) pFontName = pOption->strFontName.GetBuffer();
				if (pFontName) pControl->setFont(pFontName);
				pControl->setFontSize(pOption->nFontSize);

				pControl->SetFastMode();
				pControl->setColor(pOption->clrTitle, pOption->clrBack);
				//pControl->setTextColor(RGB(255,255,255));

				pControl->SetAlign(pOption->nTextAlign);

				pControl->setOutLine(pOption->nDrawOutlineType, pOption->clrOutline);
			}
		}
 	}

	m_CtrlOption=GroupCtrlOption;
}

CStatic* CakStaticTextCtrl::GetDlgItem( int nIDCtl )
{
	return (CStatic*)m_mapControls[nIDCtl];
}
