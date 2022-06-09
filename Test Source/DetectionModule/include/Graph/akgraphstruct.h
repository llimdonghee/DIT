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
	int m_nType; //0���콺 ��ġ�� ǥ��, 1���콺 ��ġ�� ���� ������ �� ǥ��
	int m_nTypeNum;
	CFont m_font; 
	COLORREF m_fontColor;

	int m_nPositionType; //0�϶� �Ʒ� ��ġ�� ǥ��, 1�϶� ���� �ϴ�
	unsigned int m_fontAlign; //����
	int m_nPositionX;//�׸��� ��ġ(���߿�...)
	int m_nPositionY;//�׸��� ��ġ(���߿�...)
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
	int m_nType; //-1:�׸��� ��翡 ���� �ٸ���, 0:��Ÿ��, 1:�簢��, 2:���׶��
	CFont m_font; 
	COLORREF m_fontColor;
	
	int m_nPosition;//�׸��� ��ġ(���߿�...)
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
	int m_nType; //1:��Ÿ��, 2:��Ÿ��, 3:�ͽ�, 0:hide
	int m_nLineWidth[5];
	COLORREF m_Color[5];
	char m_strLegend[32]; //����


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
	int m_nEnable; //0�� ����, 1�׻� ���, 2�ʿ��Ҷ��� ǥ��
	int m_nSize; //ǥ�� ũ��
public:

public:
	double m_nScrollMin;
	double m_nScrollMax;

public:
	CakRect m_rectScroll;
	CakRect m_rectThumb;
	bool m_bSelected;//���콺�� ������ �Ǿ����� üũ ����[2016/5/2]
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
	}; //������ ��Ʈ�� ���η� �ٲ۴�.

	void SetRotateFont(CFont* pFont, int nDeg)
	{
		LOGFONT logfont; 
		pFont->GetLogFont(&logfont);
		//logfont.lfHeight = 20;
		logfont.lfEscapement = nDeg*10; 
		pFont->DeleteObject();
		pFont->CreateFontIndirect(&logfont);
	}; //������ ��Ʈ�� ���η� �ٲ۴�.


	inline double GetRangeValue()
	{
		return m_RangeValueMax - m_RangeValueMin;
	}
	inline void SetRangeMove(double dValue)//dValue��ŭ ������
	{
		m_RangeValueMin += dValue;
		m_RangeValueMax += dValue;
	}
	inline void SetRangePos(double dValue)//�ش� ��ġ�� ����� �̵�
	{
		double dRangeHalf = GetRangeValue()/2.0;
		m_RangeValueMin = dValue-dRangeHalf;
		m_RangeValueMax = dValue+dRangeHalf;
	}

public:
	double m_RangeValueMin; //������ ����
	double m_RangeValueMax; //������ ����
	int m_Size; //�� ũ��(x���� ����ũ��, y���� ���� ũ��)
	
	int m_TickGabPixel;	//�� & ƽ�� ǥ�õǴ� �ּ� �ȼ� ����(���� ǥ�ô� �ڵ���������)
	double m_TickGabStep;  //�󺧰� ƽ�� ǥ�õǴ� ���� ����
	COLORREF m_TickColor;
	int m_MinorTickNum;	//ƽ�� ƽ������ ���� ƽ ����
	CFont m_FontTick; //��Ʈ

	CString m_strTitle; //����

	CFont m_Font; //��Ʈ
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
		m_Font.CreatePointFont(150, _T("���� ���"));
		m_FontColor = RGB(0,0,0);
	}

public:
	CString m_strTitle;
	int m_Size;
	CFont m_Font; //��Ʈ
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
	CPoint Point; //���� ���콺 ����Ʈ
	CPoint prePoint; //���� ���콺 ����Ʈ
	CPoint stPointDrag;
	bool MouseButtonState[10];
	//���콺 �巹�� ���� ����
	int bDragState;//0:���� 1:�� 2:��->����
	CPoint ClickPointL;
	CPoint ClickPointR;
	CPoint DragEndPoint;//�簢���� �׷��� �κ� ũ��
	
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
	DS_None,	//����
	DS_All,		//���
	DS_All_VER,		//���α���
	DS_All_HOR,		//���α���
	DS_Vertical, //����
	DS_Horizontal,
	DS_MAX
};
