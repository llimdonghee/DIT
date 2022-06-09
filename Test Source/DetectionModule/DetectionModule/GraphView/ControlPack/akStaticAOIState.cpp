#include "StdAfx.h"
#include "akStaticAOIState.h"
#include "akMemDC.h"

#include <process.h>

BEGIN_MESSAGE_MAP(CakStaticAOIState, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


CakStaticAOIState::CakStaticAOIState(void)
{
	m_clrStateNormal = RGB(66,160,255);
	m_clrStateRun1 = RGB(253,231,60);
	m_clrStateRun2 = RGB(0,250,0);
	m_clrStateOK = RGB(0,180,0);
	m_clrStateOutLine = RGB(105,105,105);

	m_clrTextColor1 = RGB(0,0,0);
	m_clrTextColor2 = RGB(255,255,255);



	memset(m_bScanEnd, 0, sizeof(bool)*MAX_CAMNUM);
	for(int i=0; i<MAX_SCANNUM; i++)
	{
		m_nScanStartTime[i] = 0;
		m_nScanEndTime[i] = 0;
		m_nScanEndCamNum[i] = 0;
	}

	m_bBlinkSwitch = 0;m_nBlinkCounter=0;
	m_nAOIStateStep = AS_None;
	m_nCurScanIndex = 0;
	m_nScanNum = 3;
	m_nCamNum = 0;
	m_nMaxDisplayScanNum = 3;


	m_nDrawMode = 0;
}

CakStaticAOIState::~CakStaticAOIState(void)
{

}



void CakStaticAOIState::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	//memdc.SelectObject(pOldFont);
}


BOOL CakStaticAOIState::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return true;
	return CStatic::OnEraseBkgnd(pDC);
}

// 
// void CakStaticAOIState::setFont(char* fontName)
// {
// 	LOGFONT lf;
// 	CFont* pfont = GetFont();
// 	pfont->GetLogFont(&lf);
// 
// 	strcpy(lf.lfFaceName, fontName); // font setting 
// 
// 	m_font.DeleteObject();
// 	m_font.CreateFontIndirect(&lf); 
// 
// 	SetFont(&m_font);
// }
// void CakStaticAOIState::setFontSize(int size)
// {
// 	LOGFONT lf;
// 	CFont* pfont = GetFont();
// 	pfont->GetLogFont(&lf);
// 
// 	lf.lfHeight = -size;
// 
// 	m_font.DeleteObject();
// 	m_font.CreateFontIndirect(&lf); 
// 
// 	SetFont(&m_font);
// }
// 

void CakStaticAOIState::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ModifyStyle(NULL, WS_CLIPSIBLINGS | SS_OWNERDRAW ,		0);

	makeFont(&m_fntGlassLoading, "Georgia", 16);
	makeFont(&m_fntScanReady, "Georgia", 16);
	makeFont(&m_fntScanStart, "Georgia", 16);
	makeFont(&m_fntScanStartSub, "Calibri", 11);
	makeFont(&m_fntInspectionEnd, "Georgia", 16);

	makeFont(&m_fntDisplayMessage, "Cambria", 28);
	CStatic::PreSubclassWindow();
}

