
#include "stdafx.h"
#include "akGraphAOI.h"
#include "Graph/akGraphUtil.h"

CakGraphAOI::CakGraphAOI(void)
{
	CakGraphBase::setAK();
	m_strAxisXText = NULL;
	m_AxisX.m_Size = 30;//m_AxisX.m_Size = 100;
	m_AxisX.m_TickGabStep = 1;
	m_AxisX.m_MinorTickNum = 4;
	m_AxisX.m_TickGabPixel = 30;
	m_AxisX.m_FontTick.DeleteObject();
	m_AxisX.m_FontTick.CreatePointFont(80, _T("Tahoma"));
	m_AxisX.SetRotateFont(&m_AxisX.m_FontTick, 270);
	m_AxisX.m_strTitle = "";
	
	m_AxisY.m_Size = 55;
	m_AxisY.m_strTitle = "";

	m_Title.m_Size = 30;
	m_Title.m_Font.DeleteObject();
	m_Title.m_Font.CreatePointFont(100, _T("맑은 고딕"));
	SetClossLine(false);
	m_nAxisXTextNum = 0;
	m_nWaringLine = 100;

	setColorType(0, this);
	
	m_bRangeXSetting = FALSE;
	m_bRangeYSetting = FALSE;
}

CakGraphAOI::~CakGraphAOI(void)
{
	if(m_strAxisXText) delete [] m_strAxisXText;
	m_strAxisXText = NULL;
	m_nAxisXTextNum = 0;
}


void CakGraphAOI::setDataNum( int nSeriseNum, int nDataNum )
{

	SetSeriesNum(nSeriseNum);

	for(int i=0; i<nSeriseNum; i++)
	{
		m_pSeriesData[i].SetDataNum(nDataNum);

		for(int j=0; j<nDataNum; j++)
		{
			m_pSeriesData[i].m_pData[j].x=j;
			m_pSeriesData[i].m_pData[j].y=0;
		}
	}
}



void CakGraphAOI::setData( int nSeriseIndex, int nDataIndex, int val )
{
	if(nSeriseIndex < 0 || nSeriseIndex >= GetSeriesNum()) return;
	if(nDataIndex < 0 || nDataIndex >= m_pSeriesData[nSeriseIndex].GetDataNum()) return;

	m_pSeriesData[nSeriseIndex].m_pData[nDataIndex].set(nDataIndex, val);
	
}

