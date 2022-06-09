#pragma once

#include "Graph/akGraphBasic3.h"

class CakGraphAOI : public CakGraphBasic3
{
public:
	CakGraphAOI(void);
	~CakGraphAOI(void);

public:
	virtual void RenderBegin(CDC* pDC, CRect rectDC);
	virtual void RenderLabel(Graphics* grfx, CDC* pDC);
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//마우스 움직일때 위치값 표시하는부분
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //데이터 선/바 그리기
	virtual void SetAutoScale();
	//void SetAutoScaleHeight();
public:
	void setDataNum(int nSeriseNum, int nDataNum);
	void setData(int nSeriseIndex, int nDataIndex, int val);
	void setAxisXStringNum(int nNum);
	void setColorType( int nType, CakGraphAOI* pGraph );

public:
	int			m_nWaringLine;
	int			m_nAxisXTextNum;
	CString*	m_strAxisXText;
};