void CakStaticAOIState::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
	CRect rect;
	GetClientRect(&rect);

	_AOIStep AOIStep = m_nAOIStateStep;





	CakMemDC memdc(&dc); 

	CFont* pOldFont = (CFont*)memdc.SelectObject(GetFont());
	CDC* pDC = &memdc;
	pDC->SetBkMode(TRANSPARENT);

	//draw Back 태현[2016/9/23]
	{
		memdc.FillSolidRect(rect, m_clrStateNormal);
	}

	//draw outline
	{
		CRect rectDraw = rect;
		memdc.Draw3dRect(&rectDraw,RGB(100,100,100), RGB(100,100,100)); 
		rectDraw.DeflateRect(1,1,1,1);
		memdc.Draw3dRect(&rectDraw,RGB(255,255,255), RGB(255,255,255)); 
	}

	rect.DeflateRect(2,2,2,2);
	CRect rectGlassLoading=rect;
	CRect rectInspectionEnd=rect;
	CRect rectScanReady=rect;
	CRect rectScanStart=rect;

	//영역계산 태현[2016/9/24]
	{
		CRect rectDraw = rect;
		rectDraw.DeflateRect(2,2,2,2);

		rectGlassLoading.right = rectDraw.left + 100;
		rectInspectionEnd.left = rectDraw.right - 100;

		rectScanReady.left = rectGlassLoading.right;
		rectScanReady.right = rectGlassLoading.right+70;

		rectScanStart.left = rectScanReady.right;
		rectScanStart.right = rectInspectionEnd.left;

	}


	//
	if(AOIStep < AS_DisplayMessage)
	{
		if(m_nDrawMode == 1) drawScanStartVert(pDC, rectScanStart, AOIStep);
		else drawScanStart(pDC, rectScanStart, AOIStep);

		drawScanReady(pDC, rectScanReady, AOIStep);
		drawGlassLoading(pDC, rectGlassLoading, AOIStep);
		drawInspectionEnd(pDC, rectInspectionEnd, AOIStep);
	}
	else
	{
		CRect rectDraw = rect;
		rectDraw.DeflateRect(2,2,2,2);
		drawDisplayMessage(pDC, rectDraw, AOIStep);
	}



}


void CakStaticAOIState::drawScanStart( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	int nScanIndex = m_nCurScanIndex;
	int nScanNum = m_nScanNum;

	CRect rectScanStart = rect;
	CBrush brushHatch, *pOldBrush;
	brushHatch.CreateHatchBrush(HS_BDIAGONAL, RGB(76,170,255));

	CFont* pOldFont = pDC->SelectObject(&m_fntScanStart);

	int nDisplayScanNum = nScanNum;
	if(m_nMaxDisplayScanNum<nScanNum) nDisplayScanNum = m_nMaxDisplayScanNum;

	int nDrawStartScanIndex = nScanIndex - (nDisplayScanNum-2);
	if(nDrawStartScanIndex<0) nDrawStartScanIndex = 0;
	if(nDrawStartScanIndex+nDisplayScanNum >= m_nScanNum) nDrawStartScanIndex = m_nScanNum-nDisplayScanNum;


	for(int i=0; i<nDisplayScanNum; i++)
	{
		int nPosLeft = rectScanStart.Width()*((double)i/(double)(nDisplayScanNum));
		int nPosRight = rectScanStart.Width()*((double)(i+1)/(double)(nDisplayScanNum));

		rect.left = rectScanStart.left+nPosLeft;
		rect.right = rectScanStart.left+nPosRight;

		pDC->SetTextColor(colorText1);

		int nDrawScanIndex = nDrawStartScanIndex+i;

		if((AOIStep > AS_ScanStart && nScanIndex>=nDrawScanIndex) || (AOIStep >= AS_ScanStart && nDrawScanIndex < nScanIndex)
			|| AOIStep > AS_ScanEnd
			|| ( (AOIStep == AS_ScanReady||AOIStep == AS_ScanReadyOK) && nDrawScanIndex<nScanIndex))//완료 이후
		{
			pDC->FillSolidRect(rect, colorOK);
		}
		else if(AOIStep < AS_ScanStart || nDrawScanIndex > nScanIndex)//진행전
		{
			pOldBrush = (CBrush*)pDC->SelectObject(&brushHatch);
			pDC->SelectStockObject(NULL_PEN);
			pDC->FillSolidRect(rect, colorBase);
			pDC->Rectangle(rect);
			pDC->SelectObject(pOldBrush);
		}
		else if(AOIStep == AS_ScanStart)//진행중
		{
			if(m_bBlinkSwitch)	
			{
				pDC->FillSolidRect(rect, colorRun);
			}
			else
			{
				pDC->FillSolidRect(rect, colorRun2);
				pDC->SetTextColor(colorText2);
			}
		}



		pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);

		CString strText = "Scan\r\nStart";
		strText.Format("Scan %d", nDrawScanIndex);
		CRect  rectText = rect;
		{
			pDC->SelectObject(&m_fntScanStart);
			rectText.bottom -= 30;
			//다중 문자열의 길이 연산
			CRect rc;
			pDC->DrawText(strText,rc, DT_CALCRECT);
			rectText.top = rectText.top + (rectText.Height()/2) - rc.Height()/2;
			// 문자열 표시
			pDC->DrawText(strText,rectText, DT_CENTER);
		}

		strText.Format("Cam %02d/%02d", m_nScanEndCamNum[nDrawScanIndex],m_nCamNum);
		if(m_nScanEndTime[nDrawScanIndex] != 0 && m_nScanStartTime[nDrawScanIndex] != 0) 
		{
			CString strProcessTime;
			strProcessTime.Format("\r\nProcessTime : %.1lfs", (m_nScanEndTime[nDrawScanIndex]-m_nScanStartTime[nDrawScanIndex])/1000.0);
			strText += strProcessTime;
		}
		rectText = rect;
		{
			pDC->SelectObject(&m_fntScanStartSub);
			rectText.top = rect.bottom - 30;
			//다중 문자열의 길이 연산
			CRect rc;
			pDC->DrawText(strText,rc, DT_CALCRECT);
			rectText.top = rectText.top + (rectText.Height()/2) - rc.Height()/2;
			// 문자열 표시
			pDC->DrawText(strText,rectText, DT_CENTER);
		}


	}
	pDC->SelectObject(pOldFont);

}