void CakGraphAOI::RenderLabel( Graphics* grfx, CDC* pDC )
{
	//축 틱 그리고/값넣기
	CFont* poldFont;

	CPen gdipen1,gdipen2,gdipen3,gdipen4,*pOldPen;
	gdipen1.CreatePen(PS_SOLID,2,m_AxisX.m_TickColor);//x축
	gdipen2.CreatePen(PS_SOLID,1,m_AxisX.m_TickColor);//x축
	gdipen3.CreatePen(PS_SOLID,2,m_AxisY.m_TickColor);//y축
	gdipen4.CreatePen(PS_SOLID,1,m_AxisY.m_TickColor);//y축


	
	pOldPen = (CPen*)pDC->SelectObject(&gdipen1);



	//x축
	{	
		BOOL bTextTick = FALSE;
		poldFont = (CFont*)pDC->SelectObject(&m_AxisX.m_FontTick);
		if(bTextTick == FALSE)
		{
			(CFont*)pDC->SelectObject(&m_AxisY.m_FontTick);
		}

		//눈금간격
		double tickgab;
		int nDecimal;
		CakGraphUtil::CalculationTickPostionNormal(
			m_AxisX.m_RangeValueMin, 
			m_AxisX.m_RangeValueMax, 
			m_rectData.getWidth(), 
			m_AxisX.m_TickGabPixel, 
			&tickgab, &nDecimal);

		bool m_bOK = true;
		if(GetWindowPosX(5)-GetWindowPosX(0)<20) m_bOK = false;

		if(tickgab > 0)
		{
			CString strData;
			
			pDC->SetTextAlign(TA_CENTER | TA_TOP);
			pDC->SetTextColor(m_AxisX.m_LabelColor);

			double tickval; //그리는 눈금에 해당하는 값
			int tickpoint; //그리는 눈금의 윈오우 좌표
			int miniTickPoint; //그리는 작은 눈금의 윈오우 좌표
			tickval =(tickgab*(int)(m_AxisX.m_RangeValueMin/tickgab - 1)); //그리드 그리는 시작 위치
			if(m_bOK)
			{
				tickval = (int)tickval;
				tickval = (int)(tickval/5)*5;
				tickgab = 5;
			}
			
			//int hh,mm,ss, ms;
			for(tickval; tickval<=m_AxisX.m_RangeValueMax; tickval+=tickgab)
			{
				//if(tickval < 0) continue;
				//큰눈금
				tickpoint = GetWindowPosX(tickval);
				if(tickpoint >= m_rectData.left && tickpoint <= m_rectData.right)
				{
					pDC->SelectObject(&gdipen1);

					pDC->MoveTo(tickpoint, m_rectAxisX.top - 7);
					pDC->LineTo(tickpoint, m_rectAxisX.top + 5);


					//strData.Format("%lf", tickval);
					if(bTextTick && m_bOK && tickval>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >tickval)
					{
						strData = m_strAxisXText[(int)tickval];
					}
					else
					{
						strData.Format("%.0lf",  tickval);
					}
					
					if(nDecimal == 0) nDecimal = -1;
					//pDC->TextOut(tickpoint, m_rectAxisX.top + 20, strData.Left(strData.GetLength()-(6-nDecimal)));
					pDC->TextOut(tickpoint, m_rectAxisX.top+10, strData);

				}
				//작은 눈금
				pDC->SelectObject(&gdipen2);
				for(int i=0; i<m_AxisX.m_MinorTickNum; i++)
				{
					int minitickval = (int)(tickval+(tickgab*(i+1)/(m_AxisX.m_MinorTickNum+1)));
					miniTickPoint = GetWindowPosX(tickval+(tickgab*(i+1)/(m_AxisX.m_MinorTickNum+1)));

					if(miniTickPoint < m_rectData.left) continue;
					if(miniTickPoint >= m_rectData.right) continue;

					pDC->MoveTo(miniTickPoint, m_rectAxisX.top - 5);
					pDC->LineTo(miniTickPoint, m_rectAxisX.top );

					//strData.Format("%lf", tickval);
					if(bTextTick && minitickval>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >minitickval)
					{
						strData = m_strAxisXText[(int)minitickval];
					}
					else
					{
						strData="";
					}

					if(nDecimal == 0) nDecimal = -1;
					//pDC->TextOut(tickpoint, m_rectAxisX.top + 20, strData.Left(strData.GetLength()-(6-nDecimal)));
					pDC->TextOut(miniTickPoint, m_rectAxisX.top+10, strData);
				}
			}
			pDC->SetTextAlign(TA_LEFT | TA_TOP);
		}

	}


	//y축
	{
		poldFont = (CFont*)pDC->SelectObject(&m_AxisY.m_FontTick);

		//눈금간격
		double tickgab;
		int nDecimal;
		CakGraphUtil::CalculationTickPostionNormal(
			m_AxisY.m_RangeValueMin, 
			m_AxisY.m_RangeValueMax, 
			m_rectData.getHeight(), 
			m_AxisY.m_TickGabPixel, 
			&tickgab, &nDecimal);



		if(tickgab > 0)
		{
			CString strData;
			poldFont = (CFont*)pDC->SelectObject(&m_AxisX.m_Font);
			pDC->SetTextAlign(TA_RIGHT | TA_BASELINE);
			pDC->SetTextColor(m_AxisY.m_LabelColor);

			double tickval; //그리는 눈금에 해당하는 값
			int tickpoint; //그리는 눈금의 윈오우 좌표
			int miniTickPoint; //그리는 작은 눈금의 윈오우 좌표
			tickval =(tickgab*(int)(m_AxisY.m_RangeValueMin/tickgab - 1)); //그리드 그리는 시작 위치
			for(tickval; tickval<=m_AxisY.m_RangeValueMax; tickval+=tickgab)
			{
				//큰눈금
				tickpoint = GetWindowPosY(tickval);
				if(tickpoint >= m_rectData.top
					&& tickpoint <= m_rectData.bottom)
				{
					pDC->SelectObject(&gdipen3);

					pDC->MoveTo(m_rectData.left+7, tickpoint+1);
					pDC->LineTo(m_rectData.left-5, tickpoint+1);
					if(tickgab<1)
					{
						char format1[8]={};
						sprintf(format1, "%%.%dlf", nDecimal);
						strData.Format(format1, tickval);
					}
					else
					{
						strData.Format("%d", (int)tickval);
					}
					pDC->TextOut(m_rectData.left - 10, tickpoint+4, strData.GetBuffer(0));
				}
				//작은 눈금
				pDC->SelectObject(&gdipen4);
				for(int i=0; i<m_AxisY.m_MinorTickNum; i++)
				{
					miniTickPoint = GetWindowPosY(tickval+(tickgab*(i+1)/(m_AxisY.m_MinorTickNum+1)));

					if(miniTickPoint < m_rectData.top) continue;
					if(miniTickPoint >= m_rectData.bottom) continue;

					pDC->MoveTo(m_rectData.left+0, miniTickPoint);
					pDC->LineTo(m_rectData.left+5, miniTickPoint);

				}
			}


			pDC->SetTextAlign(TA_LEFT | TA_TOP);
		}


		pDC->SetTextAlign(TA_LEFT | TA_TOP);
	}




	pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldFont);


	//외각선
	{
		CRect rect = CakGraphUtil::akRectToCRect(m_rectData);
		rect.left--;
		rect.top--;
		rect.right+=2;
		rect.bottom+=2;
		pDC->Draw3dRect(&rect,m_AxisY.m_TickColor,m_AxisX.m_TickColor);
		rect.InflateRect(1, 1);
		pDC->Draw3dRect(&rect,m_AxisY.m_TickColor,m_AxisX.m_TickColor);
	}
}

