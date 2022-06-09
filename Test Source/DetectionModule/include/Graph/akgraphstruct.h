#pragma once

#include "STL/akRect.h"

class AFX_EXT_CLASS CMaker
{
public:
	CMaker()
	{
		m_nShape = 0;
	};
	~CMaker(){};

public:
	int m_nShape;

};

class AFX_EXT_CLASS CMouseMoveInfo
{
public:
	CMouseMoveInfo()
	{
		m_bEnable = true;
		m_font.CreatePointFont(90, _T("Tahoma"));
		m_fontColor = RGB(255,255,255);
		m_fontAlign = TA_RIGHT|TA_BASELINE;
		m_nTypeNum = 2;
		m_nType = 1;
		m_nPositionType = 1;
		
		m_nPositionX = -10;
		m_nPositionY = -10;

	};
	~CMouseMoveInfo(){};

public:
	bool m_bEnable;
	int m_nType; //0마우스 위치값 표시, 1마우스 위치에 따른 데이터 값 표시
	int m_nTypeNum;
	CFont m_font; 
	COLORREF m_fontColor;

	int m_nPositionType; //0일때 아래 위치로 표시, 1일때 우측 하단
	unsigned int m_fontAlign; //정렬
	int m_nPositionX;//그리는 위치(나중에...)
	int m_nPositionY;//그리는 위치(나중에...)
};

class AFX_EXT_CLASS CLegendOption
{
public:
	CLegendOption()
	{
		m_bEnable = false;
		m_font.CreatePointFont(90, _T("Tahoma"));
		m_fontColor = RGB(0,0,0);
		m_nType = 1;
	};
	~CLegendOption(){};

public:
	bool m_bEnable;
	int m_nType; //-1:그리는 모양에 따라서 다르게, 0:선타입, 1:사각형, 2:동그라미
	CFont m_font; 
	COLORREF m_fontColor;
	
	int m_nPosition;//그리는 위치(나중에...)
};

class AFX_EXT_CLASS CSeries
{
public:
	CSeries()
	{
		m_nType = 1;
		for(int i=0; i<5; i++)
		{
			m_Color[i] = RGB(0,0,255);
			m_nLineWidth[i] = 1;
		}
		memset(m_strLegend, 0, sizeof(char)*32);
	};
	~CSeries(){};

public:
	int m_nType; //1:선타입, 2:바타입, 3:믹스, 0:hide
	int m_nLineWidth[5];
	COLORREF m_Color[5];
	char m_strLegend[32]; //제목


};

class AFX_EXT_CLASS CScroll
{
public:
	CScroll()
	{
		m_nEnable = 0;
		m_nSize = 25;

		//m_bruScrollRect.CreateSolidBrush()
		m_bSelected = false;
	};

public:
	int m_nEnable; //0은 비사용, 1항상 사용, 2필요할때만 표시
	int m_nSize; //표시 크기
public:

public:
	double m_nScrollMin;
	double m_nScrollMax;

public:
	CakRect m_rectScroll;
	CakRect m_rectThumb;
	bool m_bSelected;//마우스로 선택이 되었는지 체크 태현[2016/5/2]
};

class AFX_EXT_CLASS CAxis 
{
public:
	CAxis()
	{
		m_RangeValueMin = 0;
		m_RangeValueMax = 10;
		m_Size = 50;
		m_TickGabPixel = 50; 
		m_TickGabStep = -1;
		m_MinorTickNum = 3; 
		m_FontTick.CreatePointFont(90, _T("Tahoma"));
		m_strTitle = "Axis";
		m_Font.CreatePointFont(90, _T("Tahoma"));
		m_FontColor = RGB(0,0,0);
		m_TickColor = RGB(0,0,0);
		m_LabelColor = RGB(0,0,0);
	}