void CakStaticAOIState::drawScanStartVert( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	int nScanIndex = m_nCurScanIndex;
	int nScanNum = m_nScanNum;

	CRect rectScanStart = rect;
	CBrush brushHatch, *pOldBrush;
	brushHatch.CreateHatchBrush(HS_BDIAGONAL, RGB(76,170,255));

	CFont* pOldFont = pDC->SelectObject(&m_fntScanStart);

	int nDisplayScanNum = nScanNum;
	if(m_nMaxDisplayScanNum<nScanNum) nDisplayScanNum = m_nMaxDisplayScanNum;

	int nDrawStartScanIndex = nScanIndex - (nDisplayScanNum-2);
	if(nDrawStartScanIndex<0) nDrawStartScanIndex = 0;
	if(nDrawStartScanIndex+nDisplayScanNum >= m_nScanNum) nDrawStartScanIndex = m_nScanNum-nDisplayScanNum;

	for(int i=0; i<nDisplayScanNum; i++)
	{
		int nPosTop = rectScanStart.Height()*((double)i/(double)(nDisplayScanNum));
		int nPosBottom = rectScanStart.Height()*((double)(i+1)/(double)(nDisplayScanNum));


		int nDrawScanIndex = nDrawStartScanIndex+i;

		rect.top = rectScanStart.top+nPosTop;
		rect.bottom = rectScanStart.top+nPosBottom;

		pDC->SetTextColor(colorText1);

		if((AOIStep > AS_ScanStart && nScanIndex>=nDrawScanIndex) || (AOIStep >= AS_ScanStart && nDrawScanIndex < nScanIndex)
			|| AOIStep > AS_ScanEnd
			|| ( (AOIStep == AS_ScanReady||AOIStep == AS_ScanReadyOK) && nDrawScanIndex<nScanIndex))///완료 이후
		{
			pDC->FillSolidRect(rect, colorOK);
		}
		else if(AOIStep < AS_ScanStart || nDrawScanIndex > nScanIndex)//진행전
		{
			pOldBrush = (CBrush*)pDC->SelectObject(&brushHatch);
			pDC->SelectStockObject(NULL_PEN);
			pDC->FillSolidRect(rect, colorBase);
			pDC->Rectangle(rect);
			pDC->SelectObject(pOldBrush);
		}
		else if(AOIStep == AS_ScanStart)//진행중
		{
			if(m_bBlinkSwitch)	
			{
				pDC->FillSolidRect(rect, colorRun);
			}
			else
			{
				pDC->FillSolidRect(rect, colorRun2);
				pDC->SetTextColor(colorText2);
			}
		}



		pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);

		CString strText = "Scan\r\nStart";
		strText.Format("   Scan %02d", nDrawScanIndex);
		CRect  rectText = rect;
		{
			pDC->SelectObject(&m_fntScanStart);

			//다중 문자열의 길이 연산
			CRect rc;
			pDC->DrawText(strText,rc, DT_CALCRECT);
			rectText.top = rectText.top + (rectText.Height()/2) - rc.Height()/2;
			rectText.bottom = rectText.top+rc.Height();
			rectText.right = rectText.left + rectText.Width()/2;

			// 문자열 표시
			pDC->DrawText(strText,rectText, DT_LEFT);



			pDC->SelectObject(&m_fntScanStartSub);
			strText.Format("ScanEnd %02d/%02d", m_nScanEndCamNum[nDrawScanIndex],m_nCamNum);
			if(m_nScanEndTime[nDrawScanIndex] != 0 && m_nScanStartTime[nDrawScanIndex] != 0) 
			{
				CString strProcessTime;
				strProcessTime.Format(" ,ProcessTime : %.1lfs", (m_nScanEndTime[nDrawScanIndex]-m_nScanStartTime[nDrawScanIndex])/1000.0);
				strText += strProcessTime;
			}
			rectText.left = rectText.left+rc.Width()+10;
			rectText.right = rect.right;
			UINT oldAlign = pDC->SetTextAlign(TA_LEFT|TA_BOTTOM);
			pDC->TextOut(rectText.left, rectText.bottom, strText);
			pDC->SetTextAlign(oldAlign);
			//pDC->DrawText(strText,rectText, DT_LEFT|DT_BOTTOM);
		}




	}
	pDC->SelectObject(pOldFont);
}