void CakGraphAOI::RenderMouseMoveInfo( Graphics* grfx, CDC* pDC )
{
	
	//실행여부 체크
	if(m_strAxisXText == NULL) return;
	double valuex = GetValuePosDataX(m_MouseState.Point.x,false);
	int dataIndex = (int)(valuex+0.5);
	if(dataIndex >= 50) return;
	if(dataIndex < 0 || (int)(valuex+0.5) > m_nAxisXTextNum) return;
	if(m_strAxisXText[(int)(valuex+0.5)].GetLength() <= 0) return;

	//마우스 정보 표시
	if(m_rectData.getCheckAreaIn(m_MouseState.Point.x, m_MouseState.Point.y)
		&& m_bHover && m_MouseMoveInfo.m_bEnable)
	{
		if(m_MouseMoveInfo.m_nType == 0)
		{
			CString strData;

			double valuex = GetValuePosDataX(m_MouseState.Point.x,false);
			double valuey = GetValuePosDataY(m_MouseState.Point.y,false);
			//if(value >= 0)
			{
				strData.Format("%.0lf, %.0lf", 
					valuex,
					valuey);

				CFont* poldfont;
				pDC->SetTextAlign(m_MouseMoveInfo.m_fontAlign);
				poldfont = pDC->SelectObject(&m_MouseMoveInfo.m_font);
				pDC->SetTextColor(m_MouseMoveInfo.m_fontColor);

				pDC->TextOut(m_rectData.right+m_MouseMoveInfo.m_nPositionX,m_rectData.bottom+m_MouseMoveInfo.m_nPositionY,
					strData);

				pDC->SelectObject(poldfont);
			}	
		}
		else if(m_MouseMoveInfo.m_nType == 1)
		{
			CString strData;

			//double value = GetValuePosDataX(m_MouseState.Point.x,false);
			//if(value >= 0)
			{
				int valuex = GetValuePosDataX(m_MouseState.Point.x,false)+0.5;

				

				CFont* poldfont;
				pDC->SetTextAlign(m_MouseMoveInfo.m_fontAlign);
				poldfont = pDC->SelectObject(&m_MouseMoveInfo.m_font);
				pDC->SetTextColor(m_MouseMoveInfo.m_fontColor);

				/*for(int i=0; i<m_SeriesNum; i++)
				{
					int dataindex = getDataIndex(i, valuex);
					if(dataindex >= 0)
					{
						
						strData.Format("%s %.0lf, %.0lf", 
							m_pSeries[i].m_strLegend,
							m_pSeriesData[i].m_pData[dataindex].x,
							m_pSeriesData[i].m_pData[dataindex].y);
					}
					else
					{
						strData.Format("Series%d No Data", i);
					}

					pDC->TextOut(m_rectData.right+m_MouseMoveInfo.m_nPositionX,
						m_rectData.bottom+m_MouseMoveInfo.m_nPositionY-((m_SeriesNum-i-1)*10),
						strData);


				}*/

				if(valuex>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >valuex)
				{
					

					strData = m_strAxisXText[(int)valuex];

					CString strAdd;
					strAdd.Format(" : "); strData+=strAdd;
					int ncount=0;
					for(int i=0; i<m_SeriesNum; i++)
					{
						if(m_pSeriesHidden[i] == true) continue;

						int dataindex = getDataIndex(i, valuex);

						if(ncount==0)strAdd.Format("%.0lf", m_pSeriesData[i].m_pData[dataindex].y);
						else strAdd.Format(", %.0lf", m_pSeriesData[i].m_pData[dataindex].y); 

						strData+=strAdd;
						ncount++;

						


					}
					pDC->TextOut(m_rectData.right+m_MouseMoveInfo.m_nPositionX,
						m_rectData.bottom+m_MouseMoveInfo.m_nPositionY,
						strData);

				}

				

				pDC->SelectObject(poldfont);
			}	
		}


	}


	//마우스 위치 점 표시
	if(m_rectData.getCheckAreaIn(m_MouseState.Point.x, m_MouseState.Point.y)
		&& m_bHover && m_MouseMoveInfo.m_bEnable)
	{

		int valuex = GetValuePosDataX(m_MouseState.Point.x,false)+0.5;



		if(valuex>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >valuex)
		{

			CPen *pOldPen;
			CBrush brush, *pBOldBrush;
			CPoint point[4];
			for(int i=0; i<m_SeriesNum; i++)
			{

				if(m_pSeriesHidden[i] == true) continue;
				int dataindex = getDataIndex(i, valuex);

				CPen penLine;
				//penLine.CreatePen(PS_SOLID,  m_pSeries[i].m_nLineWidth[0], m_pSeries[i].m_Color[0]);
				penLine.CreatePen(PS_SOLID,  1, RGB(255,255,255));
				pOldPen = pDC->SelectObject(&penLine);

				brush.DeleteObject();
				brush.CreateSolidBrush(m_pSeries[i].m_Color[0]);
				pBOldBrush = pDC->SelectObject(&brush);


				int xpos = GetWindowPosX(dataindex);
				int ypos = GetWindowPosY(m_pSeriesData[i].m_pData[dataindex].y);
				
				if(m_rectData.getCheckAreaIn(xpos, ypos))
				{
					//pDC->FillSolidRect(xpos-3, ypos-3, 7, 7, m_pSeries[i].m_Color[0]);
					point[0].SetPoint(-5,0);
					point[1].SetPoint(0, -5);
					point[2].SetPoint(5, 0);
					point[3].SetPoint(0, 5);

					point[0].Offset(xpos, ypos);
					point[1].Offset(xpos, ypos);
					point[2].Offset(xpos, ypos);
					point[3].Offset(xpos, ypos);

					pDC->Polygon(point, 4);
				}
				
				
				
				
				pDC->SelectObject(pOldPen);
				pDC->SelectObject(pBOldBrush);
				

// 				{
// 					if(ncount==0)strAdd.Format("%.0lf", m_pSeriesData[i].m_pData[dataindex].y);
// 					else strAdd.Format(", %.0lf", m_pSeriesData[i].m_pData[dataindex].y); 
// 
// 					strData+=strAdd;
// 					ncount++;
// 
// 
// 					pDC->TextOut(m_rectData.right+m_MouseMoveInfo.m_nPositionX,
// 						m_rectData.bottom+m_MouseMoveInfo.m_nPositionY,
// 						strData);
// 				}

			}


		}

	}
}