	void SetVerticalFont()
	{
		LOGFONT logfont; 
		m_Font.GetLogFont(&logfont);
		//logfont.lfHeight = 20;
		logfont.lfEscapement = 900; 
		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&logfont);
	}; //설정된 폰트를 세로로 바꾼다.

	void SetRotateFont(CFont* pFont, int nDeg)
	{
		LOGFONT logfont; 
		pFont->GetLogFont(&logfont);
		//logfont.lfHeight = 20;
		logfont.lfEscapement = nDeg*10; 
		pFont->DeleteObject();
		pFont->CreateFontIndirect(&logfont);
	}; //설정된 폰트를 세로로 바꾼다.


	inline double GetRangeValue()
	{
		return m_RangeValueMax - m_RangeValueMin;
	}
	inline void SetRangeMove(double dValue)//dValue만큼 움직임
	{
		m_RangeValueMin += dValue;
		m_RangeValueMax += dValue;
	}
	inline void SetRangePos(double dValue)//해당 위치의 가운데로 이동
	{
		double dRangeHalf = GetRangeValue()/2.0;
		m_RangeValueMin = dValue-dRangeHalf;
		m_RangeValueMax = dValue+dRangeHalf;
	}

public:
	double m_RangeValueMin; //데이터 범위
	double m_RangeValueMax; //데이터 범위
	int m_Size; //축 크기(x축은 세로크기, y축은 가로 크기)
	
	int m_TickGabPixel;	//라벨 & 틱이 표시되는 최소 픽셀 간격(라벨의 표시는 자동연산으로)
	double m_TickGabStep;  //라벨과 틱이 표시되는 값의 간격
	COLORREF m_TickColor;
	int m_MinorTickNum;	//틱과 틱사이의 작은 틱 갯수
	CFont m_FontTick; //폰트

	CString m_strTitle; //제목

	CFont m_Font; //폰트
	COLORREF m_FontColor;
	COLORREF m_LabelColor;
	
	CScroll m_Scroll;
};

class AFX_EXT_CLASS CTitle
{
public:
	CTitle()
	{
		m_Size = 60;
		m_strTitle = "Advance Kim Graph 2015";
		m_Font.CreatePointFont(150, _T("맑은 고딕"));
		m_FontColor = RGB(0,0,0);
	}

public:
	CString m_strTitle;
	int m_Size;
	CFont m_Font; //폰트
	COLORREF m_FontColor;
};

class AFX_EXT_CLASS CakMouseState
{
public:
	CakMouseState()
	{
		bDragState = 0;
		apply = false;
		Point.SetPoint(0,0);
		memset(MouseButtonState,0,sizeof(MouseButtonState));
	}
	inline void setPoint(CPoint pos)
	{
		//if(apply == true)
		{
			prePoint = Point;
			Point = pos;
			apply = false;
		}
		/*else
		{
		Point = pos;
		}*/
	}

	bool apply;
	CPoint Point; //현제 마우스 포인트
	CPoint prePoint; //이전 마우스 포인트
	CPoint stPointDrag;
	bool MouseButtonState[10];
	//마우스 드레그 관리 변수
	int bDragState;//0:꺼짐 1:온 2:온->오프
	CPoint ClickPointL;
	CPoint ClickPointR;
	CPoint DragEndPoint;//사각형이 그려진 부분 크기
	
};

enum akMouseEvent
{
	MouseNone,
	MouseMove,
	MouseLeftButtonDown,
	MouseLeftButtonUp,
	MouseLeftButtonDoubleClick,
	MouseMiddleButtonDown,
	MouseMiddleButtonUp,
	MouseMiddleButtonDoubleClick,
	MouseRightButtonDown,
	MouseRightButtonUp,
	MouseRightButtonDoubleClick,
	MouseWheelDown,
	MouseWheelUp,
	MouseLeftButtonDrag,
	MouseRightButtonDrag
};

enum khMousetButtonState
{
	LEFTBUTTONDOWN,
	RIGHTBUTTONDOWN,
	WHEELUP,
	WHEELDOWN
};

enum akDragStyle
{
	DS_None,	//없음
	DS_All,		//모두
	DS_All_VER,		//세로기준
	DS_All_HOR,		//가로기준
	DS_Vertical, //세로
	DS_Horizontal,
	DS_MAX
};