void CakStaticAOIState::drawScanReady( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	int nScanIndex = m_nCurScanIndex;

	pDC->SetTextColor(colorText1);
	CFont* pOldFont = pDC->SelectObject(&m_fntScanReady);

	if(AOIStep < AS_ScanReady)
	{
		pDC->FillSolidRect(rect, colorBase);
	}
	else if(AOIStep == AS_ScanReady)//진행중
	{
		if(m_bBlinkSwitch)	pDC->FillSolidRect(rect, colorRun);
		else			
		{
			pDC->FillSolidRect(rect, colorRun2);
			pDC->SetTextColor(colorText2);
		}
	}
	else if(AOIStep > AS_ScanReady)//완료 이후
	{
		pDC->FillSolidRect(rect, colorOK);
	}

	pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);



	CString strText = "Scan\r\nReady";
	CRect  rectText = rect;
	{
		//다중 문자열의 길이 연산
		CRect rc;
		pDC->DrawText(strText,rc, DT_CALCRECT);
		rectText.top = rect.top + (rect.Height()/2) - rc.Height()/2;
		// 문자열 표시
		pDC->DrawText(strText,rectText, DT_CENTER);
	}

	pDC->SelectObject(pOldFont);
}


void CakStaticAOIState::drawGlassLoading( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	pDC->SetTextColor(colorText1);
	CFont* pOldFont = pDC->SelectObject(&m_fntGlassLoading);

	if(AOIStep < AS_GlassLoading)
	{
		pDC->FillSolidRect(rect, colorBase);
	}
	else if(AOIStep == AS_GlassLoading)//진행중
	{
		if(m_bBlinkSwitch)	pDC->FillSolidRect(rect, colorRun);
		else 
		{
			pDC->FillSolidRect(rect, colorRun2);
			pDC->SetTextColor(colorText2);
		}
	}
	else if(AOIStep > AS_GlassLoading)//완료 이후
	{
		pDC->FillSolidRect(rect, colorOK);
	}

	pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);

	CString strText = "Glass\r\nLoading";
	CRect  rectText = rect;
	{
		//다중 문자열의 길이 연산
		CRect rc;
		pDC->DrawText(strText,rc, DT_CALCRECT);
		rectText.top = rect.top + (rect.Height()/2) - rc.Height()/2;
		// 문자열 표시
		pDC->DrawText(strText,rectText, DT_CENTER);
	}

	pDC->SelectObject(pOldFont);
}