void CakGraphAOI::RenderSeries( Graphics* grfx, CDC* pDC )
{
	CakGraphBasic3::RenderSeries(grfx, pDC);

	//경고선
	{
		int waringpos = m_nWaringLine;

		CPen *pOldPen;

		CPen penLine;
		penLine.CreatePen(PS_DASH, 1, RGB(255,0,0));
		pOldPen = pDC->SelectObject(&penLine);


		pDC->MoveTo(m_rectData.left, GetWindowPosY(waringpos));
		pDC->LineTo(m_rectData.right, GetWindowPosY(waringpos));




		pDC->SelectObject(pOldPen);

	}
	


	{
		//축 틱 그리고/값넣기
		CFont* poldFont;



		//x축
		{	
			poldFont = (CFont*)pDC->SelectObject(&m_AxisX.m_FontTick);

			//눈금간격
			double tickgab;
			int nDecimal;
			CakGraphUtil::CalculationTickPostionNormal(
				m_AxisX.m_RangeValueMin, 
				m_AxisX.m_RangeValueMax, 
				m_rectData.getWidth(), 
				m_AxisX.m_TickGabPixel, 
				&tickgab, &nDecimal);

			bool m_bOK = true;
			if(GetWindowPosX(5)-GetWindowPosX(0)<20) m_bOK = false;

			if(tickgab > 0)
			{
				CString strData;

				//pDC->SetTextAlign(TA_LEFT | TA_BASELINE);
				pDC->SetTextAlign(TA_RIGHT | TA_BASELINE);
				pDC->SetTextColor(m_AxisX.m_LabelColor);

				double tickval; //그리는 눈금에 해당하는 값
				int tickpoint; //그리는 눈금의 윈오우 좌표
				int miniTickPoint; //그리는 작은 눈금의 윈오우 좌표
				tickval =(tickgab*(int)(m_AxisX.m_RangeValueMin/tickgab - 1)); //그리드 그리는 시작 위치
				if(m_bOK)
				{
					tickval = (int)tickval;
					tickval = (int)(tickval/5)*5;
					tickgab = 5;
				}

				//int hh,mm,ss, ms;
				for(tickval; tickval<=m_AxisX.m_RangeValueMax; tickval+=tickgab)
				{
					//if(tickval < 0) continue;
					//큰눈금
					tickpoint = GetWindowPosX(tickval);
					if(tickpoint >= m_rectData.left && tickpoint <= m_rectData.right)
					{


						//strData.Format("%lf", tickval);
						if(m_bOK && tickval>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >tickval)
						{
							strData = m_strAxisXText[(int)tickval];
						}
						else
						{
							//strData.Format("%.0lf",  tickval);
						}

						if(nDecimal == 0) nDecimal = -1;
						//pDC->TextOut(tickpoint, m_rectAxisX.top + 20, strData.Left(strData.GetLength()-(6-nDecimal)));
						pDC->TextOut(tickpoint-5, m_rectAxisX.top-10, strData);

					}
					//작은 눈금
					for(int i=0; i<m_AxisX.m_MinorTickNum; i++)
					{
						int minitickval = (int)(tickval+(tickgab*(i+1)/(m_AxisX.m_MinorTickNum+1)));
						miniTickPoint = GetWindowPosX(tickval+(tickgab*(i+1)/(m_AxisX.m_MinorTickNum+1)));

						if(miniTickPoint < m_rectData.left) continue;
						if(miniTickPoint >= m_rectData.right) continue;

						//strData.Format("%lf", tickval);
						if(minitickval>=0 && GetSeriesNum()>0 && m_pSeriesData[0].GetDataNum() >minitickval)
						{
							strData = m_strAxisXText[(int)minitickval];
						}
						else
						{
							strData="";
						}

						if(nDecimal == 0) nDecimal = -1;
						//pDC->TextOut(tickpoint, m_rectAxisX.top + 20, strData.Left(strData.GetLength()-(6-nDecimal)));
						pDC->TextOut(miniTickPoint-5, m_rectAxisX.top-10, strData);
					}
				}
				pDC->SetTextAlign(TA_LEFT | TA_TOP);
			}

		}


	}

	
}

