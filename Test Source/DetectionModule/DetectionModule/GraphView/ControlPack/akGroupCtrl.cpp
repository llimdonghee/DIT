#include "StdAfx.h"
#include "akGroupCtrl.h"
#include "akStaticGroup.h"
#include "akGroupDraw.h"

CakGroupCtrl::CakGroupCtrl(void)
{

}

CakGroupCtrl::~CakGroupCtrl(void)
{
	clear();
}


void CakGroupCtrl::clear()
{
	std::map<int,void*>::iterator itor;
	for(itor = m_mapControls.begin(); itor!=m_mapControls.end(); itor++)
	{
		CakStaticGroup* pControl = (CakStaticGroup*)itor->second;
		pControl->DestroyWindow();
		delete pControl;
	}

	m_mapControls.clear();
}

void CakGroupCtrl::addControl( int nIDCtl, CWnd* pParent )
{
	_GroupCtrlOption* pOption = &m_CtrlOption;
	
	if(pOption->nStyle == 0)
	{
		CakStaticGroup* pControl;

		std::map<int,void*>::iterator itor = m_mapControls.find(nIDCtl);
		if(itor != m_mapControls.end())
		{
			pControl = (CakStaticGroup*)m_mapControls[nIDCtl];
		}
		else
		{
			pControl = new CakStaticGroup;
			pControl->SubclassDlgItem(nIDCtl, pParent);
		}
		
		pControl->setImage(pOption->nResourceID);
		pControl->setTransparency(true, pOption->clrTransparency);
		pControl->m_rectMargin = pOption->rectMargin;
		char *pFontName = NULL;
		if(pOption->strFontName.IsEmpty() == FALSE) pFontName = pOption->strFontName.GetBuffer();
		pControl->setFont(pFontName, pOption->nFontSize);
		pControl->m_nTitlePosX = pOption->nTextPosX;
		pControl->m_nTitlePosY = pOption->nTextPosY;

		m_mapControls[nIDCtl] = (void*)pControl;
		//m_mapControls.push_back((void*)pControl);
	}
	else if(pOption->nStyle == 1)
	{
		CakGroupDraw* pControl;

		std::map<int,void*>::iterator itor = m_mapControls.find(nIDCtl);
		if(itor != m_mapControls.end())
		{
			pControl = (CakGroupDraw*)m_mapControls[nIDCtl];
		}
		else
		{
			pControl = new CakGroupDraw;
			_GroupDrawOption op = pControl->getGroupDrawOption();
			op.nTitleHeight = pOption->nTitleHeight;
			pControl->setGroupDrawOption(op);
			pControl->SubclassDlgItem(nIDCtl, pParent);
		}


		_GroupDrawOption op = pControl->getGroupDrawOption();
		{
			op.nDrawStyle = pOption->nStyleSub;
			
			op.clrTitle = pOption->clrTitle;
			op.clrBack = pOption->clrBack;
			op.clrOutline = pOption->clrOutline;
			op.clrTransparency = pOption->clrTransparency;
			op.clrMark = pOption->clrMark;

			op.bTextEnable = pOption->bTextEnable;
			op.strFontName = pOption->strFontName;
			op.nFontSize = pOption->nFontSize;
			op.nTextPosX = pOption->nTextPosX;
			op.nTextPosY = pOption->nTextPosY;

			op.clrFontNormal = pOption->clrFontNormal;
			op.clrFontPush = pOption->clrFontPush;
			op.clrFontMouseOver = pOption->clrFontMouseOver;
			op.clrFontDisable = pOption->clrFontDisable;

			op.bTextShadow = pOption->bTextShadow;
			op.nTextShadowX = pOption->nTextShadowX;
			op.nTextShadowY = pOption->nTextShadowY;

			op.nTitleHeight = pOption->nTitleHeight;

			for(int i=0; i<16; i++)
			{
				op.clrReserved[i] = pOption->clrReserved[i];
			}

		}
		pControl->setFont(pOption->strFontName.GetBuffer(0), pOption->nFontSize);
		pControl->setGroupDrawOption(op);

		m_mapControls[nIDCtl] = (void*)pControl;
	}
	
}

void CakGroupCtrl::setCtrlOption( _GroupCtrlOption GroupCtrlOption )
{
	if(m_CtrlOption.strFontName != GroupCtrlOption.strFontName
		|| m_CtrlOption.nFontSize != GroupCtrlOption.nFontSize)
	{
		_GroupCtrlOption* pOption = &m_CtrlOption;
		if(pOption->nStyle == 0)
		{
			char *pFontName = NULL;
			if(GroupCtrlOption.strFontName.IsEmpty() == FALSE) pFontName = GroupCtrlOption.strFontName.GetBuffer();

			std::map<int,void*>::iterator itor;
			for(itor = m_mapControls.begin(); itor!=m_mapControls.end(); itor++)
			{
				CakStaticGroup* pControl = (CakStaticGroup*)itor->second;
				pControl->setFont(pFontName, GroupCtrlOption.nFontSize);
			}
			

		}
		
	}

	m_CtrlOption=GroupCtrlOption;
}

CStatic* CakGroupCtrl::GetDlgItem( int nIDCtl )
{
	return (CStatic*)m_mapControls[nIDCtl];
}