void CakStaticAOIState::drawInspectionEnd( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	pDC->SetTextColor(colorText1);
	CFont* pOldFont = pDC->SelectObject(&m_fntInspectionEnd);

	if(AOIStep < AS_InspectionEnd)
	{
		pDC->FillSolidRect(rect, colorBase);
	}
	else if(AOIStep == AS_InspectionEnd)//진행중
	{
		if(m_bBlinkSwitch)	pDC->FillSolidRect(rect, colorRun);
		else 
		{
			pDC->FillSolidRect(rect, colorRun2);
			pDC->SetTextColor(colorText2);
		}
	}
	else if(AOIStep > AS_InspectionEnd)//완료 이후
	{
		pDC->FillSolidRect(rect, colorOK);
	}

	pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);


	CString strText = "Inspection\r\nEnd";
	CRect  rectText = rect;
	{
		//다중 문자열의 길이 연산
		CRect rc;
		pDC->DrawText(strText,rc, DT_CALCRECT);
		rectText.top = rect.top + (rect.Height()/2) - rc.Height()/2;
		// 문자열 표시
		pDC->DrawText(strText,rectText, DT_CENTER);
	}

	pDC->SelectObject(pOldFont);
}



void CakStaticAOIState::drawDisplayMessage( CDC* pDC, CRect rect, _AOIStep AOIStep )
{
	COLORREF colorBase = m_clrStateNormal;
	COLORREF colorRun = m_clrStateRun1;
	COLORREF colorRun2 = m_clrStateRun2;
	COLORREF colorOK = m_clrStateOK;
	COLORREF colorText1 = m_clrTextColor1;
	COLORREF colorText2 = m_clrTextColor2;

	pDC->SetTextColor(colorText1);
	CFont* pOldFont = pDC->SelectObject(&m_fntDisplayMessage);

	if(AOIStep == AS_DisplayMessage && m_nBlinkCounter == -1)
	{
		pDC->FillSolidRect(rect, colorOK);
	}
	else if(AOIStep == AS_DisplayMessage)//진행중
	{
		if(m_bBlinkSwitch)	
		{
			pDC->FillSolidRect(rect, colorRun);
		}
		else 
		{
			pDC->FillSolidRect(rect, colorRun2);
			pDC->SetTextColor(colorText2);
		}
	}
	else if(AOIStep > AS_DisplayMessage)//완료 이후
	{
		pDC->FillSolidRect(rect, colorOK);
	}

	pDC->Draw3dRect(rect, m_clrStateOutLine, m_clrStateOutLine);


	CString strText = m_strDisplayMessage;
	CRect  rectText = rect;
	{
		//다중 문자열의 길이 연산
		CRect rc;
		pDC->DrawText(strText,rc, DT_CALCRECT);
		rectText.top = rect.top + (rect.Height()/2) - rc.Height()/2;
		// 문자열 표시
		pDC->DrawText(strText,rectText, DT_CENTER);
	}

	pDC->SelectObject(pOldFont);
}


void CakStaticAOIState::setGlassLoading( int nScanNum, int nCamNum/*=0*/ )
{
	m_nScanNum = nScanNum;
	m_nCamNum = nCamNum;
	memset(m_bScanEnd, 0, sizeof(bool)*MAX_CAMNUM);
	for(int i=0; i<MAX_SCANNUM; i++)
	{
		m_nScanStartTime[i] = 0;
		m_nScanEndTime[i] = 0;
		m_nScanEndCamNum[i] = 0;
	}
	m_nCurScanIndex = 0;
	m_nAOIStateStep = AS_GlassLoading;
	KillTimer(777);
	SetTimer(777, 50, NULL);
	Invalidate(FALSE);
}

void CakStaticAOIState::setGlassLoadingOK()
{
	m_nAOIStateStep = AS_GlassLoadingOK;
	KillTimer(777);
	Invalidate(FALSE);
}

void CakStaticAOIState::setScanReady()
{
	m_nAOIStateStep = AS_ScanReady;
	KillTimer(777);
	SetTimer(777, 50, NULL);
	Invalidate(FALSE);
}

void CakStaticAOIState::setScanReadyOK()
{
	m_nAOIStateStep = AS_ScanReadyOK;
	KillTimer(777);
	Invalidate(FALSE);
}