void CakGraphAOI::SetAutoScale()
{
	CakGraphBasic3::SetAutoScale();
	if(m_AxisY.m_RangeValueMin == 0 &&	m_AxisY.m_RangeValueMax == 0)
	{
		m_AxisY.m_RangeValueMax = 300;
	}
	m_AxisY.m_RangeValueMin = 0;
	m_AxisX.m_RangeValueMin = -0.5;
	m_AxisX.m_RangeValueMax = 49+0.5;
}

void CakGraphAOI::setAxisXStringNum( int nNum )
{
	if(m_nAxisXTextNum == nNum) return;

	if(m_strAxisXText)
	{
		delete [] m_strAxisXText;
	}
	
	if(nNum > 0) m_strAxisXText = new CString[nNum];

// 	for(int i=0; i<nNum; i++)
// 	{
// 		m_strAxisXText[i].Format("%d***", i);
// 	}

	m_nAxisXTextNum = nNum;
}


void CakGraphAOI::setColorType( int nType, CakGraphAOI* pGraph )
{


	switch(nType)
	{
	case 0:
		{
			pGraph->m_colorBack =RGB(39, 39, 39);
			pGraph->m_colorDataArea = RGB(10,10,10);
			pGraph->m_AxisX.m_FontColor = RGB(255, 255, 255);
			pGraph->m_AxisY.m_FontColor = RGB(255, 255, 255);
			pGraph->m_AxisX.m_LabelColor = RGB(255, 255, 255);
			pGraph->m_AxisY.m_LabelColor = RGB(255, 255, 255);
			pGraph->m_Title.m_FontColor = RGB(255, 255, 255);
			pGraph->m_AxisX.m_TickColor = RGB(255,255,255);
			pGraph->m_AxisY.m_TickColor = RGB(80, 80, 80);
			//pGraph->m_ColorBarFontColor = RGB(255,255,255);

			pGraph->m_MouseMoveInfo.m_fontColor = RGB(255,255,0);
			pGraph->m_LegendOption.m_fontColor = RGB(255,255, 255);

			break;
		};
	case 1:
		{
			pGraph->m_colorBack =RGB(240, 240, 240);
			pGraph->m_colorDataArea = RGB(255,255,255);
			pGraph->m_AxisX.m_FontColor = RGB(0,0,0);
			pGraph->m_AxisY.m_FontColor = RGB(0,0,0);
			pGraph->m_AxisX.m_LabelColor = RGB(0,0,0);
			pGraph->m_AxisY.m_LabelColor = RGB(0,0,0);
			pGraph->m_Title.m_FontColor = RGB(0,0,0);
			pGraph->m_AxisX.m_TickColor = RGB(0,0,0);
			pGraph->m_AxisY.m_TickColor = RGB(0, 0, 0);
			//pGraph->m_ColorBarFontColor = RGB(0, 0, 0);

			pGraph->m_MouseMoveInfo.m_fontColor = RGB(0,0,0);
			pGraph->m_LegendOption.m_fontColor = RGB(0,0,0);
			
			break;
		};

	}


}

void CakGraphAOI::RenderBegin( CDC* pDC, CRect rectDC )
{
	CakGraphBasic3::RenderBegin(pDC, rectDC);

	pDC->FillSolidRect(rectDC.right-50, rectDC.bottom-10, 50, 10, m_colorBack);
}