void CakStaticAOIState::setScanStart( int nScanIndex )
{
	if(nScanIndex > 0 && m_nScanEndCamNum[nScanIndex-1]<=0)
	{
		setScanEnd(-1);
	}
	m_nAOIStateStep = AS_ScanStart;
	m_nCurScanIndex = nScanIndex;
	m_nScanStartTime[nScanIndex] = m_nScanEndTime[nScanIndex] = GetTickCount();
	m_nScanEndCamNum[nScanIndex] = 0;
	memset(m_bScanEnd, 0, sizeof(bool)*MAX_CAMNUM);
	KillTimer(777);
	SetTimer(777, 50, NULL);
	Invalidate(FALSE);
}

void CakStaticAOIState::setScanEnd( int nCamIndex )
{
	if(nCamIndex >= 0)
	{
		m_bScanEnd[nCamIndex] = true;

		m_nScanEndTime[m_nCurScanIndex] = GetTickCount();
		m_nScanEndCamNum[m_nCurScanIndex] = getScanEndCount();

		if(m_nScanEndCamNum[m_nCurScanIndex] < m_nCamNum)
		{
			Invalidate(FALSE);
			return ;
		}
		

	}
	else if(m_nCurScanIndex>0)//이전걸 끝낸다.
	{
		m_nScanEndTime[m_nCurScanIndex-1] = GetTickCount();
		m_nScanEndCamNum[m_nCurScanIndex-1] = getScanEndCount();

	}
	else
	{
		return;
	}

	m_nAOIStateStep = AS_ScanEnd;
	if(nCamIndex == m_nCamNum-1) KillTimer(777);
	Invalidate(FALSE);
}

void CakStaticAOIState::setInspectionEnd()
{
	m_nAOIStateStep = AS_InspectionEnd;
	KillTimer(777);
	SetTimer(777, 50, NULL);
	Invalidate(FALSE);
}

void CakStaticAOIState::setInspectionEndOK()
{
	m_nAOIStateStep = AS_InspectionEndOK;
	KillTimer(777);
	Invalidate(FALSE);
}

int CakStaticAOIState::getScanEndCount()
{
	int nCount=0;
	for(int i=0; i<MAX_CAMNUM; i++)
	{
		if(m_bScanEnd[i] == true)
		{
			nCount++;
		}
	}
	return nCount;
}


void CakStaticAOIState::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(m_nAOIStateStep == AS_GlassLoading
		|| m_nAOIStateStep == AS_InspectionEnd
		|| m_nAOIStateStep == AS_ScanStart
		|| m_nAOIStateStep == AS_ScanReady
		|| (m_nAOIStateStep == AS_DisplayMessage && m_nBlinkCounter>=0))
	{
		m_nBlinkCounter++;
		if(m_nBlinkCounter>3)
		{
			m_bBlinkSwitch = !m_bBlinkSwitch;
			m_nBlinkCounter = 0;
		}

		if(m_nAOIStateStep == AS_ScanStart)
		{
			m_nScanEndTime[m_nCurScanIndex] = GetTickCount();
		}

		Invalidate(FALSE);
	}

	CStatic::OnTimer(nIDEvent);
}

void CakStaticAOIState::makeFont( CFont* pFont, char*fontName, int nfontSize )
{

	pFont->DeleteObject();
	pFont->CreateFont(-nfontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, fontName); 

	return;

	LOGFONT lf;
	CFont* pfont = GetFont();
	pfont->GetLogFont(&lf);


	if(fontName && strlen(fontName)>0)
	{
		strcpy(lf.lfFaceName, fontName);
	}

	if(nfontSize)
	{
		lf.lfHeight = -nfontSize;
	}

	pFont->DeleteObject();
	pFont->CreateFontIndirect(&lf); 

}

void CakStaticAOIState::setDisplayMessage( char* pMessage, BOOL bBlink /*= TRUE*/ )
{
	if(pMessage)
	{
		m_nAOIStateStep = AS_DisplayMessage;
		m_strDisplayMessage = pMessage;

		if(bBlink == TRUE)
		{
			KillTimer(777);
			SetTimer(777, 50, NULL);
		}
		else
		{
			m_nBlinkCounter = -1;
		}
		Invalidate(FALSE);
	}
}

void CakStaticAOIState::setDisplayMessageOK()
{
	m_nAOIStateStep = AS_DisplayMessageOK;
	KillTimer(777);
	Invalidate(FALSE);
}

